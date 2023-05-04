#include <L298NX2.h>
int UART_IN = 0;
int UART_OLD = 0;
int New_Input = 0;
L298NX2 motors(5, 9, 6, 10);

#define Move_Delay 200

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Serial.print("Started.\n\r");
  pinMode(5, OUTPUT);   //Right motor back
  pinMode(6, OUTPUT);   //Left Motor back
  pinMode(9, OUTPUT);   //Right Motor forward
  pinMode(10, OUTPUT);  //Left Motor forward
  pinMode(12, OUTPUT);  //Right Motor enable
  pinMode(13, OUTPUT);  //Left Motor enable
  delay(100);
  }


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    UART_IN = Serial.read();
    if(UART_IN == UART_OLD){
      //Creating time out counter.

    }
    else{
      UART_OLD = UART_IN;

      switch (UART_IN) {
      case 'w':
        motors.forward();
        Serial.print("Forward\n\r");
        break;
      case 'a':
        motors.backwardA();
        motors.forwardB();
        Serial.print("Left Turn\n\r");
        break;
      case 'd':
        motors.backwardB();
        motors.forwardA();
        Serial.print("Right Turn\n\r");
        break;
      case 's':
        motors.backward();
        Serial.print("Backwards\n\r");
        break;
      default:
        motors.stop();
        Serial.print("Wating for input\n\r");
        break;
    }
    }
  }
}
