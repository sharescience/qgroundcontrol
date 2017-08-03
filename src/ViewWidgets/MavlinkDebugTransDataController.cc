/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "MavlinkDebugTransDataController.h"
#include "MultiVehicleManager.h"
#include "QGCMAVLink.h"
#include "QGCQFileDialog.h"
#include "UAS.h"
#include "QGCApplication.h"
#include "QGCToolbox.h"
#include "QGCMapEngine.h"
#include "Vehicle.h"
#include "MainWindow.h"

#include <QDebug>
#include <QSettings>
#include <QUrl>
#include <QBitArray>
#include <QtCore/qmath.h>

#define MAX_PACKET_LOST_NUM 1024
#define MS_TO_H             (60*60*1000)
#define MS_TO_MIN           (60*1000)
#define MS_TO_S             1000
#define MSG_LIST_LEN        100
#define TIMER_FREQUENCY_HZ  2

//----------------------------------------------------------------------------------------
MavlinkDebugTransDataController::MavlinkDebugTransDataController(bool standaloneUnitTesting)
    : FactPanelController(standaloneUnitTesting)
    , _rcvMavIDChooseLast(-1)
    , _sendMavIDChooseLast(-1)
    , _sendCacheEnable(false)
    , _rcvCacheEnable(false)
    , _msgSendIndex(1)
    , _msgReceiveIndex(1)
{
    MultiVehicleManager *manager = qgcApp()->toolbox()->multiVehicleManager();
    connect(manager, &MultiVehicleManager::activeVehicleChanged, this, &MavlinkDebugTransDataController::_setActiveVehicle);
    connect(&_timer, &QTimer::timeout                          , this, &MavlinkDebugTransDataController::timer_tick);
    _setActiveVehicle(manager->activeVehicle());
    _idListReceive<<"-1";
    _idListSend<<"-1";
    _rcvHz  = "";
    _sendHz = "";
    _addNewItemCauseChooseChangeReceive = false;
    _addNewItemCauseChooseChangeSend    = false;
    _msgReceiveList << "";
    _msgSendList    << "";
    _timer.start(1000/TIMER_FREQUENCY_HZ);
}

//----------------------------------------------------------------------------------------
void
MavlinkDebugTransDataController::_setActiveVehicle(Vehicle* vehicle)
{
    if(_uas) {
        disconnect(_uas   , &UASInterface::sendMessageContent   ,  this, &MavlinkDebugTransDataController::_sendMessageContent);
        disconnect(_uas   , &UASInterface::receiveMessageContent,  this, &MavlinkDebugTransDataController::_receiveMessageContent);
        _uas = NULL;
    }
    _vehicle = vehicle;
    if(_vehicle) {
        _uas = vehicle->uas();
        connect(_uas   , &UASInterface::sendMessageContent   ,  this, &MavlinkDebugTransDataController::_sendMessageContent);
        connect(_uas   , &UASInterface::receiveMessageContent,  this, &MavlinkDebugTransDataController::_receiveMessageContent);
    }
}

void
MavlinkDebugTransDataController::setsendMavIDChoose(QString id)
{
    _sendMavIDChoose = id;

    if( _sendMavIDChoose.mid(0,6).toInt(0, 16) == -1 && !_addNewItemCauseChooseChangeSend ){
        _idListSend.clear();
        _idListSend<<"-1";
        _sendMessageHex = "";
        emit sendMessageHexChanged();
        emit idListSendChanged();
    }

    if( _sendMavIDChoose.mid(0,6).toInt(0, 16) != -1 && _addNewItemCauseChooseChangeSend){
        _addNewItemCauseChooseChangeSend = false;
    }

    _sendHz         = "";
    emit sendHzChanged();
}

