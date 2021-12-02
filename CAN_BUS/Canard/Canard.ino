#include <FlexCAN_T4.h>
#include "canard.h"

#include <uavcan/protocol/dynamic_node_id/Allocation.h>
#include <uavcan/protocol/NodeStatus.h>
#include <uavcan/protocol/RestartNode.h>
#include <uavcan/protocol/GetNodeInfo.h>
#include <uavcan/protocol/file/BeginFirmwareUpdate.h>
#include <uavcan/protocol/param/GetSet.h>
#include <uavcan/protocol/param/ExecuteOpcode.h>
#include <uavcan/equipment/ahrs/MagneticFieldStrength.h>
#include <uavcan/equipment/gnss/Fix.h>
#include <uavcan/equipment/gnss/Fix2.h>
#include <uavcan/equipment/gnss/Auxiliary.h>
#include <uavcan/equipment/air_data/StaticPressure.h>
#include <uavcan/equipment/air_data/StaticTemperature.h>
#include <uavcan/equipment/air_data/RawAirData.h>
#include <uavcan/equipment/indication/BeepCommand.h>
#include <uavcan/equipment/indication/LightsCommand.h>
#include <uavcan/equipment/range_sensor/Measurement.h>
#include <uavcan/equipment/hardpoint/Command.h>
#include <uavcan/equipment/esc/Status.h>
#include <uavcan/equipment/safety/ArmingStatus.h>
#include <uavcan/equipment/gnss/RTCMStream.h>
#include <uavcan/equipment/power/BatteryInfo.h>
#include <uavcan/protocol/debug/LogMessage.h>
#include <uavcan/equipment/esc/RawCommand.h>
#include <uavcan/equipment/actuator/ArrayCommand.h>
#include <uavcan/equipment/actuator/Command.h>

//#include "uavcan/equipment/actuator/ArrayCommand.h"
//#include "uavcan/equipment/actuator/Command.h"
//#include "uavcan/protocol/GetNodeInfo.h"
//#include "uavcan/protocol/NodeStatus.h"
//#include "uavcan/protocol/RestartNode.h"
//#include "uavcan/protocol/param/GetSet.h"
//#include "uavcan/protocol/file/BeginFirmwareUpdate.h"

//#include "uavcan/protocol/dynamic_node_id/Allocation.h"
//#include "uavcan/protocol/file/BeginFirmwareUpdate.h"
//#include "uavcan/protocol/file/Read.h"
//#include "uavcan/protocol/dynamic_node_id/Allocation.h"
//#include "uavcan/protocol/NodeStatus.h"
//#include "uavcan/protocol/RestartNode.h"
//#include "uavcan/protocol/GetNodeInfo.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
#define SERVO_OUT_MOTOR_MAX     12
/*
   Node status variables
*/
static uavcan_protocol_NodeStatus node_status;

// Small enough to not be too bad, large enough to be useful
#define DYNAMIC_ARRAY_BUF_SIZE 1000
#define RX_FIFO_LEN 10

// Dynamic array buffer for decoding messages
uint8_t dynamic_array_buf[DYNAMIC_ARRAY_BUF_SIZE];
// The below is necessary, otherwise a pointer to dynamic_array_buf resolves to
// NULL
uint8_t *p_dynamic_array_buf = (uint8_t *)dynamic_array_buf;


static void* memAllocate(CanardInstance* const ins, const size_t amount)
{
  (void) ins;
  return static_cast<void *>(new char[amount]);
}

static void memFree(CanardInstance* const ins, void* const pointer)
{
  (void) ins;
  delete[] static_cast<char *>(pointer);
}


