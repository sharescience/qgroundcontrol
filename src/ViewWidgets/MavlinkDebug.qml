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

import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0

QGCView {
    viewPanel:  panel

    property real _margins       :    ScreenTools.defaultFontPixelHeight
    property real _comboboxWidth :    ScreenTools.defaultFontPixelWidth * 21
    property real _textareaWidth :    ScreenTools.defaultFontPixelWidth * 127
    property real _textareaHeight:    ScreenTools.defaultFontPixelWidth * 16
    property real _textfieldWidth :    ScreenTools.defaultFontPixelWidth * 12
    property real _textfieldHeight:    ScreenTools.defaultFontPixelWidth * 3

    MavlinkDebugTransDataController {
        id:         controller
        factPanel:  panel
    }

    QGCPalette { id: palette; colorGroupEnabled: enabled }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent
        anchors.margins:    _margins

        Column{
            spacing:        ScreenTools.defaultFontPixelWidth

            GroupBox {
                id: _groupboxSend
                title: qsTr("GCS Send Trace")

                Column{
                    spacing:        ScreenTools.defaultFontPixelWidth

                    Row{
                        spacing:        ScreenTools.defaultFontPixelWidth

                        ComboBox{
                            id   : _comboboxSend
                            width: _comboboxWidth
                            model: controller.idListSend
                            onCurrentTextChanged: {
                                controller.setsendMavIDChoose(_comboboxSend.currentText)
                            }
                        }

                        Label{
                            width : _comboboxSend.width
                            height: _comboboxSend.height
                            text  : controller.sendHz
                        }
                    }

                    Label{
                        text:   qsTr("HEX")
                    }

                    TextArea {
                        id    : _textareaSend
                        width : _textareaWidth
                        height: _textareaHeight
                        text  : controller.sendMessageHex
                    }
                }
            }

            GroupBox {
                id: _groupboxReceive
                title: qsTr("GCS Receive Trace")

                Column{
                    spacing:        ScreenTools.defaultFontPixelWidth

                    Row{
                        spacing:        ScreenTools.defaultFontPixelWidth

                        ComboBox{
                            id   : _comboboxReceive
                            width: _comboboxWidth
                            model: controller.idListReceive
                            onCurrentTextChanged: {
                                controller.setrcvMavIDChoose(_comboboxReceive.currentText)
                            }
                        }

                        Label{
                            width : _comboboxReceive.width
                            height: _comboboxReceive.height
                            text  : controller.rcvHz
                        }
                    }

                    Label{
                        text:   qsTr("HEX")
                    }

                    TextArea {
                        id    : _textareaReceiveHEX
                        width : _textareaWidth
                        height: _textareaHeight
                        text  : controller.rcvMessageHex
                    }
                }
            }
        }
    }
}
