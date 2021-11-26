#pragma once
#include "gui_tools.h"
#include "rxquality.h"
#include "samplemetainfo.h"
#include <QtAV/Filter.h>

class BaseOSDVideoFilter : public QtAV::VideoFilter
{
public:
    BaseOSDVideoFilter(QObject *parent);
    bool isSupported(QtAV::VideoFilterContext::Type ct) const override;
    virtual void paintOSD(QPainter *);

public slots:
    virtual void onMavlinkMessage(const QMap<int, mavlink_message_t> &messages);
    virtual void onMetaInfo(const SampleMetaInfo &metaInfo);
    virtual void setRxStat(const signalStatus &stat);

protected:
    void process(QtAV::Statistics *statistics, QtAV::VideoFrame *frame) override;

private:
    bool isReady();
};
