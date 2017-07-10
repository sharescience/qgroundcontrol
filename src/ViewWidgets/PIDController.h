/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef PID_H
#define PID_H

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
class PIDController : public FactPanelController
{
    Q_OBJECT
public:
    /// @param standaloneUnitTesting true: being run without factPanel, false: normal operation, factPanel is required
    PIDController(bool standaloneUnitTesting = false);

private slots:
    void _setActiveVehicle  (Vehicle* vehicle);

private:
     MAVLinkProtocol*    _mavlink;
     UASInterface*       _uas;

     /* PID Tuning */
public:
     Q_PROPERTY(QString     angleRollP              READ angleRollP              NOTIFY angleRollPChanged)
     Q_PROPERTY(QString     anglePitchP             READ anglePitchP             NOTIFY anglePitchPChanged)
     Q_PROPERTY(QString     angleYawP               READ angleYawP               NOTIFY angleYawPChanged)
     Q_PROPERTY(QString     angvelRollP             READ angvelRollP             NOTIFY angvelRollPChanged)
     Q_PROPERTY(QString     angvelPitchP            READ angvelPitchP            NOTIFY angvelPitchPChanged)
     Q_PROPERTY(QString     angvelYawP              READ angvelYawP              NOTIFY angvelYawPChanged)
     Q_PROPERTY(QString     angvelRollI             READ angvelRollI             NOTIFY angvelRollIChanged)
     Q_PROPERTY(QString     angvelPitchI            READ angvelPitchI            NOTIFY angvelPitchIChanged)
     Q_PROPERTY(QString     angvelYawI              READ angvelYawI              NOTIFY angvelYawIChanged)
     Q_PROPERTY(QString     angvelRollD             READ angvelRollD             NOTIFY angvelRollDChanged)
     Q_PROPERTY(QString     angvelPitchD            READ angvelPitchD            NOTIFY angvelPitchDChanged)
     Q_PROPERTY(QString     angvelYawD              READ angvelYawD              NOTIFY angvelYawDChanged)
     Q_PROPERTY(QString     angvelRollIMAX          READ angvelRollIMAX          NOTIFY angvelRollIMAXChanged)
     Q_PROPERTY(QString     angvelPitchIMAX         READ angvelPitchIMAX         NOTIFY angvelPitchIMAXChanged)
     Q_PROPERTY(QString     angvelYawIMAX           READ angvelYawIMAX           NOTIFY angvelYawIMAXChanged)
     Q_PROPERTY(QString     angvelRollFF            READ angvelRollFF            NOTIFY angvelRollFFChanged)
     Q_PROPERTY(QString     angvelPitchFF           READ angvelPitchFF           NOTIFY angvelPitchFFChanged)
     Q_PROPERTY(QString     angvelYawFF             READ angvelYawFF             NOTIFY angvelYawFFChanged)
     Q_PROPERTY(QString     angvelRollFILT          READ angvelRollFILT          NOTIFY angvelRollFILTChanged)
     Q_PROPERTY(QString     angvelPitchFILT         READ angvelPitchFILT         NOTIFY angvelPitchFILTChanged)
     Q_PROPERTY(QString     angvelYawFILT           READ angvelYawFILT           NOTIFY angvelYawFILTChanged)