void
MavlinkDebugTransDataController::_sendMessageContent(UASInterface *uas, int mavid, QString msg, bool is_timer)
{
    QDateTime   currentTimeSend                 = QDateTime::currentDateTime();
    static      QDateTime lastTimeSend          = currentTimeSend;
    static bool ignorOnceSend                   = false;
    int         sendMavIDChooseInQML            = _sendMavIDChoose.mid(0,6).toInt(0, 16);

    if(!_uas || uas != _uas) {
        return;
    }
    _sendMessageHex      = msg;
    _sendMavID           = mavid;

    analyzeSend();
    updateSendList(_sendCacheEnable, _sendMessageHex);

    QString sendListItem = QString("%1").arg(_sendMavID,6,16,QLatin1Char('0')).toUpper()\
                           +"("+QString("%1").arg(_sendMavID,8,10,QLatin1Char('0'))+")";

    if(_sendMavIDChooseLast != sendMavIDChooseInQML){
        ignorOnceSend   = true;
        _sendHz         = "";
        emit sendHzChanged();
        _sendMavIDChooseLast = sendMavIDChooseInQML;
        if( sendMavIDChooseInQML == -1 ){
            _addNewItemCauseChooseChangeSend = false;
        }
    }

    if (!_idListSend.contains(sendListItem)){
        _idListSend<<sendListItem;
        qSort(_idListSend);
        _addNewItemCauseChooseChangeSend = true;
        _sendMessageHex = "Message ID: "+sendListItem+" send.";
        _sendHz         = "";
        emit idListSendChanged();
        emit sendMessageHexChanged();
        emit sendHzChanged();
    }

    if(_sendMavID == sendMavIDChooseInQML && !is_timer){
        if(!ignorOnceSend){
            _sendHz = QString::number(1000.0f/lastTimeSend.msecsTo(currentTimeSend),'g',2)+"Hz";
            emit sendHzChanged();
        }else{
            ignorOnceSend = false;
        }
        lastTimeSend = currentTimeSend;

        emit sendMessageHexChanged();
    }
}

void
MavlinkDebugTransDataController::setrcvMavIDChoose(QString id)
{
    _rcvMavIDChoose = id;

    if( _rcvMavIDChoose.mid(0,6).toInt(0, 16) == -1 && !_addNewItemCauseChooseChangeReceive ){
        _idListReceive.clear();
        _idListReceive<<"-1";
        _rcvMessageHex = "";
        emit rcvMessageHexChanged();
        emit idListReceiveChanged();

    }

    if( _sendMavIDChoose.mid(0,6).toInt(0, 16) != -1 && _addNewItemCauseChooseChangeReceive){
        _addNewItemCauseChooseChangeReceive = false;
    }

    _rcvHz          = "";
    emit rcvHzChanged();
}

void
MavlinkDebugTransDataController::_receiveMessageContent(UASInterface *uas, int mavid, QString msg, bool is_timer)
{
    QDateTime   currentTime                 = QDateTime::currentDateTime();
    static      QDateTime lastTime          = currentTime;
    static bool ignorOnce                   = false;
    int         rcvMavIDChooseInQML         = _rcvMavIDChoose.mid(0,6).toInt(0, 16);

    if(!_uas || uas != _uas) {
        return;
    }
    _rcvMessageHex      = msg;
    _rcvMavID           = mavid;

    analyzeRCV();
    updateReceiveList(_rcvCacheEnable, _rcvMessageHex);

    QString rcvListItem = QString("%1").arg(_rcvMavID,6,16,QLatin1Char('0')).toUpper()\
                          +"("+QString("%1").arg(_rcvMavID,8,10,QLatin1Char('0'))+")";

    if(_rcvMavIDChooseLast != rcvMavIDChooseInQML){
        ignorOnce      = true;
        _rcvHz         = "";
        emit rcvHzChanged();
        _rcvMavIDChooseLast = rcvMavIDChooseInQML;
        if( rcvMavIDChooseInQML == -1 ){
            _addNewItemCauseChooseChangeReceive = false;
        }
    }

    if (!_idListReceive.contains(rcvListItem)){
        _idListReceive<<rcvListItem;
        qSort(_idListReceive);
        _addNewItemCauseChooseChangeReceive = true;
        _rcvMessageHex = "Message ID: "+rcvListItem+" received.";
        _rcvHz         = "";
        emit idListReceiveChanged();
        emit rcvMessageHexChanged();
        emit rcvHzChanged();
    }

    if(_rcvMavID == rcvMavIDChooseInQML && !is_timer){
        if(!ignorOnce){
            _rcvHz = QString::number(1000.0f/lastTime.msecsTo(currentTime),'g',2)+"Hz";
            emit rcvHzChanged();
        }else{
            ignorOnce = false;
        }
        lastTime = currentTime;

        emit rcvMessageHexChanged();
    }
}

