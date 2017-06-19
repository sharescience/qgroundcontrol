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

//----------------------------------------------------------------------------------------
MavlinkDebugTransDataController::MavlinkDebugTransDataController(bool standaloneUnitTesting)
    : FactPanelController(standaloneUnitTesting)
    , _rcvMavIDChooseLast(-1)
    , _sendMavIDChooseLast(-1)
{
    MultiVehicleManager *manager = qgcApp()->toolbox()->multiVehicleManager();
    connect(manager, &MultiVehicleManager::activeVehicleChanged, this, &MavlinkDebugTransDataController::_setActiveVehicle);
    _setActiveVehicle(manager->activeVehicle());
    _idListReceive<<"-1";
    _idListSend<<"-1";
    _rcvHz  = "";
    _sendHz = "";
    _addNewItemCauseChooseChangeReceive = false;
    _addNewItemCauseChooseChangeSend    = false;
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
        emit idListSendChanged();

    }

    if( _sendMavIDChoose.mid(0,6).toInt(0, 16) != -1 && _addNewItemCauseChooseChangeSend){
        _addNewItemCauseChooseChangeSend = false;
    }

    _sendHz         = "";
    emit sendHzChanged();
}

void
MavlinkDebugTransDataController::_sendMessageContent(UASInterface *uas, int mavid, QString msg)
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

    if(_sendMavID == sendMavIDChooseInQML){
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
        emit idListReceiveChanged();

    }

    if( _sendMavIDChoose.mid(0,6).toInt(0, 16) != -1 && _addNewItemCauseChooseChangeReceive){
        _addNewItemCauseChooseChangeReceive = false;
    }

    _rcvHz          = "";
    emit rcvHzChanged();
}

void
MavlinkDebugTransDataController::_receiveMessageContent(UASInterface *uas, int mavid, QString msg)
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

    if(_rcvMavID == rcvMavIDChooseInQML){
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
