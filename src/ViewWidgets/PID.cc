/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "PID.h"

PID::PID(const QString& title, QAction* action, QWidget *parent) :
    QGCQmlWidgetHolder(title, action, parent)
{
    Q_UNUSED(title);
    Q_UNUSED(action);

    // Adjust the size
    QScreen* scr = QApplication::primaryScreen();
    QSize scrSize = scr->availableSize();
    int screenWidth=scrSize.width();
    int screenHeight=scrSize.height();
    this->resize(screenWidth*1/5, screenHeight*1/2);

    setSource(QUrl::fromUserInput("qrc:/qml/PID.qml"));
    loadSettings();
}
