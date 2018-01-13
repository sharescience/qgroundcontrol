/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtQuick                  2.5
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.2
import QtQuick.Dialogs          1.2
import QtQuick.Window           2.2

import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0

QGCView {
    viewPanel:  panel

    id: mavlinkTest
    width: Screen.desktopAvailableWidth   * 1 / 5
    height: Screen.desktopAvailableHeight * 1 / 2

    property real _margins: Screen.desktopAvailableHeight * 0.5 / 100
    property real _Width  : Screen.desktopAvailableWidth  * 4.5 / 100
    property real _Height : Screen.desktopAvailableHeight * 2.75 / 100

    property real  _max_pwm: 1900
    property real  _min_pwm: 1100
    property real  _mid_pwm: 1500

    property bool _Channels_Override_CH1_Ebale  :    false
    property bool _Channels_Override_CH2_Ebale  :    false
    property bool _Channels_Override_CH3_Ebale  :    false
    property bool _Channels_Override_CH4_Ebale  :    false
    property bool _Channels_Override_CH5_Ebale  :    false
    property bool _Channels_Override_CH6_Ebale  :    false
    property bool _Channels_Override_CH7_Ebale  :    false
    property bool _Channels_Override_CH8_Ebale  :    false
    property bool _Channels_Override_CH9_Ebale  :    false
    property bool _Channels_Override_CH10_Ebale :    false
    property bool _Channels_Override_CH11_Ebale :    false
    property bool _Channels_Override_CH12_Ebale :    false
    property bool _Channels_Override_CH13_Ebale :    false
    property bool _Channels_Override_CH14_Ebale :    false
    property bool _Channels_Override_CH15_Ebale :    false
    property bool _Channels_Override_CH16_Ebale :    false
    property bool _Channels_Override_CH17_Ebale :    false
    property bool _Channels_Override_CH18_Ebale :    false

    PIDController {
        id:         controller
        factPanel:  panel
    }

    AutoResize{
       id:globalResize
    }

    QGCPalette { id: palette; colorGroupEnabled: enabled }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent
        anchors.margins:    _margins

        /* PID Tuning */
        GroupBox {
            anchors.top       : parent.top
            anchors.topMargin : _margins
            anchors.left      : parent.left
            anchors.leftMargin: _margins

            id: _groupboxAttitudeControl
            title: qsTr("Attitude Control")

            CheckBox{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxTuningEnable
                width : _Width
                height: _Height
                text:qsTr("Tuning Enable")
                onClicked: {
                    if(_checkboxTuningEnable.checked === true){
                        controller.getAllParameters()
                    }
                }
            }

            GroupBox {
                anchors.left          : _checkboxTuningEnable.left
                anchors.top           : _checkboxTuningEnable.bottom
                anchors.topMargin     : _margins

                id: _groupboxAngleCircuit
                title: qsTr("Angle Circuit")

                Label{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _labelSpace01
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("")
                }

                Label{
                    anchors.left          : _labelSpace01.left
                    anchors.top           : _labelSpace01.bottom

                    id    : _labelAngleP
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("P")
                }

                Label{
                    anchors.top           : _labelSpace01.top
                    anchors.left          : _labelSpace01.right
                    anchors.leftMargin    : _margins

                    id    : _labelAngleRoll
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Roll")
                }

                Label{
                    anchors.top           : _labelSpace01.top
                    anchors.left          : _labelAngleRoll.right
                    anchors.leftMargin    : _margins

                    id    : _labelAnglePitch
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Pitch")
                }

                Label{
                    anchors.top           : _labelSpace01.top
                    anchors.left          : _labelAnglePitch.right
                    anchors.leftMargin    : _margins

                    id    : _labelAngleYaw
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Yaw")
                }

                TextField{
                    anchors.verticalCenter: _labelAngleP.verticalCenter
                    anchors.left          : _labelAngleP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngleRollP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("4.5")
                    text: controller.angleRollP
                    textColor: {
                        if(controller.checkAngleRollP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngleRollP(_textfieldAngleRollP.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngleRollP.verticalCenter
                    anchors.left          : _textfieldAngleRollP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAnglePitchP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("4.5")
                    text: controller.anglePitchP
                    textColor: {
                        if(controller.checkAnglePitchP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAnglePitchP(_textfieldAnglePitchP.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAnglePitchP.verticalCenter
                    anchors.left          : _textfieldAnglePitchP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngleYawP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("4.5")
                    text: controller.angleYawP
                    textColor: {
                        if(controller.checkAngleYawP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngleYawP(_textfieldAngleYawP.text)
                        }
                    }
                }
            }

            GroupBox {
                anchors.left          : _groupboxAngleCircuit.left
                anchors.top           : _groupboxAngleCircuit.bottom
                anchors.topMargin     : _margins

                id: _groupboxAngVelCircuit
                title: qsTr("Angular Velocity Circuit")

                CheckBox{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id: _checkboxFFEnable
                    width : _Width
                    height: _Height
                    text: qsTr("Feedforward Enable")
                    checkedState:{
                        if(controller.feedforwardEnable){
                            2 // Qt::Checked
                        }else{
                            0 // Qt::Unchecked
                        }
                    }
                    onClicked: {
                        if(_checkboxTuningEnable.checked === true){
                            if(_checkboxFFEnable.checked === true){
                                controller.setFeedforwardEnable(true)
                            }else{
                                controller.setFeedforwardEnable(false)
                            }
                        }
                    }
                }

                Label{
                    anchors.left          : _checkboxFFEnable.left
                    anchors.top           : _checkboxFFEnable.bottom

                    id    : _labelSpace02
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("")
                }

                Label{
                    anchors.left          : _labelSpace02.left
                    anchors.top           : _labelSpace02.bottom

                    id    : _labelAngVelP
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("P")
                }

                Label{
                    anchors.top           : _labelSpace02.top
                    anchors.left          : _labelSpace02.right
                    anchors.leftMargin    : _margins

                    id    : _labelAngVelRoll
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Roll")
                }

                Label{
                    anchors.top           : _labelSpace02.top
                    anchors.left          : _labelAngVelRoll.right
                    anchors.leftMargin    : _margins

                    id    : _labelAngVelPitch
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Pitch")
                }

                Label{
                    anchors.top           : _labelSpace02.top
                    anchors.left          : _labelAngVelPitch.right
                    anchors.leftMargin    : _margins

                    id    : _labelAngVelYaw
                    width : _Width
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Yaw")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelP.verticalCenter
                    anchors.left          : _labelAngVelP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollP
                    textColor: {
                        if(controller.checkAngVelRollP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollP(_textfieldAngVelRollP.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollP.verticalCenter
                    anchors.left          : _textfieldAngVelRollP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchP
                    textColor: {
                        if(controller.checkAngVelPitchP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchP(_textfieldAngVelPitchP.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchP.verticalCenter
                    anchors.left          : _textfieldAngVelPitchP.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawP
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawP
                    textColor: {
                        if(controller.checkAngVelYawP){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawP(_textfieldAngVelYawP.text)
                        }
                    }
                }

                Label{
                    anchors.left          : _labelAngVelP.left
                    anchors.top           : _labelAngVelP.bottom
                    anchors.topMargin     : _margins

                    id    : _labelAngVelI
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("I")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelI.verticalCenter
                    anchors.left          : _labelAngVelI.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollI
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollI
                    textColor: {
                        if(controller.checkAngVelRollI){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollI(_textfieldAngVelRollI.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollI.verticalCenter
                    anchors.left          : _textfieldAngVelRollI.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchI
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchI
                    textColor: {
                        if(controller.checkAngVelPitchI){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchI(_textfieldAngVelPitchI.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchI.verticalCenter
                    anchors.left          : _textfieldAngVelPitchI.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawI
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawI
                    textColor: {
                        if(controller.checkAngVelYawI){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawI(_textfieldAngVelYawI.text)
                        }
                    }
                }

                Label{
                    anchors.left          : _labelAngVelI.left
                    anchors.top           : _labelAngVelI.bottom
                    anchors.topMargin     : _margins

                    id    : _labelAngVelD
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("D")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelD.verticalCenter
                    anchors.left          : _labelAngVelD.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollD
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollD
                    textColor: {
                        if(controller.checkAngVelRollD){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollD(_textfieldAngVelRollD.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollD.verticalCenter
                    anchors.left          : _textfieldAngVelRollD.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchD
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchD
                    textColor: {
                        if(controller.checkAngVelPitchD){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchD(_textfieldAngVelPitchD.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchD.verticalCenter
                    anchors.left          : _textfieldAngVelPitchD.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawD
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawD
                    textColor: {
                        if(controller.checkAngVelYawD){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawD(_textfieldAngVelYawD.text)
                        }
                    }
                }

                Label{
                    anchors.left          : _labelAngVelD.left
                    anchors.top           : _labelAngVelD.bottom
                    anchors.topMargin     : _margins

                    id    : _labelAngVelIMAX
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("IMAX")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelIMAX.verticalCenter
                    anchors.left          : _labelAngVelIMAX.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollIMAX
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollIMAX
                    textColor: {
                        if(controller.checkAngVelRollIMAX){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollIMAX(_textfieldAngVelRollIMAX.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollIMAX.verticalCenter
                    anchors.left          : _textfieldAngVelRollIMAX.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchIMAX
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchIMAX
                    textColor: {
                        if(controller.checkAngVelPitchIMAX){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchIMAX(_textfieldAngVelPitchIMAX.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchIMAX.verticalCenter
                    anchors.left          : _textfieldAngVelPitchIMAX.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawIMAX
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawIMAX
                    textColor: {
                        if(controller.checkAngVelYawIMAX){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawIMAX(_textfieldAngVelYawIMAX.text)
                        }
                    }
                }

                Label{
                    anchors.left          : _labelAngVelIMAX.left
                    anchors.top           : _labelAngVelIMAX.bottom
                    anchors.topMargin     : _margins

                    id    : _labelAngVelFF
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("FF")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelFF.verticalCenter
                    anchors.left          : _labelAngVelFF.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollFF
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollFF
                    textColor: {
                        if(controller.checkAngVelRollFF){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollFF(_textfieldAngVelRollFF.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollFF.verticalCenter
                    anchors.left          : _textfieldAngVelRollFF.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchFF
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchFF
                    textColor: {
                        if(controller.checkAngVelPitchFF){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchFF(_textfieldAngVelPitchFF.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchFF.verticalCenter
                    anchors.left          : _textfieldAngVelPitchFF.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawFF
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawFF
                    textColor: {
                        if(controller.checkAngVelYawFF){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawFF(_textfieldAngVelYawFF.text)
                        }
                    }
                }

                Label{
                    anchors.left          : _labelAngVelFF.left
                    anchors.top           : _labelAngVelFF.bottom
                    anchors.topMargin     : _margins

                    id    : _labelAngVelFILT
                    width : _Width/2
                    height: _Height

                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("FILT")
                }

                TextField{
                    anchors.verticalCenter: _labelAngVelFILT.verticalCenter
                    anchors.left          : _labelAngVelFILT.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelRollFILT
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelRollFILT
                    textColor: {
                        if(controller.checkAngVelRollFILT){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelRollFILT(_textfieldAngVelRollFILT.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelRollFILT.verticalCenter
                    anchors.left          : _textfieldAngVelRollFILT.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelPitchFILT
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelPitchFILT
                    textColor: {
                        if(controller.checkAngVelPitchFILT){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelPitchFILT(_textfieldAngVelPitchFILT.text)
                        }
                    }
                }

                TextField{
                    anchors.verticalCenter: _textfieldAngVelPitchFILT.verticalCenter
                    anchors.left          : _textfieldAngVelPitchFILT.right
                    anchors.leftMargin    : _margins

                    id   : _textfieldAngVelYawFILT
                    width: _Width
                    height: _Height

                    placeholderText: qsTr("")
                    text: controller.angvelYawFILT
                    textColor: {
                        if(controller.checkAngVelYawFILT){
                            "darkgreen"
                        }else{
                            "red"
                        }
                    }
                    onAccepted: {
                        if(_checkboxTuningEnable.checked === true){
                            controller.setAngVelYawFILT(_textfieldAngVelYawFILT.text)
                        }
                    }
                }
            }
        }
    }
}
