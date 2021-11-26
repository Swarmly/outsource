#include "video/videofilterbase.h"

BaseOSDVideoFilter::BaseOSDVideoFilter(QObject *parent)
: VideoFilter(parent)
    {}

void BaseOSDVideoFilter::process(QtAV::Statistics * /*statistics*/, QtAV::VideoFrame * /*frame*/)
{
    if (isReady()) {
        auto painter = context()->painter;
        painter->save();
        paintOSD(painter);
        painter->restore();
    }
}

void BaseOSDVideoFilter::onMavlinkMessage(const QMap<int, mavlink_message_t> & /*messages*/) {}

void BaseOSDVideoFilter::setRxStat(const signalStatus & /*stat*/) {}
void BaseOSDVideoFilter::onMetaInfo(const SampleMetaInfo & /*metaInfo*/) {}

bool BaseOSDVideoFilter::isReady()
{
    return context()->painter != nullptr && context()->painter->isActive();
}

bool BaseOSDVideoFilter::isSupported(QtAV::VideoFilterContext::Type) const
{
    return true;
}
void BaseOSDVideoFilter::paintOSD(QPainter *) {}
