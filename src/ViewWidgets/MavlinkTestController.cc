/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "MavlinkTestController.h"
#include "MultiVehicleManager.h"
#include "QGCMAVLink.h"
#include "QGCApplication.h"
#include "UAS.h"
#include "QGCApplication.h"
#include "QGCToolbox.h"
#include "QGCMapEngine.h"
#include "Vehicle.h"
#include "MainWindow.h"
#include "ParameterManager.h"

#include <QDebug>
#include <QSettings>
#include <QUrl>
#include <QBitArray>
#include <QtCore/qmath.h>

#define LOW2BYTE(i) ((uint16_t)(i))
#define HIGH2BYTE(i) ((uint16_t)(((uint16_t)(i))>>16))

#define MAX_PACKET_LOST_NUM 1024
#define MS_TO_H             (60*60*1000)
#define MS_TO_MIN           (60*1000)
#define MS_TO_S             1000

//----------------------------------------------------------------------------------------
MavlinkTestController::MavlinkTestController(bool standaloneUnitTesting)
    : FactPanelController(standaloneUnitTesting)
    , _mavlink(qgcApp()->toolbox()->mavlinkProtocol())
    , _speedRAW_SENS(0)
    , _speedEXT_STAT(0)
    , _speedRC_CHAN(0)
    , _speedRAW_CTRL(0)
    , _speedPOSITION(0)
    , _speedEXTRA1(0)
    , _speedEXTRA2(0)
    , _speedEXTRA3(0)
    , _speedPARAMS(0)
    , _speedADSB(0)
    , _speedPID(0)
    , _bitmask(0)
    , _sr0(false)
    , _sr1(false)
    , _sr2(false)
    , _sr3(false)
    , _elasticitySendCount(0U)
    , _elasticityRcvCount(0U)
    , _elasticityLostCount(0U)
    , _elasticityStartTime(QDateTime::currentDateTime())
    , _FIFOLenChoose(180)
    , _averageLatency(0U)
    , _minLatency(0U)
    , _maxLatency(0U)
{
    MultiVehicleManager *manager = qgcApp()->toolbox()->multiVehicleManager();
    connect(manager, &MultiVehicleManager::activeVehicleChanged, this, &MavlinkTestController::_setActiveVehicle);
    _setActiveVehicle(manager->activeVehicle());

    /* Stream speed */
    _listRAW_SENS<<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listEXT_STAT<<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listRC_CHAN <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listRAW_CTRL<<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listPOSITION<<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listEXTRA1  <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listEXTRA2  <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listEXTRA3  <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listPARAMS  <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listADSB    <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";
    _listPID     <<"0Hz"<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz";

    /* Evaluating Communication Quality */
    connect(&_timer, &QTimer::timeout                          , this, &MavlinkTestController::_elasticityPackSend);
    _elasticitySendList<<"";
    _elasticitySendHzList<<"1Hz"<<"2Hz"<<"3Hz"<<"4Hz"<<"5Hz"<<"6Hz"<<"7Hz"<<"8Hz"<<"9Hz"<<"10Hz"<<"0Hz";
    _elasticityPassTime = "";
    memset(_data, 0, sizeof(_data));
    _latencyFIFO.init(180);
}

//----------------------------------------------------------------------------------------
void
MavlinkTestController::_setActiveVehicle(Vehicle* vehicle)
{
    if(_uas) {
        disconnect(vehicle, &Vehicle::mavlinkRCChannels    , this, &MavlinkTestController::_handleRCChannels);
        disconnect(vehicle, &Vehicle::mavlinkRCChannelsRaw , this, &MavlinkTestController::_handleRCChannelsRaw);
        disconnect(vehicle, &Vehicle::mavlinkElasticity    , this, &MavlinkTestController::_handleElasticity);
        disconnect(vehicle, &Vehicle::mavlinkServoOutputRaw, this, &MavlinkTestController::_handleServoOutputRaw);
        _uas = NULL;
    }
    _vehicle = vehicle;
    if(_vehicle) {
        _uas = vehicle->uas();
        connect(vehicle, &Vehicle::mavlinkRCChannels    , this, &MavlinkTestController::_handleRCChannels);
        connect(vehicle, &Vehicle::mavlinkRCChannelsRaw , this, &MavlinkTestController::_handleRCChannelsRaw);
        connect(vehicle, &Vehicle::mavlinkElasticity    , this, &MavlinkTestController::_handleElasticity);
        connect(vehicle, &Vehicle::mavlinkServoOutputRaw, this, &MavlinkTestController::_handleServoOutputRaw);
    }
}

