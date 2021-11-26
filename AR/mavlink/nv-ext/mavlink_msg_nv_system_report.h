#pragma once

// MESSAGE MAVLINK NextVision Extension System Report

#define MAVLINK_MSG_NV_EXT_SYSTEM_REPORT 248

MAVPACKED(
    typedef struct __mavlink_nv_ext_system_report_t {
        uint16_t            report_type;
        float               roll;
        float               pitch;
        float               fov;
        uint8_t             tracker_status;
        uint8_t             recording_status;
        uint8_t             sensor;
        uint8_t             polarity;
        uint8_t             mode;
        uint8_t             laser_status;
        int16_t             tracker_roi_x;
        int16_t             tracker_roi_y;
        float               single_yaw_cmd;

        // Added in v3.3
        uint8_t             snapshot_status;
    }) mavlink_nv_ext_system_report_t;

#define MAVLINK_MSG_NV_EXT_SYSTEM_REPORT_LEN 29
#define MAVLINK_MSG_NV_EXT_SYSTEM_REPORT_MIN_LEN 28
#define MAVLINK_MSG_NV_EXT_SYSTEM_REPORT_CRC 8

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_NV_EXT_SYSTEM_REPORT { \
    248, \
    "V2_EXTENSION", \
    5, \
    {  { "target_network", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_v2_extension_t, target_network) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_v2_extension_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_v2_extension_t, target_component) }, \
         { "message_type", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_v2_extension_t, message_type) }, \
         { "payload", NULL, MAVLINK_TYPE_UINT8_T, 249, 5, offsetof(mavlink_v2_extension_t, payload) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_NV_EXT_SYSTEM_REPORT { \
    "NVEXT_SYS_REPORT", \
    13, \
    {  { "report_type", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_nv_ext_system_report_t, report_type) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 2, offsetof(mavlink_nv_ext_system_report_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 6, offsetof(mavlink_nv_ext_system_report_t, pitch) }, \
         { "fov", NULL, MAVLINK_TYPE_FLOAT, 0, 10, offsetof(mavlink_nv_ext_system_report_t, fov) }, \
         { "tracker_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_nv_ext_system_report_t, tracker_status) }, \
         { "recording_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_nv_ext_system_report_t, recording_status) }, \
         { "sensor", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_nv_ext_system_report_t, sensor) }, \
         { "polarity", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_nv_ext_system_report_t, polarity) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_nv_ext_system_report_t, mode) }, \
         { "laser_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_nv_ext_system_report_t, laser_status) }, \
         { "tracker_roi_x", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_nv_ext_system_report_t, tracker_roi_x) }, \
         { "tracker_roi_y", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_nv_ext_system_report_t, tracker_roi_y) }, \
         { "single_yaw_cmd", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_nv_ext_system_report_t, single_yaw_cmd) }, \
         } \
}
#endif


// MESSAGE MAVLINK_MESSAGE_INFO_NV_EXT_SYSTEM_REPORT UNPACKING


/**
 * @brief Get field report_type from mavlink_nv_ext_system_report message
 *
 * @return Report type
 */
static inline uint16_t mavlink_nv_ext_system_report_get_report_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field roll from mavlink_nv_ext_system_report message
 *
 * @return Roll
 */
static inline float mavlink_nv_ext_system_report_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  2);
}

/**
 * @brief Get field pitch from mavlink_nv_ext_system_report message
 *
 * @return Pitch
 */
static inline float mavlink_nv_ext_system_report_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  6);
}

/**
 * @brief Get field fov from mavlink_nv_ext_system_report message
 *
 * @return FOV
 */
static inline float mavlink_nv_ext_system_report_get_fov(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  10);
}

/**
 * @brief Get field tracker_status from mavlink_nv_ext_system_report message
 *
 * @return The Tracker Status
 */
static inline uint8_t mavlink_nv_ext_system_report_get_tracker_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field recording_status from mavlink_nv_ext_system_report message
 *
 * @return The Recording Status
 */
static inline uint8_t mavlink_nv_ext_system_report_get_recording_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Get field sensor from mavlink_nv_ext_system_report message
 *
 * @return Sensor type
 */
static inline uint8_t mavlink_nv_ext_system_report_get_sensor(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field polarity from mavlink_nv_ext_system_report message
 *
 * @return Polarity mode
 */
static inline uint8_t mavlink_nv_ext_system_report_get_polarity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field mode from mavlink_nv_ext_system_report message
 *
 * @return Mode
 */
static inline uint8_t mavlink_nv_ext_system_report_get_mode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field laser_status from mavlink_nv_ext_system_report message
 *
 * @return laser_status
 */
static inline uint8_t mavlink_nv_ext_system_report_get_laser_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field tracker_roi_x from mavlink_nv_ext_system_report message
 *
 * @return tracker_roi_x
 */
static inline int16_t mavlink_nv_ext_system_report_get_tracker_roi_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Get field tracker_roi_y from mavlink_nv_ext_system_report message
 *
 * @return tracker_roi_y
 */
static inline int16_t mavlink_nv_ext_system_report_get_tracker_roi_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  22);
}

/**
 * @brief Get field single_yaw_cmd from mavlink_nv_ext_system_report message
 *
 * @return single_yaw_cmd
 */
static inline float mavlink_nv_ext_system_report_get_single_yaw_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field snapshot_status from mavlink_nv_ext_system_report message
 *
 * @return snapshot_status
 */
static inline uint8_t mavlink_nv_ext_system_report_get_snapshot_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  28);
}

/**
 * @brief Decode a v2_extension message into a struct
 *
 * @param msg The message to decode
 * @param system_report C-struct to decode the message contents into
 */
static inline void mavlink_nv_ext_system_report_decode(
    const mavlink_message_t* msg, mavlink_nv_ext_system_report_t* system_report)
{
    system_report->report_type = mavlink_nv_ext_system_report_get_report_type(msg);
    system_report->roll = mavlink_nv_ext_system_report_get_roll(msg);
    system_report->pitch = mavlink_nv_ext_system_report_get_pitch(msg);
    system_report->fov = mavlink_nv_ext_system_report_get_fov(msg);
    system_report->tracker_status = mavlink_nv_ext_system_report_get_tracker_status(msg);
    system_report->recording_status = mavlink_nv_ext_system_report_get_recording_status(msg);
    system_report->sensor = mavlink_nv_ext_system_report_get_sensor(msg);
    system_report->polarity = mavlink_nv_ext_system_report_get_polarity(msg);
    system_report->mode = mavlink_nv_ext_system_report_get_mode(msg);
    system_report->laser_status = mavlink_nv_ext_system_report_get_laser_status(msg);
    system_report->tracker_roi_x = mavlink_nv_ext_system_report_get_tracker_roi_x(msg);
    system_report->tracker_roi_y = mavlink_nv_ext_system_report_get_tracker_roi_y(msg);
    system_report->single_yaw_cmd = mavlink_nv_ext_system_report_get_single_yaw_cmd(msg);

    if (msg->len > MAVLINK_MSG_NV_EXT_SYSTEM_REPORT_MIN_LEN) {
        system_report->snapshot_status = mavlink_nv_ext_system_report_get_snapshot_status(msg);
    }
}
