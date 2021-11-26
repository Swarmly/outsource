#pragma once

namespace mavlink {

/**
 * Mavlink NextVision Extension: Observation System Report Type
 */
typedef enum {
    NextVisionSystemReport = 0,
    NextVisionLineOfSightReport,
    NextVisionGroundCrossingReport
} MavlinkNextVisionReportType;

/**
 * System report: System mode
 */
typedef enum {
    NVSysModeStow,
    NVSysModePilot,
    NVSysModeRetract,
    NVSysModeRetractLock,
    NVSysModeObservation,
    NVSysModeGRR,
    NVSysModeHoldCoordinate,
    NVSysModePointToCoordinate,
    NVSysModeLocalPosition,
    NVSysModeGlobalPosition,
    NVSysModeGlobalTrack,
    NVSysModeExtendedPitchRange,
    NVSysModeBIT
} MavlinkNVSystemMode;

/**
 * System report: Snapshot status.
 */
typedef enum {
    NVSnapshotStatusIdle,
    NVSnapshotStatusBusy
} MavlinkNVSnapshotStatus;


/**
 * System report: Recording status.
 */
typedef enum {
    //! Idle, not recording. SD card is detected.
    NVRecordingStatusIdle,

    //! Active, video is being recorded. SD card is detected.
    NVRecordingStatusActive,

    //! Disabled, when SD card is not detected.
    NVRecordingStatusDisabled
} MavlinkNVRecordingStatus;

/**
 * System report: Tracker status
 */
typedef enum {
    //! Tracker is Idle, ready for all tracking commands.
    NVTrackerStatusIdle,

    //! Crosshair is displayed, ready for track on command.
    NVTrackerStatusReadyForTrack,

    //! Tracker is actively tracking an object.
    NVTrackerStatusReadyActive,

    //! Re-Track crosshair is displayed, movable by the user, ready for track on command.
    NVTrackerStatusReadyReTrack,

    //! Tracker is actively tracking an object.
    NVTrackerStatusReadyActive1,

    //! Tracker is actively tracking an object.
    NVTrackerStatusReadyActive2,
} MavlinkNVTrackerStatus;

}