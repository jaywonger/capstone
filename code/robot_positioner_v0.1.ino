#include "Servo.h"

// global variable declarations
Servo myservo;
int PWM_OUT_1 =11; // PWM output on pin 11
int PWM_OUT_2 =10; // PWM output on pin 10
int PWM_OUT_3 = 9; // PWM output on pin 9
int LED_PIN = 13; // LED status on pin 13

int val;

// need some math calculations for the servo motor for specific rotations

/******Setup******/
void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(LED_PIN, OUTPUT); // sets the digital pin as output

  pinMode(PWM_OUT_1, OUTPUT); // PWM output / frequency output
  pinMode(PWM_OUT_2, OUTPUT); // PWM output / frequency output
  pinMode(PWM_OUT_3, OUTPUT); // PWM output / frequency output
}

void setspeed(int throttle) {
   val = map(throttle, 1, 1023, 1, 180);
   myservo.write(val);
}

/******Emergency Stop******/
void e_stop() {
  
}

/******Rotate Left******/
void rotate_left() {
  myservo.write(val-5);
  delay(500);
}

/******Rotate Left 45******/
void rotate_left_45() {
  myservo.write(val-45);
  delay(1000);  // wait 1 second
}

/******Rotate Right******/
void rotate_right() {
  myservo.write(val+5);
  delay(500);
}

/******Rotate Right 45******/
void rotate_right_45() {
  myservo.write(val+45);
  delay(1000);  // wait 1 second
}

/******Rotate Home Position******/
void home_position() {
  myservo.write(90);
  delay(1000); // wait 1 second
}

/******Running******/
void loop() {
  if (!e_stop()) {
    if(button left is pressed) {
      rotate_left();
    } else if(button left 45) {
      rotate_left_45();
    } else if(button right) {
      rotate_right();
    } else if(button right 45) {
      rotate_right_45();
    }
  } else {
    // stop the machine
  }
  

}
