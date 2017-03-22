#include <Time.h>
#include <TimeLib.h>


int pin[] ={D5, D6, D7, D8};
int lightThreashold[] = {100, 100, 100, 100};
int sensorValue[] = {0, 0, 0, 0};
int checkoutTime[] = {0, 0, 0, 0};
bool overdue[] {false, false, false, false};
time_t currentTime = 0;
int allotedTime = 50;
int i = 0;


void setup() {
  // put your setup code here, to run once:
pinMode(D5, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);
pinMode(D8, OUTPUT);
pinMode(A0, INPUT);
digitalWrite(D5, LOW);
digitalWrite(D6, LOW);
digitalWrite(D7, LOW);
digitalWrite(D8, LOW);
Serial.begin(9600);
}

void readSensor(){
  for( i=0; i<4; i++){
         digitalWrite(pin[i], HIGH);
        delay(250);
        sensorValue[i] = analogRead(A0);  
        digitalWrite(pin[i], LOW);
        
        delay(1000);
        
  }

    Serial.print( "SensorValue[0] = ");
        Serial.println(sensorValue[0]);
        Serial.print( "SensorValue[1] = ");
        Serial.println(sensorValue[1]);
         Serial.print( "SensorValue[2] = ");
        Serial.println(sensorValue[2]);
         Serial.print( "SensorValue[3] = ");
        Serial.println(sensorValue[3]);
}

void assignTimer() {
  checkoutTime[i] = now() + allotedTime;
  
}




/*See if it is time for the marker to be returned*/
void checkTimer(){
currentTime = now();
  if (checkoutTime[i] < currentTime) {
    overdue[i] = true;
    
  }
}

void compareSensor(){
  if(sensorValue[i] < lightThreashold[i]){
     if (checkoutTime[i] == 0) {
      assignTimer();
    }
    else {
      checkTimer();
    }    
  }
}

void loop() {
  
  readSensor();
  compareSensor();

//  Serial.print("overdue = ");
//  Serial.println(overdue[0]);
//  Serial.print("now = ");
//  Serial.println(currentTime);
//  Serial.print("checkoutTime[0] = ");
//  Serial.println(checkoutTime[0]);
//   Serial.print("checkoutTime[1] = ");
//  Serial.println(checkoutTime[1]);
  
  Serial.println();
  Serial.println();
  Serial.println();
  delay(3000);
}
