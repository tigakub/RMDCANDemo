#include "RMD_L.hpp"

CANSAME5x gCan;       // Adafruit's CAN Driver API
RMDCAN rmd(gCan);     // Wrapper that can read and write CAN packets
RMD_L servo(1, rmd);  // Wrapper that 

uint16_t currentSpeed = 60;

// Function to print help message
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

// One-time Setup
void setup() {
  // Enable serial communication
  Serial.begin(115200);
  while(!Serial) delay(10);

  // Configure CAN hardware
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false);
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true);

  // Enable CAN communication at 1 Mbps
  if(!gCan.begin(1000000)) {
    Serial.println("Failed to start CAN");
    while(1);
  }

  // Print greeting
  Serial.println("RMDCAN Demo");
  Serial.println("CAN Online");
  Serial.println("\n-------------------------------\n");

  // Print help message
  printHelp();
}

void loop() {
  // If bytes are available on the console
  if(Serial.available()) {
    // Read the first character
    char cmd = Serial.read();
    switch(cmd) {
      // Help requested
      case '?': printHelp(); break;
      // Query the acutator for motor status 1
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
      // Query the acutator for motor status 2
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
      // Query the acutator for motor status 3
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
      // Display the speed to be used for position commands
      case 'S':
        Serial.print("Current speed: ");
        Serial.println(currentSpeed);
        Serial.println("\n-------------------------------\n");
        break;
      // Set the speed to be used for subsequent position commands
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
      // Display the actuator's angular position
      case 'P': {
          const RMDMsg &msg = servo.getPosition();
          Serial.print("Current position: ");
          Serial.print(msg.multiTurnAngleReply.angleDegrees());
          Serial.println(" degrees");
        }
        Serial.println("\n-------------------------------\n");
        break;
      // Command the actuator to an angle at the currently set speed
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
