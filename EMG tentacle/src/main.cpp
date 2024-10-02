#include <Arduino.h>

#include <Servo.h>
#define pinServo 3
#define pinServo2 5
#define pinServo3 6
#define pinServo4 9
#define pinServo5 10
#define pinServo6 11

Servo servoMotor;
Servo servoMotor2;
Servo servoMotor3;
Servo servoMotor4;
Servo servoMotor5;
Servo servoMotor6;

int pot, pot2, pot3, pot4, contN, contP, contH, contV, nivelP, nivelN, modoH,modoV,modo3,modo4;
int velH = 90, velV = 90 ;
unsigned long t0, t1;

void setup() {
  Serial.begin(115200);
  pinMode(pot, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
  pinMode(pot4, INPUT);
  // SERVOMOTOR 4 (HORIZONTAL) Y 3 (VERTICAL) --> NIVEL 1  
  // SERVOMOTOR 2 (HORIZONTAL) Y 1 (VERTICAL) --> NIVEL 2  
  // SERVOMOTOR 6 (HORIZONTAL) Y 5 (VERTICAL) --> NIVEL 3  
  servoMotor.attach(pinServo);
  servoMotor2.attach(pinServo2);
  servoMotor3.attach(pinServo3);
  servoMotor4.attach(pinServo4);
  servoMotor5.attach(pinServo5);
  servoMotor6.attach(pinServo6);
  
  modoH = 0;
  modoV = 0;
  t0 = millis();
  t1 = millis();
  contH = 0;
  contV = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  pot = analogRead(A0);
  pot2 = analogRead(A1);
  pot3 = analogRead(A2);
  pot4 =  analogRead(A3);

  //float volt1 = 5.0 * (pot/1023.0);
  //float volt2 = 5.0 * (pot2/1023.0);
  //float volt3 = 5.0 * (pot3/1023.0);

//MODO DE MOVIMIENTO HORIZONTAL 
if (millis() - contH >= 1000){
  //---------EMG 1-----------------------------------------------------------------
  if ((pot >= 330)&&(modoH == 0)){
    

    modoH = 1;
    Serial.print(modoH);
    velH = 80;
    
  }
  else if ((pot >= 330)&&(modoH == 2)){
    contH=0;

    modoH = 0;
    velH = 90;
    
  }
 



//servoMotor6.write(90);

// -----SENSOR EMG2-----------------------------------------------------------------
  if  ((pot2 >= 380)&&(modoH == 0)){
    modoH = 2;
    velH = 110;
    
  }

  else if ((pot2 >= 380)&&(modoH == 1)){
    contH=0;

    modoH = 0;
    velH = 90;
    
  }
}
servoMotor2.write(velH);
//****************************************************************************************************************************************************
// MODO DE MOVIMIENTO VERTICAL -----------------------------------------------------------------------------------------------------
if (millis()-contV >= 1000){
  // sensor EMG 3 -----------------------------------------------------------------------------------------------------

if  ((pot3 >= 320)&&(modoV == 0)){

    
    modoV = 1;
    velV = 80;

  }

else if ((pot3 >= 320)&&(modoV == 2)){
    contV=0;

    modoV = 0;
    velV = 90;
    
  }

  
 

//servoMotor5.write(90);
 // sensor EMG4--------------------------------------------------------------------------------------------------------

  if  ((pot4 >= 350)&&(modoV == 0)){
    modoV = 2;
    velV=110;
    
  }
  else if ((pot4 >= 350)&&(modoV == 1)){
    contV=0;

    modoV = 0;
    velV = 90;
    
  }
  
}
servoMotor.write(velV);
// PRINT---------------------------------------------------------
  if(millis() - t0 >= 10){
    t0 = millis();
    Serial.print(pot);
    Serial.print(";");
    Serial.print(pot2);
    Serial.print(";");
    Serial.print(pot3);
    Serial.print(";");
    Serial.println(pot4);
  
  }

}
