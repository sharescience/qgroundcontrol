/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include "MavlinkTest.h"

MavlinkTest::MavlinkTest(const QString& title, QAction* action, QWidget *parent) :
    QGCQmlWidgetHolder(title, action, parent)
{
    Q_UNUSED(title);
    Q_UNUSED(action);
    int screenWidth=QApplication::desktop()->width();
    int screenHeight=QApplication::desktop()->height();
    this->resize(screenWidth*8/15, screenHeight*4/5);
    setSource(QUrl::fromUserInput("qrc:/qml/MavlinkTest.qml"));
    loadSettings();
}
