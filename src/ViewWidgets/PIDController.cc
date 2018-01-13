/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "PIDController.h"
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

//----------------------------------------------------------------------------------------
PIDController::PIDController(bool standaloneUnitTesting)
    : FactPanelController(standaloneUnitTesting)
    , _mavlink(qgcApp()->toolbox()->mavlinkProtocol())
    , _angleRollPRCV(      4.5f    )
    , _anglePitchPRCV(     4.5f    )
    , _angleYawPRCV(       4.5f    )
    , _angvelRollPRCV(     0.135f  )
    , _angvelPitchPRCV(    0.135f  )
    , _angvelYawPRCV(      0.18f   )
    , _angvelRollIRCV(     0.090f  )
    , _angvelPitchIRCV(    0.090f  )
    , _angvelYawIRCV(      0.018f  )
    , _angvelRollDRCV(     0.0036f )
    , _angvelPitchDRCV(    0.0036f )
    , _angvelYawDRCV(      0.0f    )
    , _angvelRollIMAXRCV(  0.5f    )
    , _angvelPitchIMAXRCV( 0.5f    )
    , _angvelYawIMAXRCV(   0.5f    )
    , _angvelRollFFRCV(    0.0f    )
    , _angvelPitchFFRCV(   0.0f    )
    , _angvelYawFFRCV(     0.0f    )
    , _angvelRollFILTRCV(  20.0f   )
    , _angvelPitchFILTRCV( 20.0f   )
    , _angvelYawFILTRCV(   2.5f    )
    , _feedforwardEnable(false)
    , _checkAngleRollP(false)
    , _checkAnglePitchP(false)
    , _checkAngleYawP(false)
    , _checkAngVelRollP(false)
    , _checkAngVelPitchP(false)
    , _checkAngVelYawP(false)
    , _checkAngVelRollI(false)
    , _checkAngVelPitchI(false)
    , _checkAngVelYawI(false)
    , _checkAngVelRollD(false)
    , _checkAngVelPitchD(false)
    , _checkAngVelYawD(false)
    , _checkAngVelRollIMAX(false)
    , _checkAngVelPitchIMAX(false)
    , _checkAngVelYawIMAX(false)
    , _checkAngVelRollFF(false)
    , _checkAngVelPitchFF(false)
    , _checkAngVelYawFF(false)
    , _checkAngVelRollFILT(false)
    , _checkAngVelPitchFILT(false)
    , _checkAngVelYawFILT(false)
{
    MultiVehicleManager *manager = qgcApp()->toolbox()->multiVehicleManager();
    connect(manager, &MultiVehicleManager::activeVehicleChanged, this, &PIDController::_setActiveVehicle);
    _setActiveVehicle(manager->activeVehicle());
}

//----------------------------------------------------------------------------------------
void
PIDController::_setActiveVehicle(Vehicle* vehicle)
{
    if(_uas) {
        if(vehicle){
            disconnect(vehicle->uas(), &UASInterface::parameterUpdate, this, &PIDController::_parameterUpdate);
        }
        _uas = NULL;
    }
    _vehicle = vehicle;
    if(_vehicle) {
        _uas = vehicle->uas();
        connect(vehicle->uas(), &UASInterface::parameterUpdate, this, &PIDController::_parameterUpdate);
    }
}