//----------------------------------------------------------------------------------------
/* Stream speed */
void
MavlinkTestController::speedRAW_SENS(QString hz){
    _speedRAW_SENS = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedRAW_SENS: "<<QString("%1").arg(_speedRAW_SENS);

    changeStreamSpeed();
}
void
MavlinkTestController::speedEXT_STAT(QString hz){
    _speedEXT_STAT = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedEXT_STAT: "<<QString("%1").arg(_speedEXT_STAT);

    changeStreamSpeed();
}
void
MavlinkTestController::speedRC_CHAN(QString hz){
    _speedRC_CHAN = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedRC_CHAN: "<<QString("%1").arg(_speedRC_CHAN);

    changeStreamSpeed();
}
void
MavlinkTestController::speedRAW_CTRL(QString hz){
    _speedRAW_CTRL = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedRAW_CTRL: "<<QString("%1").arg(_speedRAW_CTRL);

    changeStreamSpeed();
}
void
MavlinkTestController::speedPOSITION(QString hz){
    _speedPOSITION = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedPOSITION: "<<QString("%1").arg(_speedPOSITION);

    changeStreamSpeed();
}
void
MavlinkTestController::speedEXTRA1(QString hz){
    _speedEXTRA1 = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedEXTRA1: "<<QString("%1").arg(_speedEXTRA1);

    changeStreamSpeed();
}
void
MavlinkTestController::speedEXTRA2(QString hz){
    _speedEXTRA2 = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedEXTRA2: "<<QString("%1").arg(_speedEXTRA2);

    changeStreamSpeed();
}
void
MavlinkTestController::speedEXTRA3(QString hz){
    _speedEXTRA3 = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedEXTRA3: "<<QString("%1").arg(_speedEXTRA3);

    changeStreamSpeed();
}
void
MavlinkTestController::speedPARAMS(QString hz){
    _speedPARAMS = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedPARAMS: "<<QString("%1").arg(_speedPARAMS);

    changeStreamSpeed();
}
void
MavlinkTestController::speedADSB(QString hz){
    _speedADSB = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedADSB: "<<QString("%1").arg(_speedADSB);

    changeStreamSpeed();
}
void
MavlinkTestController::speedPID(QString hz){
    _speedPID = hz.left(hz.length()-2).toInt();
    qDebug()<<"speedPID: "<<QString("%1").arg(_speedPID);

    changeStreamSpeed();
}
void
MavlinkTestController::clearStreamSpeed(){
    _speedRAW_SENS = 0;
    _speedEXT_STAT = 0;
    _speedRC_CHAN  = 0;
    _speedRAW_CTRL = 0;
    _speedPOSITION = 0;
    _speedEXTRA1   = 0;
    _speedEXTRA2   = 0;
    _speedEXTRA3   = 0;
    _speedPARAMS   = 0;
    _speedADSB     = 0;
    _speedPID      = 0;

    changeStreamSpeed();
}

void
MavlinkTestController::setBitMask(){
    _bitmask = (_sr0 << 0) | (_sr1 << 1) | (_sr2 << 2) | (_sr3 << 3);
    qDebug()<<"sr0"<<QString(_sr0);
    qDebug()<<"sr1"<<QString(_sr1);
    qDebug()<<"sr2"<<QString(_sr2);
    qDebug()<<"sr3"<<QString(_sr3);
    qDebug()<<"bitmask"<<QString("%1").arg(_bitmask).toInt();
}

