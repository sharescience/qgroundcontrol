/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef MavlinkDebugTransData_H
#define MavlinkDebugTransData_H

#include <QObject>
#include <QTimer>
#include <QAbstractListModel>
#include <QLocale>
#include <QElapsedTimer>

#include <memory>

#include "UASInterface.h"
#include "AutoPilotPlugin.h"
#include "FactPanelController.h"

class  MultiVehicleManager;
class  UASInterface;
class  Vehicle;

//-----------------------------------------------------------------------------
class MavlinkDebugTransDataController : public FactPanelController
{
    Q_OBJECT
public:
    /// @param standaloneUnitTesting true: being run without factPanel, false: normal operation, factPanel is required
    MavlinkDebugTransDataController(bool standaloneUnitTesting = false);

    Q_PROPERTY(QString         rcvMessageHex   READ rcvMessageHex   NOTIFY rcvMessageHexChanged)
    Q_PROPERTY(QString         sendMessageHex  READ sendMessageHex  NOTIFY sendMessageHexChanged)
    Q_PROPERTY(QStringList     idListReceive   READ idListReceive   NOTIFY idListReceiveChanged)
    Q_PROPERTY(QStringList     idListSend      READ idListSend      NOTIFY idListSendChanged)
    Q_PROPERTY(QString         rcvHz           READ rcvHz           NOTIFY rcvHzChanged)
    Q_PROPERTY(QString         sendHz          READ sendHz          NOTIFY sendHzChanged)

    QString         rcvMessageHex           () { return _rcvMessageHex; }
    QString         sendMessageHex          () { return _sendMessageHex; }
    QStringList     idListSend              () { return _idListSend; }
    QStringList     idListReceive           () { return _idListReceive; }
    QString         rcvMavIDChoose          () { return _rcvMavIDChoose; }
    QString         rcvHz                   () { return _rcvHz; }
    QString         sendHz                  () { return _sendHz; }

    Q_INVOKABLE void setrcvMavIDChoose (QString id);
    Q_INVOKABLE void setsendMavIDChoose(QString id);

signals:
    void rcvMessageHexChanged   ();
    void sendMessageHexChanged  ();
    void idListReceiveChanged   ();
    void idListSendChanged      ();
    void rcvHzChanged           ();
    void sendHzChanged          ();

private slots:
    void _setActiveVehicle  (Vehicle* vehicle);
    void _receiveMessageContent(UASInterface *uas, int mavid, QString msg);
    void _sendMessageContent(UASInterface *uas, int mavid, QString msg);

private:
    Vehicle*            _vehicle;
    QString             _rcvMessageHex;
    int                 _rcvMavID;
    QString             _sendMessageHex;
    int                 _sendMavID;
    QStringList         _idListReceive;
    QStringList         _idListSend;
    QString             _rcvMavIDChoose;
    QString             _sendMavIDChoose;
    int                 _rcvMavIDChooseLast;
    int                 _sendMavIDChooseLast;
    QString             _rcvHz;
    QString             _sendHz;
    bool                _addNewItemCauseChooseChangeReceive;
    bool                _addNewItemCauseChooseChangeSend;
};

#endif
