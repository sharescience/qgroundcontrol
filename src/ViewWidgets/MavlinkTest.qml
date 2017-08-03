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

    property real mm      : Math.round(Screen.pixelDensity)
    property real _margins: 1  * mm
    property real _Width  : 15 * mm
    property real _Height : 4  * mm

    property real  _max_pwm: 2000
    property real  _min_pwm: 1000
    property real  _mid_pwm: 0

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

    MavlinkTestController {
        id:         controller
        factPanel:  panel
    }

    QGCPalette { id: palette; colorGroupEnabled: enabled }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent
        anchors.margins:    _margins

        /* Stream speed */
        GroupBox {
            anchors.top       : parent.top
            anchors.topMargin : _margins
            anchors.left      : parent.left
            anchors.leftMargin: _margins

            id: _groupboxStreamSpeed
            title: qsTr("Stream Speed")

            Button{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _buttonStreamSpeed
                width : _Width
                height: _Height
                text: qsTr("Clear All")
                onClicked: {              
                    controller.clearStreamSpeed()
                    _comboboxRAW_SENS.currentIndex = 0
                    _comboboxEXT_STAT.currentIndex = 0
                    _comboboxRC_CHAN.currentIndex  = 0
                    _comboboxRAW_CTRL.currentIndex = 0
                    _comboboxPOSITION.currentIndex = 0
                    _comboboxEXTRA1.currentIndex   = 0
                    _comboboxEXTRA2.currentIndex   = 0
                    _comboboxEXTRA3.currentIndex   = 0
                    _comboboxPARAMS.currentIndex   = 0
                    _comboboxADSB.currentIndex     = 0
                    _comboboxPID.currentIndex      = 0
                }
            }

            CheckBox{
                anchors.top       : _buttonStreamSpeed.bottom
                anchors.left      : _buttonStreamSpeed.left

                id: _checkboxSR0
                width : _Width
                height: _Height
                text:qsTr("SR0")
                onClicked:{
                    if(_checkboxSR0.checked === false){
                        controller.setSR0(0)
                    }else{
                        controller.setSR0(1)
                    }
                }
            }

            CheckBox{
                anchors.top       : _checkboxSR0.top
                anchors.left      : _checkboxSR0.right

                id: _checkboxSR1
                width : _Width
                height: _Height
                text:qsTr("SR1")
                onClicked:{
                    if(_checkboxSR1.checked === false){
                        controller.setSR1(0)
                    }else{
                        controller.setSR1(1)
                    }
                }
            }

            CheckBox{
                anchors.top       : _checkboxSR0.bottom
                anchors.left      : _checkboxSR0.left

                id: _checkboxSR2
                width : _Width
                height: _Height
                text:qsTr("SR2")
                onClicked:{
                    if(_checkboxSR2.checked === false){
                        controller.setSR2(0)
                    }else{
                        controller.setSR2(1)
                    }
                }
            }

            CheckBox{
                anchors.top       : _checkboxSR2.top
                anchors.left      : _checkboxSR2.right

                id: _checkboxSR3
                width : _Width
                height: _Height
                text:qsTr("SR3")
                onClicked:{
                    if(_checkboxSR3.checked === false){
                        controller.setSR3(0)
                    }else{
                        controller.setSR3(1)
                    }
                }
            }

            /* ----- RAW_SENS -----*/
            Label{
                anchors.top       : _checkboxSR2.bottom
                anchors.left      : _checkboxSR2.left

                id: _labelRAW_SENS
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("RAW_SENS")
            }
            ComboBox{
                anchors.verticalCenter: _labelRAW_SENS.verticalCenter
                anchors.left          : _labelRAW_SENS.right
                anchors.leftMargin    : _margins

                id   : _comboboxRAW_SENS
                width: _Width
                height: _Height
                model: controller.streamRAW_SENSList
                onCurrentTextChanged: {
                    controller.speedRAW_SENS(_comboboxRAW_SENS.currentText)
                }
            }
            /* ----- EXT_STAT -----*/
            Label{
                anchors.top       : _labelRAW_SENS.bottom
                anchors.left      : _labelRAW_SENS.left

                id: _labelEXT_STAT
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("EXT_STAT")
            }
            ComboBox{
                anchors.verticalCenter: _labelEXT_STAT.verticalCenter
                anchors.left          : _labelEXT_STAT.right
                anchors.leftMargin    : _margins

                id   : _comboboxEXT_STAT
                width: _Width
                height: _Height
                model: controller.streamEXT_STATList
                onCurrentTextChanged: {
                    controller.speedEXT_STAT(_comboboxEXT_STAT.currentText)
                }
            }
            /* ----- RC_CHAN -----*/
            Label{
                anchors.top       : _labelEXT_STAT.bottom
                anchors.left      : _labelEXT_STAT.left

                id: _labelRC_CHAN
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("RC_CHAN")
            }
            ComboBox{
                anchors.verticalCenter: _labelRC_CHAN.verticalCenter
                anchors.left          : _labelRC_CHAN.right
                anchors.leftMargin    : _margins

                id   : _comboboxRC_CHAN
                width: _Width
                height: _Height
                model: controller.streamRC_CHANList
                onCurrentTextChanged: {
                    controller.speedRC_CHAN(_comboboxRC_CHAN.currentText)
                }
            }
            /* ----- RAW_CTRL -----*/
            Label{
                anchors.top       : _labelRC_CHAN.bottom
                anchors.left      : _labelRC_CHAN.left

                id: _labelRAW_CTRL
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("RAW_CTRL")
            }
            ComboBox{
                anchors.verticalCenter: _labelRAW_CTRL.verticalCenter
                anchors.left          : _labelRAW_CTRL.right
                anchors.leftMargin    : _margins

                id   : _comboboxRAW_CTRL
                width: _Width
                height: _Height
                model: controller.streamRAW_CTRLList
                onCurrentTextChanged: {
                    controller.speedRAW_CTRL(_comboboxRAW_CTRL.currentText)
                }
            }
            /* ----- POSITION -----*/
            Label{
                anchors.top       : _labelRAW_CTRL.bottom
                anchors.left      : _labelRAW_CTRL.left

                id: _labelPOSITION
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("POSITION")
            }
            ComboBox{
                anchors.verticalCenter: _labelPOSITION.verticalCenter
                anchors.left          : _labelPOSITION.right
                anchors.leftMargin    : _margins

                id   : _comboboxPOSITION
                width: _Width
                height: _Height
                model: controller.streamPOSITIONList
                onCurrentTextChanged: {
                    controller.speedPOSITION(_comboboxPOSITION.currentText)
                }
            }
            /* ----- EXTRA1 -----*/
            Label{
                anchors.top       : _labelPOSITION.bottom
                anchors.left      : _labelPOSITION.left

                id: _labelEXTRA1
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("EXTRA1")
            }
            ComboBox{
                anchors.verticalCenter: _labelEXTRA1.verticalCenter
                anchors.left          : _labelEXTRA1.right
                anchors.leftMargin    : _margins

                id   : _comboboxEXTRA1
                width: _Width
                height: _Height
                model: controller.streamEXTRA1List
                onCurrentTextChanged: {
                    controller.speedEXTRA1(_comboboxEXTRA1.currentText)
                }
            }
            /* ----- EXTRA2 -----*/
            Label{
                anchors.top       : _labelEXTRA1.bottom
                anchors.left      : _labelEXTRA1.left

                id: _labelEXTRA2
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("EXTRA2")
            }
            ComboBox{
                anchors.verticalCenter: _labelEXTRA2.verticalCenter
                anchors.left          : _labelEXTRA2.right
                anchors.leftMargin    : _margins

                id   : _comboboxEXTRA2
                width: _Width
                height: _Height
                model: controller.streamEXTRA2List
                onCurrentTextChanged: {
                    controller.speedEXTRA2(_comboboxEXTRA2.currentText)
                }
            }
            /* ----- EXTRA3 -----*/
            Label{
                anchors.top       : _labelEXTRA2.bottom
                anchors.left      : _labelEXTRA2.left

                id: _labelEXTRA3
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("EXTRA3")
            }
            ComboBox{
                anchors.verticalCenter: _labelEXTRA3.verticalCenter
                anchors.left          : _labelEXTRA3.right
                anchors.leftMargin    : _margins

                id   : _comboboxEXTRA3
                width: _Width
                height: _Height
                model: controller.streamEXTRA3List
                onCurrentTextChanged: {
                    controller.speedEXTRA3(_comboboxEXTRA3.currentText)
                }
            }
            /* ----- PARAMS -----*/
            Label{
                anchors.top       : _labelEXTRA3.bottom
                anchors.left      : _labelEXTRA3.left

                id: _labelPARAMS
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("PARAMS")
            }
            ComboBox{
                anchors.verticalCenter: _labelPARAMS.verticalCenter
                anchors.left          : _labelPARAMS.right
                anchors.leftMargin    : _margins

                id   : _comboboxPARAMS
                width: _Width
                height: _Height
                model: controller.streamPARAMSList
                onCurrentTextChanged: {
                    controller.speedPARAMS(_comboboxPARAMS.currentText)
                }
            }
            /* ----- ADSB -----*/
            Label{
                anchors.top       : _labelPARAMS.bottom
                anchors.left      : _labelPARAMS.left

                id: _labelADSB
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("ADSB")
            }
            ComboBox{
                anchors.verticalCenter: _labelADSB.verticalCenter
                anchors.left          : _labelADSB.right
                anchors.leftMargin    : _margins

                id   : _comboboxADSB
                width: _Width
                height: _Height
                model: controller.streamADSBList
                onCurrentTextChanged: {
                    controller.speedADSB(_comboboxADSB.currentText)
                }
            }
            /* ----- PID -----*/
            Label{
                anchors.top       : _labelADSB.bottom
                anchors.left      : _labelADSB.left

                id: _labelPID
                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   qsTr("PID")
            }
            ComboBox{
                anchors.verticalCenter: _labelPID.verticalCenter
                anchors.left          : _labelPID.right
                anchors.leftMargin    : _margins

                id   : _comboboxPID
                width: _Width
                height: _Height
                model: controller.streamPIDList
                onCurrentTextChanged: {
                    controller.speedPID(_comboboxPID.currentText)
                }
            }
        }

        /* Channels Override */
        GroupBox {
            anchors.top       : _groupboxStreamSpeed.top
            anchors.left      : _groupboxElasticity.right
            anchors.leftMargin: _margins

            id: _groupboxChannelsOverride
            title: qsTr("Channels Override(Need RC_CHAN)")

            checkable: true
            checked  : false

            onCheckedChanged: {
                controller.enableChannelsOverride(_groupboxChannelsOverride.checked)
            }

            /* ----- CH1 -----*/
            CheckBox{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH1
                width : _Width
                height: _Height
                text:qsTr("CH1")
                onClicked:{
                    _Channels_Override_CH1_Ebale = !_Channels_Override_CH1_Ebale
                    if(_Channels_Override_CH1_Ebale){
                        controller.ch1Changed(_sliderCH1.value)
                    }else{
                        controller.ch1Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH1.top
                anchors.left      : _checkboxCH1.right
                anchors.leftMargin: _margins

                id: _sliderCH1
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH1_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch1RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH1_Ebale){
                        controller.ch1Changed(_sliderCH1.value)
                    }else{
                        controller.ch1Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH1.top
                anchors.left      : _sliderCH1.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch1RCV
            }

            /* ----- CH2 -----*/
            CheckBox{
                anchors.top       : _checkboxCH1.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH2
                width : _Width
                height: _Height
                text:   qsTr("CH2")
                onClicked:{
                    _Channels_Override_CH2_Ebale = !_Channels_Override_CH2_Ebale
                    if(_Channels_Override_CH2_Ebale){
                        controller.ch2Changed(_sliderCH2.value)
                    }else{
                        controller.ch2Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH2.top
                anchors.left      : _checkboxCH2.right
                anchors.leftMargin: _margins

                id: _sliderCH2
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH2_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch2RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH2_Ebale){
                        controller.ch2Changed(_sliderCH2.value)
                    }else{
                        controller.ch2Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH2.top
                anchors.left      : _sliderCH2.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch2RCV
            }
            /* ----- CH3 -----*/
            CheckBox{
                anchors.top       : _checkboxCH2.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH3
                width : _Width
                height: _Height
                text:   qsTr("CH3")
                onClicked:{
                    _Channels_Override_CH3_Ebale = !_Channels_Override_CH3_Ebale
                    if(_Channels_Override_CH3_Ebale){
                        controller.ch3Changed(_sliderCH3.value)
                    }else{
                        controller.ch3Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH3.top
                anchors.left      : _checkboxCH3.right
                anchors.leftMargin: _margins

                id: _sliderCH3
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH3_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch3RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH3_Ebale){
                        controller.ch3Changed(_sliderCH3.value)
                    }else{
                        controller.ch3Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH3.top
                anchors.left      : _sliderCH3.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch3RCV
            }
            /* ----- CH4 -----*/
            CheckBox{
                anchors.top       : _checkboxCH3.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH4
                width : _Width
                height: _Height
                text:   qsTr("CH4")
                onClicked:{
                    _Channels_Override_CH4_Ebale = !_Channels_Override_CH4_Ebale
                    if(_Channels_Override_CH4_Ebale){
                        controller.ch4Changed(_sliderCH4.value)
                    }else{
                        controller.ch4Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH4.top
                anchors.left      : _checkboxCH4.right
                anchors.leftMargin: _margins

                id: _sliderCH4
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH4_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch4RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH4_Ebale){
                        controller.ch4Changed(_sliderCH4.value)
                    }else{
                        controller.ch4Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH4.top
                anchors.left      : _sliderCH4.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch4RCV
            }
            /* ----- CH5 -----*/
            CheckBox{
                anchors.top       : _checkboxCH4.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH5
                width : _Width
                height: _Height
                text:   qsTr("CH5")
                onClicked:{
                    _Channels_Override_CH5_Ebale = !_Channels_Override_CH5_Ebale
                    if(_Channels_Override_CH5_Ebale){
                        controller.ch5Changed(_sliderCH5.value)
                    }else{
                        controller.ch5Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH5.top
                anchors.left      : _checkboxCH5.right
                anchors.leftMargin: _margins

                id: _sliderCH5
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH5_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch5RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH5_Ebale){
                        controller.ch5Changed(_sliderCH5.value)
                    }else{
                        controller.ch5Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH5.top
                anchors.left      : _sliderCH5.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch5RCV
            }
            /* ----- CH6 -----*/
            CheckBox{
                anchors.top       : _checkboxCH5.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH6
                width : _Width
                height: _Height
                text:   qsTr("CH6")
                onClicked:{
                    _Channels_Override_CH6_Ebale = !_Channels_Override_CH6_Ebale
                    if(_Channels_Override_CH6_Ebale){
                        controller.ch6Changed(_sliderCH6.value)
                    }else{
                        controller.ch6Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH6.top
                anchors.left      : _checkboxCH6.right
                anchors.leftMargin: _margins

                id: _sliderCH6
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH6_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch6RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH6_Ebale){
                        controller.ch6Changed(_sliderCH6.value)
                    }else{
                        controller.ch6Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH6.top
                anchors.left      : _sliderCH6.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch6RCV
            }
            /* ----- CH7 -----*/
            CheckBox{
                anchors.top       : _checkboxCH6.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH7
                width : _Width
                height: _Height
                text:   qsTr("CH7")
                onClicked:{
                    _Channels_Override_CH7_Ebale = !_Channels_Override_CH7_Ebale
                    if(_Channels_Override_CH7_Ebale){
                        controller.ch7Changed(_sliderCH7.value)
                    }else{
                        controller.ch7Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH7.top
                anchors.left      : _checkboxCH7.right
                anchors.leftMargin: _margins

                id: _sliderCH7
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH7_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch7RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH7_Ebale){
                        controller.ch7Changed(_sliderCH7.value)
                    }else{
                        controller.ch7Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH7.top
                anchors.left      : _sliderCH7.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch7RCV
            }
            /* ----- CH8 -----*/
            CheckBox{
                anchors.top       : _checkboxCH7.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH8
                width : _Width
                height: _Height
                text:   qsTr("CH8")
                onClicked:{
                    _Channels_Override_CH8_Ebale = !_Channels_Override_CH8_Ebale
                    if(_Channels_Override_CH8_Ebale){
                        controller.ch8Changed(_sliderCH8.value)
                    }else{
                        controller.ch8Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH8.top
                anchors.left      : _checkboxCH8.right
                anchors.leftMargin: _margins

                id: _sliderCH8
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH8_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch8RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH8_Ebale){
                        controller.ch8Changed(_sliderCH8.value)
                    }else{
                        controller.ch8Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH8.top
                anchors.left      : _sliderCH8.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch8RCV
            }
            /* ----- CH9 -----*/
            CheckBox{
                anchors.top       : _checkboxCH8.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH9
                width : _Width
                height: _Height
                text:   qsTr("CH9")
                onClicked:{
                    _Channels_Override_CH9_Ebale = !_Channels_Override_CH9_Ebale
                    if(_Channels_Override_CH9_Ebale){
                        controller.ch9Changed(_sliderCH9.value)
                    }else{
                        controller.ch9Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH9.top
                anchors.left      : _checkboxCH9.right
                anchors.leftMargin: _margins

                id: _sliderCH9
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH9_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch9RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH9_Ebale){
                        controller.ch9Changed(_sliderCH9.value)
                    }else{
                        controller.ch9Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH9.top
                anchors.left      : _sliderCH9.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch9RCV
            }
            /* ----- CH10 -----*/
            CheckBox{
                anchors.top       : _checkboxCH9.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH10
                width : _Width
                height: _Height
                text:   qsTr("CH10")
                onClicked:{
                    _Channels_Override_CH10_Ebale = !_Channels_Override_CH10_Ebale
                    if(_Channels_Override_CH10_Ebale){
                        controller.ch10Changed(_sliderCH10.value)
                    }else{
                        controller.ch10Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH10.top
                anchors.left      : _checkboxCH10.right
                anchors.leftMargin: _margins

                id: _sliderCH10
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH10_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch10RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH10_Ebale){
                        controller.ch10Changed(_sliderCH10.value)
                    }else{
                        controller.ch10Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH10.top
                anchors.left      : _sliderCH10.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch10RCV
            }
            /* ----- CH11 -----*/
            CheckBox{
                anchors.top       : _checkboxCH10.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH11
                width : _Width
                height: _Height
                text:qsTr("CH11")
                onClicked:{
                    _Channels_Override_CH11_Ebale = !_Channels_Override_CH11_Ebale
                    if(_Channels_Override_CH11_Ebale){
                        controller.ch11Changed(_sliderCH11.value)
                    }else{
                        controller.ch11Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH11.top
                anchors.left      : _checkboxCH11.right
                anchors.leftMargin: _margins

                id: _sliderCH11
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH11_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch11RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH11_Ebale){
                        controller.ch11Changed(_sliderCH11.value)
                    }else{
                        controller.ch11Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH11.top
                anchors.left      : _sliderCH11.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch11RCV
            }

            /* ----- CH12 -----*/
            CheckBox{
                anchors.top       : _checkboxCH11.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH12
                width : _Width
                height: _Height
                text:   qsTr("CH12")
                onClicked:{
                    _Channels_Override_CH12_Ebale = !_Channels_Override_CH12_Ebale
                    if(_Channels_Override_CH12_Ebale){
                        controller.ch12Changed(_sliderCH12.value)
                    }else{
                        controller.ch12Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH12.top
                anchors.left      : _checkboxCH12.right
                anchors.leftMargin: _margins

                id: _sliderCH12
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH12_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch12RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH12_Ebale){
                        controller.ch12Changed(_sliderCH12.value)
                    }else{
                        controller.ch12Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH12.top
                anchors.left      : _sliderCH12.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch12RCV
            }
            /* ----- CH13 -----*/
            CheckBox{
                anchors.top       : _checkboxCH12.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH13
                width : _Width
                height: _Height
                text:   qsTr("CH13")
                onClicked:{
                    _Channels_Override_CH13_Ebale = !_Channels_Override_CH13_Ebale
                    if(_Channels_Override_CH13_Ebale){
                        controller.ch13Changed(_sliderCH13.value)
                    }else{
                        controller.ch13Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH13.top
                anchors.left      : _checkboxCH13.right
                anchors.leftMargin: _margins

                id: _sliderCH13
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH13_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch13RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH13_Ebale){
                        controller.ch13Changed(_sliderCH13.value)
                    }else{
                        controller.ch13Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH13.top
                anchors.left      : _sliderCH13.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch13RCV
            }
            /* ----- CH14 -----*/
            CheckBox{
                anchors.top       : _checkboxCH13.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH14
                width : _Width
                height: _Height
                text:   qsTr("CH14")
                onClicked:{
                    _Channels_Override_CH14_Ebale = !_Channels_Override_CH14_Ebale
                    if(_Channels_Override_CH14_Ebale){
                        controller.ch14Changed(_sliderCH14.value)
                    }else{
                        controller.ch14Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH14.top
                anchors.left      : _checkboxCH14.right
                anchors.leftMargin: _margins

                id: _sliderCH14
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH14_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch14RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH14_Ebale){
                        controller.ch14Changed(_sliderCH14.value)
                    }else{
                        controller.ch14Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH14.top
                anchors.left      : _sliderCH14.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch14RCV
            }
            /* ----- CH15 -----*/
            CheckBox{
                anchors.top       : _checkboxCH14.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH15
                width : _Width
                height: _Height
                text:   qsTr("CH15")
                onClicked:{
                    _Channels_Override_CH15_Ebale = !_Channels_Override_CH15_Ebale
                    if(_Channels_Override_CH15_Ebale){
                        controller.ch15Changed(_sliderCH15.value)
                    }else{
                        controller.ch15Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH15.top
                anchors.left      : _checkboxCH15.right
                anchors.leftMargin: _margins

                id: _sliderCH15
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH15_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch15RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH15_Ebale){
                        controller.ch15Changed(_sliderCH15.value)
                    }else{
                        controller.ch15Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH15.top
                anchors.left      : _sliderCH15.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch15RCV
            }
            /* ----- CH16 -----*/
            CheckBox{
                anchors.top       : _checkboxCH15.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH16
                width : _Width
                height: _Height
                text:   qsTr("CH16")
                onClicked:{
                    _Channels_Override_CH16_Ebale = !_Channels_Override_CH16_Ebale
                    if(_Channels_Override_CH16_Ebale){
                        controller.ch16Changed(_sliderCH16.value)
                    }else{
                        controller.ch16Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH16.top
                anchors.left      : _checkboxCH16.right
                anchors.leftMargin: _margins

                id: _sliderCH16
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH16_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch16RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH16_Ebale){
                        controller.ch16Changed(_sliderCH16.value)
                    }else{
                        controller.ch16Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH16.top
                anchors.left      : _sliderCH16.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch16RCV
            }
            /* ----- CH17 -----*/
            CheckBox{
                anchors.top       : _checkboxCH16.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH17
                width : _Width
                height: _Height
                text:   qsTr("CH17")
                onClicked:{
                    _Channels_Override_CH17_Ebale = !_Channels_Override_CH17_Ebale
                    if(_Channels_Override_CH7_Ebale){
                        controller.ch17Changed(_sliderCH17.value)
                    }else{
                        controller.ch7Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH17.top
                anchors.left      : _checkboxCH17.right
                anchors.leftMargin: _margins

                id: _sliderCH17
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH17_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch17RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH17_Ebale){
                        controller.ch17Changed(_sliderCH17.value)
                    }else{
                        controller.ch17Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH17.top
                anchors.left      : _sliderCH17.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch17RCV
            }
            /* ----- CH18 -----*/
            CheckBox{
                anchors.top       : _checkboxCH17.bottom
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id: _checkboxCH18
                width : _Width
                height: _Height
                text:   qsTr("CH18")
                onClicked:{
                    _Channels_Override_CH18_Ebale = !_Channels_Override_CH18_Ebale
                    if(_Channels_Override_CH18_Ebale){
                        controller.ch18Changed(_sliderCH18.value)
                    }else{
                        controller.ch18Changed(0)
                    }
                }
            }
            Slider{
                anchors.top       : _checkboxCH18.top
                anchors.left      : _checkboxCH18.right
                anchors.leftMargin: _margins

                id: _sliderCH18
                width: _Width*2
                height: _Height
                stepSize: 1
                maximumValue: _max_pwm
                minimumValue: _min_pwm
                value:
                    if(_Channels_Override_CH18_Ebale){
                        _mid_pwm
                    }else{
                        controller.ch18RCV
                    }
                onValueChanged: {
                    if(_Channels_Override_CH18_Ebale){
                        controller.ch18Changed(_sliderCH18.value)
                    }else{
                        controller.ch18Changed(0)
                    }
                }
            }
            Label{
                anchors.top       : _sliderCH18.top
                anchors.left      : _sliderCH18.right
                anchors.leftMargin: _margins

                verticalAlignment: Text.AlignVCenter
                width : _Width
                height: _Height
                text:   controller.ch18RCV
            }
        }

        /* Servos Output */
        GroupBox {
            anchors.top       : _groupboxChannelsOverride.top
            anchors.left      : _groupboxChannelsOverride.right
            anchors.leftMargin: _margins

            id: _groupboxServosOutput
            title: qsTr("Servos Output(Need RC_CHAN)")

            /* ----- SERVO #1 -----*/
            Label{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id    : _labelServo1Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 1:")
            }
            Label{
                anchors.top       : _labelServo1Output.top
                anchors.left      : _labelServo1Output.right
                anchors.leftMargin: _margins

                id    : _labelServo1OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch1OUT
            }
            /* ----- SERVO #2 -----*/
            Label{
                anchors.top       : _labelServo1Output.bottom
                anchors.left      : _labelServo1Output.left

                id    : _labelServo2Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 2:")
            }
            Label{
                anchors.top       : _labelServo2Output.top
                anchors.left      : _labelServo2Output.right
                anchors.leftMargin: _margins

                id    : _labelServo2OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch2OUT
            }
            /* ----- SERVO #3 -----*/
            Label{
                anchors.top       : _labelServo2Output.bottom
                anchors.left      : _labelServo2Output.left

                id    : _labelServo3Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 3:")
            }
            Label{
                anchors.top       : _labelServo3Output.top
                anchors.left      : _labelServo3Output.right
                anchors.leftMargin: _margins

                id    : _labelServo3OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch3OUT
            }
            /* ----- SERVO #4 -----*/
            Label{
                anchors.top       : _labelServo3Output.bottom
                anchors.left      : _labelServo3Output.left

                id    : _labelServo4Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 4:")
            }
            Label{
                anchors.top       : _labelServo4Output.top
                anchors.left      : _labelServo4Output.right
                anchors.leftMargin: _margins

                id    : _labelServo4OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch4OUT
            }
            /* ----- SERVO #5 -----*/
            Label{
                anchors.top       : _labelServo4Output.bottom
                anchors.left      : _labelServo4Output.left

                id    : _labelServo5Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 5:")
            }
            Label{
                anchors.top       : _labelServo5Output.top
                anchors.left      : _labelServo5Output.right
                anchors.leftMargin: _margins

                id    : _labelServo5OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch5OUT
            }
            /* ----- SERVO #6 -----*/
            Label{
                anchors.top       : _labelServo5Output.bottom
                anchors.left      : _labelServo5Output.left

                id    : _labelServo6Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 6:")
            }
            Label{
                anchors.top       : _labelServo6Output.top
                anchors.left      : _labelServo6Output.right
                anchors.leftMargin: _margins

                id    : _labelServo6OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch6OUT
            }
            /* ----- SERVO #7 -----*/
            Label{
                anchors.top       : _labelServo6Output.bottom
                anchors.left      : _labelServo6Output.left

                id    : _labelServo7Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 7:")
            }
            Label{
                anchors.top       : _labelServo7Output.top
                anchors.left      : _labelServo7Output.right
                anchors.leftMargin: _margins

                id    : _labelServo7OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch7OUT
            }
            /* ----- SERVO #8 -----*/
            Label{
                anchors.top       : _labelServo7Output.bottom
                anchors.left      : _labelServo7Output.left

                id    : _labelServo8Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 8:")
            }
            Label{
                anchors.top       : _labelServo8Output.top
                anchors.left      : _labelServo8Output.right
                anchors.leftMargin: _margins

                id    : _labelServo8OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch8OUT
            }
            /* ----- SERVO #9 -----*/
            Label{
                anchors.top       : _labelServo8Output.bottom
                anchors.left      : _labelServo8Output.left

                id    : _labelServo9Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 9:")
            }
            Label{
                anchors.top       : _labelServo9Output.top
                anchors.left      : _labelServo9Output.right
                anchors.leftMargin: _margins

                id    : _labelServo9OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch9OUT
            }
            /* ----- SERVO #10 -----*/
            Label{
                anchors.top       : _labelServo9Output.bottom
                anchors.left      : _labelServo9Output.left

                id    : _labelServo10Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 10:")
            }
            Label{
                anchors.top       : _labelServo10Output.top
                anchors.left      : _labelServo10Output.right
                anchors.leftMargin: _margins

                id    : _labelServo10OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch10OUT
            }
            /* ----- SERVO #11 -----*/
            Label{
                anchors.top       : _labelServo10Output.bottom
                anchors.left      : _labelServo10Output.left

                id    : _labelServo11Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 11:")
            }
            Label{
                anchors.top       : _labelServo11Output.top
                anchors.left      : _labelServo11Output.right
                anchors.leftMargin: _margins

                id    : _labelServo11OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch11OUT
            }
            /* ----- SERVO #12 -----*/
            Label{
                anchors.top       : _labelServo11Output.bottom
                anchors.left      : _labelServo11Output.left

                id    : _labelServo12Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 12:")
            }
            Label{
                anchors.top       : _labelServo12Output.top
                anchors.left      : _labelServo12Output.right
                anchors.leftMargin: _margins

                id    : _labelServo12OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch12OUT
            }
            /* ----- SERVO #13 -----*/
            Label{
                anchors.top       : _labelServo12Output.bottom
                anchors.left      : _labelServo12Output.left

                id    : _labelServo13Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 13:")
            }
            Label{
                anchors.top       : _labelServo13Output.top
                anchors.left      : _labelServo13Output.right
                anchors.leftMargin: _margins

                id    : _labelServo13OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch13OUT
            }
            /* ----- SERVO #14 -----*/
            Label{
                anchors.top       : _labelServo13Output.bottom
                anchors.left      : _labelServo13Output.left

                id    : _labelServo14Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 14:")
            }
            Label{
                anchors.top       : _labelServo14Output.top
                anchors.left      : _labelServo14Output.right
                anchors.leftMargin: _margins

                id    : _labelServo14OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch14OUT
            }
            /* ----- SERVO #15 -----*/
            Label{
                anchors.top       : _labelServo14Output.bottom
                anchors.left      : _labelServo14Output.left

                id    : _labelServo15Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 15:")
            }
            Label{
                anchors.top       : _labelServo15Output.top
                anchors.left      : _labelServo15Output.right
                anchors.leftMargin: _margins

                id    : _labelServo15OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch15OUT
            }
            /* ----- SERVO #16 -----*/
            Label{
                anchors.top       : _labelServo15Output.bottom
                anchors.left      : _labelServo15Output.left

                id    : _labelServo16Output
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Servo 16:")
            }
            Label{
                anchors.top       : _labelServo16Output.top
                anchors.left      : _labelServo16Output.right
                anchors.leftMargin: _margins

                id    : _labelServo16OutputShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.ch16OUT
            }
        }

        /* Elasticity */
        GroupBox {
            anchors.top       : _groupboxStreamSpeed.bottom
            anchors.left      : _groupboxStreamSpeed.left
            anchors.topMargin : _margins

            id: _groupboxElasticity
            title: qsTr("Elasticity Packet")


            ComboBox{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id    : _comboboxElasticitySendHz
                width : _Width
                height: _Height
                model : controller.elasticitySendHzList

                onCurrentTextChanged: {
                    controller.elasticitySendHzChoose(_comboboxElasticitySendHz.currentText)
                }
            }
            /* ----- Elasticity Send -----*/
            Label{
                anchors.top       : _comboboxElasticitySendHz.bottom
                anchors.left      : _comboboxElasticitySendHz.left

                id    : _labelElasticitySend
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Send:")
            }
            Label{
                anchors.top       : _labelElasticitySend.top
                anchors.left      : _labelElasticitySend.right
                anchors.leftMargin: _margins

                id    : _labelElasticitySendCount
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.elasticitySendCount
            }
            /* ----- Elasticity Receive -----*/
            Label{
                anchors.top       : _labelElasticitySend.bottom
                anchors.left      : _labelElasticitySend.left

                id    : _labelElasticityReceive
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Receive:")
            }
            Label{
                anchors.top       : _labelElasticityReceive.top
                anchors.left      : _labelElasticityReceive.right
                anchors.leftMargin: _margins

                id    : _labelElasticityReceiveCount
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.elasticityRcvCount
            }
            /* ----- Elasticity Lost -----*/
            Label{
                anchors.top       : _labelElasticityReceive.bottom
                anchors.left      : _labelElasticityReceive.left

                id    : _labelElasticityLost
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Lost:")
            }
            Label{
                anchors.top       : _labelElasticityLost.top
                anchors.left      : _labelElasticityLost.right
                anchors.leftMargin: _margins

                id    : _labelElasticityLostCount
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.elasticityLostCount
            }
            /* ----- Average Sample -----*/
            Label{
                anchors.top       : _comboboxElasticitySendHz.top
                anchors.left      : _labelElasticitySendCount.right
                anchors.leftMargin: _margins

                id    : _labelElasticityAverage
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Number:")
            }
            ComboBox{
                anchors.top       : _labelElasticityAverage.top
                anchors.left      : _labelElasticityAverage.right
                anchors.leftMargin: _margins

                id   : _comboboxAvgSample
                width: _Width
                height: _Height
                editable: true

                model: ListModel {
                    id: model
                    ListElement { text: "180" }
                }
                onAccepted: {
                    if(_comboboxAvgSample.find(currentText) === -1){
                        model.append({text: editText})
                        currentIndex = _comboboxAvgSample.find(editText)
                        controller.setFIFOLenChoose(_comboboxAvgSample.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.setFIFOLenChoose(_comboboxAvgSample.currentText)
                }
            }
            Label{
                anchors.top       : _comboboxAvgSample.top
                anchors.left      : _comboboxAvgSample.right
                anchors.leftMargin: _margins

                id    : _labelElasticityFIFO
                width : _Width
                height: _Height

                text:   controller.FIFODisplay
            }
            /* ----- Average Latency -----*/
            Label{
                anchors.top       : _labelElasticityAverage.bottom
                anchors.left      : _labelElasticityAverage.left

                id    : _labelElasticityAverageLatency
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Avg late:")
            }
            Label{
                anchors.top       : _labelElasticityAverageLatency.top
                anchors.left      : _labelElasticityAverageLatency.right
                anchors.leftMargin: _margins

                id    : _labelElasticityAverageLatencyShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.averageLatency
            }
            /* ----- Max Latency -----*/
            Label{
                anchors.top       : _labelElasticityAverageLatency.bottom
                anchors.left      : _labelElasticityAverageLatency.left

                id    : _labelElasticityMaxLatency
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Max late:")
            }
            Label{
                anchors.top       : _labelElasticityMaxLatency.top
                anchors.left      : _labelElasticityMaxLatency.right
                anchors.leftMargin: _margins

                id    : _labelElasticityMaxLatencyShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.maxLatency
            }
            /* ----- Min Latency -----*/
            Label{
                anchors.top       : _labelElasticityMaxLatency.bottom
                anchors.left      : _labelElasticityMaxLatency.left

                id    : _labelElasticityMinLatency
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Min late:")
            }
            Label{
                anchors.top       : _labelElasticityMinLatency.top
                anchors.left      : _labelElasticityMinLatency.right
                anchors.leftMargin: _margins

                id    : _labelElasticityMinLatencyShow
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.minLatency
            }
            /* ----- Start Time -----*/
            Label{
                anchors.top       : _labelElasticityLost.bottom
                anchors.left      : _labelElasticityLost.left

                id    : _labelElasticityStartTime
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Start time:")
            }
            Label{
                anchors.top       : _labelElasticityStartTime.top
                anchors.left      : _labelElasticityStartTime.right
                anchors.leftMargin: _margins

                id    : _labelElasticityStartTimeShow
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.elasticityStartTime
            }
            /* ----- Pass Time -----*/
            Label{
                anchors.top       : _labelElasticityStartTime.bottom
                anchors.left      : _labelElasticityStartTime.left

                id    : _labelElasticityPassTime
                width : _Width
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Pass time:")
            }
            Label{
                anchors.top       : _labelElasticityPassTime.top
                anchors.left      : _labelElasticityPassTime.right
                anchors.leftMargin: _margins

                id    : _labelElasticityPassTimeShow
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: controller.elasticityPassTime
            }
        }

        /* Servos Test */
        GroupBox {
            anchors.top       : _groupboxElasticity.bottom
            anchors.left      : _groupboxElasticity.left
            anchors.topMargin : _margins

            id: _groupboxServosTest
            title: qsTr("Servos Test")

            checkable: true
            checked  : false

            /* ----- Test Time -----*/
            Label{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id    : _labelServosTestTime
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("Test Time")
            }
            ComboBox{
                anchors.verticalCenter: _labelServosTestTime.verticalCenter
                anchors.left          : _labelServosTestTime.right
                anchors.leftMargin    : _margins

                id   : _comboboxServosTestTime
                width: _Width
                height: _Height
                editable: true
                model: ListModel {
                    id: modelServosTestTime
                    ListElement { text: "1" }
                    ListElement { text: "30" }
                }
                onAccepted: {
                    if(_comboboxServo1Test.find(currentText) === -1){
                        modelServosTestTime.append({text: editText})
                        currentIndex = _comboboxServosTestTime.find(editText)
                        controller.servosTestTimeSet(_comboboxServosTestTime.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.servosTestTimeSet(_comboboxServosTestTime.currentText)
                }
            }
            Label{
                anchors.verticalCenter: _comboboxServosTestTime.verticalCenter
                anchors.left          : _comboboxServosTestTime.right
                anchors.leftMargin    : _margins

                id    : _labelServosTestTimeNote
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("s (Max: 30s)")
            }

            /* ----- Servo #1 -----*/
            CheckBox{
                anchors.top       : _labelServosTestTime.bottom
                anchors.left      : _labelServosTestTime.left
                anchors.topMargin : _margins

                id: _checkboxServo1Test
                width : _Width
                height: _Height
                text:qsTr("Servo #1")
            }
            ComboBox{
                anchors.verticalCenter: _checkboxServo1Test.verticalCenter
                anchors.left          : _checkboxServo1Test.right
                anchors.leftMargin    : _margins

                id   : _comboboxServo1Test
                width: _Width
                height: _Height
                editable: true
                model: ListModel {
                    id: modelServo1TestPercent
                    ListElement { text: "10" }
                    ListElement { text: "15" }
                    ListElement { text: "20" }
                }
                onAccepted: {
                    if(_comboboxServo1Test.find(currentText) === -1){
                        modelServo1TestPercent.append({text: editText})
                        currentIndex = _comboboxServo1Test.find(editText)
                        controller.servo1TestPercent(_comboboxServo1Test.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.servo1TestPercent(_comboboxServo1Test.currentText)
                }
            }
            Label{
                anchors.verticalCenter: _comboboxServo1Test.verticalCenter
                anchors.left          : _comboboxServo1Test.right
                anchors.leftMargin    : _margins

                id    : _labelServo1TestUnit
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("%")
            }
            Button{
                anchors.top       : _labelServo1TestUnit.top
                anchors.left      : _labelServo1TestUnit.right
                anchors.leftMargin: _margins

                id: _buttonServo1TestStart
                width : _Width
                height: _Height
                text  : qsTr("Start")
                onClicked:{
                    if(_checkboxServo1Test.checked === false){
                        controller.stopServoTest()
                    }else{
                        controller.startServoTest(1)
                    }
                }
            }

            /* ----- Servo #2 -----*/
            CheckBox{
                anchors.top       : _checkboxServo1Test.bottom
                anchors.left      : _checkboxServo1Test.left
                anchors.topMargin : _margins

                id: _checkboxServo2Test
                width : _Width
                height: _Height
                text:qsTr("Servo #2")
            }
            ComboBox{
                anchors.verticalCenter: _checkboxServo2Test.verticalCenter
                anchors.left          : _checkboxServo2Test.right
                anchors.leftMargin    : _margins

                id   : _comboboxServo2Test
                width: _Width
                height: _Height
                editable: true
                model: ListModel {
                    id: modelServo2TestPercent
                    ListElement { text: "10" }
                    ListElement { text: "15" }
                    ListElement { text: "20" }
                }
                onAccepted: {
                    if(_comboboxServo2Test.find(currentText) === -1){
                        modelServo2TestPercent.append({text: editText})
                        currentIndex = _comboboxServo2Test.find(editText)
                        controller.servo2TestPercent(_comboboxServo2Test.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.servo2TestPercent(_comboboxServo2Test.currentText)
                }
            }
            Label{
                anchors.verticalCenter: _comboboxServo2Test.verticalCenter
                anchors.left          : _comboboxServo2Test.right
                anchors.leftMargin    : _margins

                id    : _labelServo2TestUnit
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("%")
            }
            Button{
                anchors.top       : _labelServo2TestUnit.top
                anchors.left      : _labelServo2TestUnit.right
                anchors.leftMargin: _margins

                id: _buttonServo2TestStart
                width : _Width
                height: _Height
                text  : qsTr("Start")
                onClicked:{
                    if(_checkboxServo2Test.checked === false){
                        controller.stopServoTest()
                    }else{
                        controller.startServoTest(2)
                    }
                }
            }

            /* ----- Servo #3 -----*/
            CheckBox{
                anchors.top       : _checkboxServo2Test.bottom
                anchors.left      : _checkboxServo2Test.left
                anchors.topMargin : _margins

                id: _checkboxServo3Test
                width : _Width
                height: _Height
                text:qsTr("Servo #3")
            }
            ComboBox{
                anchors.verticalCenter: _checkboxServo3Test.verticalCenter
                anchors.left          : _checkboxServo3Test.right
                anchors.leftMargin    : _margins

                id   : _comboboxServo3Test
                width: _Width
                height: _Height
                editable: true
                model: ListModel {
                    id: modelServo3TestPercent
                    ListElement { text: "10" }
                    ListElement { text: "15" }
                    ListElement { text: "20" }
                }
                onAccepted: {
                    if(_comboboxServo3Test.find(currentText) === -1){
                        modelServo3TestPercent.append({text: editText})
                        currentIndex = _comboboxServo3Test.find(editText)
                        controller.servo3TestPercent(_comboboxServo3Test.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.servo3TestPercent(_comboboxServo3Test.currentText)
                }
            }
            Label{
                anchors.verticalCenter: _comboboxServo3Test.verticalCenter
                anchors.left          : _comboboxServo3Test.right
                anchors.leftMargin    : _margins

                id    : _labelServo3TestUnit
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("%")
            }
            Button{
                anchors.top       : _labelServo3TestUnit.top
                anchors.left      : _labelServo3TestUnit.right
                anchors.leftMargin: _margins

                id: _buttonServo3TestStart
                width : _Width
                height: _Height
                text  : qsTr("Start")
                onClicked:{
                    if(_checkboxServo3Test.checked === false){
                        controller.stopServoTest()
                    }else{
                        controller.startServoTest(3)
                    }
                }
            }

            /* ----- Servo #4 -----*/
            CheckBox{
                anchors.top       : _checkboxServo3Test.bottom
                anchors.left      : _checkboxServo3Test.left
                anchors.topMargin : _margins

                id: _checkboxServoTailTest
                width : _Width
                height: _Height
                text:qsTr("Servo #4")
            }
            ComboBox{
                anchors.verticalCenter: _checkboxServoTailTest.verticalCenter
                anchors.left          : _checkboxServoTailTest.right
                anchors.leftMargin    : _margins

                id   : _comboboxServoTailTest
                width: _Width
                height: _Height
                editable: true
                model: ListModel {
                    id: modelServoTailTestPercent
                    ListElement { text: "10" }
                    ListElement { text: "15" }
                    ListElement { text: "20" }
                }
                onAccepted: {
                    if(_comboboxServoTailTest.find(currentText) === -1){
                        modelServoTailTestPercent.append({text: editText})
                        currentIndex = _comboboxServoTailTest.find(editText)
                        controller.servoTailTestPercent(_comboboxServoTailTest.currentText)
                    }
                }
                onCurrentTextChanged: {
                    controller.servoTailTestPercent(_comboboxServoTailTest.currentText)
                }
            }
            Label{
                anchors.verticalCenter: _comboboxServoTailTest.verticalCenter
                anchors.left          : _comboboxServoTailTest.right
                anchors.leftMargin    : _margins

                id    : _labelServoTailTestUnit
                height: _Height

                verticalAlignment: Text.AlignVCenter
                text: qsTr("%")
            }
            Button{
                anchors.top       : _labelServoTailTestUnit.top
                anchors.left      : _labelServoTailTestUnit.right
                anchors.leftMargin: _margins

                id: _buttonServoTailTestStart
                width : _Width
                height: _Height
                text  : qsTr("Start")
                onClicked:{
                    if(_checkboxServoTailTest.checked === false){
                        controller.stopServoTest()
                    }else{
                        controller.startServoTest(4)
                    }
                }
            }
        }
    }
}