void
MavlinkTestController::changeStreamSpeed()
{
    mavlink_message_t msg;
    uint32_t speed1 = 0;
    uint32_t speed2 = 0;

    if(_vehicle){
        speed1 |= (uint32_t)(_speedRAW_SENS & 0x0000000F)<<0;
        speed1 |= (uint32_t)(_speedEXT_STAT & 0x0000000F)<<4;
        speed1 |= (uint32_t)(_speedRC_CHAN  & 0x0000000F)<<8;
        speed1 |= (uint32_t)(_speedRAW_CTRL & 0x0000000F)<<12;
        speed1 |= (uint32_t)(_speedPOSITION & 0x0000000F)<<16;
        speed1 |= (uint32_t)(_speedEXTRA1   & 0x0000000F)<<20;
        speed1 |= (uint32_t)(_speedEXTRA2   & 0x0000000F)<<24;
        speed1 |= (uint32_t)(_speedEXTRA3   & 0x0000000F)<<28;
        speed2 |= (uint32_t)(_speedPARAMS   & 0x0000000F)<<0;
        speed2 |= (uint32_t)(_speedADSB     & 0x0000000F)<<4;
        speed2 |= (uint32_t)(_speedPID      & 0x0000000F)<<8;

        mavlink_msg_stream_speed_pack(_mavlink->getSystemId(),
                                      _mavlink->getComponentId(),
                                      &msg,
                                      speed1,
                                      speed2,
                                      _bitmask);
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }

}

//----------------------------------------------------------------------------------------
/* Channels Override */
void
MavlinkTestController::_handleRCChannels(mavlink_message_t message){
    mavlink_rc_channels_t packet;
    mavlink_msg_rc_channels_decode(&message, &packet);

    _ch1RCV  = packet.chan1_raw;
    _ch2RCV  = packet.chan2_raw;
    _ch3RCV  = packet.chan3_raw;
    _ch4RCV  = packet.chan4_raw;
    _ch5RCV  = packet.chan5_raw;
    _ch6RCV  = packet.chan6_raw;
    _ch7RCV  = packet.chan7_raw;
    _ch8RCV  = packet.chan8_raw;
    _ch9RCV  = packet.chan9_raw;
    _ch10RCV = packet.chan10_raw;
    _ch11RCV = packet.chan11_raw;
    _ch12RCV = packet.chan12_raw;
    _ch13RCV = packet.chan13_raw;
    _ch14RCV = packet.chan14_raw;
    _ch15RCV = packet.chan15_raw;
    _ch16RCV = packet.chan16_raw;
    _ch17RCV = packet.chan17_raw;
    _ch18RCV = packet.chan18_raw;
    emit ch1RCVChanged();
    emit ch2RCVChanged();
    emit ch3RCVChanged();
    emit ch4RCVChanged();
    emit ch5RCVChanged();
    emit ch6RCVChanged();
    emit ch7RCVChanged();
    emit ch8RCVChanged();
    emit ch9RCVChanged();
    emit ch10RCVChanged();
    emit ch11RCVChanged();
    emit ch12RCVChanged();
    emit ch13RCVChanged();
    emit ch14RCVChanged();
    emit ch15RCVChanged();
    emit ch16RCVChanged();
    emit ch17RCVChanged();
    emit ch18RCVChanged();
}

void
MavlinkTestController::_handleRCChannelsRaw(mavlink_message_t message){
    mavlink_rc_channels_raw_t packet;
    mavlink_msg_rc_channels_raw_decode(&message, &packet);

    _ch1RCV  = packet.chan1_raw;
    _ch2RCV  = packet.chan2_raw;
    _ch3RCV  = packet.chan3_raw;
    _ch4RCV  = packet.chan4_raw;
    _ch5RCV  = packet.chan5_raw;
    _ch6RCV  = packet.chan6_raw;
    _ch7RCV  = packet.chan7_raw;
    _ch8RCV  = packet.chan8_raw;
    emit ch1RCVChanged();
    emit ch2RCVChanged();
    emit ch3RCVChanged();
    emit ch4RCVChanged();
    emit ch5RCVChanged();
    emit ch6RCVChanged();
    emit ch7RCVChanged();
    emit ch8RCVChanged();
}

