#include "timeouttimer.h"
#include "coreutils.h"
#include "datasample.h"

TimeOutTimer::TimeOutTimer(int ms)
    : m_timeout(ms)
{
    m_lastResetTime = GetCurrentTimeStampMs();
    m_lastResetTime2 = GetCurrentTimeStampMs();
}

void TimeOutTimer::RestartTimer()
{
    m_lastResetTime = GetCurrentTimeStampMs();
}

void TimeOutTimer::RestartFullTimer()
{
    RestartTimer();
    m_lastResetTime2 = m_lastResetTime;
}

bool TimeOutTimer::IsElapsed()
{
    if (m_timeout <= 0) {
        return false;
    }
    return (GetCurrentTimeStampMs() - m_lastResetTime) >= m_timeout;
}

bool TimeOutTimer::IsSecondElapsed()
{
    if (m_timeout2 <= 0) {
        return false;
    }
    return (GetCurrentTimeStampMs() - m_lastResetTime2) >= m_timeout2;
}

void TimeOutTimer::setElapsed()
{
    m_lastResetTime = 0;
    m_lastResetTime2 = 0;
}

void TimeOutTimer::SetNewTimeout(int ms)
{
    m_timeout = ms;
}

void TimeOutTimer::SetSecondTimeout(int ms)
{
    m_timeout2 = ms;
}

int TimeOutTimer::getTimeout()
{
    return m_timeout;
}