void
MavlinkDebugTransDataController::analyzeSend(){
    int     sendMavIDChooseInQML = _sendMavIDChoose.mid(0,6).toInt(0, 16);
    QString message              = "";

    if(_sendMessageHex.mid(0, 2).toInt(0, 16) == MAVLINK_STX_MAVLINK1){
        _sendVersion = 1;
    }else if(_sendMessageHex.mid(0, 2).toInt(0, 16) == MAVLINK_STX){
        _sendVersion = 2;
    }else{
        if(_vehicle){
            qDebug()<<"send unknown mavlink version!";
        }
    }
    emit sendVersionChanged();

    if(_sendMavID == sendMavIDChooseInQML){
        message = _sendMessageHex;
    } else if(sendMavIDChooseInQML == -1){
        message = _msgSendList.at(_msgSendList.length() - _msgSendIndex);
    } else {
        return;
    }

    analyzeMessage(&_sendVersion,
                   &message,
                   _sendMagic,
                   _sendLen,
                   _sendSeq,
                   _sendSysid,
                   _sendCompid,
                   _sendMsgid,
                   _sendIncompatFlag,
                   _sendCompatFlag,
                   _sendPayload,
                   _sendCkA,
                   _sendCkB,
                   _sendSignature);

    emit sendMagicChanged        ();
    emit sendLenChanged          ();
    emit sendSeqChanged          ();
    emit sendSysidChanged        ();
    emit sendCompidChanged       ();
    emit sendMsgidChanged        ();
    emit sendIncompatFlagChanged ();
    emit sendCompatFlagChanged   ();
    emit sendPayloadChanged      ();
    emit sendCkAChanged          ();
    emit sendCkBChanged          ();
    emit sendSignatureChanged    ();
}

void
MavlinkDebugTransDataController::analyzeRCV(){
    int     rcvMavIDChooseInQML = _rcvMavIDChoose.mid(0,6).toInt(0, 16);
    QString message             = "";

    if(_rcvMessageHex.mid(0, 2).toInt(0, 16) == MAVLINK_STX_MAVLINK1){
        _rcvVersion = 1;
    }else if(_rcvMessageHex.mid(0, 2).toInt(0, 16) == MAVLINK_STX){
        _rcvVersion = 2;
    }else{
        if(_vehicle){
            qDebug()<<"receive unknown mavlink version!";
        }
    }
    emit rcvVersionChanged();

    if(_rcvMavID == rcvMavIDChooseInQML){
        message = _rcvMessageHex;
    } else if(rcvMavIDChooseInQML == -1){
        message = _msgReceiveList.at(_msgReceiveList.length() - _msgReceiveIndex);
    } else {
        return;
    }

    analyzeMessage(&_rcvVersion,
                   &message,
                   _rcvMagic,
                   _rcvLen,
                   _rcvSeq,
                   _rcvSysid,
                   _rcvCompid,
                   _rcvMsgid,
                   _rcvIncompatFlag,
                   _rcvCompatFlag,
                   _rcvPayload,
                   _rcvCkA,
                   _rcvCkB,
                   _rcvSignature);
    emit rcvMagicChanged        ();
    emit rcvLenChanged          ();
    emit rcvSeqChanged          ();
    emit rcvSysidChanged        ();
    emit rcvCompidChanged       ();
    emit rcvMsgidChanged        ();
    emit rcvIncompatFlagChanged ();
    emit rcvCompatFlagChanged   ();
    emit rcvPayloadChanged      ();
    emit rcvCkAChanged          ();
    emit rcvCkBChanged          ();
    emit rcvSignatureChanged    ();

}