void
MavlinkTestController::sendChannelsOverride(){
    mavlink_message_t msg;
    mavlink_msg_rc_channels_override_pack(_mavlink->getSystemId(),
                                          _mavlink->getComponentId(),
                                          &msg,
                                          0,
                                          0,
                                          _ch1Value,
                                          _ch2Value,
                                          _ch3Value,
                                          _ch4Value,
                                          _ch5Value,
                                          _ch6Value,
                                          _ch7Value,
                                          _ch8Value);
    if(_vehicle){
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }
}

//----------------------------------------------------------------------------------------
/* Evaluating Communication Quality */
void
MavlinkTestController::setFIFOLenChoose(QString length)
{
    int FIFOLenChoose = length.toInt();

    if(FIFOLenChoose < 1){
        _FIFOLenChoose = 1;
    }else{
        _FIFOLenChoose = FIFOLenChoose;
    }

    _latencyFIFO.init(_FIFOLenChoose);

    _averageLatency      = 0;
    _minLatency          = 0;
    _maxLatency          = 0;
    emit averageLatencyChanged();
    emit minLatencyChanged();
    emit maxLatencyChanged();
}

void
MavlinkTestController::elasticitySendHzChoose(QString hz)
{
    int elasticitySendHz = hz.left(hz.length()-2).toInt();

    if(elasticitySendHz < 0){
        _elasticitySendHz = 0;
    }else{
        _elasticitySendHz = elasticitySendHz;
    }

    if ( _timer.isActive() ){
        _timer.stop();
    }

    if(_elasticitySendHz > 0){
        _timer.start(1000/_elasticitySendHz);
    }

    _elasticityStartTime = QDateTime::currentDateTime();
    emit elasticityStartTimeChanged();

    _data[0]             = 0;

    _elasticitySendCount = 0;
    _elasticityRcvCount  = 0;
    _elasticityLostCount = 0;
    _averageLatency      = 0;
    _minLatency          = 0;
    _maxLatency          = 0;
    _elasticityPassTime  = "";
    emit elasticitySendCountChanged();
    emit elasticityRcvCountChanged();
    emit elasticityLostCountChanged();
    emit averageLatencyChanged();
    emit minLatencyChanged();
    emit maxLatencyChanged();
    emit elasticityPassTimeChanged();
}

void
MavlinkTestController::_elasticityPackSend(){

    mavlink_message_t msg;

    if(_vehicle){
        qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        memcpy(&_data[1], &timestamp, 8);
        mavlink_msg_elasticity_pack(_mavlink->getSystemId(),
                                    _mavlink->getComponentId(),
                                    &msg,
                                    _data);
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);

        QString send_id = QString("%1 %2").arg(_data[0]).arg(timestamp);
        if (!_elasticitySendList.contains(send_id)){
            _elasticitySendList<<send_id;
            if(_elasticitySendList.length() > MAX_PACKET_LOST_NUM){
                _elasticitySendList.removeFirst();
            }
        }

        _elasticitySendCount++;
        _data[0]++;
        emit elasticitySendCountChanged();
    }else{
        qDebug()<<"No vehicle linked, don't send message!";
    }
}