//can_msg_handler can_request_handlers[] = {
//    CAN_MSG_HANDLER(UAVCAN_PROTOCOL_PARAM_GETSET_ID,
//                    UAVCAN_PROTOCOL_PARAM_GETSET_SIGNATURE, handle_GetSet),
//    CAN_MSG_HANDLER(UAVCAN_PROTOCOL_GETNODEINFO_ID,
//                    UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE, handle_GetNodeInfo),
//    CAN_MSG_HANDLER(UAVCAN_PROTOCOL_RESTARTNODE_ID,
//                    UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE, handle_RestartNode),
//    CAN_MSG_HANDLER_END};
//
//can_msg_handler can_broadcast_handlers[] = {
//    CAN_MSG_HANDLER(UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID,
//                    UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_SIGNATURE,
//                    handle_Actuator_ArrayCommand),
//    // this one shouldn't actually really be sent
//    CAN_MSG_HANDLER(2155, UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_SIGNATURE,
//                    handle_Actuator_Command),
//    CAN_MSG_HANDLER_END};
//

static CanardInstance canard;
static uint32_t canard_memory_pool[4096 / 4];

//CanardInstance ins = canardInit(&memAllocate, &memFree);
//CanardTransfer transfer;
//CanardRxSubscription my_service_subscription;
//CanardRxSubscription heartbeat_subscription;
/*
  handle a GET_NODE_INFO request
*/
static void handle_get_node_info(CanardInstance* ins,
                                 CanardRxTransfer* transfer)
{

  Serial.println("GetNodeInfo");

  uint8_t buffer[UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_MAX_SIZE] {};
  uavcan_protocol_GetNodeInfoResponse pkt {};

  node_status.uptime_sec = micros() / 1000000U;

  pkt.status = node_status;
  pkt.software_version.major = 1;
  pkt.software_version.minor = 1;
  pkt.software_version.optional_field_flags = UAVCAN_PROTOCOL_SOFTWAREVERSION_OPTIONAL_FIELD_FLAG_VCS_COMMIT | UAVCAN_PROTOCOL_SOFTWAREVERSION_OPTIONAL_FIELD_FLAG_IMAGE_CRC;
  pkt.software_version.vcs_commit = 11;
  uint32_t *crc = (uint32_t *)&pkt.software_version.image_crc;
  crc[0] = 555;
  crc[1] = 777;

  // pkt.hardware_version.unique_id = 84;

  // use hw major/minor for APJ_BOARD_ID so we know what fw is
  // compatible with this hardware
  pkt.hardware_version.major = 1 >> 8;
  pkt.hardware_version.minor = 1 & 0xFF;

  char text[] = "ddddd\0";
  //    if (periph.g.serial_number > 0) {
  //        hal.util->snprintf(text, sizeof(text), "%s(%u)", CAN_APP_NODE_NAME, (unsigned)periph.g.serial_number);
  //    } else {
  //        hal.util->snprintf(text, sizeof(text), "%s", CAN_APP_NODE_NAME);
  //    }
  pkt.name.len = strlen(text);
  pkt.name.data = (uint8_t *)text;

  uint16_t total_size = uavcan_protocol_GetNodeInfoResponse_encode(&pkt, buffer);

  const int16_t resp_res = canardRequestOrRespond(ins,
                           transfer->source_node_id,
                           UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
                           UAVCAN_PROTOCOL_GETNODEINFO_ID,
                           &transfer->transfer_id,
                           transfer->priority,
                           CanardResponse,
                           &buffer[0],
                           total_size);
  if (resp_res <= 0) {
    printf("Could not respond to GetNodeInfo: %d\n", resp_res);
  }
}

void rcout_esc(int16_t *rc, uint8_t num_channels)
{
  if (rc == nullptr) {
    return;
  }

  const uint8_t channel_count = min(num_channels, SERVO_OUT_MOTOR_MAX);
  for (uint8_t i = 0; i < channel_count; i++) {
    Serial.print(max(0, rc[i]));
    Serial.print(":");
  }
   Serial.println();
}