//----------------------------------------------------------------------------------------
/* PID Tuning */
void
PIDController::_parameterUpdate(int uas, int component, QString parameterName, int parameterCount, int parameterId, int type, QVariant value)
{
    _uasRCV         = uas;
    _component      = component;
    _parameterName  = parameterName;
    _parameterCount = parameterCount;
    _parameterId    = parameterId;
    _type           = type;
    _value          = value;

//    qDebug()<<"uas:"<<QString("%1").arg(_uasRCV);
//    qDebug()<<"component:"<<QString("%1").arg(_component);
//    qDebug()<<"parameterName:"<<_parameterName;
//    qDebug()<<"parameterCount:"<<QString("%1").arg(_parameterCount);
//    qDebug()<<"parameterId:"<<QString("%1").arg(_parameterId);
//    qDebug()<<"type:"<<QString("%1").arg(_type);
//    qDebug()<<"value:"<<_value;

    /********** Angle P **********/
    if(parameterName.compare("ATC_ANG_RLL_P") == 0){
        _angleRollPRCV = value.toFloat();
        emit angleRollPChanged();

        if(qAbs(_angleRollPRCV - _angleRollP) < 1.0e-6){
            _checkAngleRollP = true;
            emit checkAngleRollPChanged();
        }
    }

    if(parameterName.compare("ATC_ANG_PIT_P") == 0){
        _anglePitchPRCV = value.toFloat();
        emit anglePitchPChanged();

        if(qAbs(_anglePitchPRCV - _anglePitchP) < 1.0e-6){
            _checkAnglePitchP = true;
            emit checkAnglePitchPChanged();
        }
    }

    if(parameterName.compare("ATC_ANG_YAW_P") == 0){
        _angleYawPRCV = value.toFloat();
        emit angleYawPChanged();

        if(qAbs(_angleYawPRCV - _angleYawP) < 1.0e-6){
            _checkAngleYawP = true;
            emit checkAngleYawPChanged();
        }
    }

    /********** Feedforward Enable **********/
    if(parameterName.compare("ATC_RATE_FF_ENAB") == 0){
        _feedforwardEnable = value.toBool();
        emit feedforwardEnableChanged();
    }

    /********** AngVel P **********/
    if(parameterName.compare("ATC_RAT_RLL_P") == 0){
        _angvelRollPRCV = value.toFloat();
        emit angvelRollPChanged();

        if(qAbs(_angvelRollPRCV - _angvelRollP) < 1.0e-6){
            _checkAngVelRollP = true;
            emit checkAngVelRollPChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_P") == 0){
        _angvelPitchPRCV = value.toFloat();
        emit angvelPitchPChanged();

        if(qAbs(_angvelPitchPRCV - _angvelPitchP) < 1.0e-6){
            _checkAngVelPitchP = true;
            emit checkAngVelPitchPChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_P") == 0){
        _angvelYawPRCV = value.toFloat();
        emit angvelYawPChanged();

        if(qAbs(_angvelYawPRCV - _angvelYawP) < 1.0e-6){
            _checkAngVelYawP = true;
            emit checkAngVelYawPChanged();
        }
    }

    /********** AngVel I **********/
    if(parameterName.compare("ATC_RAT_RLL_I") == 0){
        _angvelRollIRCV = value.toFloat();
        emit angvelRollIChanged();

        if(qAbs(_angvelRollIRCV - _angvelRollI) < 1.0e-6){
            _checkAngVelRollI = true;
            emit checkAngVelRollIChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_I") == 0){
        _angvelPitchIRCV = value.toFloat();
        emit angvelPitchIChanged();

        if(qAbs(_angvelPitchIRCV - _angvelPitchI) < 1.0e-6){
            _checkAngVelPitchI = true;
            emit checkAngVelPitchIChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_I") == 0){
        _angvelYawIRCV = value.toFloat();
        emit angvelYawIChanged();

        if(qAbs(_angvelYawIRCV - _angvelYawI) < 1.0e-6){
            _checkAngVelYawI = true;
            emit checkAngVelYawIChanged();
        }
    }

    /********** AngVel D **********/
    if(parameterName.compare("ATC_RAT_RLL_D") == 0){
        _angvelRollDRCV = value.toFloat();
        emit angvelRollDChanged();

        if(qAbs(_angvelRollDRCV - _angvelRollD) < 1.0e-6){
            _checkAngVelRollD = true;
            emit checkAngVelRollDChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_D") == 0){
        _angvelPitchDRCV = value.toFloat();
        emit angvelPitchDChanged();

        if(qAbs(_angvelPitchDRCV - _angvelPitchD) < 1.0e-6){
            _checkAngVelPitchD = true;
            emit checkAngVelPitchDChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_D") == 0){
        _angvelYawDRCV = value.toFloat();
        emit angvelYawDChanged();

        if(qAbs(_angvelYawDRCV - _angvelYawD) < 1.0e-6){
            _checkAngVelYawD = true;
            emit checkAngVelYawDChanged();
        }
    }

    /********** AngVel IMAX **********/
    if(parameterName.compare("ATC_RAT_RLL_IMAX") == 0){
        _angvelRollIMAXRCV = value.toFloat();
        emit angvelRollIMAXChanged();

        if(qAbs(_angvelRollIMAXRCV - _angvelRollIMAX) < 1.0e-6){
            _checkAngVelRollIMAX = true;
            emit checkAngVelRollIMAXChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_IMAX") == 0){
        _angvelPitchIMAXRCV = value.toFloat();
        emit angvelPitchIMAXChanged();

        if(qAbs(_angvelPitchIMAXRCV - _angvelPitchIMAX) < 1.0e-6){
            _checkAngVelPitchIMAX = true;
            emit checkAngVelPitchIMAXChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_IMAX") == 0){
        _angvelYawIMAXRCV = value.toFloat();
        emit angvelYawIMAXChanged();

        if(qAbs(_angvelYawIMAXRCV - _angvelYawIMAX) < 1.0e-6){
            _checkAngVelYawIMAX = true;
            emit checkAngVelYawIMAXChanged();
        }
    }

    /********** AngVel FF **********/
    if(parameterName.compare("ATC_RAT_RLL_FF") == 0){
        _angvelRollFFRCV = value.toFloat();
        emit angvelRollFFChanged();

        if(qAbs(_angvelRollFFRCV - _angvelRollFF) < 1.0e-6){
            _checkAngVelRollFF = true;
            emit checkAngVelRollFFChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_FF") == 0){
        _angvelPitchFFRCV = value.toFloat();
        emit angvelPitchFFChanged();

        if(qAbs(_angvelPitchFFRCV - _angvelPitchFF) < 1.0e-6){
            _checkAngVelPitchFF = true;
            emit checkAngVelPitchFFChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_FF") == 0){
        _angvelYawFFRCV = value.toFloat();
        emit angvelYawFFChanged();

        if(qAbs(_angvelYawFFRCV - _angvelYawFF) < 1.0e-6){
            _checkAngVelYawFF = true;
            emit checkAngVelYawFFChanged();
        }
    }

    /********** AngVel FILT **********/
    if(parameterName.compare("ATC_RAT_RLL_FILT") == 0){
        _angvelRollFILTRCV = value.toFloat();
        emit angvelRollFILTChanged();

        if(qAbs(_angvelRollFILTRCV - _angvelRollFILT) < 1.0e-6){
            _checkAngVelRollFILT = true;
            emit checkAngVelRollFILTChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_PIT_FILT") == 0){
        _angvelPitchFILTRCV = value.toFloat();
        emit angvelPitchFILTChanged();

        if(qAbs(_angvelPitchFILTRCV - _angvelPitchFILT) < 1.0e-6){
            _checkAngVelPitchFILT = true;
            emit checkAngVelPitchFILTChanged();
        }
    }

    if(parameterName.compare("ATC_RAT_YAW_FILT") == 0){
        _angvelYawFILTRCV = value.toFloat();
        emit angvelYawFILTChanged();

        if(qAbs(_angvelYawFILTRCV - _angvelYawFILT) < 1.0e-6){
            _checkAngVelYawFILT = true;
            emit checkAngVelYawFILTChanged();
        }
    }
}

void
PIDController::param_request(QString paramName)
{
    mavlink_message_t msg;

    QByteArray ba = paramName.toLatin1();
    char *name = ba.data();

    if(_vehicle){
        mavlink_msg_param_request_read_pack(_mavlink->getSystemId(),
                                            _mavlink->getComponentId(),
                                            &msg,
                                            0,
                                            0,
                                            name,
                                            -1);
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }
}

MAV_PARAM_TYPE
PIDController::_factTypeToMavType(FactMetaData::ValueType_t factType)
{
    switch (factType) {
        case FactMetaData::valueTypeUint8:
            return MAV_PARAM_TYPE_UINT8;

        case FactMetaData::valueTypeInt8:
            return MAV_PARAM_TYPE_INT8;

        case FactMetaData::valueTypeUint16:
            return MAV_PARAM_TYPE_UINT16;

        case FactMetaData::valueTypeInt16:
            return MAV_PARAM_TYPE_INT16;

        case FactMetaData::valueTypeUint32:
            return MAV_PARAM_TYPE_UINT32;

        case FactMetaData::valueTypeFloat:
            return MAV_PARAM_TYPE_REAL32;

        default:
            qWarning() << "Unsupported fact type" << factType;
            // fall through

        case FactMetaData::valueTypeInt32:
            return MAV_PARAM_TYPE_INT32;
    }
}

void
PIDController::_writeParameterRaw(int componentId, const QString& paramName, const QVariant& value)
{
    mavlink_param_set_t     p;
    mavlink_param_union_t   union_value;

    memset(&p, 0, sizeof(p));

    if(_vehicle){
        FactMetaData::ValueType_t factType = _vehicle->parameterManager()->getParameter(componentId, paramName)->type();
        p.param_type = _factTypeToMavType(factType);

        switch (factType) {
            case FactMetaData::valueTypeUint8:
                union_value.param_uint8 = (uint8_t)value.toUInt();
                break;

            case FactMetaData::valueTypeInt8:
                union_value.param_int8 = (int8_t)value.toInt();
                break;

            case FactMetaData::valueTypeUint16:
                union_value.param_uint16 = (uint16_t)value.toUInt();
                break;

            case FactMetaData::valueTypeInt16:
                union_value.param_int16 = (int16_t)value.toInt();
                break;

            case FactMetaData::valueTypeUint32:
                union_value.param_uint32 = (uint32_t)value.toUInt();
                break;

            case FactMetaData::valueTypeFloat:
                union_value.param_float = value.toFloat();
                break;

            default:
                qCritical() << "Unsupported fact type" << factType;
                // fall through

            case FactMetaData::valueTypeInt32:
                union_value.param_int32 = (int32_t)value.toInt();
                break;
        }

        p.param_value = union_value.param_float;
        p.target_system = (uint8_t)_vehicle->id();
        p.target_component = (uint8_t)_mavlink->getComponentId();

        strncpy(p.param_id, paramName.toStdString().c_str(), sizeof(p.param_id));

        mavlink_message_t msg;
        mavlink_msg_param_set_encode_chan(_mavlink->getSystemId(),
                                          _mavlink->getComponentId(),
                                          _vehicle->priorityLink()->mavlinkChannel(),
                                          &msg,
                                          &p);
        _vehicle->sendMessageOnLink(_vehicle->priorityLink(), msg);
    }
}

void
PIDController::getAllParameters(){
    param_request("ATC_ANG_RLL_P");
    param_request("ATC_ANG_PIT_P");
    param_request("ATC_ANG_YAW_P");
    param_request("ATC_RATE_FF_ENAB");
    param_request("ATC_RAT_RLL_P");
    param_request("ATC_RAT_PIT_P");
    param_request("ATC_RAT_YAW_P");
    param_request("ATC_RAT_RLL_I");
    param_request("ATC_RAT_PIT_I");
    param_request("ATC_RAT_YAW_I");
    param_request("ATC_RAT_RLL_D");
    param_request("ATC_RAT_PIT_D");
    param_request("ATC_RAT_YAW_D");
    param_request("ATC_RAT_RLL_IMAX");
    param_request("ATC_RAT_PIT_IMAX");
    param_request("ATC_RAT_YAW_IMAX");
    param_request("ATC_RAT_RLL_FF");
    param_request("ATC_RAT_PIT_FF");
    param_request("ATC_RAT_YAW_FF");
    param_request("ATC_RAT_RLL_FILT");
    param_request("ATC_RAT_PIT_FILT");
    param_request("ATC_RAT_YAW_FILT");

    _checkAngleRollP = true;
    emit checkAngleRollPChanged();
    _checkAnglePitchP = true;
    emit checkAnglePitchPChanged();
    _checkAngleYawP = true;
    emit checkAngleYawPChanged();
    _checkAngVelRollP = true;
    emit checkAngVelRollPChanged();
    _checkAngVelPitchP = true;
    emit checkAngVelPitchPChanged();
    _checkAngVelYawP = true;
    emit checkAngVelYawPChanged();
    _checkAngVelRollI = true;
    emit checkAngVelRollIChanged();
    _checkAngVelPitchI = true;
    emit checkAngVelPitchIChanged();
    _checkAngVelYawI = true;
    emit checkAngVelYawIChanged();
    _checkAngVelRollD = true;
    emit checkAngVelRollDChanged();
    _checkAngVelPitchD = true;
    emit checkAngVelPitchDChanged();
    _checkAngVelYawD = true;
    emit checkAngVelYawDChanged();
    _checkAngVelRollIMAX = true;
    emit checkAngVelRollIMAXChanged();
    _checkAngVelPitchIMAX = true;
    emit checkAngVelPitchIMAXChanged();
    _checkAngVelYawIMAX = true;
    emit checkAngVelYawIMAXChanged();
    _checkAngVelRollFF = true;
    emit checkAngVelRollFFChanged();
    _checkAngVelPitchFF = true;
    emit checkAngVelPitchFFChanged();
    _checkAngVelYawFF = true;
    emit checkAngVelYawFFChanged();
    _checkAngVelRollFILT = true;
    emit checkAngVelRollFILTChanged();
    _checkAngVelPitchFILT = true;
    emit checkAngVelPitchFILTChanged();
    _checkAngVelYawFILT = true;
    emit checkAngVelYawFILTChanged();
}

//---------- Angle P ----------
void
PIDController::setAngleRollP(QString p){
    QString paramName("ATC_ANG_RLL_P");

    _angleRollP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angleRollP));
    param_request(paramName);

    _checkAngleRollP = false;
    emit checkAngleRollPChanged();
}

void
PIDController::setAnglePitchP(QString p){
    QString paramName("ATC_ANG_PIT_P");

    _anglePitchP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_anglePitchP));
    param_request(paramName);

    _checkAnglePitchP = false;
    emit checkAnglePitchPChanged();
}

void
PIDController::setAngleYawP(QString p){
    QString paramName("ATC_ANG_YAW_P");

    _angleYawP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angleYawP));
    param_request(paramName);

    _checkAngleYawP = false;
    emit checkAngleYawPChanged();
}

//---------- Feedforward Enable ----------
void
PIDController::setFeedforwardEnable(bool b){
    QString paramName("ATC_RATE_FF_ENAB");

    _writeParameterRaw(_component, paramName, QVariant(b));
    param_request(paramName);
}

//---------- AngVel P ----------
void
PIDController::setAngVelRollP(QString p){
    QString paramName("ATC_RAT_RLL_P");

    _angvelRollP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollP));
    param_request(paramName);

    _checkAngVelRollP = false;
    emit checkAngVelRollPChanged();
}

void
PIDController::setAngVelPitchP(QString p){
    QString paramName("ATC_RAT_PIT_P");

    _angvelPitchP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchP));
    param_request(paramName);

    _checkAngVelPitchP = false;
    emit checkAngVelPitchPChanged();
}

void
PIDController::setAngVelYawP(QString p){
    QString paramName("ATC_RAT_YAW_P");

    _angvelYawP = p.left(p.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawP));
    param_request(paramName);

    _checkAngVelYawP = false;
    emit checkAngVelYawPChanged();
}

