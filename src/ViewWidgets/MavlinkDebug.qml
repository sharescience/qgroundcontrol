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

    MavlinkDebugTransDataController {
        id:         controller
        factPanel:  panel
    }

    QGCPalette { id: palette; colorGroupEnabled: enabled }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent
        anchors.margins:    _margins

        GroupBox {
            anchors.top       : parent.top
            anchors.topMargin : _margins
            anchors.left      : parent.left
            anchors.leftMargin: _margins

            id: _groupboxSend
            title: qsTr("GCS Send Trace")

            ComboBox{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id    : _comboboxSend
                width : _Width *2  + _margins
                height: _Height
                model : controller.idListSend
                onCurrentTextChanged: {
                    controller.setsendMavIDChoose(_comboboxSend.currentText)
                }
            }

            Label{
                anchors.top       : _comboboxSend.top
                anchors.left      : _comboboxSend.right
                anchors.leftMargin: _margins

                id    : _labelSendHz
                width : _Width
                height: _Height
                text  : controller.sendHz
            }

            CheckBox{
                anchors.top       : _labelSendHz.top
                anchors.left      : _labelSendHz.right
                anchors.leftMargin: _margins

                id: _checkboxSendAnalyzeEnable
                width : _Width
                height: _Height
                text:qsTr("Analyze")
                onClicked:{
                    if(_checkboxSendAnalyzeEnable.checked === false){
                        controller.setsendAnalyzeEnable(0)
                    }else{
                        controller.setsendAnalyzeEnable(1)
                    }
                }
            }

            CheckBox{
                anchors.top       : _checkboxSendAnalyzeEnable.top
                anchors.left      : _checkboxSendAnalyzeEnable.right
                anchors.leftMargin: _margins

                id: _checkboxSendCacheEnable
                width : _Width *2
                height: _Height
                text:qsTr("Cache Enable")
                onClicked:{
                    if(_checkboxSendCacheEnable.checked === false){
                        controller.setsendCacheEnable(0)
                        controller.setmsgSendIndex(1)
                    }else{
                        controller.clearSendList()
                        controller.setsendCacheEnable(1)
                    }
                }
            }

            Button{
                anchors.top       : _checkboxSendCacheEnable.top
                anchors.left      : _checkboxSendCacheEnable.right
                anchors.leftMargin: _margins

                id: _buttonSendBack
                width : _Width
                height: _Height
                text  : qsTr("Back")
                onClicked:{
                    controller.setmsgSendIndex(controller.msgSendIndex + 1)
                }
            }

            Button{
                anchors.top       : _buttonSendBack.top
                anchors.left      : _buttonSendBack.right
                anchors.leftMargin: _margins

                id: _buttonSendNext
                width : _Width
                height: _Height
                text  : qsTr("Next")
                onClicked:{
                    controller.setmsgSendIndex(controller.msgSendIndex - 1)
                }
            }

            TextArea {
                anchors.top       : _comboboxSend.bottom
                anchors.topMargin : _margins
                anchors.left      : _comboboxSend.left

                visible: !_checkboxSendAnalyzeEnable.checked

                id    : _textareaSend
                width : _Width * 10
                height: _Height* 10 + 6 * _margins
                text  : controller.sendMessageHex
            }

            GroupBox {
                anchors.top       : _comboboxSend.bottom
                anchors.topMargin : _margins
                anchors.left      : _comboboxSend.left

                visible: _checkboxSendAnalyzeEnable.checked

                id: _groupboxSendMsgHead
                title: qsTr("Head")

                Label{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _labelSendAnalyzeMagic
                    width : _Width
                    height: _Height
                    text  : qsTr("magic")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeMagic.top
                    anchors.left      : _labelSendAnalyzeMagic.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendMagic
                    width : _Width
                    height: _Height
                    text  : controller.sendMagic
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeMagic.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeMagic.left

                    id    : _labelSendAnalyzeLen
                    width : _Width
                    height: _Height
                    text  : qsTr("len")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeLen.top
                    anchors.left      : _labelSendAnalyzeLen.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSend
                    width : _Width
                    height: _Height
                    text  : controller.sendLen
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeLen.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeLen.left

                    id    : _labelSendAnalyzeIncompat
                    width : _Width
                    height: _Height
                    text  : qsTr("incompat")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeIncompat.top
                    anchors.left      : _labelSendAnalyzeIncompat.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendIncompat
                    width : _Width
                    height: _Height
                    text  : controller.sendIncompatFlag
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeIncompat.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeIncompat.left

                    id    : _labelSendAnalyzeCompat
                    width : _Width
                    height: _Height
                    text  : qsTr("compat")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeCompat.top
                    anchors.left      : _labelSendAnalyzeCompat.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendCompat
                    width : _Width
                    height: _Height
                    text  : controller.sendCompatFlag
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeCompat.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeCompat.left

                    id    : _labelSendAnalyzeSeq
                    width : _Width
                    height: _Height
                    text  : qsTr("seq")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeSeq.top
                    anchors.left      : _labelSendAnalyzeSeq.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendSeq
                    width : _Width
                    height: _Height
                    text  : controller.sendSeq
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeSeq.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeSeq.left

                    id    : _labelSendAnalyzeSysid
                    width : _Width
                    height: _Height
                    text  : qsTr("sysid")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeSysid.top
                    anchors.left      : _labelSendAnalyzeSysid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendSysid
                    width : _Width
                    height: _Height
                    text  : controller.sendSysid
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeSysid.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeSysid.left

                    id    : _labelSendAnalyzeCompid
                    width : _Width
                    height: _Height
                    text  : qsTr("compid")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeCompid.top
                    anchors.left      : _labelSendAnalyzeCompid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendCompid
                    width : _Width
                    height: _Height
                    text  : controller.sendCompid
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeCompid.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeCompid.left

                    id    : _labelSendAnalyzeMsgid
                    width : _Width
                    height: _Height
                    text  : qsTr("msgid")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeMsgid.top
                    anchors.left      : _labelSendAnalyzeMsgid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendMsgid
                    width : _Width
                    height: _Height
                    text  : controller.sendMsgid
                    verticalAlignment: Text.AlignVCenter
                }
            }

            GroupBox {
                anchors.top       : _groupboxSendMsgHead.top
                anchors.left      : _groupboxSendMsgHead.right
                anchors.leftMargin: _margins

                visible: _checkboxSendAnalyzeEnable.checked

                id: _groupboxSendMsgPayload
                title: qsTr("Payload")

                TextArea {
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _textareaSendPayload
                    width : _Width  * 4
                    height: (_Height * 8 + _margins * 7)
                    text  : controller.sendPayload
                }

                TextArea {
                    anchors.top       : _textareaSendPayload.bottom
                    anchors.left      : _textareaSendPayload.left

                    visible: false

                    id    : _textareaSendPayloadEmpty
                    height:  0
                    text  : ""
                }
            }

            GroupBox {
                anchors.top       : _groupboxSendMsgPayload.top
                anchors.left      : _groupboxSendMsgPayload.right
                anchors.leftMargin: _margins

                visible: _checkboxSendAnalyzeEnable.checked

                id: _groupboxSendMsgCRC
                title: qsTr("CRC")

                Label{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _labelSendAnalyzeCkA
                    width : _Width
                    height: _Height
                    text  : qsTr("ck_a")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeCkA.top
                    anchors.left      : _labelSendAnalyzeCkA.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendCkA
                    width : _Width
                    height: _Height
                    text  : controller.sendCkA
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelSendAnalyzeCkA.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelSendAnalyzeCkA.left

                    id    : _labelSendAnalyzeCkB
                    width : _Width
                    height: _Height
                    text  : qsTr("ck_b")
                }

                TextField {
                    anchors.top       : _labelSendAnalyzeCkB.top
                    anchors.left      : _labelSendAnalyzeCkB.right
                    anchors.leftMargin: _margins

                    id    : _textfieldSendCkB
                    width : _Width
                    height: _Height
                    text  : controller.sendCkB
                    verticalAlignment: Text.AlignVCenter
                }
            }

            GroupBox {
                anchors.top       : _groupboxSendMsgCRC.bottom
                anchors.left      : _groupboxSendMsgCRC.left
                anchors.topMargin : _margins

                visible: _checkboxSendAnalyzeEnable.checked

                id: _groupboxSendMsgSignature
                title: qsTr("Signature")

                TextArea {
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _textareaSendSignature
                    width : _Width  * 3
                    height: _Height * 5 + 1 * _margins
                    text  : controller.sendSignature
                }

                TextArea {
                    anchors.top       : _textareaSendSignature.bottom
                    anchors.left      : _textareaSendSignature.left

                    visible: false

                    id    : _textareaSendSignatureEmpty1
                    height:  0
                    text  : ""
                }
            }
        }

        GroupBox {
            anchors.top       : _groupboxSend.bottom
            anchors.topMargin : _margins
            anchors.left      : _groupboxSend.left

            id: _groupboxReceive
            title: qsTr("GCS Receive Trace")

            ComboBox{
                anchors.top       : parent.top
                anchors.topMargin : _margins
                anchors.left      : parent.left
                anchors.leftMargin: _margins

                id    : _comboboxReceive
                width : _Width * 2 + _margins
                height: _Height
                model : controller.idListReceive
                onCurrentTextChanged: {
                    controller.setrcvMavIDChoose(_comboboxReceive.currentText)
                }
            }

            Label{
                anchors.top       : _comboboxReceive.top
                anchors.left      : _comboboxReceive.right
                anchors.leftMargin: _margins

                id    : _labelReceiveHz
                width : _Width
                height: _Height
                text  : controller.rcvHz
            }

            CheckBox{
                anchors.top       : _labelReceiveHz.top
                anchors.left      : _labelReceiveHz.right
                anchors.leftMargin: _margins

                id: _checkboxReceiveAnalyzeEnable
                width : _Width
                height: _Height
                text:qsTr("Analyze")
                onClicked:{
                    if(_checkboxReceiveAnalyzeEnable.checked === false){
                        controller.setrcvAnalyzeEnable(0)
                    }else{
                        controller.setrcvAnalyzeEnable(1)
                    }
                }
            }

            CheckBox{
                anchors.top       : _checkboxReceiveAnalyzeEnable.top
                anchors.left      : _checkboxReceiveAnalyzeEnable.right
                anchors.leftMargin: _margins

                id: _checkboxReceiveCacheEnable
                width : _Width *2
                height: _Height
                text:qsTr("Cache Enable")
                onClicked:{
                    if(_checkboxReceiveCacheEnable.checked === false){
                        controller.setrcvCacheEnable(0)
                        controller.setmsgReceiveIndex(1)
                    }else{
                        controller.clearReceiveList()
                        controller.setrcvCacheEnable(1)
                    }
                }
            }

            Button{
                anchors.top       : _checkboxReceiveCacheEnable.top
                anchors.left      : _checkboxReceiveCacheEnable.right
                anchors.leftMargin: _margins

                id: _buttonReceiveBack
                width : _Width
                height: _Height
                text  : qsTr("Back")
                onClicked:{
                    controller.setmsgReceiveIndex(controller.msgReceiveIndex + 1)
                }
            }

            Button{
                anchors.top       : _buttonReceiveBack.top
                anchors.left      : _buttonReceiveBack.right
                anchors.leftMargin: _margins

                id: _buttonReceiveNext
                width : _Width
                height: _Height
                text  : qsTr("Next")
                onClicked:{
                    controller.setmsgReceiveIndex(controller.msgReceiveIndex - 1)
                }
            }

            TextArea {
                anchors.top       : _comboboxReceive.bottom
                anchors.topMargin : _margins
                anchors.left      : _comboboxReceive.left

                visible: !_checkboxReceiveAnalyzeEnable.checked

                id    : _textareaReceiveHEX
                width : _Width * 10
                height: _Height* 10 + 6 * _margins
                text  : controller.rcvMessageHex
            }

            GroupBox {
                anchors.top       : _comboboxReceive.bottom
                anchors.topMargin : _margins
                anchors.left      : _comboboxReceive.left

                visible: _checkboxReceiveAnalyzeEnable.checked

                id: _groupboxReceiveMsgHead
                title: qsTr("Head")

                Label{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _labelReceiveAnalyzeMagic
                    width : _Width
                    height: _Height
                    text  : qsTr("magic")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeMagic.top
                    anchors.left      : _labelReceiveAnalyzeMagic.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveMagic
                    width : _Width
                    height: _Height
                    text  : controller.rcvMagic
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeMagic.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeMagic.left

                    id    : _labelReceiveAnalyzeLen
                    width : _Width
                    height: _Height
                    text  : qsTr("len")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeLen.top
                    anchors.left      : _labelReceiveAnalyzeLen.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceive
                    width : _Width
                    height: _Height
                    text  : controller.rcvLen
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeLen.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeLen.left

                    id    : _labelReceiveAnalyzeIncompat
                    width : _Width
                    height: _Height
                    text  : qsTr("incompat")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeIncompat.top
                    anchors.left      : _labelReceiveAnalyzeIncompat.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveIncompat
                    width : _Width
                    height: _Height
                    text  : controller.rcvIncompatFlag
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeIncompat.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeIncompat.left

                    id    : _labelReceiveAnalyzeCompat
                    width : _Width
                    height: _Height
                    text  : qsTr("compat")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeCompat.top
                    anchors.left      : _labelReceiveAnalyzeCompat.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveCompat
                    width : _Width
                    height: _Height
                    text  : controller.rcvCompatFlag
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeCompat.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeCompat.left

                    id    : _labelReceiveAnalyzeSeq
                    width : _Width
                    height: _Height
                    text  : qsTr("seq")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeSeq.top
                    anchors.left      : _labelReceiveAnalyzeSeq.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveSeq
                    width : _Width
                    height: _Height
                    text  : controller.rcvSeq
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeSeq.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeSeq.left

                    id    : _labelReceiveAnalyzeSysid
                    width : _Width
                    height: _Height
                    text  : qsTr("sysid")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeSysid.top
                    anchors.left      : _labelReceiveAnalyzeSysid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveSysid
                    width : _Width
                    height: _Height
                    text  : controller.rcvSysid
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeSysid.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeSysid.left

                    id    : _labelReceiveAnalyzeCompid
                    width : _Width
                    height: _Height
                    text  : qsTr("compid")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeCompid.top
                    anchors.left      : _labelReceiveAnalyzeCompid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveCompid
                    width : _Width
                    height: _Height
                    text  : controller.rcvCompid
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeCompid.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeCompid.left

                    id    : _labelReceiveAnalyzeMsgid
                    width : _Width
                    height: _Height
                    text  : qsTr("msgid")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeMsgid.top
                    anchors.left      : _labelReceiveAnalyzeMsgid.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveMsgid
                    width : _Width
                    height: _Height
                    text  : controller.rcvMsgid
                    verticalAlignment: Text.AlignVCenter
                }
            }

            GroupBox {
                anchors.top       : _groupboxReceiveMsgHead.top
                anchors.left      : _groupboxReceiveMsgHead.right
                anchors.leftMargin: _margins

                visible: _checkboxReceiveAnalyzeEnable.checked

                id: _groupboxReceiveMsgPayload
                title: qsTr("Payload")

                TextArea {
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _textareaReceivePayload
                    width : _Width  * 4
                    height: (_Height * 8 + _margins * 7)
                    text  : controller.rcvPayload
                }

                TextArea {
                    anchors.top       : _textareaReceivePayload.bottom
                    anchors.left      : _textareaReceivePayload.left

                    visible: false

                    id    : _textareaReceivePayloadEmpty
                    height:  0
                    text  : ""
                }
            }

            GroupBox {
                anchors.top       : _groupboxReceiveMsgPayload.top
                anchors.left      : _groupboxReceiveMsgPayload.right
                anchors.leftMargin: _margins

                visible: _checkboxReceiveAnalyzeEnable.checked

                id: _groupboxReceiveMsgCRC
                title: qsTr("CRC")

                Label{
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _labelReceiveAnalyzeCkA
                    width : _Width
                    height: _Height
                    text  : qsTr("ck_a")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeCkA.top
                    anchors.left      : _labelReceiveAnalyzeCkA.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveCkA
                    width : _Width
                    height: _Height
                    text  : controller.rcvCkA
                    verticalAlignment: Text.AlignVCenter
                }

                Label{
                    anchors.top       : _labelReceiveAnalyzeCkA.bottom
                    anchors.topMargin : _margins
                    anchors.left      : _labelReceiveAnalyzeCkA.left

                    id    : _labelReceiveAnalyzeCkB
                    width : _Width
                    height: _Height
                    text  : qsTr("ck_b")
                }

                TextField {
                    anchors.top       : _labelReceiveAnalyzeCkB.top
                    anchors.left      : _labelReceiveAnalyzeCkB.right
                    anchors.leftMargin: _margins

                    id    : _textfieldReceiveCkB
                    width : _Width
                    height: _Height
                    text  : controller.rcvCkB
                    verticalAlignment: Text.AlignVCenter
                }
            }

            GroupBox {
                anchors.top       : _groupboxReceiveMsgCRC.bottom
                anchors.left      : _groupboxReceiveMsgCRC.left
                anchors.topMargin : _margins

                visible: _checkboxReceiveAnalyzeEnable.checked

                id: _groupboxReceiveMsgSignature
                title: qsTr("Signature")

                TextArea {
                    anchors.top       : parent.top
                    anchors.topMargin : _margins
                    anchors.left      : parent.left
                    anchors.leftMargin: _margins

                    id    : _textareaReceiveSignature
                    width : _Width  * 3
                    height: _Height * 5 + 1 * _margins
                    text  : controller.rcvSignature
                }

                TextArea {
                    anchors.top       : _textareaReceiveSignature.bottom
                    anchors.left      : _textareaReceiveSignature.left

                    visible: false

                    id    : _textareaReceiveSignatureEmpty1
                    height:  0
                    text  : ""
                }
            }
        }
    }
}
