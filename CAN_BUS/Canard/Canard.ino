#include <FlexCAN_T4.h>
#include "canard.h"
//#include "uavcan/protocol/dynamic_node_id/Allocation.h"
//#include "uavcan/protocol/file/BeginFirmwareUpdate.h"
//#include "uavcan/protocol/file/Read.h"
//#include "uavcan/protocol/dynamic_node_id/Allocation.h"
//#include "uavcan/protocol/NodeStatus.h"
//#include "uavcan/protocol/RestartNode.h"
//#include "uavcan/protocol/GetNodeInfo.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

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


static CanardInstance canard;
static uint32_t canard_memory_pool[4096 / 4];

//CanardInstance ins = canardInit(&memAllocate, &memFree);
//CanardTransfer transfer;
//CanardRxSubscription my_service_subscription;
//CanardRxSubscription heartbeat_subscription;

static void onTransferReceived(CanardInstance* ins,
                               CanardRxTransfer* transfer)
{
  //Serial.println("onTransferReceived");
}

static bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{
  //if (data_type_id != 1080 && data_type_id != 1081 && data_type_id != 1030 && data_type_id != 1020 && data_type_id != 1010)
  {
    Serial.print("Node: ");
    Serial.println(source_node_id);
    Serial.print("ID: ");
    Serial.println(data_type_id);
  }

  //(void)source_node_id;

  //  if (canardGetLocalNodeID(ins) == CANARD_BROADCAST_NODE_ID) {
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

  //  switch (data_type_id) {
  //    case UAVCAN_PROTOCOL_GETNODEINFO_ID:
  //      *out_data_type_signature = UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE;
  //      return true;
  //    case UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_ID:
  //      *out_data_type_signature = UAVCAN_PROTOCOL_FILE_BEGINFIRMWAREUPDATE_SIGNATURE;
  //      return true;
  //    case UAVCAN_PROTOCOL_RESTARTNODE_ID:
  //      *out_data_type_signature = UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE;
  //      return true;
  //    case UAVCAN_PROTOCOL_FILE_READ_ID:
  //      *out_data_type_signature = UAVCAN_PROTOCOL_FILE_READ_SIGNATURE;
  //      return true;
  //    default:
  //      break;
  //  }

  return true;
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
