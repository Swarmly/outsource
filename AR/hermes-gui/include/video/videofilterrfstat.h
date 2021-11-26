#pragma once
#include "videofilterbase.h"

class VideoFilterRFStat : public BaseOSDVideoFilter
{
public:
    VideoFilterRFStat(QObject *parent);

public slots:
    void setRxStat(const signalStatus &stat) override;

protected:
    void paintOSD(QPainter *painter) override;

private:
    signalStatus m_RXStatus_A;
    signalStatus m_RXStatus_B;
};
