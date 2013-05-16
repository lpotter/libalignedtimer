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

#include "alignedtimer.h"

#include "qalignedtimer_linux_p.h"


//#ifdef QT_SIMULATOR
//Q_GLOBAL_STATIC(QAlignedTimerPrivate, alignedTimerPrivate)
//QAlignedTimerPrivate *getSystemAlignedTimerPrivate() { return alignedTimerPrivate(); }
//#endif // QT_SIMULATOR

/*!
    \class QAlignedTimer
    \brief The QAlignedTimer class provides a service for applications to synchronize their activity.

    QAlignedTimer is a fuzzy timer that allows applications that must do periodic activity like
    after being in sleep mode a certain period, to synchronize their activities in the same window of time.

    It is intended to be a drop-in replacement for QTimer.

    For example send network "alive" messages at the same time (i.e. turn the wireless radio on at the same time).

    The service is not only for network-aware applications, it is for use by any applications
    that need to periodic wake-ups.

    The recommended use case is when app uses single-shot timer only: set interval 0 for the first call
    'to jump to the train' and interval > 0 after 1st wakeup.
*/

/*!
    \fn void QAlignedTimer::timeout()

    This signal is emitted when the timer times out.
*/

/*!
    \fn void QAlignedTimer::error(QAlignedTimer::AlignedTimerError error)

        This signal is emitted when an \a error happens.
*/

/*!
    \enum QAlignedTimer::AlignedTimerError
    This enum describes the last known AlignedTimerError

    \value NoError                        No error.
    \value AlignedTimerNotSupported       The aligned timer is not support on this platform
    \value InvalidArgument                Interval arguments are invalid.
    \value TimerFailed                    General timer failure.
    \value InternalError                  Internal error.
*/

/*!
    Constructs a QAlignedTimer object with the given \a parent.
*/
QAlignedTimer::QAlignedTimer(QObject *parent)
    : QObject(parent)
{
    d = new QAlignedTimerPrivate(this);
    connect(d, SIGNAL(timeout()), this, SIGNAL(timeout()));
    connect(d, SIGNAL(error(QAlignedTimer::AlignedTimerError)), this, SIGNAL(error(QAlignedTimer::AlignedTimerError)));
}

/*!
    Destructs the QAlignedTimer
*/
QAlignedTimer::~QAlignedTimer()
{
}

/*!
    Starts the timer with the minimal interval of \a minimumTime, and maximum interval \a maximumTime in seconds.

    This is not a guaranteed interval, and the timeout signal may be fired at any time,
    depending on other clients attached to this timer.

    In the case of minimalInterval of 0, it means 'wake me up when someone else is woken up'.

    If you need a window of time in which your timer should fire, use QSystemAlignedTimer::setWindow
*/
void QAlignedTimer::start(int interval, int windowTime)
{
    if (interval <= 0) {
        d->m_lastError = QAlignedTimer::InvalidArgument;
        Q_EMIT error(d->m_lastError);
        return;
    }
    d->start(interval, windowTime);
}

/*!
    Starts the alignedtimer.
*/
void QAlignedTimer::start()
{
    if (windowOfTime <= 0) {
        d->m_lastError = QAlignedTimer::InvalidArgument;
        Q_EMIT error(d->m_lastError);
        return;
    }
    d->start();
}

/*!
    This should be called when the application wakes up via other means than QAlignedTimer timeout.

    Other applications that are in their wakeup window *may* be woken up. Single-shot timer is canceled,
    and reoccuring timer interval will restart.
*/
void QAlignedTimer::wokeUp()
{
    d->wokeUp();
}

/*!
    Stops this timer request.
*/
void QAlignedTimer::stop()
{
    d->stop();
}

/*!
    Set the preferred timeout interval to \a seconds in seconds that should be waited before timeout
    signal is emitted. This is not a hard interval, meaning along with the timerWindow

    Value 0 means 'wake me up when someboy else is woken'.

    seconds value 0 should be used with special care, as it may cause too frequent wakeups.
    It is recommended that the first wait (if possible) uses seconds value of 0 to
    "jump to the train" and seconds value > 0 after 1st wakeup
*/
void QAlignedTimer::setInterval(int seconds)
{
    d->setInterval(seconds);
}

/*!
    \property QAlignedTimer::interval
    \brief The timers's interval.

    Returns this current timer interval.
*/
int QAlignedTimer::interval() const
{
    return d->interval();
}

/*!
    Sets the timer's timeout window to \a seconds.

    The timeout window is a window of time set around the interval in which the timer will timeout.

    It is wise to have timeout window quite big so all users of this service get synced.

    For example if your preferred wait is 120 seconds and you can wait anywhere within 10 seconds,
    use interval of 120 and timerWindow of 10. This means the timer will timeout anywhere from
    115 seconds to 125 seconds.

*/
void QAlignedTimer::setTimerWindow(int seconds)
{
    d->setTimerWindow(seconds);
}

/*!
    \property QAlignedTimer::timerWindow
    \brief The timer's timerWindow.

    Returns this current timer window interval.
*/
int QAlignedTimer::timerWindow() const
{
    return d->timerWindow();
}

/*!
    Sets this timer to be a single shot \a singleShot is true, otherwise false.
*/
void QAlignedTimer::setSingleShot(bool singleShot)
{
    d->setSingleShot(singleShot);
}

/*!
    This static function starts a timer to call a slot around \a interval
    interval has elapsed, and ensures that it will be called within the
    \a timeWindow amount of time.

    These values are specified in seconds. Default timeWindow is 120 seconds

    The receiver is the \a receiver object and the \a member is the slot.
*/
void QAlignedTimer::singleShot(int interval, QObject *receiver, const char *member, int timeWindow)
{
    if (interval >  timeWindow || timeWindow <= 0)
        return;
    QAlignedTimerPrivate::singleShot(interval, receiver, member, timeWindow);
}

/*!
    \property QAlignedTimer::singleShot
    Whether the timer is single shot.
*/

/*!
    Returns true if this timer is a single shot, otherwise false.
*/
bool QAlignedTimer::isSingleShot() const
{
    return d->isSingleShot();
}

/*!
    Returns the last AlignedTimerError.
*/
QAlignedTimer::AlignedTimerError QAlignedTimer::lastError() const
{
    return d->lastError();
}

/*!
    \property QAlignedTimer::active
    Returns true if the timer is running; otherwise false.
*/

/*!
    Returns true if the timer is running; otherwise false.
*/
bool QAlignedTimer::isActive () const
{
    return d->isActive();
}

#include "moc_alignedtimer.cpp"
