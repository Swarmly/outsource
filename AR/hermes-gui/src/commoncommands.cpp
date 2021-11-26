#include "commoncommands.h"

const QString cmd_rx_stat("RF_STAT");
const QString cmd_rx_stat_bin("RF_STAT_BIN");
const QString cmd_channel_engaged("CH_ENGAGED");
const QString cmd_muxer_reset("RESET_MUXER");
const QString cmd_stream_oppen("STREAM_OPEN");
const QString cmd_exit_app("EXIT");
const QString cmd_wrong_key("WRONG_KEY");
const QString cmd_good_key("GOOD_KEY");
const QString cmd_filter_depth("filter_depth");
const QString cmd_calibrate_depth("calibrate_depth");
const QString cmd_calibrate_pos("calibrate_pos");
const QString cmd_set_calibrate_values("calibrate_val");
const QString cmd_set_min_temp("set_min_tmp");
const QString cmd_set_threshold_temp("set_threshold_temp");
const QString cmd_set_reference_point("set_reference_temp");
const QString cmd_set_max_detection("set_max_detection");
const QString cmd_set_reference_tmp("set_reference_tmp");
const QString cmd_set_offsetX("set_offsetx");
const QString cmd_set_offsetY("set_offsety");
const QString cmd_set_scaleX("set_lenx");
const QString cmd_set_scaleY("set_leny");
const QString cmd_collected_samples("collected_samples");
const QString cmd_set_center("set_center");
const QString cmd_calc_scale("calc_scale");
const QString cmd_mouse_click("mouse_click");

const QString cmd_gimbal_IMG_CAPTURE("gmb_img");
const QString cmd_gimbal_HOLD_COORD("gmb_hold");
const QString cmd_gimbal_RECORD("gmb_record");
const QString cmd_gimbal_OBSERVATION("gmv_observ");
const QString cmd_gimbal_PILOT("gmb_pilot");
const QString cmd_gimbal_LPOS("gmb_view1");
const QString cmd_gimbal_GPOS("gmb_view2");
const QString cmd_gimbal_PV1("gmb_pv1");
const QString cmd_gimbal_PV2("gmb_pv2");
const QString cmd_gimbal_STOW("gmb_stow");
const QString cmd_gimbal_NUC("gmb_nuc");
const QString cmd_gimbal_FOLLOW("gmb_follow");
const QString cmd_gimbal_GRR("gmb_grr");
const QString cmd_gimbal_BIT("gmb_bit");
const QString cmd_gimbal_SINGLE_YAW("gmb_yaw");
const QString cmd_gimbal_IR("gmb_ir");
const QString cmd_gimbal_DAY("gmb_day");
const QString cmd_gimbal_PALETTE("gmb_palette");
const QString cmd_gimbal_BH("gmb_bh");
const QString cmd_gimbal_WH("gmb_wh");
const QString cmd_gimbal_RETRACT("gmb_retract");
const QString cmd_gimbal_EPR("gmb_epr");
const QString cmd_gimbal_TRACK("gmb_track");
const QString cmd_gimbal_TRACK_POS("gmb_track_pos");
const QString cmd_gimbal_POINT_COORD("gmb_point_coord");
const QString cmd_gimbal_PTZ("gmb_PTZ");
const QString cmd_gimbal_ZOOM_IN("gmb_Z_I");
const QString cmd_gimbal_ZOOM_OUT("gmb_Z_O");
const QString cmd_gimbal_ZOOM_STOP("gmb_Z_STOP");
const QString cmd_gimbal_ZOOM_SET("gmb_Z_S");
const QString cmd_gimbal_ROLL_DEROT("gmb_R_D");
const QString cmd_test_cmd("test_cmd");

const QString cmd_bombing_SEND_TARGET("bmb_send_target");
const QString cmd_telem_SET_MODE("telem_set_mode");
const QString cmd_uav_SET_MODE("uav_set_mode");

const QString cmd_reboot_gimbal("RF_REBOOT_GMB");
const QString cmd_rf_reboot_rx("RF_REBOOT_RX");
const QString cmd_rf_reopen_rx("RF_REOPEN_RX");
const QString cmd_rf_sync_rx("RF_SYNC_RX");
const QString cmd_rf_reboot_tx("RF_REBOOT_TX");
const QString cmd_rf_set_rf_channel("SET_RF_CHANNEL");
const QString cmd_rf_set_gain_tx("SET_RF_GAIN");

const QString cmd_plain_text("cmd_plain_text");
const QString cmd_armed("armed");
const QString cmd_watchdog_keep_alive("wd");

const QString cmd_vieo_pause("vd_pause");
const QString cmd_vieo_un_pause("vd_unpause");
const QString cmd_vieo_step_forward("vd_step_forward");
const QString cmd_vieo_step_back("vd_step_back");

const QString event_seek("ev_seek");
const QString event_set_mode("ev_set_mode");
const QString event_start_stream("ev_start");
const QString event_EOS_stream("ev_EOS");

const QString event_src_diration("ev_duration");
const QString event_src_pos("ev_pos");
const QString event_rt_replay("ev_rt");
const QString event_set_baudrate("ev_set_baud");
const QString event_direct_read("ev_dr_read");
const QString event_direct_write("ev_dr_write");
