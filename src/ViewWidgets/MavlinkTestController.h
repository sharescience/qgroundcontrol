/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef MavlinkTest_H
#define MavlinkTest_H

#include <QObject>
#include <QTimer>
#include <QAbstractListModel>
#include <QLocale>
#include <QElapsedTimer>

#include <memory>
#include <FactMetaData.h>

#include "UASInterface.h"
#include "AutoPilotPlugin.h"
#include "FactPanelController.h"

#include "fifo.h"

class  MultiVehicleManager;
class  UASInterface;
class  Vehicle;

//-----------------------------------------------------------------------------
class MavlinkTestController : public FactPanelController
{
    Q_OBJECT
public:
    /// @param standaloneUnitTesting true: being run without factPanel, false: normal operation, factPanel is required
    MavlinkTestController(bool standaloneUnitTesting = false);

private slots:
    void _setActiveVehicle  (Vehicle* vehicle);

private:
     MAVLinkProtocol*    _mavlink;

     /* Stream speed */
public:
    Q_PROPERTY(QStringList     streamRAW_SENSList         READ streamRAW_SENSList         NOTIFY streamRAW_SENSListChanged)
    Q_PROPERTY(QStringList     streamEXT_STATList         READ streamEXT_STATList         NOTIFY streamEXT_STATListChanged)
    Q_PROPERTY(QStringList     streamRC_CHANList          READ streamRC_CHANList          NOTIFY streamRC_CHANListChanged)
    Q_PROPERTY(QStringList     streamRAW_CTRLList         READ streamRAW_CTRLList         NOTIFY streamRAW_CTRLListChanged)
    Q_PROPERTY(QStringList     streamPOSITIONList         READ streamPOSITIONList         NOTIFY streamPOSITIONListChanged)
    Q_PROPERTY(QStringList     streamEXTRA1List           READ streamEXTRA1List           NOTIFY streamEXTRA1ListChanged)
    Q_PROPERTY(QStringList     streamEXTRA2List           READ streamEXTRA2List           NOTIFY streamEXTRA2ListChanged)
    Q_PROPERTY(QStringList     streamEXTRA3List           READ streamEXTRA3List           NOTIFY streamEXTRA3ListChanged)
    Q_PROPERTY(QStringList     streamPARAMSList           READ streamPARAMSList           NOTIFY streamPARAMSListChanged)
    Q_PROPERTY(QStringList     streamADSBList             READ streamADSBList             NOTIFY streamADSBListChanged)
    Q_PROPERTY(QStringList     streamPIDList              READ streamPIDList              NOTIFY streamPIDListChanged)

    QStringList     streamRAW_SENSList      () { return _listRAW_SENS; }
    QStringList     streamEXT_STATList      () { return _listEXT_STAT; }
    QStringList     streamRC_CHANList       () { return _listRC_CHAN; }
    QStringList     streamRAW_CTRLList      () { return _listRAW_CTRL; }
    QStringList     streamPOSITIONList      () { return _listPOSITION; }
    QStringList     streamEXTRA1List        () { return _listEXTRA1; }
    QStringList     streamEXTRA2List        () { return _listEXTRA2; }
    QStringList     streamEXTRA3List        () { return _listEXTRA3; }
    QStringList     streamPARAMSList        () { return _listPARAMS; }
    QStringList     streamADSBList          () { return _listADSB; }
    QStringList     streamPIDList           () { return _listPID; }

