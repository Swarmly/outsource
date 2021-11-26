#pragma once
#include "gtu-core_global.h"
#include <QString>

extern const QString GTUCORESHARED_EXPORT cmd_rx_stat;
extern const QString GTUCORESHARED_EXPORT cmd_rx_stat_bin;
extern const QString GTUCORESHARED_EXPORT cmd_channel_engaged;
extern const QString GTUCORESHARED_EXPORT cmd_muxer_reset;
extern const QString GTUCORESHARED_EXPORT cmd_stream_oppen;
extern const QString GTUCORESHARED_EXPORT cmd_exit_app;
extern const QString GTUCORESHARED_EXPORT cmd_wrong_key;
extern const QString GTUCORESHARED_EXPORT cmd_good_key;
extern const QString GTUCORESHARED_EXPORT cmd_filter_depth;
extern const QString GTUCORESHARED_EXPORT cmd_calibrate_depth;
extern const QString GTUCORESHARED_EXPORT cmd_calibrate_pos;
extern const QString GTUCORESHARED_EXPORT cmd_set_calibrate_values;
extern const QString GTUCORESHARED_EXPORT cmd_set_min_temp;
extern const QString GTUCORESHARED_EXPORT cmd_set_threshold_temp;
extern const QString GTUCORESHARED_EXPORT cmd_set_reference_point;
extern const QString GTUCORESHARED_EXPORT cmd_set_reference_tmp;
extern const QString GTUCORESHARED_EXPORT cmd_set_max_detection;
extern const QString GTUCORESHARED_EXPORT cmd_set_offsetX;
extern const QString GTUCORESHARED_EXPORT cmd_set_offsetY;
extern const QString GTUCORESHARED_EXPORT cmd_set_scaleX;
extern const QString GTUCORESHARED_EXPORT cmd_set_scaleY;
extern const QString GTUCORESHARED_EXPORT cmd_collected_samples;
extern const QString GTUCORESHARED_EXPORT cmd_set_center;
extern const QString GTUCORESHARED_EXPORT cmd_calc_scale;
extern const QString GTUCORESHARED_EXPORT cmd_mouse_click;

extern const QString GTUCORESHARED_EXPORT cmd_gimbal_IMG_CAPTURE;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_HOLD_COORD;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_RECORD;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_OBSERVATION;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_PILOT;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_LPOS;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_GPOS;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_PV1;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_PV2;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_STOW;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_NUC;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_FOLLOW;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_TRACK;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_GRR;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_BIT;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_SINGLE_YAW;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_IR;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_DAY;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_PALETTE;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_BH;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_WH;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_RETRACT;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_EPR;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_TRACK_POS;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_POINT_COORD;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_PTZ;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_ZOOM_IN;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_ZOOM_OUT;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_ZOOM_STOP;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_ZOOM_SET;
extern const QString GTUCORESHARED_EXPORT cmd_gimbal_ROLL_DEROT;

extern const QString GTUCORESHARED_EXPORT cmd_test_cmd;
extern const QString GTUCORESHARED_EXPORT cmd_bombing_SEND_TARGET;
extern const QString GTUCORESHARED_EXPORT cmd_telem_SET_MODE;
extern const QString GTUCORESHARED_EXPORT cmd_uav_SET_MODE;

extern const QString GTUCORESHARED_EXPORT cmd_reboot_gimbal;
extern const QString GTUCORESHARED_EXPORT cmd_rf_reboot_rx;
extern const QString GTUCORESHARED_EXPORT cmd_rf_reopen_rx; //reopen
extern const QString GTUCORESHARED_EXPORT cmd_rf_sync_rx;   //start grabbing
extern const QString GTUCORESHARED_EXPORT cmd_rf_reboot_tx;
extern const QString GTUCORESHARED_EXPORT cmd_rf_set_gain_tx;
extern const QString GTUCORESHARED_EXPORT cmd_rf_set_rf_channel;

extern const QString GTUCORESHARED_EXPORT cmd_plain_text;
extern const QString GTUCORESHARED_EXPORT cmd_armed;

extern const QString GTUCORESHARED_EXPORT cmd_watchdog_keep_alive;

extern const QString GTUCORESHARED_EXPORT cmd_vieo_pause;
extern const QString GTUCORESHARED_EXPORT cmd_vieo_un_pause;
extern const QString GTUCORESHARED_EXPORT cmd_vieo_step_forward;
extern const QString GTUCORESHARED_EXPORT cmd_vieo_step_back;

extern const QString GTUCORESHARED_EXPORT event_seek;
extern const QString GTUCORESHARED_EXPORT event_set_mode;
extern const QString GTUCORESHARED_EXPORT event_start_stream;
extern const QString GTUCORESHARED_EXPORT event_EOS_stream;
extern const QString GTUCORESHARED_EXPORT event_src_diration;
extern const QString GTUCORESHARED_EXPORT event_src_pos;
extern const QString GTUCORESHARED_EXPORT event_rt_replay;
extern const QString GTUCORESHARED_EXPORT event_set_baudrate;
extern const QString GTUCORESHARED_EXPORT event_direct_read;
extern const QString GTUCORESHARED_EXPORT event_direct_write;
