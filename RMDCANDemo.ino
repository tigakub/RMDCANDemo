#include "RMD_L.hpp"

CANSAME5x gCan;
RMDCAN rmd(gCan);
RMD_L servo(1, rmd);

uint16_t currentSpeed = 60;

void printHelp() {
  Serial.println("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  Serial.println("USAGE:");
  Serial.println("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  Serial.println("Type a one character command followed by <return>");
  Serial.println("Valid commands:");
  Serial.println(" 1 -- get motor status 1");
  Serial.println(" 2 -- get motor status 2");
  Serial.println(" 3 -- get motor status 3");
  Serial.println(" S -- display current speed");
  Serial.print(" s -- set current movement speed (currently ");
  Serial.print(currentSpeed);
  Serial.println(") follow subsequent prompt to set the speed in integer degrees per second");
  Serial.println(" P -- display actuator current position in degrees");
  Serial.println(" p -- set position in floating point degrees");
  Serial.println(" ? -- print this help message");
  Serial.println("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

void setup() {
  Serial.begin(115200);
  while(!Serial) delay(10);

  Serial.setTimeout(30000);

  Serial.println("RMDCAN Demo");

  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false);
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true);

  if(!gCan.begin(1000000)) {
    Serial.println("Failed to start CAN");
    while(1);
  }
  Serial.println("CAN Online");
  Serial.println("\n-------------------------------\n");

  servo.motorResume();

  printHelp();
}

void loop() {
  if(Serial.available()) {
    char cmd = Serial.read();
    switch(cmd) {
      case '?': printHelp(); break;
      case '1': {
        Serial.println("Get motor status 1");
        const RMDMsg &msg = servo.getMotorStatus1();
        Serial.print(" temp: ");
        Serial.println(msg.motorStatus1Reply.temp);
        Serial.print(" voltage: ");
        Serial.println(msg.motorStatus1Reply.volts());
        Serial.print(" error: ");
        Serial.println(msg.motorStatus1Reply.error);
        Serial.println("\n-------------------------------\n");
        } break;
      case '2': {
        Serial.println("Get motor status 2");
        const RMDMsg &msg = servo.getMotorStatus2();
        Serial.print(" temp: ");
        Serial.println(msg.motorStatus2Reply.temp);
        Serial.print(" torque current: ");
        Serial.println(msg.motorStatus2Reply.torqueCurrentAmps());
        Serial.print(" speed: ");
        Serial.println(msg.motorStatus2Reply.speed);
        Serial.print(" encoder: ");
        Serial.println(msg.motorStatus2Reply.encoderPos);
        Serial.println("\n-------------------------------\n");
        } break;
      case '3': {
        Serial.println("Get motor status 3");
        const RMDMsg &msg = servo.getMotorStatus3();
        Serial.print(" phase A current: ");
        Serial.println(msg.motorStatus3Reply.phaseAAmps());
        Serial.print(" phase B current: ");
        Serial.println(msg.motorStatus3Reply.phaseBAmps());
        Serial.print(" phase C current: ");
        Serial.println(msg.motorStatus3Reply.phaseCAmps());
        Serial.println("\n-------------------------------\n");
        } break;
      case 'S':
        Serial.print("Current speed: ");
        Serial.println(currentSpeed);
        Serial.println("\n-------------------------------\n");
        break;
      case 's': {
          Serial.print("Set speed (currently ");
          Serial.print(currentSpeed);
          Serial.println(")");
          Serial.print(" enter absolute max speed > ");
          while(Serial.available()) { Serial.read(); }
          while(!Serial.available()) { }
          String speedStr(Serial.readStringUntil('\n'));
          if(!speedStr.equals("")) {
            currentSpeed = (uint16_t) abs(speedStr.toInt());
            Serial.println(currentSpeed);
          }
        }
        Serial.println("\n-------------------------------\n");
        break;
      case 'P': {
          const RMDMsg &msg = servo.getPosition();
          Serial.print("Current position: ");
          Serial.print(msg.multiTurnAngleReply.angleDegrees());
          Serial.println(" degrees");
        }
        Serial.println("\n-------------------------------\n");
        break;
      case 'p': {
          Serial.println("Set position");
          Serial.print(" enter position in degrees > ");
          while(Serial.available()) { Serial.read(); }
          while(!Serial.available()) { }
          String angleStr(Serial.readStringUntil('\n'));
          if(!angleStr.equals("")) {
            float angle = angleStr.toFloat();
            Serial.println(angle);
            servo.setPosition(currentSpeed, angle);
          }
        }
        Serial.println("\n-------------------------------\n");
        break;
    }
  }
}
