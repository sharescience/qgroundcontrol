/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "MavlinkDebug.h"

MavlinkDebug::MavlinkDebug(const QString& title, QAction* action, QWidget *parent) :
    QGCQmlWidgetHolder(title, action, parent)
{
    Q_UNUSED(title);
    Q_UNUSED(action);

    // Adjust the size
    QScreen* scr = QApplication::primaryScreen();
    QSize scrSize = scr->availableSize();
    int screenWidth=scrSize.width();
    int screenHeight=scrSize.height();
    this->resize(screenWidth*1/2, screenHeight*2/3);

    setSource(QUrl::fromUserInput("qrc:/qml/MavlinkDebug.qml"));
    loadSettings();
}
