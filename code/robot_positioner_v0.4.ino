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
int MCVC_freq = 9; // Sets motor frequency
int MCVC_dir = 6; // Sets motor direction
int MCVC_en = 7; 
int enc_return = 8; // input of encoder

long pulses = 4;
long gear_ratio = 1000;
float pos = 90; // variable to store the servo position
int home_pos = 90;
int stop_flag = 0;

// need some math calculations for the servo motor for specific rotations

/******Setup******/
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT); // sets the digital pin as output
  
  pinMode(e_stop, OUTPUT);

  //pinMode(MCVC_freq, OUTPUT);
  pinMode(MCVC_dir, OUTPUT);
  pinMode(MCVC_en, OUTPUT);

  pinMode(enc_return, INPUT);
  pinMode(CCW, INPUT);
  pinMode(CW, INPUT);
  pinMode(CW_45, INPUT);
  pinMode(CCW_45, INPUT);
  pinMode(home_rot, INPUT);

  Timer1.initialize(100); // set period
  attachInterrupt(digitalPinToInterrupt(stop_rot), stop_rotating , RISING);
}


/******Wait for Rotations to Finish******/
void wait_rotate(int degrees) {
  int pulse_count = 0;
  int n = LOW;
  int rot_direction;
  int enc_return_last = LOW;
  long abs_degrees = degrees;
  if (degrees > 0){
    rot_direction = -1;
  }
  else{
    rot_direction = 1;
    abs_degrees = degrees * (-1);
    Serial.print(degrees);
    Serial.print(abs_degrees);
  }
  long pulses_to_count = abs_degrees*pulses*gear_ratio; //-16608
  Serial.print((pulses_to_count));
  while ((pulse_count < pulses_to_count) && (stop_flag == 0)){
    n = digitalRead(enc_return);
    if ((enc_return_last == LOW) && (n == HIGH)) {
      pulse_count++;
      pos = pos + rot_direction/(pulses*gear_ratio);
      Serial.print("Count Encoder\n");
    }
    enc_return_last = n;
    Serial.print((stop_flag));
    Serial.print((pulse_count < pulses_to_count) && (stop_flag == 0));
    Serial.print("\n");
    Serial.print((pulse_count));
    Serial.print((pulses_to_count));
    Serial.print("\n");
    
  }
  
  Serial.print("Degrees is now: ");
  Serial.print((pos));
  Serial.print("\n");
}

/******Rotation Routine******/
void rotate(int degrees) {
  Timer1.pwm(MCVC_freq, 512); // 512 should be 50% duty cycle
  if (degrees < 0){ 
    digitalWrite(MCVC_dir, LOW); // set rotation direction to right
    Serial.print("Direction is CW\n");
  }
  else{ 
    digitalWrite(MCVC_dir, HIGH);
    Serial.print("Direction is CCW\n");
  }
  
  digitalWrite(MCVC_en, HIGH); // turn on rotation
  wait_rotate(degrees);
  
  digitalWrite(MCVC_en, LOW); // turn off
  Timer1.disablePwm(MCVC_freq);
}


/******Rotate Left******/
void rotate_CCW() { 
  Serial.print("Rotating CCW...\n"); // doesn't work
  rotate(1); 
}

/******Rotate Left 45******/
void rotate_CCW_45() {
  Serial.print("Rotating CCW 45...\n");
  rotate(45); 
}

/******Rotate Right******/
void rotate_CW() {
  Serial.print("Rotating CW...\n");
  rotate(-1); 
}

/******Rotate Right 45******/
void rotate_CW_45() { 
  Serial.print("Rotating CW 45...\n"); // doesn't work
  rotate(-45); 
}

/******Rotate Home Position******/
void home_position() {
  Serial.print("Rotating to home...\n");
  rotate((int)home_pos - pos); // wrap 360 degrees
}

/******Running******/
void loop() {
    digitalWrite(e_stop, HIGH);
    stop_flag = 0;
    Serial.print("TEST CYCLE\n");
    //home_position();  // always start in home position (90)
      if(digitalRead(CCW) == HIGH) {
          Serial.print("CCW\n"); // doesn't work
          rotate_CCW();
      } else if(digitalRead(CCW_45) == HIGH) {
          Serial.print("CCW_45\n");
          rotate_CCW_45();
      } else if(digitalRead(CW) == HIGH) {
          Serial.print("CW\n");
          rotate_CW();
      } else if(digitalRead(CW_45) == HIGH) { // doesn't work
          Serial.print("CW_45\n");
          rotate_CW_45();
      } else if(digitalRead(home_rot) == HIGH) {
          Serial.print("home_rot\n");
          home_position();
      }
}

/******Interrupt Routine******/
void stop_rotating(){
  stop_flag = 1; 
  Serial.print("Stop\n");
}
