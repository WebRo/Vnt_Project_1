

#include <Wire.h>

int en1 = 1;
int en2 = 0;
const unsigned long interval = 7200000;
unsigned long previousTime = 0;
int enableMotor1;
int enableMotor2;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.setTimeout(10);
}



void loop() {

  data_request1();
  data_request2();

  unsigned long currentTime = millis();
  if (currentTime - previousTime >= interval) {
    if (en1 == 1 && en2 == 0 && enableMotor1 == 0) {
      en2 = 1;

      en1 = 0;
      
      previousTime = currentTime;
    }else if (en1 == 0 && en2 == 1 && enableMotor2 == 0) {
      en1 = 1;

      en2 = 0;
      
      

      previousTime = currentTime;
    }
    //previousTime = currentTime;
        Serial.print("EN1 = ");
        Serial.println(en1);
        Serial.print("EN2 = ");
        Serial.println(en2);
        Serial.println("------------");
        Serial.println(enableMotor1);
        Serial.println(enableMotor2);
  }



  int speedPotentiometer = analogRead(A0);
  int speedVal  = map(speedPotentiometer, 0, 1023, 0, 255);
  int travelPotentiometer = analogRead(A1);
  int travelVal  = map(travelPotentiometer, 0, 1023, 0, 255);
  int breathePotentiometer = analogRead(A2);
  int breatheVal  = map(breathePotentiometer, 0, 1023, 0, 255);
  int delaylPotentiometer = analogRead(A3);
  int delaylVal  = map(delaylPotentiometer, 0, 1023, 0, 255);


  for (int x = 1; x < 3; x++) {
    Wire.beginTransmission(x); // transmit to device #8

    Wire.write(speedVal);
    Wire.write(travelVal);
    Wire.write(breatheVal);
    Wire.write(delaylVal);
    Wire.write(en1);
    Wire.write(en2);
    Wire.endTransmission();    // stop transmitting
  }

}
/*----------------------------------------------------------*/
void data_request1() {
  Wire.requestFrom( 1, 1);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    int slaveMoterPostion1 = Wire.read(); // receive a byte as character
    enableMotor1 = slaveMoterPostion1;
    //Serial.println(slaveMoterPostion1);
  }
}
void data_request2() {
  Wire.requestFrom( 2, 1);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    int slaveMoterPostion2 = Wire.read(); // receive a byte as character
    enableMotor2 = slaveMoterPostion2;
    //Serial.println(slaveMoterPostion2);
  }
}
