#include <L298NX2.h>
#include <Wire.h>
int UART_IN = 0;
int UART_OLD = 0;
int New_Input = 0;
L298NX2 motors(5, 9, 6, 10);

#define alpha .5
#define Move_Delay 200
void Spinner(void);
void Set_UI(void);
float Xacceleration;
float Yacceleration;
float ylastSensorReading;
float xlastSensorReading;
float xa;
float ya;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Wire.begin();
  Wire.beginTransmission(0x53);
  Wire.write(0x2C);
  Wire.write(0x08);
  Wire.beginTransmission(0x53);
  Wire.write(0x31);
  Wire.write(0x08);
  Wire.endTransmission();

  Wire.beginTransmission(0x53);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();
  //Serial.print("Started.\n\r");
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);   //Right motor back
  pinMode(6, OUTPUT);   //Left Motor back
  pinMode(9, OUTPUT);   //Right Motor forward
  pinMode(10, OUTPUT);  //Left Motor forward
  pinMode(12, OUTPUT);  //Right Motor enable
  pinMode(13, OUTPUT);  //Left Motor enable
  delay(100);
  delay(2000);
  Serial.print("\e{H\033[2JSystem Booting \e[?25l");
  Spinner();
  Set_UI();
}


void loop() {
  xlastSensorReading = xa;
  Wire.beginTransmission(0x53);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte x0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x33);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte x1 = Wire.read();
  x1 = x1 & 0x03;

  uint16_t x = (x1 << 8) + x0;
  int16_t xf = x;
  if (xf > 511) {
    xf = xf - 1024;
  }
  xa = xf * 0.004;
  Xacceleration = alpha * xa + (1 - alpha) * xlastSensorReading;
  Wire.beginTransmission(0x53);
  ylastSensorReading = ya;
  Wire.write(0x34);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte y0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x35);
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte y1 = Wire.read();
  y1 = y1 & 0x03;

  uint16_t y = (y1 << 8) + y0;
  int16_t yf = y;
  if (yf > 511) {
    yf = yf - 1024;
  }
  ya = yf * 0.004;
  Yacceleration = .5 * ya + (1 - alpha) * ylastSensorReading;

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    UART_IN = Serial.read();
    if (UART_IN == UART_OLD) {
      //Creating time out counter.
    } else {
      UART_OLD = UART_IN;

      switch (UART_IN) {
        case 'w':
          motors.forward();
          Serial.print("\rForward.   ");
          break;
        case 'a':
          motors.backwardA();
          motors.forwardB();
          Serial.print("\rLeft Turn. ");
          break;
        case 'd':
          motors.backwardB();
          motors.forwardA();
          Serial.print("\rRight Turn.");
          break;
        case 's':
          motors.backward();
          Serial.print("\rBackwards. ");
          break;
        case 'x':
          motors.stop();
          Serial.print("\rStopped.   ") default : motors.stop();
          Serial.print("\rIdle.      ");
          break;
      }
      Serial.print("X Acceleration: ");
      Serial.print(Xacceleration);
      Serial.print(" Y Acceleration: ");
      Serial.print(Yacceleration);
    }
  }
}
char Spin = { "|/-\\" };  //Idle spinner do-dat

void Spinner(void) {
  delay(300);
  Serial.print("|");
  delay(300);
  Serial.print("\b/");
  delay(300);
  Serial.print("\b-");
  delay(300);
  Serial.print("\b\\");
  delay(300);
  Serial.print("\b");
  delay(50);
  Serial.print("\r\n");
}

void Set_UI(void) {
  Serial.print("\e{H\033[2J\b\033[1A");
  delay(50);
  Serial.print("Team B5 B.A.R. Firmware Version 0.1.1 Successfully Loaded.\n\r");
  delay(50);
  Serial.print("Code is open source publishe under the MIT license. See https://github.com/Direxfire/B5-Rover-Arduino for more information. \n\r");
  delay(100);
  Serial.print("Rover connected and ready to use.");
  delay(50);
  Serial.print("\n\rDriving Instructions: \n\r w: Forward \n\r s: Reverse \n\r a: Left Turn \n\r d: Right Turn \n\r Throttle Control Not Support on this version. ");
  Serial.print("\n\rStatus:\n");
}