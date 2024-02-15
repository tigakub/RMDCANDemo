#ifndef __RMDCAN_HPP__
#define __RMDCAN_HPP__

#include <CANSAME5x.h>
#include "RMDMsg.hpp"

class RMDCAN {
  protected:
    // Reference to instance of Adafruit's CAN driver object
    CANSAME5x &can;

    // Buffer for receiving a standard frame
    RMDMsg buffer;

  public:
    // Constructor
    RMDCAN(CANSAME5x &iCan) : can (iCan) { }

    // Virtual destructor
    virtual ~RMDCAN() { }

    // Writes 8 bytes to the can bus tagging it with a specified ID
    void writePacket(uint8_t iID, const RMDMsg &iData) {
      // The RMD CAN implementation is specifies the packet identifier to be the code 0x140 plus the id of the target device
      can.beginPacket(0x140 + iID, 8);

      // Loop through the data
      const uint8_t *ptr = iData.bytes.data;
      int i = sizeof(RMDMsg);
      while(i--) {
        // Write each byte into the CAN buffer
        can.write(*ptr);
        ptr++;
      }

      // Send the data
      can.endPacket();
    }
    
    // Reads a packet if any is available
    bool readPacket() {
      // Attempt to parse any incoming packet
      int packetSize = can.parsePacket();

      // If the packetSize is not 0
      if(packetSize) {
        // Extract the packet id (should be 0x140 + the source device id)
        int packetID = can.packetId();

        // If the packet is an RTR packet, just print it for now
        if(can.packetRtr()) {
          Serial.print("Received RTR packet ");
          Serial.print(can.packetDlc());

        // Otherwise it's data
        } else {
          uint8_t *ptr = buffer.bytes.data;
          // Read bytes while available until the whole packet has been read
          while(can.available() && packetSize) {
            *ptr = can.read();
            ptr++;
            packetSize--;
          }
        }

        // Return success
        return true;
      }

      // Return failure
      return false;
    }

    // Buffer pointer accessor
    const RMDMsg &data() const {
      return buffer;
    }
};

#endif
