#ifndef __RMDMSG_HPP__
#define __RMDMSG_HPP__

#pragma pack(1)
typedef union RMDMsg {
  RMDMsg() { int i = 8; while(i--) bytes.data[i] = 0; }

  struct {
    uint8_t data[8];
    void print() {
      for(int i = 0; i < 8; i++) {
        if(i) Serial.print(", ");
        Serial.print(String(data[i], HEX));
      }
      Serial.println();
    }
  } bytes;

  struct {
    uint8_t cmd;
    uint8_t zero[7];
  } oneByte;

  struct {
    uint8_t cmd;
    uint8_t pad0;
    uint8_t angleKp;
    uint8_t angleKi;
    uint8_t speedKp;
    uint8_t speedKi;
    uint8_t torqueKp;
    uint8_t torqueKi;
  } pid;

  struct {
    uint8_t cmd;
    uint8_t pad0;
    uint8_t posLoopP;
    uint8_t posLoopI;
    int32_t acceleration;
  } accelerationReply;

  struct {
    uint8_t cmd;
    uint8_t pad0;
    uint16_t position;
    uint16_t originalPosition;
    uint16_t offset;
  } encodingReply;

  struct {
    int64_t angle;
    float angleDegrees() const {
      return ((float) (angle >> 8)) / 100.0;
    }
  } multiTurnAngleReply;

  struct {
    uint8_t cmd;
    uint8_t temp;
    uint8_t pad0;
    uint16_t voltage;
    uint8_t pad1;
    uint8_t pad2;
    uint8_t error;
    float volts() const {
      return ((float) voltage) / 10.0;
    }
  } motorStatus1Reply;

  struct {
    uint8_t cmd;
    uint8_t temp;
    uint16_t torqueCurrent;
    uint16_t speed;
    uint16_t encoderPos;
    float torqueCurrentAmps() const {
      return ((float) torqueCurrent) * 33.0 / 2048.0;
    }
    float speedDegreesPerSecond() const {
      return ((float) speed);
    }
  } motorStatus2Reply;

  struct {
    uint8_t cmd;
    uint8_t pad0;
    uint16_t phaseA;
    uint16_t phaseB;
    uint16_t phaseC;
    float phaseAAmps() const{
      return ((float) phaseA) / 64.0;
    }
    float phaseBAmps() const {
      return ((float) phaseB) / 64.0;
    }
    float phaseCAmps() const {
      return ((float) phaseC) / 64.0;
    }
  } motorStatus3Reply;
  
  struct {
    uint8_t cmd;
    uint8_t pad0, pad1, pad2;
    int16_t torqueCurrent;
    uint8_t pad3, pad4;

    void setTorqueCurrentAmps(float iAmps) {
      // Yes, the documentation from MyActuator specifies a factor of 32 for writing, but 33 for reading
      torqueCurrent = (int32_t) (iAmps / 32.0 * 2000.0);
    }
  } torqueCurrent;
 
  struct {
    uint8_t cmd;
    uint8_t temperature;
    int16_t torqueCurrent;
    int16_t speed;
    int16_t position;

    float torqueCurrentAmps() const {
      return ((float) torqueCurrent) * 33.0 / 2000.0;
    }
  } torqueCurrentReply;

  struct {
    uint8_t cmd;
    uint8_t pad0, pad1, pad2;
    int32_t speed;

    void setSpeedDegreesPerSecond(float iDPS) {
      speed = (int32_t) (iDPS * 100.0);
    }

  } speed;

  struct {
    uint8_t cmd;
    uint8_t pad0, pad1, pad2;
    int32_t angle;

    void setAngleDegrees(float iAngle) {
      angle = (int32_t) (iAngle * 100.0);
    }
    
  } position;

  struct {
    uint8_t cmd;
    uint8_t pad0;
    uint16_t speed;
    int32_t angle;

    void setAngleDegrees(float iAngle) {
      angle = (int32_t) (iAngle * 100.0);
    }
    
    void print() {
      Serial.print(" packet size: ");
      Serial.print(" max speed: ");
      Serial.println(speed);
      Serial.print(" raw position: ");
      Serial.println(angle);
    }
  } speedAndPosition;

} RMDMsg;
#pragma pack(0)

#endif
