/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: lorn.potter@jollamobile.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qalignedtimer_plugin.h"
#include "alignedtimer.h"

#include <qqml.h>

void QAlignedTimerPlugin::registerTypes(const char *uri)
{
    // @uri com.jolla.alignedtimer
    qmlRegisterType<QAlignedTimer>(uri, 1, 0, "AlignedTimer");
}


