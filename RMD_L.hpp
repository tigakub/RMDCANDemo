#ifndef __RMD_L_HPP__
#define __RMD_L_HPP__

#include "RMDCAN.hpp"

class RMD_L {
  protected:
    uint8_t id;
    RMDCAN &rmd;

  public:
    RMD_L(uint8_t iDeviceID, RMDCAN &iRMD) : id(iDeviceID), rmd(iRMD) { }
    virtual ~RMD_L() { }

    // Retrieve the actuator's PID parameters
    const RMDMsg &getPID() {
      RMDMsg command;
      command.oneByte.cmd = 0x30;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Write PID parameters to actuator RAM
    const RMDMsg &setPID(uint8_t iAngleKp, uint8_t iAngleKi, uint8_t iSpeedKp, uint8_t iSpeedKi, uint8_t iTorqueKp, uint8_t iTorqueKi) {
      RMDMsg command;
      command.pid.cmd = 0x31;
      command.pid.angleKp = iAngleKp;
      command.pid.angleKi = iAngleKi;
      command.pid.speedKp = iSpeedKp;
      command.pid.speedKi = iSpeedKi;
      command.pid.torqueKp = iTorqueKp;
      command.pid.torqueKi = iTorqueKi;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Save PID parameters to actuator ROM
    const RMDMsg &savePID(uint8_t iAngleKp, uint8_t iAngleKi, uint8_t iSpeedKp, uint8_t iSpeedKi, uint8_t iTorqueKp, uint8_t iTorqueKi) {
      RMDMsg command;
      command.pid.cmd = 0x32;
      command.pid.angleKp = iAngleKp;
      command.pid.angleKi = iAngleKi;
      command.pid.speedKp = iSpeedKp;
      command.pid.speedKi = iSpeedKi;
      command.pid.torqueKp = iTorqueKp;
      command.pid.torqueKi = iTorqueKi;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve the actuator's current acceleration
    const RMDMsg &getAcceleration() {
      RMDMsg command;
      command.oneByte.cmd = 0x33;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve the actuator's current encoder position
    const RMDMsg &getEncoder() {
      RMDMsg command;
      command.oneByte.cmd = 0x90;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve the actuator's current angular positon
    const RMDMsg &getMultiTurnAngle() {
      RMDMsg command;
      command.oneByte.cmd = 0x92;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Disable torque
    const RMDMsg &motorOff() {
      RMDMsg command;
      command.oneByte.cmd = 0x80;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Stop motion and break
    const RMDMsg &motorStop() {
      RMDMsg command;
      command.oneByte.cmd = 0x81;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Resume after a stop or off
    const RMDMsg &motorResume() {
      RMDMsg command;
      command.oneByte.cmd = 0x88;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve the current angukar position
    const RMDMsg &getPosition() {
      RMDMsg command;
      command.oneByte.cmd = 0x92;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve motor status 1
    const RMDMsg &getMotorStatus1() {
      RMDMsg command;
      command.oneByte.cmd = 0x9a;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve motor status 2
    const RMDMsg &getMotorStatus2() {
      RMDMsg command;
      command.oneByte.cmd = 0x9c;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Retrieve motor status 3
    const RMDMsg &getMotorStatus3() {
      RMDMsg command;
      command.oneByte.cmd = 0x9d;
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }
    
    // Induce motion by setting the torque current in amps
    const RMDMsg &setTorqueCurrent(float iCurrent) {
      RMDMsg command;
      command.torqueCurrent.cmd = 0xa1;
      command.torqueCurrent.setTorqueCurrentAmps(iCurrent);
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Induce motion by setting the speed in degrees per second
    const RMDMsg &setSpeed(float iDPS) {
      RMDMsg command;
      command.speed.cmd = 0xa2;
      command.speed.setSpeedDegreesPerSecond(iDPS);
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Induce motion by setting the angular position in degrees
    const RMDMsg &setPosition(float iAngle) {
      RMDMsg command;
      command.position.cmd = 0xa3;
      command.position.setAngleDegrees(iAngle);
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }

    // Induce motion to an angular position at a specific speed
    const RMDMsg &setPosition(uint16_t iSpeed, float iAngle) {
      RMDMsg command;
      command.speedAndPosition.cmd = 0xa4;
      command.speedAndPosition.speed = iSpeed;
      command.speedAndPosition.setAngleDegrees(iAngle);
      rmd.writePacket(id, command);
      while(!rmd.readPacket()) { }
      return rmd.data();
    }
};

#endif