static void handle_esc_rawcommand(CanardInstance* ins, CanardRxTransfer* transfer)
{
  uavcan_equipment_esc_RawCommand cmd;
  uint8_t arraybuf[UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_MAX_SIZE];
  uint8_t *arraybuf_ptr = arraybuf;
  if (uavcan_equipment_esc_RawCommand_decode(transfer, transfer->payload_len, &cmd, &arraybuf_ptr) < 0) {
    return;
  }
//  for (int i = 0; i < cmd.cmd.len; i++) {
//    Serial.print(cmd.cmd.data[i]);
//    Serial.print(":");
//  }
//  Serial.println();
  //Serial.println(cmd.cmd.len);
  rcout_esc(cmd.cmd.data, cmd.cmd.len);
}

static void handle_act_command(CanardInstance* ins, CanardRxTransfer* transfer)
{
  // manual decoding due to TAO bug in libcanard generated code
  if (transfer->payload_len < 1 || transfer->payload_len > UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_SIZE + 1) {
    return;
  }

  const uint8_t data_count = (transfer->payload_len / UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_MAX_SIZE);
  uavcan_equipment_actuator_Command data[data_count] {};

  uint32_t offset = 0;
  for (uint8_t i = 0; i < data_count; i++) {
    canardDecodeScalar(transfer, offset, 8, false, (void*)&data[i].actuator_id);
    offset += 8;
    canardDecodeScalar(transfer, offset, 8, false, (void*)&data[i].command_type);
    offset += 8;

#ifndef CANARD_USE_FLOAT16_CAST
    uint16_t tmp_float = 0;
#else
    CANARD_USE_FLOAT16_CAST tmp_float = 0;
#endif
    canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);
    offset += 16;
#ifndef CANARD_USE_FLOAT16_CAST
    data[i].command_value = canardConvertFloat16ToNativeFloat(tmp_float);
#else
    data[i].command_value = (float)tmp_float;
#endif
  }

  for (uint8_t i = 0; i < data_count; i++) {
    if (data[i].command_type != UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_UNITLESS) {
      // this is the only type we support
      continue;
    }
    //  for (int i = 0; i < msg.commands.len; i++) {
    //    Serial.print(data[i].actuator_id);
    //    Serial.print(" :");
    //    Serial.print(int( data[i].command_value * 1000 + 1000));
    //    Serial.print(" ");
    //periph.rcout_srv(data[i].actuator_id, data[i].command_value);
  }
  //Serial.println("");
}

//
//static void handle_act_command( CanardInstance* ins, CanardRxTransfer* transfer)
//{
//  // manual decoding due to TAO bug in libcanard generated code
//  if (transfer->payload_len < 1 || transfer->payload_len > UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_SIZE + 1) {
//    return;
//  }
//  uavcan_equipment_actuator_ArrayCommand msg;
//
//  uavcan_equipment_actuator_ArrayCommand_decode(transfer, transfer->payload_len,
//      &msg, &p_dynamic_array_buf);
//
//  for (int i = 0; i < msg.commands.len; i++) {
//    // Serial.print(int(msg.commands.data[i].command_value * 1000 + 1000));
//    // Serial.print(" :");
//  }
//  //Serial.println();
//  //uavcan_equipment_actuator_ArrayCommand_decode ();
//
//  //    const uint8_t data_count = (transfer->payload_len / UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_MAX_SIZE);
//  //    uavcan_equipment_actuator_Command data[data_count] {};
//  //
//  //    uint32_t offset = 0;
//  //    for (uint8_t i=0; i<data_count; i++) {
//  //        canardDecodeScalar(transfer, offset, 8, false, (void*)&data[i].actuator_id);
//  //        offset += 8;
//  //        canardDecodeScalar(transfer, offset, 8, false, (void*)&data[i].command_type);
//  //        offset += 8;
//  //
//  //#ifndef CANARD_USE_FLOAT16_CAST
//  //    uint16_t tmp_float = 0;
//  //#else
//  //    CANARD_USE_FLOAT16_CAST tmp_float = 0;
//  //#endif
//  //        canardDecodeScalar(transfer, offset, 16, false, (void*)&tmp_float);
//  //        offset += 16;
//  //#ifndef CANARD_USE_FLOAT16_CAST
//  //        data[i].command_value = canardConvertFloat16ToNativeFloat(tmp_float);
//  //#else
//  //        data[i].command_value = (float)tmp_float;
//  //#endif
//  //    }
//  //
//  //    for (uint8_t i=0; i < data_count; i++) {
//  //        if (data[i].command_type != UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_COMMAND_TYPE_UNITLESS) {
//  //            // this is the only type we support
//  //            continue;
//  //        }
//  //        periph.rcout_srv(data[i].actuator_id, data[i].command_value);
//  //    }
//}