    Q_INVOKABLE void speedRAW_SENS(QString hz);
    Q_INVOKABLE void speedEXT_STAT(QString hz);
    Q_INVOKABLE void speedRC_CHAN(QString hz);
    Q_INVOKABLE void speedRAW_CTRL(QString hz);
    Q_INVOKABLE void speedPOSITION(QString hz);
    Q_INVOKABLE void speedEXTRA1(QString hz);
    Q_INVOKABLE void speedEXTRA2(QString hz);
    Q_INVOKABLE void speedEXTRA3(QString hz);
    Q_INVOKABLE void speedPARAMS(QString hz);
    Q_INVOKABLE void speedADSB(QString hz);
    Q_INVOKABLE void speedPID(QString hz);
    Q_INVOKABLE void clearStreamSpeed();
    Q_INVOKABLE void setSR0(bool b) { _sr0 = b; setBitMask(); }
    Q_INVOKABLE void setSR1(bool b) { _sr1 = b; setBitMask(); }
    Q_INVOKABLE void setSR2(bool b) { _sr2 = b; setBitMask(); }
    Q_INVOKABLE void setSR3(bool b) { _sr3 = b; setBitMask(); }
                void setBitMask();
                void changeStreamSpeed();

signals:
    void streamRAW_SENSListChanged();
    void streamEXT_STATListChanged();
    void streamRC_CHANListChanged();
    void streamRAW_CTRLListChanged();
    void streamPOSITIONListChanged();
    void streamEXTRA1ListChanged();
    void streamEXTRA2ListChanged();
    void streamEXTRA3ListChanged();
    void streamPARAMSListChanged();
    void streamADSBListChanged();
    void streamPIDListChanged();

private:
    QStringList         _listRAW_SENS;
    QStringList         _listEXT_STAT;
    QStringList         _listRC_CHAN;
    QStringList         _listRAW_CTRL;
    QStringList         _listPOSITION;
    QStringList         _listEXTRA1;
    QStringList         _listEXTRA2;
    QStringList         _listEXTRA3;
    QStringList         _listPARAMS;
    QStringList         _listADSB;
    QStringList         _listPID;
    int                 _speedRAW_SENS;
    int                 _speedEXT_STAT;
    int                 _speedRC_CHAN;
    int                 _speedRAW_CTRL;
    int                 _speedPOSITION;
    int                 _speedEXTRA1;
    int                 _speedEXTRA2;
    int                 _speedEXTRA3;
    int                 _speedPARAMS;
    int                 _speedADSB;
    int                 _speedPID;
    int                 _bitmask;
    bool                _sr0;
    bool                _sr1;
    bool                _sr2;
    bool                _sr3;

    /* Channels Override */
public:
    Q_PROPERTY( int  ch1RCV    READ ch1RCV   NOTIFY ch1RCVChanged )
    Q_PROPERTY( int  ch2RCV    READ ch2RCV   NOTIFY ch2RCVChanged )
    Q_PROPERTY( int  ch3RCV    READ ch3RCV   NOTIFY ch3RCVChanged )
    Q_PROPERTY( int  ch4RCV    READ ch4RCV   NOTIFY ch4RCVChanged )
    Q_PROPERTY( int  ch5RCV    READ ch5RCV   NOTIFY ch5RCVChanged )
    Q_PROPERTY( int  ch6RCV    READ ch6RCV   NOTIFY ch6RCVChanged )
    Q_PROPERTY( int  ch7RCV    READ ch7RCV   NOTIFY ch7RCVChanged )
    Q_PROPERTY( int  ch8RCV    READ ch8RCV   NOTIFY ch8RCVChanged )
    Q_PROPERTY( int  ch9RCV    READ ch9RCV   NOTIFY ch9RCVChanged )
    Q_PROPERTY( int  ch10RCV   READ ch10RCV  NOTIFY ch10RCVChanged )
    Q_PROPERTY( int  ch11RCV   READ ch11RCV  NOTIFY ch11RCVChanged )
    Q_PROPERTY( int  ch12RCV   READ ch12RCV  NOTIFY ch12RCVChanged )
    Q_PROPERTY( int  ch13RCV   READ ch13RCV  NOTIFY ch13RCVChanged )
    Q_PROPERTY( int  ch14RCV   READ ch14RCV  NOTIFY ch14RCVChanged )
    Q_PROPERTY( int  ch15RCV   READ ch15RCV  NOTIFY ch15RCVChanged )
    Q_PROPERTY( int  ch16RCV   READ ch16RCV  NOTIFY ch16RCVChanged )
    Q_PROPERTY( int  ch17RCV   READ ch17RCV  NOTIFY ch17RCVChanged )
    Q_PROPERTY( int  ch18RCV   READ ch18RCV  NOTIFY ch18RCVChanged )

