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

    Q_PROPERTY(QString         rcvMessageHex    READ rcvMessageHex    NOTIFY rcvMessageHexChanged)
    Q_PROPERTY(QString         sendMessageHex   READ sendMessageHex   NOTIFY sendMessageHexChanged)
    Q_PROPERTY(QStringList     idListReceive    READ idListReceive    NOTIFY idListReceiveChanged)
    Q_PROPERTY(QStringList     idListSend       READ idListSend       NOTIFY idListSendChanged)
    Q_PROPERTY(QString         rcvHz            READ rcvHz            NOTIFY rcvHzChanged)
    Q_PROPERTY(QString         sendHz           READ sendHz           NOTIFY sendHzChanged)
    Q_PROPERTY(int             sendVersion      READ sendVersion      NOTIFY sendVersionChanged)
    Q_PROPERTY(int             rcvVersion       READ rcvVersion       NOTIFY rcvVersionChanged)
    Q_PROPERTY(QString         sendMagic        READ sendMagic        NOTIFY sendMagicChanged)
    Q_PROPERTY(QString         sendLen          READ sendLen          NOTIFY sendLenChanged)
    Q_PROPERTY(QString         sendSeq          READ sendSeq          NOTIFY sendSeqChanged)
    Q_PROPERTY(QString         sendSysid        READ sendSysid        NOTIFY sendSysidChanged)
    Q_PROPERTY(QString         sendCompid       READ sendCompid       NOTIFY sendCompidChanged)
    Q_PROPERTY(QString         sendMsgid        READ sendMsgid        NOTIFY sendMsgidChanged)
    Q_PROPERTY(QString         sendIncompatFlag READ sendIncompatFlag NOTIFY sendIncompatFlagChanged)
    Q_PROPERTY(QString         sendCompatFlag   READ sendCompatFlag   NOTIFY sendCompatFlagChanged)
    Q_PROPERTY(QString         sendPayload      READ sendPayload      NOTIFY sendPayloadChanged)
    Q_PROPERTY(QString         sendCkA          READ sendCkA          NOTIFY sendCkAChanged)
    Q_PROPERTY(QString         sendCkB          READ sendCkB          NOTIFY sendCkBChanged)
    Q_PROPERTY(QString         sendSignature    READ sendSignature    NOTIFY sendSignatureChanged)
    Q_PROPERTY(QString         rcvMagic         READ rcvMagic         NOTIFY rcvMagicChanged)
    Q_PROPERTY(QString         rcvLen           READ rcvLen           NOTIFY rcvLenChanged)
    Q_PROPERTY(QString         rcvSeq           READ rcvSeq           NOTIFY rcvSeqChanged)
    Q_PROPERTY(QString         rcvSysid         READ rcvSysid         NOTIFY rcvSysidChanged)
    Q_PROPERTY(QString         rcvCompid        READ rcvCompid        NOTIFY rcvCompidChanged)
    Q_PROPERTY(QString         rcvMsgid         READ rcvMsgid         NOTIFY rcvMsgidChanged)
    Q_PROPERTY(QString         rcvIncompatFlag  READ rcvIncompatFlag  NOTIFY rcvIncompatFlagChanged)
    Q_PROPERTY(QString         rcvCompatFlag    READ rcvCompatFlag    NOTIFY rcvCompatFlagChanged)
    Q_PROPERTY(QString         rcvPayload       READ rcvPayload       NOTIFY rcvPayloadChanged)
    Q_PROPERTY(QString         rcvCkA           READ rcvCkA           NOTIFY rcvCkAChanged)
    Q_PROPERTY(QString         rcvCkB           READ rcvCkB           NOTIFY rcvCkBChanged)
    Q_PROPERTY(QString         rcvSignature     READ rcvSignature     NOTIFY rcvSignatureChanged)
    Q_PROPERTY(int             msgSendIndex     READ msgSendIndex     NOTIFY msgSendIndexChanged)
    Q_PROPERTY(int             msgReceiveIndex  READ msgReceiveIndex  NOTIFY msgReceiveIndexChanged)

    QString         rcvMessageHex           () { return _rcvMessageHex; }
    QString         sendMessageHex          () { return _sendMessageHex; }
    QStringList     idListSend              () { return _idListSend; }
    QStringList     idListReceive           () { return _idListReceive; }
    QString         rcvMavIDChoose          () { return _rcvMavIDChoose; }
    QString         rcvHz                   () { return _rcvHz; }
    QString         sendHz                  () { return _sendHz; }
    int             rcvVersion              () { return _rcvVersion; }
    int             sendVersion             () { return _sendVersion; }
    QString         sendMagic               () { return _sendMagic; }
    QString         sendLen                 () { return _sendLen; }
    QString         sendSeq                 () { return _sendSeq; }
    QString         sendSysid               () { return _sendSysid; }
    QString         sendCompid              () { return _sendCompid; }
    QString         sendMsgid               () { return _sendMsgid; }
    QString         sendIncompatFlag        () { return _sendIncompatFlag; }
    QString         sendCompatFlag          () { return _sendCompatFlag; }
    QString         sendPayload             () { return _sendPayload; }
    QString         sendCkA                 () { return _sendCkA; }
    QString         sendCkB                 () { return _sendCkB; }
    QString         sendSignature           () { return _sendSignature; }
    QString         rcvMagic                () { return _rcvMagic; }
    QString         rcvLen                  () { return _rcvLen; }
    QString         rcvSeq                  () { return _rcvSeq; }
    QString         rcvSysid                () { return _rcvSysid; }
    QString         rcvCompid               () { return _rcvCompid; }
    QString         rcvMsgid                () { return _rcvMsgid; }
    QString         rcvIncompatFlag         () { return _rcvIncompatFlag; }
    QString         rcvCompatFlag           () { return _rcvCompatFlag; }
    QString         rcvPayload              () { return _rcvPayload; }
    QString         rcvCkA                  () { return _rcvCkA; }
    QString         rcvCkB                  () { return _rcvCkB; }
    QString         rcvSignature            () { return _rcvSignature; }
    int             msgSendIndex            () { return _msgSendIndex; }
    int             msgReceiveIndex         () { return _msgReceiveIndex; }

    Q_INVOKABLE void setrcvMavIDChoose (QString id);
    Q_INVOKABLE void setsendMavIDChoose(QString id);
    Q_INVOKABLE void setsendAnalyzeEnable (bool enable) { _sendAnalyzeEnable = enable; }
    Q_INVOKABLE void setrcvAnalyzeEnable  (bool enable) { _rcvAnalyzeEnable  = enable; }
    Q_INVOKABLE void setsendCacheEnable   (bool enable) { _sendCacheEnable   = enable; }
    Q_INVOKABLE void setrcvCacheEnable    (bool enable) { _rcvCacheEnable    = enable; }
    Q_INVOKABLE void setmsgSendIndex      (int index);
    Q_INVOKABLE void setmsgReceiveIndex   (int index);

    void analyzeMessage(int*     version,
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
                        QString& signature);
    void analyzeRCV();
    void analyzeSend();
    void updateReceiveList(bool enable, QString msg);
    void updateSendList(bool enable, QString msg);
    void timer_tick();
