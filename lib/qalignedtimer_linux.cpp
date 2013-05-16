/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Copyright 2013 Jolla Ltd.
** Contact: http://www.qt-project.org/legal
** COntact: Lorn Potter <lorn.potter@jollamobile.com>
**
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

#include "alignedtimer.h"
#include "qalignedtimer_linux_p.h"
#include <QDebug>

#include <errno.h>
#include <stdio.h>


QAlignedTimerPrivate::QAlignedTimerPrivate(QObject *parent)
    : QObject(parent)
    , m_lastError(QAlignedTimer::NoError)
    , m_interval(0)
    , m_timerWindow(0)
    , m_running(false)
    , m_singleShot(false)
    , m_iphbdHandler(0)
    , m_notifier(0)
{
    m_iphbdHandler = iphb_open(0);

    if (!m_iphbdHandler) {
        m_lastError = QAlignedTimer::InternalError;
        qDebug() << "iphb_open error" << m_iphbdHandler<< errno <<strerror(errno);
        return;
    }

    int sockfd = iphb_get_fd(m_iphbdHandler);
    if (!(sockfd > -1)) {
        m_lastError = QAlignedTimer::InternalError;
        qDebug() << "socket failure"<<strerror(errno);
        return;
    }

    m_notifier = new QSocketNotifier(sockfd, QSocketNotifier::Read);
    if (!QObject::connect(m_notifier, SIGNAL(activated(int)), this, SLOT(heartbeatReceived(int)))) {
        delete m_notifier, m_notifier = 0;
        m_lastError = QAlignedTimer::TimerFailed;
        qDebug() << "timer failure";
        return;
    }
    m_notifier->setEnabled(false);
}

QAlignedTimerPrivate::~QAlignedTimerPrivate()
{
    if (m_iphbdHandler)
        (void)iphb_close(m_iphbdHandler);

    if (m_notifier)
        delete m_notifier;
}

void QAlignedTimerPrivate::wokeUp()
{
    if (!m_running)
        return;

    if (!(m_iphbdHandler && m_notifier)) {
        m_lastError = QAlignedTimer::InternalError;
        emit error(m_lastError);
        return;
    }

    m_notifier->setEnabled(false);

    (void)iphb_I_woke_up(m_iphbdHandler);

    m_running = false;
    m_lastError = QAlignedTimer::NoError;

    if (!m_singleShot)
        start();
}

int QAlignedTimerPrivate::interval() const
{
    return m_interval;
}

void QAlignedTimerPrivate::setInterval(int seconds)
{
    m_interval = seconds;
}

int QAlignedTimerPrivate::timerWindow() const
{
    return m_timerWindow;
}

void QAlignedTimerPrivate::setTimerWindow(int seconds)
{
    m_timerWindow = seconds;
}

void QAlignedTimerPrivate::setSingleShot(bool singleShot)
{
    m_singleShot = singleShot;
}

bool QAlignedTimerPrivate::isSingleShot() const
{
    return m_singleShot;
}

void QAlignedTimerPrivate::singleShot(int interval, QObject *receiver, const char *member,  int timeWindow)
{
    if (receiver && member) {
        QAlignedTimerPrivate *alignedTimer = new QAlignedTimerPrivate(receiver);
        alignedTimer->m_singleShot = true;

        connect(alignedTimer, SIGNAL(timeout()), receiver, member);
        alignedTimer->start(interval, timeWindow);
    }
}

QAlignedTimer::AlignedTimerError QAlignedTimerPrivate::lastError() const
{
    return m_lastError;
}

void QAlignedTimerPrivate::start(int interval, int timeWindow)
{
    m_interval = interval;
    m_timerWindow = timeWindow;

    start();
}

void QAlignedTimerPrivate::start()
{
    if (m_running)
        return;

    if (!(m_iphbdHandler && m_notifier)) {
        m_lastError = QAlignedTimer::InternalError;
        emit error(m_lastError);
        return;
    }

    int mustWait = 0;
    time_t unixTime = iphb_wait(m_iphbdHandler, m_interval - (m_timerWindow * .5)
                                , m_interval + (m_timerWindow * .5) , mustWait);

    if (unixTime == (time_t)-1) {
        m_lastError = QAlignedTimer::TimerFailed;
        emit error(m_lastError);
        return;
    }

    m_notifier->setEnabled(true);
    m_running = true;
    m_lastError = QAlignedTimer::NoError;
}

void QAlignedTimerPrivate::stop()
{
    if (!m_running)
        return;

    if (!(m_iphbdHandler && m_notifier)) {
        m_lastError = QAlignedTimer::InternalError;
        emit error(m_lastError);
        return;
    }

    m_notifier->setEnabled(false);

    (void)iphb_discard_wakeups(m_iphbdHandler);

    m_running = false;
    m_lastError = QAlignedTimer::NoError;
}

void QAlignedTimerPrivate::heartbeatReceived(int sock) {
    Q_UNUSED(sock);

    stop();
    emit timeout();

    if (!m_singleShot)
        start();
}

bool QAlignedTimerPrivate::isActive() const
{
    return m_running;
}

#include "moc_qalignedtimer_linux_p.cpp"
