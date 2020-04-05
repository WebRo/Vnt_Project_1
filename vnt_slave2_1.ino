#include <AccelStepper.h>
#include <Wire.h>

AccelStepper stepperX(1, 6, 7);
long aSpeed = 3000;
int aAcc = 5000;
int en = 8;
// Define the Pins used
#define home_switch 9 // Pin 9 connected to Home Switch (MicroSwitch)

// Stepper Travel Variables
long TravelX; // Used to store the X value entered in the Serial Monitor

int int_counter  = 0;
int speedVal_Master;
int travelVal_Master;
int breatheVal_Master;
int delayVal_Master;
int en1 = 1;
int en2 = 0;


//int move_finished = 1; // Used to check if move is completed
long initial_homing = -1; // Used to Home Stepper at startup

void setup()
{
  Serial.begin(9600); // Start the Serial monitor with speed of 9600 Bauds
  Wire.begin(2);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
pinMode(LED_BUILTIN, OUTPUT);
  pinMode(home_switch, INPUT_PULLUP);
  pinMode(en, OUTPUT);
  digitalWrite(home_switch, HIGH);
  digitalWrite(en, HIGH);


  homing();

}
void loop()
{




  int speedPotentiometer = speedVal_Master ;
  long speedVal = map(speedPotentiometer, 0, 255, 500, 9000);
  int travelPotentiometer = travelVal_Master;
  int TravelX = map(travelPotentiometer, 0, 255, 200, 2000);
  int breatheOutPotentiometer = breatheVal_Master;
  int breatheOut = map(breatheOutPotentiometer, 0, 255, 1, 10);
  int delayPotentiometer = delayVal_Master;
  int delayVal = map(delayPotentiometer, 0, 255, 0, 2000);


  while (stepperX.currentPosition() < TravelX) {

    stepperX.setMaxSpeed(speedVal); // Set Max Speed of Stepper (Slower to get better accuracy)
    stepperX.setAcceleration(speedVal);
    stepperX.moveTo(TravelX); // Set new moveto position of Stepper
    stepperX.run();
  }

  delay(delayVal);
  TravelX = 0;

  while (stepperX.currentPosition() > 0 )
  {
    stepperX.setMaxSpeed(speedVal / breatheOut); // Set Max Speed of Stepper (Slower to get better accuracy)
    stepperX.setAcceleration(speedVal / breatheOut);
    //stepperX.moveTo(TravelX); // Set new moveto position of Stepper
    stepperX.runToNewPosition(0);
    if(!digitalRead(home_switch)){
      stepperX.stop();
     stepperX.setCurrentPosition(-2);
      }
  }
//stepperX.setCurrentPosition(0);
  delay(delayVal);

  //Serial.println(en2);



}

void receiveEvent(int howMany) {
  while ( Wire.available() > 0) { // loop through all but the last
    int c = Wire.read(); // receive byte as a character
    //Serial.println(c);         // print the character
    int_counter++;
    switch (int_counter) {
      case 1:
        speedVal_Master = c;
      case 2:
        travelVal_Master = c;
      case 3:
        breatheVal_Master = c;
      case 4:
        delayVal_Master = c;
      case 5:
        en1 = c;
      case 6:
        en2 = c;
    }

  }
  if (int_counter == 6) {
    int_counter = 0;
  }

  if (en2 == 0 ) {
    //delay(100);
    digitalWrite(en, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  } else if (en2 == 1) {
    //delay(100);
    digitalWrite(en, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  
  }

}

void requestEvent() {
  long ponstion = map(stepperX.currentPosition(), 0, 1000, 0, 255);
  Wire.write(ponstion);

}

void homing() {

  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  stepperX.setMaxSpeed(aSpeed); // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(aAcc); // Set Acceleration of Stepper

  // Start Homing procedure of Stepper Motor at startup


  while (digitalRead(home_switch)) { // Make the Stepper move CCW until the switch is activated
    stepperX.moveTo(initial_homing); // Set the position to move to
    initial_homing--; // Decrease by 1 for next move if needed
    stepperX.run(); // Start moving the stepper
    delay(1);
  }

  stepperX.setCurrentPosition(0); // Set the current position as zero for now
  stepperX.setMaxSpeed(aSpeed); // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(aAcc); // Set Acceleration of Stepper
  initial_homing = 1;

//  while (!digitalRead(home_switch)) { // Make the Stepper move CW until the switch is deactivated
//    stepperX.moveTo(initial_homing);
//    stepperX.run();
//    initial_homing++;
//    //delay(5);
//  }

  stepperX.setCurrentPosition(0);
  digitalWrite(en, LOW);
  delay(100);
}