     Q_PROPERTY(bool      feedforwardEnable       READ feedforwardEnable       NOTIFY feedforwardEnableChanged)
     Q_PROPERTY(bool      checkAngleRollP         READ checkAngleRollP         NOTIFY checkAngleRollPChanged)
     Q_PROPERTY(bool      checkAnglePitchP        READ checkAnglePitchP        NOTIFY checkAnglePitchPChanged)
     Q_PROPERTY(bool      checkAngleYawP          READ checkAngleYawP          NOTIFY checkAngleYawPChanged)
     Q_PROPERTY(bool      checkAngVelRollP        READ checkAngVelRollP        NOTIFY checkAngVelRollPChanged)
     Q_PROPERTY(bool      checkAngVelPitchP       READ checkAngVelPitchP       NOTIFY checkAngVelPitchPChanged)
     Q_PROPERTY(bool      checkAngVelYawP         READ checkAngVelYawP         NOTIFY checkAngVelYawPChanged)
     Q_PROPERTY(bool      checkAngVelRollI        READ checkAngVelRollI        NOTIFY checkAngVelRollIChanged)
     Q_PROPERTY(bool      checkAngVelPitchI       READ checkAngVelPitchI       NOTIFY checkAngVelPitchIChanged)
     Q_PROPERTY(bool      checkAngVelYawI         READ checkAngVelYawI         NOTIFY checkAngVelYawIChanged)
     Q_PROPERTY(bool      checkAngVelRollD        READ checkAngVelRollD        NOTIFY checkAngVelRollDChanged)
     Q_PROPERTY(bool      checkAngVelPitchD       READ checkAngVelPitchD       NOTIFY checkAngVelPitchDChanged)
     Q_PROPERTY(bool      checkAngVelYawD         READ checkAngVelYawD         NOTIFY checkAngVelYawDChanged)
     Q_PROPERTY(bool      checkAngVelRollIMAX     READ checkAngVelRollIMAX     NOTIFY checkAngVelRollIMAXChanged)
     Q_PROPERTY(bool      checkAngVelPitchIMAX    READ checkAngVelPitchIMAX    NOTIFY checkAngVelPitchIMAXChanged)
     Q_PROPERTY(bool      checkAngVelYawIMAX      READ checkAngVelYawIMAX      NOTIFY checkAngVelYawIMAXChanged)
     Q_PROPERTY(bool      checkAngVelRollFF       READ checkAngVelRollFF       NOTIFY checkAngVelRollFFChanged)
     Q_PROPERTY(bool      checkAngVelPitchFF      READ checkAngVelPitchFF      NOTIFY checkAngVelPitchFFChanged)
     Q_PROPERTY(bool      checkAngVelYawFF        READ checkAngVelYawFF        NOTIFY checkAngVelYawFFChanged)
     Q_PROPERTY(bool      checkAngVelRollFILT     READ checkAngVelRollFILT     NOTIFY checkAngVelRollFILTChanged)
     Q_PROPERTY(bool      checkAngVelPitchFILT    READ checkAngVelPitchFILT    NOTIFY checkAngVelPitchFILTChanged)
     Q_PROPERTY(bool      checkAngVelYawFILT      READ checkAngVelYawFILT      NOTIFY checkAngVelYawFILTChanged)

     Q_INVOKABLE void  getAllParameters();
     Q_INVOKABLE void  setAngleRollP(QString p);
     Q_INVOKABLE void  setAnglePitchP(QString p);
     Q_INVOKABLE void  setAngleYawP(QString p);
     Q_INVOKABLE void  setFeedforwardEnable(bool b);
     Q_INVOKABLE void  setAngVelRollP(QString p);
     Q_INVOKABLE void  setAngVelPitchP(QString p);
     Q_INVOKABLE void  setAngVelYawP(QString p);
     Q_INVOKABLE void  setAngVelRollI(QString i);
     Q_INVOKABLE void  setAngVelPitchI(QString i);
     Q_INVOKABLE void  setAngVelYawI(QString i);
     Q_INVOKABLE void  setAngVelRollD(QString d);
     Q_INVOKABLE void  setAngVelPitchD(QString d);
     Q_INVOKABLE void  setAngVelYawD(QString d);
     Q_INVOKABLE void  setAngVelRollIMAX(QString imax);
     Q_INVOKABLE void  setAngVelPitchIMAX(QString imax);
     Q_INVOKABLE void  setAngVelYawIMAX(QString imax);
     Q_INVOKABLE void  setAngVelRollFF(QString ff);
     Q_INVOKABLE void  setAngVelPitchFF(QString ff);
     Q_INVOKABLE void  setAngVelYawFF(QString ff);
     Q_INVOKABLE void  setAngVelRollFILT(QString filt);
     Q_INVOKABLE void  setAngVelPitchFILT(QString filt);
     Q_INVOKABLE void  setAngVelYawFILT(QString filt);

                 void  param_request(QString paramName);