void
MavlinkTestController::_calcPassTime(){
    QDateTime now = QDateTime::currentDateTime();

    qint64 pass_time = _elasticityStartTime.msecsTo(now);
    uint   h         = 0;
    uint   min       = 0;
    uint   s         = 0;
    uint   ms        = 0;

    if(pass_time >= MS_TO_H){
        h   = pass_time / MS_TO_H;
        pass_time -= h * MS_TO_H;
        min = pass_time / MS_TO_MIN;
        pass_time -= min * MS_TO_MIN;
        s   = pass_time / MS_TO_S;
        pass_time -= s * MS_TO_S;
        ms  = pass_time;
    }else if(pass_time >= MS_TO_MIN){
        min = pass_time / MS_TO_MIN;
        pass_time -= min * MS_TO_MIN;
        s   = pass_time / MS_TO_S;
        pass_time -= s * MS_TO_S;
        ms  = pass_time;
    }else if(pass_time >= MS_TO_S){
        s   = pass_time / MS_TO_S;
        pass_time -= s * MS_TO_S;
        ms  = pass_time;
    }else{
        ms  = pass_time;
    }

    if(h > 0){
        _elasticityPassTime = QString("%1").arg(h)  +"h "
                             +QString("%1").arg(min)+"min "
                             +QString("%1").arg(s)  +"s "
                             +QString("%1").arg(ms) +"ms ";
    }else if(min > 0){
        _elasticityPassTime = QString("%1").arg(min)+"min "
                             +QString("%1").arg(s)  +"s "
                             +QString("%1").arg(ms) +"ms ";
    }else if(s > 0){
        _elasticityPassTime = QString("%1").arg(s)  +"s "
                             +QString("%1").arg(ms) +"ms ";
    }else{
        _elasticityPassTime = QString("%1").arg(ms)+"ms";
    }

    emit elasticityPassTimeChanged();
}

void
MavlinkTestController::_handleElasticity(mavlink_message_t message)
{
    QDateTime now_timestamp = QDateTime::currentDateTime();
    _calcPassTime();

    mavlink_elasticity_t packet;
    mavlink_msg_elasticity_decode(&message, &packet);

    qint64 timestamp;
    memcpy(&timestamp, &packet.data[1], 8);

    _elasticityRcvCount++;
    if(_elasticitySendCount > _elasticityRcvCount){
        _elasticityLostCount = _elasticitySendCount - _elasticityRcvCount;
    }
    emit elasticityRcvCountChanged();
    emit elasticityLostCountChanged();

    if (_elasticitySendList.contains(QString("%1 %2").arg(packet.data[0]).arg(timestamp))){
        QStringList elasticitySendList_filter = _elasticitySendList.filter(QString("%1 ").arg(packet.data[0]));
        QString     filt_timestamp            = elasticitySendList_filter.at(0);
        int indexOfSendList                   = _elasticitySendList.indexOf(filt_timestamp);
        _elasticitySendList.removeAt(indexOfSendList);
        qDebug()<<"Lost list:"<<_elasticitySendList;
        filt_timestamp.replace(QRegExp("^(.*) (.*)$"), "\\2");
        qint64      send_timestamp            = filt_timestamp.toLongLong();

        uint latency = (uint)QDateTime::fromMSecsSinceEpoch(send_timestamp).msecsTo(now_timestamp);
        _latencyFIFO.to_fifo(latency);
        _averageLatency = _latencyFIFO.average;
        _minLatency     = _latencyFIFO.min;
        _maxLatency     = _latencyFIFO.max;
        emit averageLatencyChanged();
        emit minLatencyChanged();
        emit maxLatencyChanged();

        emit FIFODisplayChanged();
    }
}

QString
MavlinkTestController::FIFODisplay(){
    if(_latencyFIFO.get_count() <= _latencyFIFO.get_length()){
        return "("+QString("%1").arg(_latencyFIFO.get_count())
                  +"/"
                  +QString("%1").arg(_latencyFIFO.get_length())
                  +")";
    }else{
        return "";
    }
}