signals:
    void rcvMessageHexChanged   ();
    void sendMessageHexChanged  ();
    void idListReceiveChanged   ();
    void idListSendChanged      ();
    void rcvHzChanged           ();
    void sendHzChanged          ();
    void sendVersionChanged     ();
    void rcvVersionChanged      ();
    void sendMagicChanged       ();
    void sendLenChanged         ();
    void sendSeqChanged         ();
    void sendSysidChanged       ();
    void sendCompidChanged      ();
    void sendMsgidChanged       ();
    void sendIncompatFlagChanged();
    void sendCompatFlagChanged  ();
    void sendPayloadChanged     ();
    void sendCkAChanged         ();
    void sendCkBChanged         ();
    void sendSignatureChanged   ();
    void rcvMagicChanged        ();
    void rcvLenChanged          ();
    void rcvSeqChanged          ();
    void rcvSysidChanged        ();
    void rcvCompidChanged       ();
    void rcvMsgidChanged        ();
    void rcvIncompatFlagChanged ();
    void rcvCompatFlagChanged   ();
    void rcvPayloadChanged      ();
    void rcvCkAChanged          ();
    void rcvCkBChanged          ();
    void rcvSignatureChanged    ();
    void msgSendIndexChanged    ();
    void msgReceiveIndexChanged ();

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
    bool                _sendAnalyzeEnable;
    bool                _rcvAnalyzeEnable;
    int                 _sendVersion;
    int                 _rcvVersion;
    QString             _sendMagic;
    QString             _sendLen;
    QString             _sendSeq;
    QString             _sendSysid;
    QString             _sendCompid;
    QString             _sendMsgid;
    QString             _sendIncompatFlag;
    QString             _sendCompatFlag;
    QString             _sendPayload;
    QString             _sendCkA;
    QString             _sendCkB;
    QString             _sendSignature;
    QString             _rcvMagic;
    QString             _rcvLen;
    QString             _rcvSeq;
    QString             _rcvSysid;
    QString             _rcvCompid;
    QString             _rcvMsgid;
    QString             _rcvIncompatFlag;
    QString             _rcvCompatFlag;
    QString             _rcvPayload;
    QString             _rcvCkA;
    QString             _rcvCkB;
    QString             _rcvSignature;

    QStringList         _msgSendList;
    QStringList         _msgReceiveList;
    bool                _sendCacheEnable;
    bool                _rcvCacheEnable;
    int                 _msgSendIndex;
    int                 _msgReceiveIndex;
    QTimer              _timer;
};

#endif