                 QString angleRollP()       { return QString::number(_angleRollPRCV, 'f', 1); }
                 QString anglePitchP()      { return QString::number(_anglePitchPRCV, 'f', 1); }
                 QString angleYawP()        { return QString::number(_angleYawPRCV, 'f', 1); }
                 QString angvelRollP()      { return QString::number(_angvelRollPRCV, 'f', 3); }
                 QString angvelPitchP()     { return QString::number(_angvelPitchPRCV, 'f', 3); }
                 QString angvelYawP()       { return QString::number(_angvelYawPRCV, 'f', 3); }
                 QString angvelRollI()      { return QString::number(_angvelRollIRCV, 'f', 3); }
                 QString angvelPitchI()     { return QString::number(_angvelPitchIRCV, 'f', 3); }
                 QString angvelYawI()       { return QString::number(_angvelYawIRCV, 'f', 3); }
                 QString angvelRollD()      { return QString::number(_angvelRollDRCV, 'f', 4); }
                 QString angvelPitchD()     { return QString::number(_angvelPitchDRCV, 'f', 4); }
                 QString angvelYawD()       { return QString::number(_angvelYawDRCV, 'f', 4); }
                 QString angvelRollIMAX()   { return QString::number(_angvelRollIMAXRCV, 'f', 3); }
                 QString angvelPitchIMAX()  { return QString::number(_angvelPitchIMAXRCV, 'f', 3); }
                 QString angvelYawIMAX()    { return QString::number(_angvelYawIMAXRCV, 'f', 3); }
                 QString angvelRollFF()     { return QString::number(_angvelRollFFRCV, 'f', 4); }
                 QString angvelPitchFF()    { return QString::number(_angvelPitchFFRCV, 'f', 4); }
                 QString angvelYawFF()      { return QString::number(_angvelYawFFRCV, 'f', 4); }
                 QString angvelRollFILT()   { return QString::number(_angvelRollFILTRCV, 'f', 0); }
                 QString angvelPitchFILT()  { return QString::number(_angvelPitchFILTRCV, 'f', 0); }
                 QString angvelYawFILT()    { return QString::number(_angvelYawFILTRCV, 'f', 0); }

                 bool  feedforwardEnable()    {return _feedforwardEnable; }
                 bool  checkAngleRollP()      {return _checkAngleRollP; }
                 bool  checkAnglePitchP()     {return _checkAnglePitchP; }
                 bool  checkAngleYawP()       {return _checkAngleYawP; }
                 bool  checkAngVelRollP()     {return _checkAngVelRollP; }
                 bool  checkAngVelPitchP()    {return _checkAngVelPitchP; }
                 bool  checkAngVelYawP()      {return _checkAngVelYawP; }
                 bool  checkAngVelRollI()     {return _checkAngVelRollI; }
                 bool  checkAngVelPitchI()    {return _checkAngVelPitchI; }
                 bool  checkAngVelYawI()      {return _checkAngVelYawI; }
                 bool  checkAngVelRollD()     {return _checkAngVelRollD; }
                 bool  checkAngVelPitchD()    {return _checkAngVelPitchD; }
                 bool  checkAngVelYawD()      {return _checkAngVelYawD; }
                 bool  checkAngVelRollIMAX()  {return _checkAngVelRollIMAX; }
                 bool  checkAngVelPitchIMAX() {return _checkAngVelPitchIMAX; }
                 bool  checkAngVelYawIMAX()   {return _checkAngVelYawIMAX; }
                 bool  checkAngVelRollFF()    {return _checkAngVelRollFF; }
                 bool  checkAngVelPitchFF()   {return _checkAngVelPitchFF; }
                 bool  checkAngVelYawFF()     {return _checkAngVelYawFF; }
                 bool  checkAngVelRollFILT()  {return _checkAngVelRollFILT; }
                 bool  checkAngVelPitchFILT() {return _checkAngVelPitchFILT; }
                 bool  checkAngVelYawFILT()   {return _checkAngVelYawFILT; }

signals:
    void angleRollPChanged();
    void anglePitchPChanged();
    void angleYawPChanged();
    void angvelRollPChanged();
    void angvelPitchPChanged();
    void angvelYawPChanged();
    void angvelRollIChanged();
    void angvelPitchIChanged();
    void angvelYawIChanged();
    void angvelRollDChanged();
    void angvelPitchDChanged();
    void angvelYawDChanged();
    void angvelRollIMAXChanged();
    void angvelPitchIMAXChanged();
    void angvelYawIMAXChanged();
    void angvelRollFFChanged();
    void angvelPitchFFChanged();
    void angvelYawFFChanged();
    void angvelRollFILTChanged();
    void angvelPitchFILTChanged();
    void angvelYawFILTChanged();

