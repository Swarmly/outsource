#include "enum_mapping.h"
#include "geo/geo_mgrs.h"
//#include "variablemanager.h"
#include "video/videofiltergmb.h"

namespace {
const std::array<const gtu::NameValuePair<MavlinkNVSystemMode>, 13> MavlinkNVSystemModes{{
    {MavlinkNVSystemMode::NVSysModeStow, "STOW"},
    {MavlinkNVSystemMode::NVSysModePilot, "PILOT"},
    {MavlinkNVSystemMode::NVSysModeRetract, "PARK"},
    {MavlinkNVSystemMode::NVSysModeRetractLock, "LOCK"},
    {MavlinkNVSystemMode::NVSysModeObservation, "OBS"},
    {MavlinkNVSystemMode::NVSysModeGRR, "GRR"},
    {MavlinkNVSystemMode::NVSysModeHoldCoordinate, "HOLD"},
    {MavlinkNVSystemMode::NVSysModePointToCoordinate, "POINT"},
    {MavlinkNVSystemMode::NVSysModeLocalPosition, "L.POS"},
    {MavlinkNVSystemMode::NVSysModeGlobalPosition, "G.POS"},
    {MavlinkNVSystemMode::NVSysModeGlobalTrack, "TRACK"},
    {MavlinkNVSystemMode::NVSysModeExtendedPitchRange, "EPR"},
    {MavlinkNVSystemMode::NVSysModeBIT, "BIT"},
}};
}

GimbalStatOSDVideoFilter::GimbalStatOSDVideoFilter(QObject *parent)
    : BaseOSDVideoFilter{parent}
    , coordFormat_{OSDCoordinationFormatMGRS}
{
    memset(&system_report, 0, sizeof(system_report));
    memset(&ground_crossing_report, 0, sizeof(ground_crossing_report));
}

void GimbalStatOSDVideoFilter::onMetaInfo(const SampleMetaInfo &metaInfo)
{
    if (metaInfo.hasProp("mavlink_nv_ext_system_report_t")) {
        system_report = metaInfo.getProp("mavlink_nv_ext_system_report_t", system_report);
    }
    if (metaInfo.hasProp("mavlink_nv_ext_ground_crossing_report_t")) {
        ground_crossing_report = metaInfo.getProp("mavlink_nv_ext_ground_crossing_report_t",
                                                  ground_crossing_report);
    }
    //    if (metaInfo.hasProp("mavlink_nv_ext_system_report_t")) {
    //        system_report = metaInfo.getProp("mavlink_nv_ext_system_report_t", system_report);
    //    }
}

void GimbalStatOSDVideoFilter::paintOSD(QPainter *painter) {
    QOSDVerticalLayout secondary_layout;
    QString text;

    // System mode
    text = "Mode:"
           + QString::fromStdString(
               gtu::get_name_for_value(MavlinkNVSystemModes, system_report.mode));
    if (system_report.mode == MavlinkNVSystemMode::NVSysModeStow
        || system_report.mode == MavlinkNVSystemMode::NVSysModePilot
        || system_report.mode == MavlinkNVSystemMode::NVSysModeRetract) {
        secondary_layout.addText(text, Qt::GlobalColor::red);
    } else {
        secondary_layout.addText(text);
    }

    if (OSDCoordinationFormat::OSDCoordinationFormatMGRS == coordFormat_) {
        // Ground Crossing Latitude
        text = "T.MGRS:";
        secondary_layout.addText(text);
        text = "  " + QString::fromStdString(gtu::geo::MGRS(
           ground_crossing_report.gnd_crossing_lat, ground_crossing_report.gnd_crossing_lon
       ).coord());
        secondary_layout.addText(text);
    } else {
        // Ground Crossing Latitude
        text = "T.Lat:";
        secondary_layout.addText(text);
        text = "  " + QString::number(ground_crossing_report.gnd_crossing_lat, 'f', 8);
        secondary_layout.addText(text);

        // Ground Crossing Longitude
        text = "T.Lon:";
        secondary_layout.addText(text);
        text = "  " + QString::number(ground_crossing_report.gnd_crossing_lon, 'f', 8);
        secondary_layout.addText(text);
    }

    // Ground Crossing Altitude
    text = "T.Alt:";
    secondary_layout.addText(text);
    text = "  " + QString::number(ground_crossing_report.gnd_crossing_alt, 'f', 2);
    secondary_layout.addText(text);

    // Slant Range
    text = "SR : " + QString::number(ground_crossing_report.slant_range, 'f', 2) + " m";
    secondary_layout.addText(text);

    // FOV
    text = "FOV: " + QString::number(system_report.fov, 'f', 2);
    secondary_layout.addText(text);

    // Recording status
    text = "";
    if (system_report.recording_status == MavlinkNVRecordingStatus::NVRecordingStatusActive) {
        text = "REC";
    }

    if (system_report.recording_status == MavlinkNVRecordingStatus::NVRecordingStatusDisabled) {
        text = "NO SD Card";
    }

    if (system_report.snapshot_status == MavlinkNVSnapshotStatus::NVSnapshotStatusBusy) {
        if (!text.size()) {
            text += "   ";
        }
        text += "   SNAP";
    }

    if (system_report.recording_status == MavlinkNVRecordingStatus::NVRecordingStatusDisabled
        || system_report.recording_status == MavlinkNVRecordingStatus::NVRecordingStatusActive
        || system_report.snapshot_status == MavlinkNVSnapshotStatus::NVSnapshotStatusBusy) {
        secondary_layout.addText(text, Qt::GlobalColor::red);
    }

    // paint system stats
    QRect system_rect = secondary_layout.boundingRect();
    secondary_layout.paint(painter->window().width() - std::max(160, system_rect.width()),
                           20,
                           painter);
}