static void onTransferReceived(CanardInstance* ins,
                               CanardRxTransfer* transfer)
{

  switch (transfer->data_type_id) {
    case UAVCAN_PROTOCOL_GETNODEINFO_ID:
      handle_get_node_info(ins, transfer);
      break;

    case UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_ID:
      //handle_begin_firmware_update(ins, transfer);
      break;

    case UAVCAN_PROTOCOL_RESTARTNODE_ID:
      break;

    case UAVCAN_PROTOCOL_PARAM_GETSET_ID:
      //handle_param_getset(ins, transfer);
      break;

    case UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_ID:
      //handle_param_executeopcode(ins, transfer);
      break;

    case UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_ID:
      //handle_beep_command(ins, transfer);
      break;

    //    case ARDUPILOT_INDICATION_SAFETYSTATE_ID:
    //        //handle_safety_state(ins, transfer);
    //        break;
    case UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_ID:
      //handle_arming_status(ins, transfer);
      break;

    case UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_ID:
      //handle_RTCMStream(ins, transfer);
      break;

    case UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_ID:
      //handle_lightscommand(ins, transfer);
      break;

    case UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_ID:
      handle_esc_rawcommand(ins, transfer);
      break;

    case UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID:
      handle_act_command(ins, transfer);
      break;
  }
}

static bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{

  //  if (canardGetLocalNodeID(ins) == CANARD_BROADCAST_NODE_ID)
  //  {
  //    /*
  //       If we're in the process of allocation of dynamic node ID, accept only relevant transfers.
  //    */
  //    if ((transfer_type == CanardTransferTypeBroadcast) &&
  //        (data_type_id == UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_ID))
  //    {
  //      *out_data_type_signature = UAVCAN_PROTOCOL_DYNAMIC_NODE_ID_ALLOCATION_SIGNATURE;
  //      return true;
  //    }
  //    return false;
  //  }


  switch (data_type_id) {
    case UAVCAN_PROTOCOL_GETNODEINFO_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE;
      return true;
    case UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_SIGNATURE;
      return true;
    case UAVCAN_PROTOCOL_RESTARTNODE_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE;
      return true;
    case UAVCAN_PROTOCOL_PARAM_GETSET_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_PARAM_GETSET_SIGNATURE;
      return true;
    case UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_ID:
      *out_data_type_signature = UAVCAN_PROTOCOL_PARAM_EXECUTEOPCODE_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_SIGNATURE;
      return true;
    //    case ARDUPILOT_INDICATION_SAFETYSTATE_ID:
    //      *out_data_type_signature = ARDUPILOT_INDICATION_SAFETYSTATE_SIGNATURE;
    //      return true;
    case UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_SAFETY_ARMINGSTATUS_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_INDICATION_LIGHTSCOMMAND_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_GNSS_RTCMSTREAM_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_SIGNATURE;
      return true;
    case UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID:
      *out_data_type_signature = UAVCAN_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_SIGNATURE;
      return true;
    default:
      break;
  }

  return false;
}

