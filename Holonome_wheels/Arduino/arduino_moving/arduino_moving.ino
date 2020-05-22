/*  Author: Fabien AUBRET
 *  Date: March 2020 
 */


#include <SoftwareSerial.h>

#define AXIS_STATE_UNDEFINED 0           //<! will fall through to idle
#define AXIS_STATE_IDLE 1                //<! disable PWM and do nothing
#define AXIS_STATE_STARTUP_SEQUENCE 2 //<! the actual sequence is defined by the config.startup_... flags
#define AXIS_STATE_FULL_CALIBRATION_SEQUENCE 3   //<! run all calibration procedures, then idle
#define AXIS_STATE_MOTOR_CALIBRATION 4   //<! run motor calibration
#define AXIS_STATE_SENSORLESS_CONTROL 5  //<! run sensorless control
#define AXIS_STATE_ENCODER_INDEX_SEARCH 6 //<! run encoder index search
#define AXIS_STATE_ENCODER_OFFSET_CALIBRATION 7 //<! run encoder offset calibration
#define AXIS_STATE_CLOSED_LOOP_CONTROL 8  //<! run closed loop control

#define CTRL_MODE_VOLTAGE_CONTROL 0
#define CTRL_MODE_CURRENT_CONTROL 1
#define CTRL_MODE_VELOCITY_CONTROL 2
#define CTRL_MODE_POSITION_CONTROL 3
#define CTRL_MODE_TRAJECTORY_CONTROL 4

#define INTENSITY_LIMIT 10
#define MAX_VELOCITY 20000

SoftwareSerial odrv1(10, 3);
SoftwareSerial odrv0(11, 4);

void setState(SoftwareSerial interface, int motor, int state);                  // Set the current motor state
int getState(SoftwareSerial interface, int motor);                              // Return the current motor state
String readString(SoftwareSerial interface);                                    // Read incoming string from interface
void setControlMode(SoftwareSerial interface, int motor, int control_mode);     // Set control mode for the motor
void setVelocity(SoftwareSerial interface, int motor, int velocity);            // Set the motor's speed (speed is in count / s)
void setCurrentLimit(SoftwareSerial interface, int motor, int limit);           // Set the current limit for the motor
void setVelocityLimit(SoftwareSerial interface, int motor, int limit);          // Set the velocity limit for the motor             // TO FIX
void setVelocityRamp(SoftwareSerial interface, int motor, int value);
void enableVelocityRamp(SoftwareSerial interface, int motor, int value);

void setup() {
  Serial.begin(9600);
  odrv0.begin(115200);
  odrv1.begin(115200);

  //initAxis(odrv0, 1);
  // initAxis(odrv0, 1);
  initAxis(odrv1, 0);
  delay(500);

  // setControlMode(odrv0, 1, CTRL_MODE_VELOCITY_CONTROL);
  // delay(250);
  setVelocityRamp(odrv1, 0, 5000);
  delay(250);
  enableVelocityRamp(odrv1, 0, 1);
  delay(250);
  setVelocity(odrv1, 0, 15000);
  
  // setVelocity(odrv0, 0, 0);
  // setVelocity(odrv0, 1, 0);
  //setVelocity(odrv1, 0, 0);
  //delay(500);
  // setVelocity(odrv0, 0, 4000);
  // setVelocity(odrv0, 1, 4000);
  //setVelocity(odrv1, 0, 4000);
  //delay(250);

  Serial.println("Motor ON");
}

void loop() {
  // Serial.println(getState(odrv1, 0));

}

void initAxis(SoftwareSerial interface, int motor){
  setState(interface, motor, AXIS_STATE_FULL_CALIBRATION_SEQUENCE);
  while(getState(interface, motor) != AXIS_STATE_IDLE);

  //setControlMode(interface, motor, CTRL_MODE_VELOCITY_CONTROL);
  //delay(100);
  //setCurrentLimit(interface, motor, INTENSITY_LIMIT);
  //delay(100);
  //setVelocityLimit(interface, motor, MAX_VELOCITY);
  //delay(500);

  setState(interface, motor, AXIS_STATE_CLOSED_LOOP_CONTROL);
}

void setState(SoftwareSerial interface, int motor, int state){
  interface.print("w axis");
  interface.print(String(motor));
  interface.print(".requested_state ");
  interface.println(String(state));
}

int getState(SoftwareSerial interface, int motor){  
  int timeout = 1000;
  int readInt = readString(interface).toInt();
  while(readInt != AXIS_STATE_IDLE && timeout-- > 0){
    delay(100);
    interface.print("r axis");
    interface.print(String(motor));
    interface.println(".current_state");
    readInt = readString(interface).toInt();
    Serial.println(readInt);
  }

  if(timeout > 0){
    return readInt;
  }
  else {
    return timeout;
  }
}

void setControlMode(SoftwareSerial interface, int motor, int control_mode){
  interface.print("w axis");
  interface.print(motor);
  interface.print(".controller.config.control_mode ");
  interface.println(control_mode);
}

void setVelocity(SoftwareSerial interface, int motor, int velocity){
  interface.print("v ");
  interface.print(motor);
  interface.print(" ");
  interface.println(velocity);
}

void setCurrentLimit(SoftwareSerial interface, int motor, int limit){
  interface.print("w axis");
  interface.print(motor);
  interface.print(".motor.config.current_lim ");
  interface.println(limit);
}

void setVelocityLimit(SoftwareSerial interface, int motor, int limit){
  interface.print("w axis");
  interface.print(motor);
  interface.print(".controller.config.vel_limit ");
  interface.println(limit);
}

void setVelocityRamp(SoftwareSerial interface, int motor, int value){
  interface.print("w axis");
  interface.print(motor);
  interface.print(".controller.config.vel_ramp_rate ");
  interface.println(value);
}

void enableVelocityRamp(SoftwareSerial interface, int motor, int value){
  interface.print("w axis");
  interface.print(motor);
  interface.print(".controller.vel_ramp_enable ");
  interface.println(value);
}

String readString(SoftwareSerial interface){
  unsigned long timeout = 1000;
  unsigned long timeout_start = millis();
  String str = "";
  while(odrv1.available() > 0){
    char c = (char)odrv1.read();
    if(c == '\n')
      break;
    str += c;
  }

  return str;
}
