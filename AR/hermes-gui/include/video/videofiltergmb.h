#pragma once
#include "hermes.h"
#include "videofilterbase.h"
#include "videofiltervertical.h"

class GimbalStatOSDVideoFilter : public BaseOSDVideoFilter
{
public:
    typedef enum {
        OSDCoordinationFormatWGS84,
        OSDCoordinationFormatMGRS
    } OSDCoordinationFormat;

    GimbalStatOSDVideoFilter(QObject *parent);

    void setCoordFormat(OSDCoordinationFormat coordFormat) { coordFormat_ = coordFormat; }

public slots:
    void onMetaInfo(const SampleMetaInfo &metaInfo) override;

protected:
    void paintOSD(QPainter *painter) override;

private:
    mavlink_nv_ext_ground_crossing_report_t ground_crossing_report;
    mavlink_nv_ext_system_report_t system_report;

    OSDCoordinationFormat coordFormat_;
};