//----------------------------------------------------------------------------------------
/* Servos Output */
void
MavlinkTestController::_handleServoOutputRaw(mavlink_message_t message)
{
    mavlink_servo_output_raw_t packet;
    mavlink_msg_servo_output_raw_decode(&message, &packet);

    _ch1OUT = packet.servo1_raw;
    _ch2OUT = packet.servo2_raw;
    _ch3OUT = packet.servo3_raw;
    _ch4OUT = packet.servo4_raw;
    _ch5OUT = packet.servo5_raw;
    _ch6OUT = packet.servo6_raw;
    _ch7OUT = packet.servo7_raw;
    _ch8OUT = packet.servo8_raw;
    _ch9OUT = packet.servo9_raw;
    _ch10OUT = packet.servo10_raw;
    _ch11OUT = packet.servo11_raw;
    _ch12OUT = packet.servo12_raw;
    _ch13OUT = packet.servo13_raw;
    _ch14OUT = packet.servo14_raw;
    _ch15OUT = packet.servo15_raw;
    _ch16OUT = packet.servo16_raw;

    emit ch1OUTChanged();
    emit ch2OUTChanged();
    emit ch3OUTChanged();
    emit ch4OUTChanged();
    emit ch5OUTChanged();
    emit ch6OUTChanged();
    emit ch7OUTChanged();
    emit ch8OUTChanged(); 
    emit ch9OUTChanged();
    emit ch10OUTChanged();
    emit ch11OUTChanged();
    emit ch12OUTChanged();
    emit ch13OUTChanged();
    emit ch14OUTChanged();
    emit ch15OUTChanged();
    emit ch16OUTChanged();
}

//----------------------------------------------------------------------------------------
/* Servos Test */
void
MavlinkTestController::servosTestTimeSet(QString time){
    _servosTestTime = time.left(time.length()).toInt();
    qDebug()<<"servosTestTime: "<<QString("%1").arg(_servosTestTime);
}

void
MavlinkTestController::servo1TestPercent(QString percent){
    _servo1TestPercent = percent.left(percent.length()).toInt();
    qDebug()<<"servo1TestPercent: "<<QString("%1").arg(_servo1TestPercent);
}

void
MavlinkTestController::servo2TestPercent(QString percent){
    _servo2TestPercent = percent.left(percent.length()).toInt();
    qDebug()<<"servo2TestPercent: "<<QString("%1").arg(_servo2TestPercent);
}

void
MavlinkTestController::servo3TestPercent(QString percent){
    _servo3TestPercent = percent.left(percent.length()).toInt();
    qDebug()<<"servo3TestPercent: "<<QString("%1").arg(_servo3TestPercent);
}

void
MavlinkTestController::servoTailTestPercent(QString percent){
    _servoTailTestPercent = percent.left(percent.length()).toInt();
    qDebug()<<"servoTailTestPercent: "<<QString("%1").arg(_servoTailTestPercent);
}

void
MavlinkTestController::mainRotorTestPercent(QString percent){
    _mainRotorTestPercent = percent.left(percent.length()).toInt();
    qDebug()<<"mainRotorTestPercent: "<<QString("%1").arg(_mainRotorTestPercent);
}

void
MavlinkTestController::startServoTest(int servo_seq){
    mavlink_message_t msg;
    int               percent;

    switch(servo_seq){
    case 1:
        percent = _servo1TestPercent;
        break;
    case 2:
        percent = _servo2TestPercent;
        break;
    case 3:
        percent = _servo3TestPercent;
        break;
    case 4:
        percent = _servoTailTestPercent;
        break;
    case 5:
        percent = _mainRotorTestPercent;
        break;
    }

    mavlink_msg_command_long_pack(_mavlink->getSystemId(),
                                  _mavlink->getComponentId(),
                                  &msg,
                                  0,
                                  0,
                                  MAV_CMD_DO_MOTOR_TEST,
                                  0,
                                  servo_seq,
                                  MOTOR_TEST_THROTTLE_PERCENT,
                                  percent,
                                  _servosTestTime,
                                  0,
                                  0,
                                  0);

    qDebug()<<"servo_seq: "<<QString("%1").arg(servo_seq);
    qDebug()<<"test time: "<<QString("%1").arg(_servosTestTime);
    qDebug()<<"percent  : "<<percent;

    if(_vehicle){
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }
}

void
MavlinkTestController:: stopServoTest(void){
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(_mavlink->getSystemId(),
                                  _mavlink->getComponentId(),
                                  &msg,
                                  0,
                                  0,
                                  MAV_CMD_DO_MOTOR_TEST,
                                  0,
                                  1,
                                  MOTOR_TEST_THROTTLE_PERCENT,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0);

    qDebug()<<"stopServoTest";

    if(_vehicle){
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }
}
