#ifndef	PUBLISHER_H
#define	PUBLISHER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/gnss/Fix.hpp>
#include "phoenix_can_shield.h"

using namespace uavcan;
using namespace equipment;
using namespace gnss;

// GPS - publish at fixed rate
int gps_update_rate = 10; // # times per second, 10 = 100ms update
MonotonicTime last_gps_update = MonotonicTime::fromMSec(0);

// publisher
Publisher<Fix> *FixPublisher;


// initialize all publisher
void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  FixPublisher = new Publisher<Fix>(*node);

  // initiliaze publishers
  if(FixPublisher->init() < 0)
  {
    Serial.println("Unable to initialize FixPublisher!");
  } else {
    Serial.println("FixPublisher initialize!");
  }
  // set TX timeout
  FixPublisher->setTxTimeout(MonotonicDuration::fromUSec(1000));

}

// cycle Fix publisher
void cyclePublisher()
{
  // gps update -> at gps_update_rate -> check time first
  if((last_gps_update +
     MonotonicDuration::fromMSec(1000/(float)gps_update_rate)) <
     systemClock->getMonotonic())
  {
    // it is time for an update of the gps readings
    last_gps_update = systemClock->getMonotonic();

    //Read acual GPS data here in place of test data
    equipment::gnss::Fix Fix_msg;
    Fix_msg.gnss_timestamp = systemClock->getUtc();
    Fix_msg.gnss_time_standard = 2;
    Fix_msg.num_leap_seconds = 0;
    Fix_msg.longitude_deg_1e8 =  4076358000; 
    Fix_msg.latitude_deg_1e8  = -7383004000;   
    Fix_msg.height_ellipsoid_mm = 0;
    Fix_msg.height_msl_mm = 0;   
    Fix_msg.ned_velocity[0] = 1.5;
    Fix_msg.ned_velocity[1] = 1.5;
    Fix_msg.ned_velocity[2]  = 1.5;
    Fix_msg.sats_used = 9;
    Fix_msg.status = 3;
    Fix_msg.pdop = 1.1;

    if (FixPublisher->broadcast(Fix_msg) < 0)
    {
      Serial.println("Error while broadcasting key message");
      Serial.println(FixPublisher->broadcast(Fix_msg));
    }
  }
}

#endif
