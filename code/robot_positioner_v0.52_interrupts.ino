#include <TimerOne.h>
// global variable declarations
int LED_PIN = 13; // LED status on pin 13
int e_stop = 12;
int CCW = 15;
int CW = 14;
int CW_45 = 16;
int CCW_45 = 17;
int home_rot = 19;
int stop_rot = 2;
int pulse_detect = 3; // interrupt pin for encoder
int MCVC_freq = 9; // Sets motor frequency
int MCVC_dir = 6; // Sets motor direction
int MCVC_en = 7;

long pulses = 4;
long gear_ratio = 500;
float pos = 90; // variable to store the servo position
float home_pos = 90;
int stop_flag = 0;
int update_count = 0;

int previous_home_press = 0;
int home_reading = 0;
unsigned long home_counter = 0;
float degrees_to_rotate = 0;

// need some math calculations for the servo motor for specific rotations

/******Setup******/
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // sets the digital pin as output

  pinMode(e_stop, OUTPUT);

  //pinMode(MCVC_freq, OUTPUT);
  pinMode(MCVC_dir, OUTPUT);
  pinMode(MCVC_en, OUTPUT);

  pinMode(CCW, INPUT);
  pinMode(CW, INPUT);
  pinMode(CW_45, INPUT);
  pinMode(CCW_45, INPUT);
  pinMode(home_rot, INPUT);

  Timer1.initialize(200); // set period
  attachInterrupt(digitalPinToInterrupt(stop_rot), stop_rotating , RISING);

  digitalWrite(e_stop, HIGH);
}


/******Wait for Rotations to Finish******/
void wait_rotate(int degrees) {
  float pulse_count = 0;
  float rot_direction;
  long abs_degrees = degrees;
  if (degrees > 0){
    rot_direction = -1;
  }
  else {
    rot_direction = 1;
    abs_degrees = degrees * (-1);
    Serial.print(degrees);
    Serial.print(abs_degrees);
  }
  float pulses_to_count = (abs_degrees*pulses*gear_ratio)/360.0;
  Serial.print((pulses_to_count));
  Serial.print("\n");
  attachInterrupt(digitalPinToInterrupt(pulse_detect), pulse_count_ISR , RISING);
  interrupts();
  while ((pulse_count < pulses_to_count) && (stop_flag == 0)){
    if (update_count == 1) {
      update_count = 0;
      pulse_count++;
      pos = pos + rot_direction/((pulses*gear_ratio)/360.0);
    }
    Serial.print(pulse_count);
    Serial.print("\n");
  }
  detachInterrupt(digitalPinToInterrupt(pulse_detect));
  noInterrupts();
  if (pos < 0){
    pos = pos + 360;
  }
  else if (pos > 360){
    pos = pos - 360;
  }
  Serial.print("Degrees is now: ");
  Serial.print((pos));
  Serial.print("****************************************************************************");
  Serial.print("\n");

  stop_flag = 0;
}

/******Rotation Routine******/
void rotate(int degrees) {
  Timer1.pwm(MCVC_freq, 512); // 512 should be 50% duty cycle
  if (degrees < 0){
    digitalWrite(MCVC_dir, LOW); // set rotation direction to right
    Serial.print("Direction is CW\n");
  }
  else {
    digitalWrite(MCVC_dir, HIGH);
    Serial.print("Direction is CCW\n");
  }
  digitalWrite(MCVC_en, HIGH); // turn on rotation
  wait_rotate(degrees);
}


void disable_Encoder() {
  Timer1.disablePwm(MCVC_freq);
  digitalWrite(MCVC_en, LOW); // turn off
}


/******Running******/
void loop() {
    //Serial.print("TEST CYCLE\n");

    if (digitalRead(e_stop) == LOW) { // added this in for the e_stop check
      stop_rotating();
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
      if(digitalRead(CCW) == HIGH) {
          while (digitalRead(CCW) == HIGH){
            Serial.print("CCW\n"); // doesn't work
            rotate(1);
          }
          disable_Encoder();
      } else if(digitalRead(CCW_45) == HIGH) {
          Serial.print("CCW_45\n");
          rotate(45);
          disable_Encoder();
      } else if(digitalRead(CW) == HIGH) {
          while (digitalRead(CW) == HIGH){  
            Serial.print("CW\n");
            rotate(-1);
          }
          disable_Encoder();
      } else if(digitalRead(CW_45) == HIGH) { // doesn't work
          Serial.print("CW_45\n");
          rotate(-45);
          disable_Encoder();
      } else if(digitalRead(home_rot) == HIGH) {
          if (previous_home_press == 0){
            Serial.print("Start counting:\n");
          }
          home_counter ++;
          Serial.print(home_counter);
          Serial.print("\n");
          if (digitalRead(home_rot) == 0){
            if (home_counter >= 500) {  // hold
              Serial.print("home_position: long press\n");
              home_pos = pos;  // new home position
              Serial.print("New home position is: ");
              Serial.print(home_pos);
              Serial.print("\n");
            } else if (home_counter >= 25) { // short press
              Serial.print("home_position: short press\n");
              rotate(pos - home_pos); 
              disable_Encoder();
            }
          }
       }
       previous_home_press = digitalRead(home_rot);
       if (previous_home_press == 0){
        home_counter = 0;
       }       
    }
}

/******Interrupt Routine******/
void stop_rotating() {
  stop_flag = 1;
  //detachInterrupt(digitalPinToInterrupt(pulse_detect));
  noInterrupts();
  disable_Encoder();
  Serial.print("Stop\n");
}

void pulse_count_ISR() {
  if (stop_flag == 0){
    update_count = 1;
    //Serial.print("P\n");
  }
  else {
    Serial.print("NP\n");
  }
}
