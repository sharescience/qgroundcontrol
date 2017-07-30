/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef MavlinkDebug_H
#define MavlinkDebug_H

#include "QGCQmlWidgetHolder.h"

class MavlinkDebug : public QGCQmlWidgetHolder
{
    Q_OBJECT
	
public:
    MavlinkDebug(const QString& title, QAction* action, QWidget *parent = 0);
};

#endif