                int      ch1RCV       () { return (int)_ch1RCV; }
                int      ch2RCV       () { return (int)_ch2RCV; }
                int      ch3RCV       () { return (int)_ch3RCV; }
                int      ch4RCV       () { return (int)_ch4RCV; }
                int      ch5RCV       () { return (int)_ch5RCV; }
                int      ch6RCV       () { return (int)_ch6RCV; }
                int      ch7RCV       () { return (int)_ch7RCV; }
                int      ch8RCV       () { return (int)_ch8RCV; }
                int      ch9RCV       () { return (int)_ch9RCV; }
                int      ch10RCV      () { return (int)_ch10RCV; }
                int      ch11RCV      () { return (int)_ch11RCV; }
                int      ch12RCV      () { return (int)_ch12RCV; }
                int      ch13RCV      () { return (int)_ch13RCV; }
                int      ch14RCV      () { return (int)_ch14RCV; }
                int      ch15RCV      () { return (int)_ch15RCV; }
                int      ch16RCV      () { return (int)_ch16RCV; }
                int      ch17RCV      () { return (int)_ch17RCV; }
                int      ch18RCV      () { return (int)_ch18RCV; }

    Q_INVOKABLE void     sendChannelsOverride();
    Q_INVOKABLE void     ch1Changed(int value) { _ch1Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch2Changed(int value) { _ch2Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch3Changed(int value) { _ch3Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch4Changed(int value) { _ch4Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch5Changed(int value) { _ch5Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch6Changed(int value) { _ch6Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch7Changed(int value) { _ch7Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch8Changed(int value) { _ch8Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch9Changed(int value) { _ch9Value = (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch10Changed(int value){ _ch10Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch11Changed(int value){ _ch11Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch12Changed(int value){ _ch12Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch13Changed(int value){ _ch13Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch14Changed(int value){ _ch14Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch15Changed(int value){ _ch15Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch16Changed(int value){ _ch16Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch17Changed(int value){ _ch17Value= (uint16_t)value; sendChannelsOverride(); }
    Q_INVOKABLE void     ch18Changed(int value){ _ch18Value= (uint16_t)value; sendChannelsOverride(); }

signals:
    void                 ch1RCVChanged();
    void                 ch2RCVChanged();
    void                 ch3RCVChanged();
    void                 ch4RCVChanged();
    void                 ch5RCVChanged();
    void                 ch6RCVChanged();
    void                 ch7RCVChanged();
    void                 ch8RCVChanged();
    void                 ch9RCVChanged();
    void                 ch10RCVChanged();
    void                 ch11RCVChanged();
    void                 ch12RCVChanged();
    void                 ch13RCVChanged();
    void                 ch14RCVChanged();
    void                 ch15RCVChanged();
    void                 ch16RCVChanged();
    void                 ch17RCVChanged();
    void                 ch18RCVChanged();
private slots:
    void                 _handleRCChannels(mavlink_message_t message);
    void                 _handleRCChannelsRaw(mavlink_message_t message);
private:
    uint16_t             _ch1RCV;
    uint16_t             _ch2RCV;
    uint16_t             _ch3RCV;
    uint16_t             _ch4RCV;
    uint16_t             _ch5RCV;
    uint16_t             _ch6RCV;
    uint16_t             _ch7RCV;
    uint16_t             _ch8RCV;
    uint16_t             _ch9RCV;
    uint16_t             _ch10RCV;
    uint16_t             _ch11RCV;
    uint16_t             _ch12RCV;
    uint16_t             _ch13RCV;
    uint16_t             _ch14RCV;
    uint16_t             _ch15RCV;
    uint16_t             _ch16RCV;
    uint16_t             _ch17RCV;
    uint16_t             _ch18RCV;
    uint16_t             _ch1Value;
    uint16_t             _ch2Value;
    uint16_t             _ch3Value;
    uint16_t             _ch4Value;
    uint16_t             _ch5Value;
    uint16_t             _ch6Value;
    uint16_t             _ch7Value;
    uint16_t             _ch8Value;
    uint16_t             _ch9Value;
    uint16_t             _ch10Value;
    uint16_t             _ch11Value;
    uint16_t             _ch12Value;
    uint16_t             _ch13Value;
    uint16_t             _ch14Value;
    uint16_t             _ch15Value;
    uint16_t             _ch16Value;
    uint16_t             _ch17Value;
    uint16_t             _ch18Value;

    /* Evaluating Communication Quality */
public:
    Q_PROPERTY(QStringList     elasticitySendHzList         READ elasticitySendHzList         NOTIFY elasticitySendHzListChanged)
    Q_PROPERTY(QString         elasticitySendCount          READ elasticitySendCount          NOTIFY elasticitySendCountChanged)
    Q_PROPERTY(QString         elasticityRcvCount           READ elasticityRcvCount           NOTIFY elasticityRcvCountChanged)
    Q_PROPERTY(QDateTime       elasticityStartTime          READ elasticityStartTime          NOTIFY elasticityStartTimeChanged)
    Q_PROPERTY(QString         elasticityPassTime           READ elasticityPassTime           NOTIFY elasticityPassTimeChanged)
    Q_PROPERTY(QString         elasticityLostCount          READ elasticityLostCount          NOTIFY elasticityLostCountChanged)
    Q_PROPERTY(QString         averageLatency               READ averageLatency               NOTIFY averageLatencyChanged)
    Q_PROPERTY(QString         minLatency                   READ minLatency                   NOTIFY minLatencyChanged)
    Q_PROPERTY(QString         maxLatency                   READ maxLatency                   NOTIFY maxLatencyChanged)
    Q_PROPERTY(QString         FIFODisplay                  READ FIFODisplay                  NOTIFY FIFODisplayChanged)

    QStringList      elasticitySendHzList    () { return _elasticitySendHzList; }
    QString          elasticitySendCount     () { return QString("%1").arg(_elasticitySendCount); }
    QString          elasticityRcvCount      () { return QString("%1").arg(_elasticityRcvCount); }
    QDateTime        elasticityStartTime     () { return _elasticityStartTime; }
    QString          elasticityPassTime      () { return _elasticityPassTime; }
    QString          elasticityLostCount     () { return QString("%1").arg(_elasticityLostCount); }
    QString          averageLatency          () { return QString("%1").arg(_averageLatency)+"ms"; }
    QString          FIFODisplay             ();
    QString          minLatency              () { return QString("%1").arg(_minLatency)+"ms"; }
    QString          maxLatency              () { return QString("%1").arg(_maxLatency)+"ms"; }

    Q_INVOKABLE void setFIFOLenChoose (QString length);
    Q_INVOKABLE void elasticitySendHzChoose (QString hz);

signals:
    void elasticitySendHzListChanged();
    void elasticitySendCountChanged();
    void elasticityRcvCountChanged();
    void elasticityStartTimeChanged();
    void elasticityPassTimeChanged();
    void elasticityLostCountChanged();
    void averageLatencyChanged();
    void minLatencyChanged();
    void maxLatencyChanged();
    void FIFODisplayChanged();

private slots:
    void _elasticityPackSend();
    void _handleElasticity(mavlink_message_t message);

private:
    void                _calcPassTime();

    QStringList         _elasticitySendHzList;
    int                 _elasticitySendHz;
    uint                _elasticitySendCount;
    uint                _elasticityRcvCount;
    uint                _elasticityLostCount;
    QDateTime           _elasticityStartTime;
    QStringList         _elasticitySendList;
    QStringList         _elasticityRcvList;
    QString             _elasticityPassTime;
    int                 _FIFOLenChoose;
    uint                _averageLatency;
    uint                _minLatency;
    uint                _maxLatency;
    FIFO<uint>          _latencyFIFO;
    QTimer              _timer;
    uint32_t            _data[8];

    /* Servos Output */
public:
    Q_PROPERTY( int  ch1OUT    READ ch1OUT   NOTIFY ch1OUTChanged )
    Q_PROPERTY( int  ch2OUT    READ ch2OUT   NOTIFY ch2OUTChanged )
    Q_PROPERTY( int  ch3OUT    READ ch3OUT   NOTIFY ch3OUTChanged )
    Q_PROPERTY( int  ch4OUT    READ ch4OUT   NOTIFY ch4OUTChanged )
    Q_PROPERTY( int  ch5OUT    READ ch5OUT   NOTIFY ch5OUTChanged )
    Q_PROPERTY( int  ch6OUT    READ ch6OUT   NOTIFY ch6OUTChanged )
    Q_PROPERTY( int  ch7OUT    READ ch7OUT   NOTIFY ch7OUTChanged )
    Q_PROPERTY( int  ch8OUT    READ ch8OUT   NOTIFY ch8OUTChanged )    
    Q_PROPERTY( int  ch9OUT    READ ch9OUT   NOTIFY ch9OUTChanged )
    Q_PROPERTY( int  ch10OUT   READ ch10OUT  NOTIFY ch10OUTChanged )
    Q_PROPERTY( int  ch11OUT   READ ch11OUT  NOTIFY ch11OUTChanged )
    Q_PROPERTY( int  ch12OUT   READ ch12OUT  NOTIFY ch12OUTChanged )
    Q_PROPERTY( int  ch13OUT   READ ch13OUT  NOTIFY ch13OUTChanged )
    Q_PROPERTY( int  ch14OUT   READ ch14OUT  NOTIFY ch14OUTChanged )
    Q_PROPERTY( int  ch15OUT   READ ch15OUT  NOTIFY ch15OUTChanged )
    Q_PROPERTY( int  ch16OUT   READ ch16OUT  NOTIFY ch16OUTChanged )

                int      ch1OUT       () { return (int)_ch1OUT; }
                int      ch2OUT       () { return (int)_ch2OUT; }
                int      ch3OUT       () { return (int)_ch3OUT; }
                int      ch4OUT       () { return (int)_ch4OUT; }
                int      ch5OUT       () { return (int)_ch5OUT; }
                int      ch6OUT       () { return (int)_ch6OUT; }
                int      ch7OUT       () { return (int)_ch7OUT; }
                int      ch8OUT       () { return (int)_ch8OUT; }               
                int      ch9OUT       () { return (int)_ch9OUT; }
                int      ch10OUT      () { return (int)_ch10OUT;}
                int      ch11OUT      () { return (int)_ch11OUT;}
                int      ch12OUT      () { return (int)_ch12OUT;}
                int      ch13OUT      () { return (int)_ch13OUT;}
                int      ch14OUT      () { return (int)_ch14OUT;}
                int      ch15OUT      () { return (int)_ch15OUT;}
                int      ch16OUT      () { return (int)_ch16OUT;}

signals:
    void                 ch1OUTChanged();
    void                 ch2OUTChanged();
    void                 ch3OUTChanged();
    void                 ch4OUTChanged();
    void                 ch5OUTChanged();
    void                 ch6OUTChanged();
    void                 ch7OUTChanged();
    void                 ch8OUTChanged();  
    void                 ch9OUTChanged();
    void                 ch10OUTChanged();
    void                 ch11OUTChanged();
    void                 ch12OUTChanged();
    void                 ch13OUTChanged();
    void                 ch14OUTChanged();
    void                 ch15OUTChanged();
    void                 ch16OUTChanged();

private slots:
    void                 _handleServoOutputRaw(mavlink_message_t message);
private:
    uint16_t             _ch1OUT;
    uint16_t             _ch2OUT;
    uint16_t             _ch3OUT;
    uint16_t             _ch4OUT;
    uint16_t             _ch5OUT;
    uint16_t             _ch6OUT;
    uint16_t             _ch7OUT;
    uint16_t             _ch8OUT;
    uint16_t             _ch9OUT;
    uint16_t             _ch10OUT;
    uint16_t             _ch11OUT;
    uint16_t             _ch12OUT;
    uint16_t             _ch13OUT;
    uint16_t             _ch14OUT;
    uint16_t             _ch15OUT;
    uint16_t             _ch16OUT;

    /* Servos Test */
public:
    Q_INVOKABLE void servosTestTimeSet(QString time);
    Q_INVOKABLE void servo1TestPercent(QString percent);
    Q_INVOKABLE void servo2TestPercent(QString percent);
    Q_INVOKABLE void servo3TestPercent(QString percent);
    Q_INVOKABLE void servoTailTestPercent(QString percent);
    Q_INVOKABLE void mainRotorTestPercent(QString percent);
    Q_INVOKABLE void startServoTest(int servo_seq);
    Q_INVOKABLE void stopServoTest(void);

private:
    int                 _servosTestTime;
    int                 _servo1TestPercent;
    int                 _servo2TestPercent;
    int                 _servo3TestPercent;
    int                 _servoTailTestPercent;
    int                 _mainRotorTestPercent;

};
#endif
