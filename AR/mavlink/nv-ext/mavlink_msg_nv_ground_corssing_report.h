#pragma once

// MESSAGE MAVLINK NextVision Extension Ground Cross Report

#define MAVLINK_MSG_NV_EXT_GND_CRS_REPORT 248

MAVPACKED(
    typedef struct __mavlink_nv_ext_ground_crossing_report_t {
        uint16_t            report_type;
        float               gnd_crossing_lat;
        float               gnd_crossing_lon;
        float               gnd_crossing_alt;
        float               slant_range;
    }) mavlink_nv_ext_ground_crossing_report_t;

#define MAVLINK_MSG_NV_EXT_GND_CRS_REPORT_LEN 18
#define MAVLINK_MSG_NV_EXT_GND_CRS_REPORT_MIN_LEN 18
#define MAVLINK_MSG_NV_EXT_GND_CRS_REPORT_CRC 8

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_NV_EXT_GND_CRS_REPORT { \
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
#define MAVLINK_MESSAGE_INFO_NV_EXT_GND_CRS_REPORT { \
    "NVEXT_GND_CRS_REPORT", \
    5, \
    {  { "report_type", NULL, MAVLINK_TYPE_UINT16_T, 0, 0, offsetof(mavlink_nv_ext_ground_crossing_report_t, report_type) }, \
         { "gnd_crossing_lat", NULL, MAVLINK_TYPE_FLOAT, 0, 2, offsetof(mavlink_nv_ext_ground_crossing_report_t, gnd_crossing_lat) }, \
         { "gnd_crossing_lon", NULL, MAVLINK_TYPE_FLOAT, 0, 6, offsetof(mavlink_nv_ext_ground_crossing_report_t, gnd_crossing_lon) }, \
         { "gnd_crossing_alt", NULL, MAVLINK_TYPE_FLOAT, 0, 10, offsetof(mavlink_nv_ext_ground_crossing_report_t, gnd_crossing_alt) }, \
         { "slant_range", NULL, MAVLINK_TYPE_FLOAT, 0, 14, offsetof(mavlink_nv_ext_ground_crossing_report_t, slant_range) }, \
         } \
}
#endif


// MESSAGE MAVLINK_MSG_NV_EXT_GND_CRS_REPORT UNPACKING


/**
 * @brief Get field report_type from mavlink_nv_ext_ground_crossing_report message
 *
 * @return Report type
 */
static inline uint16_t mavlink_nv_ext_ground_crossing_report_get_report_type(
    const mavlink_message_t* msg
)
{
    return _MAV_RETURN_uint16_t(msg,  0);
}

/**
 * @brief Get field gnd_crossing_lat from mavlink_nv_ext_ground_crossing_report message
 *
 * @return gnd_crossing_lat
 */
static inline float mavlink_nv_ext_ground_crossing_report_get_gnd_crs_lat(
    const mavlink_message_t* msg
)
{
    return _MAV_RETURN_float(msg,  2);
}

/**
 * @brief Get field gnd_crossing_lon from mavlink_nv_ext_ground_crossing_report message
 *
 * @return gnd_crossing_lon
 */
static inline float mavlink_nv_ext_ground_crossing_report_get_gnd_crs_lon(
    const mavlink_message_t* msg
)
{
    return _MAV_RETURN_float(msg,  6);
}

/**
 * @brief Get field gnd_crossing_alt from mavlink_nv_ext_ground_crossing_report message
 *
 * @return gnd_crossing_alt
 */
static inline float mavlink_nv_ext_ground_crossing_report_get_gnd_crs_alt(
    const mavlink_message_t* msg
)
{
    return _MAV_RETURN_float(msg,  10);
}

/**
 * @brief Get field slant_range from mavlink_nv_ext_ground_crossing_report message
 *
 * @return slant_range
 */
static inline float mavlink_nv_ext_ground_crossing_report_get_slant_range(
    const mavlink_message_t* msg
)
{
    return _MAV_RETURN_float(msg,  14);
}

/**
 * @brief Decode a v2_extension message into a struct
 *
 * @param msg The message to decode
 * @param gnd_crs_report C-struct to decode the message contents into
 */
static inline void mavlink_nv_ext_ground_crossing_report_decode(
    const mavlink_message_t* msg,
    mavlink_nv_ext_ground_crossing_report_t* gnd_crs_report)
{
    gnd_crs_report->report_type = mavlink_nv_ext_ground_crossing_report_get_report_type(msg);
    gnd_crs_report->gnd_crossing_lat = mavlink_nv_ext_ground_crossing_report_get_gnd_crs_lat(msg);
    gnd_crs_report->gnd_crossing_lon = mavlink_nv_ext_ground_crossing_report_get_gnd_crs_lon(msg);
    gnd_crs_report->gnd_crossing_alt = mavlink_nv_ext_ground_crossing_report_get_gnd_crs_alt(msg);
    gnd_crs_report->slant_range = mavlink_nv_ext_ground_crossing_report_get_slant_range(msg);
}
