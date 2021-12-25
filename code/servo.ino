
/ ducks color filter wheel Ver.1

#include <Servo.h>


Servo myservo;
int CurrentFilter = 0; //current filter pisition
int  HomePin = 2;  //home position switch
int PositionPin = 3; //position switch for changing filters
int homePos;
int switchVal;


void setup()
{

  Serial.begin(19200);
  Serial.flush();
  pinMode(HomePin, INPUT);
  pinMode(PositionPin, INPUT);
  myservo.attach(9);
  myservo.writeMicroseconds(1501);
  boolean homePos = true;
  homeset(); // runs once till home position is set.


}
void loop()
{
  String cmd;

  if (Serial.available() >0) {
    cmd = Serial.readStringUntil('#');
    if (cmd=="GETFILTER") {
      Serial.print(CurrentFilter);
      Serial.println("#");
    }
    else if (cmd=="FILTER1") MoveFilter(1);
    else if (cmd=="FILTER2") MoveFilter(2);
    else if (cmd=="FILTER3") MoveFilter(3);
    else if (cmd=="FILTER4") MoveFilter(4);
    else if (cmd=="FILTER5") MoveFilter(5);
  }




}
void MoveFilter(int pos)
{
  delay(2000);
  CurrentFilter = pos;
  Serial.println("0#");

}


void homeset()   // used to set the home position when first powered up.
// thanks to Robin2 for this section

{
  int switchVal;

  switchVal = digitalRead(HomePin);

  while (switchVal == LOW) { // assumes LOW when contacts close
    myservo.write(1400);
    switchVal = digitalRead(HomePin);
  }
  myservo.write(1501);
  CurrentFilter = (1);


} 