    void feedforwardEnableChanged();
    void checkAngleRollPChanged();
    void checkAnglePitchPChanged();
    void checkAngleYawPChanged();
    void checkAngVelRollPChanged();
    void checkAngVelPitchPChanged();
    void checkAngVelYawPChanged();
    void checkAngVelRollIChanged();
    void checkAngVelPitchIChanged();
    void checkAngVelYawIChanged();
    void checkAngVelRollDChanged();
    void checkAngVelPitchDChanged();
    void checkAngVelYawDChanged();
    void checkAngVelRollIMAXChanged();
    void checkAngVelPitchIMAXChanged();
    void checkAngVelYawIMAXChanged();
    void checkAngVelRollFFChanged();
    void checkAngVelPitchFFChanged();
    void checkAngVelYawFFChanged();
    void checkAngVelRollFILTChanged();
    void checkAngVelPitchFILTChanged();
    void checkAngVelYawFILTChanged();

private slots:
    void                 _parameterUpdate(int uas, int component, QString parameterName, int parameterCount, int parameterId, int type, QVariant value);

private:
    void _writeParameterRaw(int componentId, const QString& paramName, const QVariant& value);
    MAV_PARAM_TYPE _factTypeToMavType(FactMetaData::ValueType_t factType);

    QString             _parameterName;
    QVariant            _value;
    int                 _uasRCV;
    int                 _component;
    int                 _parameterCount;
    int                 _parameterId;
    int                 _type;

    float               _angleRollP;
    float               _anglePitchP;
    float               _angleYawP;
    float               _angvelRollP;
    float               _angvelPitchP;
    float               _angvelYawP;
    float               _angvelRollI;
    float               _angvelPitchI;
    float               _angvelYawI;
    float               _angvelRollD;
    float               _angvelPitchD;
    float               _angvelYawD;
    float               _angvelRollIMAX;
    float               _angvelPitchIMAX;
    float               _angvelYawIMAX;
    float               _angvelRollFF;
    float               _angvelPitchFF;
    float               _angvelYawFF;
    float               _angvelRollFILT;
    float               _angvelPitchFILT;
    float               _angvelYawFILT;

    float               _angleRollPRCV;
    float               _anglePitchPRCV;
    float               _angleYawPRCV;
    float               _angvelRollPRCV;
    float               _angvelPitchPRCV;
    float               _angvelYawPRCV;
    float               _angvelRollIRCV;
    float               _angvelPitchIRCV;
    float               _angvelYawIRCV;
    float               _angvelRollDRCV;
    float               _angvelPitchDRCV;
    float               _angvelYawDRCV;
    float               _angvelRollIMAXRCV;
    float               _angvelPitchIMAXRCV;
    float               _angvelYawIMAXRCV;
    float               _angvelRollFFRCV;
    float               _angvelPitchFFRCV;
    float               _angvelYawFFRCV;
    float               _angvelRollFILTRCV;
    float               _angvelPitchFILTRCV;
    float               _angvelYawFILTRCV;

    bool                _feedforwardEnable;
    bool                _checkAngleRollP;
    bool                _checkAnglePitchP;
    bool                _checkAngleYawP;
    bool                _checkAngVelRollP;
    bool                _checkAngVelPitchP;
    bool                _checkAngVelYawP;
    bool                _checkAngVelRollI;
    bool                _checkAngVelPitchI;
    bool                _checkAngVelYawI;
    bool                _checkAngVelRollD;
    bool                _checkAngVelPitchD;
    bool                _checkAngVelYawD;
    bool                _checkAngVelRollIMAX;
    bool                _checkAngVelPitchIMAX;
    bool                _checkAngVelYawIMAX;
    bool                _checkAngVelRollFF;
    bool                _checkAngVelPitchFF;
    bool                _checkAngVelYawFF;
    bool                _checkAngVelRollFILT;
    bool                _checkAngVelPitchFILT;
    bool                _checkAngVelYawFILT;
};
#endif