//---------- AngVel I ----------
void
PIDController::setAngVelRollI(QString i){
    QString paramName("ATC_RAT_RLL_I");

    _angvelRollI = i.left(i.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollI));
    param_request(paramName);

    _checkAngVelRollI = false;
    emit checkAngVelRollIChanged();
}

void
PIDController::setAngVelPitchI(QString i){
    QString paramName("ATC_RAT_PIT_I");

    _angvelPitchI = i.left(i.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchI));
    param_request(paramName);

    _checkAngVelPitchI = false;
    emit checkAngVelPitchIChanged();
}

void
PIDController::setAngVelYawI(QString i){
    QString paramName("ATC_RAT_YAW_I");

    _angvelYawI = i.left(i.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawI));
    param_request(paramName);

    _checkAngVelYawI = false;
    emit checkAngVelYawIChanged();
}

//---------- AngVel D ----------
void
PIDController::setAngVelRollD(QString d){
    QString paramName("ATC_RAT_RLL_D");

    _angvelRollD = d.left(d.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollD));
    param_request(paramName);

    _checkAngVelRollD = false;
    emit checkAngVelRollDChanged();
}

void
PIDController::setAngVelPitchD(QString d){
    QString paramName("ATC_RAT_PIT_D");

    _angvelPitchD = d.left(d.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchD));
    param_request(paramName);

    _checkAngVelPitchD = false;
    emit checkAngVelPitchDChanged();
}