void setup(void) {
  Serial.begin(115200); delay(400);

  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canGet);
  Can0.mailboxStatus();

  canardInit(&canard, (uint8_t *)canard_memory_pool, sizeof(canard_memory_pool),
             onTransferReceived, shouldAcceptTransfer, NULL);

  //  ins.mtu_bytes = CANARD_MTU_CAN_CLASSIC;  // Defaults to 64 (CAN FD); here we select Classic CAN.
  //  ins.node_id   = 1;

  //  (void) canardRxSubscribe(&ins,   // Subscribe to messages uavcan.node.Heartbeat.
  //                           CanardTransferKindMessage,
  //                           7509,   // The fixed Subject-ID of the Heartbeat message type (see DSDL definition).
  //                           16,     // The extent (the maximum possible payload size); pick a huge value if not sure.
  //                           CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
  //                           &heartbeat_subscription);
  //
  //  (void) canardRxSubscribe(&ins,   // Subscribe to an arbitrary service response.
  //                           CanardTransferKindMessage,  // Specify that we want service responses, not requests.
  //                           10,    // The Service-ID whose responses we will receive.
  //                           32,   // The extent (the maximum payload size); pick a huge value if not sure.
  //                           CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
  //                           &my_service_subscription);
}

void canGet(const CAN_message_t &msg) {
  //Serial.print("MB "); Serial.print(msg.mb);
  //Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  //  Serial.print("  LEN: "); Serial.print(msg.len);
  //  //Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  //  Serial.print(" ID: ");
  //  Serial.println(msg.id & CANARD_CAN_EXT_ID_MASK);

  CanardCANFrame rx_frame {};
  // int * ddd = &Can0;

  memcpy(rx_frame.data, msg.buf, 8);

  const uint64_t timestamp = micros();

  rx_frame.data_len = msg.len;
  if (msg.flags.extended) {
    rx_frame.id = CANARD_CAN_FRAME_EFF | msg.id;
  } else {
    rx_frame.id = msg.id;
  }
  canardHandleRxFrame(&canard, &rx_frame, timestamp);

  //typedef struct CAN_message_t {
  //  uint32_t id; // can identifier
  //  uint8_t ext; // identifier is extended
  //  uint8_t len; // length of data
  //  uint16_t timeout; // milliseconds, zero will disable waiting
  //  uint8_t buf[8];
  //} CAN_message_t;


  //  Serial.print(" Buffer: ");
  //  for ( uint8_t i = 0; i < msg.len; i++ ) {
  //    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  //  } Serial.println();

  //  CanardFrame frame;
  //  frame.payload_size = msg.len;
  //  frame.payload = msg.buf;
  //  frame.timestamp_usec = micros();
  //  frame.extended_can_id = msg.id;
  //
  //
  //  const int8_t result = canardRxAccept(&ins,
  //                                       &frame,            // The CAN frame received from the bus.
  //                                       0,  // If the transport is not redundant, use 0.
  //                                       &transfer);
  //  if (result < 0)
  //  {
  //    // An error has occurred: either an argument is invalid or we've ran out of memory.
  //    // It is possible to statically prove that an out-of-memory will never occur for a given application if
  //    // the heap is sized correctly; for background, refer to the Robson's Proof and the documentation for O1Heap.
  //    // Reception of an invalid frame is NOT an error.
  //    abort();
  //  }
  //  else if (result == 1)
  //  {
  //    Serial.println("YES");
  //    ins.memory_free(&ins, (void*)transfer.payload);  // Deallocate the dynamic memory afterwards.
  //  }
  //  else if (result == 10)
  //  {
  //    Serial.println("can't parse");
  //  }
}

void loop() {
  //  CAN_message_t msg;
  //  if (Can0.isAvailable())
  //  {
  //   // Serial.println(Can0.getRXQueueCount());
  //    if (Can0.read(msg) > 0)
  //    {
  //      Serial.print(" ID: ");
  //      Serial.println(msg.id & CANARD_CAN_EXT_ID_MASK);
  //    }
  //  }
  Can0.events();

}
