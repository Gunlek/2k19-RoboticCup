#include <SoftwareSerial.h>
#include <ODriveArduino.h>

template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }


ODriveArduino odrv0(Serial2);
ODriveArduino odrv1(Serial3);

float CURRENT_LIMIT = 10.0f;
float VELOCITY_LIMIT = 25000.0f;
float VELOCITY_RAMP = 5000.0f;

bool initialized = false;

bool DEBUG=true;

void readSerialInput();
void handleNewCommand(String cmd);
bool calibrate();
void sendSpeeds(float speed_1, float speed_2, float speed_3);
void checkMoveDuration();
void startMoving();

float global_speeds[3] = {0.0f, 0.0f, 0.0f};
unsigned long move_duration = -1;
unsigned long move_start = -1;

void setup(){
  Serial2.begin(115200);
  Serial3.begin(115200);

  Serial.begin(115200);

  // Set various settings
  for (int axis = 0; axis < 2; ++axis) {
    Serial2 << "w axis" << axis << ".controller.config.vel_limit " << 25000.0f << '\n';
    Serial2 << "w axis" << axis << ".motor.config.current_lim " << 20.0f << '\n';
    Serial2 << "w axis" << axis << ".controller.vel_ramp_enable " << true << '\n';

    Serial3 << "w axis" << axis << ".controller.config.vel_limit " << 25000.0f << '\n';
    Serial3 << "w axis" << axis << ".motor.config.current_lim " << 20.0f << '\n';
    Serial3 << "w axis" << axis << ".controller.vel_ramp_enable " << true << '\n';
  }
}

void loop(){
  if(!initialized){
    //calibrate();
    //odrv0.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    //odrv0.run_state(1, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    //odrv1.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    initialized = true;
  }
  else {
    readSerialInput();
    sendSpeeds(global_speeds[0], global_speeds[1], global_speeds[2]);
    checkMoveDuration();
  }
}

void readSerialInput(){
  while(Serial.available() > 0){
    String msg = "";
    char c = (char) Serial.read();
    while(c != '\n'){
      msg += c;
      delay(10);
      c = (char) Serial.read();
    }
    msg.toLowerCase();
    if(msg.startsWith("[motorcontrol]")){
      msg.replace("[motorcontrol]", "");
      handleNewCommand(msg);
      Serial.flush();
    }
  }
}

void handleNewCommand(String cmd){
  if(DEBUG) Serial.print("[DEBUG] Received new command: ");
  if(DEBUG) Serial.println(cmd);
  if(cmd.startsWith("speed")){
    if(DEBUG) Serial.println("[DEBUG] Sending new speeds to ODrive");
    // cmd format: speed;speed1;speed2;speed3;delay
    cmd.replace("speed;", "");
    float speed_1, speed_2, speed_3, duration;
    int index = 0;
    while(cmd.indexOf(";") >= 0){
      if(index==0)
        speed_1 = cmd.substring(0, cmd.indexOf(";")).toFloat();
      if(index==1)
        speed_2 = cmd.substring(0, cmd.indexOf(";")).toFloat();
      if(index==2){
        speed_3 = cmd.substring(0, cmd.indexOf(";")).toFloat();
        duration = cmd.substring(cmd.indexOf(";")+1).toFloat();
      }
      cmd = cmd.substring(cmd.indexOf(";")+1);
      index++;
      if(DEBUG) Serial.println("[DEBUG] Updated command is: "+cmd);
    }
    if(DEBUG) Serial.print("[DEBUG] Speed 1: ");
    if(DEBUG) Serial.println(speed_1);
    if(DEBUG) Serial.print("[DEBUG] Speed 2: ");
    if(DEBUG) Serial.println(speed_2);
    if(DEBUG) Serial.print("[DEBUG] Speed 3: ");
    if(DEBUG) Serial.println(speed_3);
    if(DEBUG) Serial.print("[DEBUG] Duration: ");
    if(DEBUG) Serial.println(duration);

    global_speeds[0] = speed_1;
    global_speeds[1] = speed_2;
    global_speeds[2] = speed_3;
    move_duration = (int) duration;

    startMoving();
  }
  else if(cmd.startsWith("state")){
    // cmd format: state;newState
    // Available states: speedControl, idle
    cmd.replace("state;", "");
    if(cmd.startsWith("speedcontrol")){
      if(DEBUG) Serial.println("[DEBUG] Going into speed control mode");
      odrv0.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
      odrv0.run_state(1, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
      odrv1.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    }
    else if(cmd.startsWith("idle")){
      if(DEBUG) Serial.println("[DEBUG] Going into idle mode");
      odrv0.run_state(0, ODriveArduino::AXIS_STATE_IDLE, false);
      odrv0.run_state(1, ODriveArduino::AXIS_STATE_IDLE, false);
      odrv1.run_state(0, ODriveArduino::AXIS_STATE_IDLE, false);
    }
    else if(cmd.startsWith("calibrate")){
      if(DEBUG) Serial.println("[DEBUG] Going into calibration mode");
      calibrate();
      if(DEBUG) Serial.println("[DEBUG] Calibration done !");
    }
  }
}

void sendSpeeds(float speed_1, float speed_2, float speed_3){
  /*if(speed_1 != 0 or speed_2 != 0 or speed_3 != 0){
    if(DEBUG) Serial.print("[DEBUG] Sending speeds: ");
    if(DEBUG) Serial.print(speed_1);
    if(DEBUG) Serial.print(", ");
    if(DEBUG) Serial.print(speed_2);
    if(DEBUG) Serial.print(", ");
    if(DEBUG) Serial.println(speed_3);
  }*/
  odrv0.SetVelocity(0, speed_1);
  odrv0.SetVelocity(1, speed_2);
  odrv1.SetVelocity(0, speed_3);
}

void startMoving(){
  move_start = millis();
}

void checkMoveDuration(){
  if(move_duration > 0){
    if(move_start > 0){
      if(DEBUG) Serial.print("[DEBUG] Move duration: ");
      if(DEBUG) Serial.print(move_duration);
      if(DEBUG) Serial.print(" started at ");
      if(DEBUG) Serial.print(move_start);
      if(DEBUG) Serial.print(" difference with current millis(): ");
      if(DEBUG) Serial.println(millis() - move_start);
      // We're then moving
      if((millis() - move_start) >= move_duration){
        if(DEBUG) Serial.println("[DEBUG] Stopping movement");
        global_speeds[0] = 0.0f;
        global_speeds[1] = 0.0f;
        global_speeds[2] = 0.0f;
        move_duration = 0;
        move_start = 0;
      }
    }
  }
}

bool calibrate(){
  int motornum = 0;
  int requested_state;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  if(DEBUG) Serial << "[DEBUG] Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv0.run_state(motornum, requested_state, true);

  motornum++;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  if(DEBUG) Serial << "[DEBUG] Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv0.run_state(motornum, requested_state, true);

  motornum=0;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  if(DEBUG) Serial << "[DEBUG] Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv1.run_state(motornum, requested_state, true);

  return true;
}