void
PIDController::setAngVelYawD(QString d){
    QString paramName("ATC_RAT_YAW_D");

    _angvelYawD = d.left(d.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawD));
    param_request(paramName);

    _checkAngVelYawD = false;
    emit checkAngVelYawDChanged();
}

//---------- AngVel IMAX ----------
void
PIDController::setAngVelRollIMAX(QString imax){
    QString paramName("ATC_RAT_RLL_IMAX");

    _angvelRollIMAX = imax.left(imax.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollIMAX));
    param_request(paramName);

    _checkAngVelRollIMAX = false;
    emit checkAngVelRollIMAXChanged();
}

void
PIDController::setAngVelPitchIMAX(QString imax){
    QString paramName("ATC_RAT_PIT_IMAX");

    _angvelPitchIMAX = imax.left(imax.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchIMAX));
    param_request(paramName);

    _checkAngVelPitchIMAX = false;
    emit checkAngVelPitchIMAXChanged();
}

void
PIDController::setAngVelYawIMAX(QString imax){
    QString paramName("ATC_RAT_YAW_IMAX");

    _angvelYawIMAX = imax.left(imax.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawIMAX));
    param_request(paramName);

    _checkAngVelYawIMAX = false;
    emit checkAngVelYawIMAXChanged();
}

