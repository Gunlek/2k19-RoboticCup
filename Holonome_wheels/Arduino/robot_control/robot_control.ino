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
    calibrate();
    odrv0.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    odrv0.run_state(1, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    odrv1.run_state(0, ODriveArduino::AXIS_STATE_CLOSED_LOOP_CONTROL, false);
    initialized = true;
  }
  else {
    odrv0.SetVelocity(0, 20000.0f);
    odrv0.SetVelocity(1, 20000.0f);
    odrv1.SetVelocity(0, 20000.0f);
  }
}

void calibrate(){
  int motornum = 0;
  int requested_state;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  Serial << "Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv0.run_state(motornum, requested_state, true);

  motornum++;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  Serial << "Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv0.run_state(motornum, requested_state, true);

  motornum=0;
  requested_state = ODriveArduino::AXIS_STATE_FULL_CALIBRATION_SEQUENCE;
  Serial << "Axis" << motornum << ": Requesting state " << requested_state << '\n';
  odrv1.run_state(motornum, requested_state, true);
}