void
MavlinkDebugTransDataController::analyzeMessage(int*     version,
                                                QString* msg,
                                                QString& magic,
                                                QString& len,
                                                QString& seq,
                                                QString& sysid,
                                                QString& compid,
                                                QString& msgid,
                                                QString& incompat_flags,
                                                QString& compat_flags,
                                                QString& payload,
                                                QString& ck_a,
                                                QString& ck_b,
                                                QString& signature){
    switch(*version){
    case 1:{
        int payload_len   = 0;
        int crc_pos       = 0;

        magic          = msg->mid(0,2);
        len            = msg->mid(3, 2);
        incompat_flags = "N/A";
        compat_flags   = "N/A";
        seq            = msg->mid(6, 2);
        sysid          = msg->mid(9, 2);
        compid         = msg->mid(12, 2);
        msgid          = msg->mid(15, 2);

        payload_len    = 3 * len.toInt(0, 16) - 1;
        crc_pos        = 18 + len.toInt(0, 16)*3;

        payload        = msg->mid(18, payload_len);
        ck_a           = msg->mid(crc_pos, 2);
        ck_b           = msg->mid(crc_pos+3, 2);

        signature  = "N/A";

        break;
    }
    case 2:{
        int payload_len   = 0;
        int have_signature= 0;
        int crc_pos       = 0;
        int signature_pos = 0;
        int signature_len = (3 * MAVLINK_SIGNATURE_BLOCK_LEN - 1);

        magic          = msg->mid(0,2);
        len            = msg->mid(3, 2);
        incompat_flags = msg->mid(6, 2);
        compat_flags   = msg->mid(9, 2);
        seq            = msg->mid(12, 2);
        sysid          = msg->mid(15, 2);
        compid         = msg->mid(18, 2);
        msgid          = msg->mid(21, 8);

        payload_len    = 3 * len.toInt(0, 16) - 1;
        have_signature = incompat_flags.toInt() & MAVLINK_IFLAG_SIGNED;
        crc_pos        = 30 + len.toInt(0, 16)*3;
        signature_pos = crc_pos + 3;

        payload        = msg->mid(30, payload_len);
        ck_a           = msg->mid(crc_pos, 2);
        ck_b           = msg->mid(crc_pos+3, 2);

        if(have_signature){
            signature  = msg->mid(signature_pos, signature_len);
        }else{
            signature  = "";
        }

        break;
    }
    default:
        break;
    }
}

void
MavlinkDebugTransDataController::updateReceiveList(bool enable, QString msg){
    if(enable){
        if(_msgReceiveList.length() >= MSG_LIST_LEN){
            _msgReceiveList.removeFirst();
        }
        _msgReceiveList<<msg;
    }
}

void
MavlinkDebugTransDataController::updateSendList(bool enable, QString msg){
    if(enable){
        if(_msgSendList.length() >= MSG_LIST_LEN){
            _msgSendList.removeFirst();
        }
        _msgSendList<<msg;
    }
}

void
MavlinkDebugTransDataController::setmsgSendIndex(int index){
    if(index > _msgSendList.length()){
        _msgSendIndex = _msgSendList.length();
        return;
    }

    if(index < 1){
        _msgSendIndex = 1;
        return;
    }

    _msgSendIndex = index;
}

void
MavlinkDebugTransDataController::setmsgReceiveIndex(int index){
    if(index > _msgReceiveList.length()){
        _msgReceiveIndex = _msgReceiveList.length();
        return;
    }

    if(index < 1){
        _msgReceiveIndex = 1;
        return;
    }

    _msgReceiveIndex   = index;
}

void
MavlinkDebugTransDataController::timer_tick(){
    int rcvMavIDChooseInQML  = _rcvMavIDChoose.mid(0,6).toInt(0, 16);
    int sendMavIDChooseInQML = _sendMavIDChoose.mid(0,6).toInt(0, 16);

    if(sendMavIDChooseInQML == -1){
        analyzeSend();
    }else{
        _sendMessageContent(_uas, _sendMavID, _sendMessageHex, true);
    }

    if(rcvMavIDChooseInQML == -1){
        analyzeRCV();
    }else{
        _receiveMessageContent(_uas, _rcvMavID, _rcvMessageHex, true);
    }
}

void
MavlinkDebugTransDataController::clearSendList(){
    if(_msgSendList.length() > 1){
        _msgSendList.clear();
        _msgSendList << "";
    }
}

void
MavlinkDebugTransDataController::clearReceiveList(){
    if(_msgReceiveList.length() > 1){
        _msgReceiveList.clear();
        _msgReceiveList << "";
    }
}