//---------- AngVel FF ----------
void
PIDController::setAngVelRollFF(QString ff){
    QString paramName("ATC_RAT_RLL_FF");

    _angvelRollFF = ff.left(ff.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollFF));
    param_request(paramName);

    _checkAngVelRollFF = false;
    emit checkAngVelRollFFChanged();
}

void
PIDController::setAngVelPitchFF(QString ff){
    QString paramName("ATC_RAT_PIT_FF");

    _angvelPitchFF = ff.left(ff.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchFF));
    param_request(paramName);

    _checkAngVelPitchFF = false;
    emit checkAngVelPitchFFChanged();
}

void
PIDController::setAngVelYawFF(QString ff){
    QString paramName("ATC_RAT_YAW_FF");

    _angvelYawFF = ff.left(ff.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawFF));
    param_request(paramName);

    _checkAngVelYawFF = false;
    emit checkAngVelYawFFChanged();
}

//---------- AngVel FILT ----------
void
PIDController::setAngVelRollFILT(QString filt){
    QString paramName("ATC_RAT_RLL_FILT");

    _angvelRollFILT = filt.left(filt.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelRollFILT));
    param_request(paramName);

    _checkAngVelRollFILT = false;
    emit checkAngVelRollFILTChanged();
}

void
PIDController::setAngVelPitchFILT(QString filt){
    QString paramName("ATC_RAT_PIT_FILT");

    _angvelPitchFILT = filt.left(filt.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelPitchFILT));
    param_request(paramName);

    _checkAngVelPitchFILT = false;
    emit checkAngVelPitchFILTChanged();
}

void
PIDController::setAngVelYawFILT(QString filt){
    QString paramName("ATC_RAT_YAW_FILT");

    _angvelYawFILT = filt.left(filt.length()).toFloat();

    _writeParameterRaw(_component, paramName, QVariant(_angvelYawFILT));
    param_request(paramName);

    _checkAngVelYawFILT = false;
    emit checkAngVelYawFILTChanged();
}
