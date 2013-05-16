/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Copyright 2013 Jolla Ltd.
** Contact: http://www.qt-project.org/legal
** COntact: Lorn Potter <lorn.potter@jollamobile.com>
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ALIGNEDTIMER_H
#define ALIGNEDTIMER_H

#include <QObject>
#include "libalignedtimer_global.h"

class QAlignedTimerPrivate;

class LIBALIGNEDTIMERSHARED_EXPORT QAlignedTimer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int interval READ interval WRITE setInterval)
    Q_PROPERTY(int timerWindow READ timerWindow WRITE setTimerWindow)
    Q_PROPERTY(bool singleShot READ isSingleShot WRITE setSingleShot)
    Q_PROPERTY(bool active READ isActive)

public:
    explicit QAlignedTimer(QObject *parent = 0);
    ~QAlignedTimer();

    enum AlignedTimerError {
        NoError = 0,
        AlignedTimerNotSupported,
        InvalidArgument,
        TimerFailed,
        InternalError
    };

    Q_INVOKABLE void wokeUp();

    int interval() const;
    void setInterval(int seconds);

    int timerWindow() const;
    void setTimerWindow(int seconds);

    void setSingleShot(bool single);
    bool isSingleShot() const;

    Q_INVOKABLE static void singleShot(int interval, QObject *receiver, const char *member, int timerWindow = 120);

    AlignedTimerError lastError() const;
    bool isActive () const;

public Q_SLOTS:
    void start(int interval, int timerWindow = 120);
    void start();
    void stop();

Q_SIGNALS:
    void timeout();
    void error(QAlignedTimer::AlignedTimerError error);

private:
    QAlignedTimerPrivate *d;
};


//QML_DECLARE_TYPE(AlignedTimer)

#endif // ALIGNEDTIMER_H

