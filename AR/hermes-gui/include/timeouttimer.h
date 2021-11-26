#pragma once
#include "gtu-core_global.h"

class GTUCORESHARED_EXPORT TimeOutTimer
{
public:
    TimeOutTimer(int ms = 1000); // 1 sec
    //! reset first timer
    void RestartTimer();
    //!reset first and second timer
    void RestartFullTimer();
    bool IsElapsed();
    bool IsSecondElapsed();
    void SetNewTimeout(int ms);
    void SetSecondTimeout(int ms);
    void setElapsed();
    int getTimeout();

private:
    quint64 m_lastResetTime = 0;
    quint64 m_lastResetTime2 = 0;
    quint64 m_timeout = -1;
    quint64 m_timeout2 = -1;
};
