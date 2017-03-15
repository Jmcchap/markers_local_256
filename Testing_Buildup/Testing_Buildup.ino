#include <Time.h>
#include <TimeLib.h>


int pin[] ={D5};
int lightThreashold[] = {100};
int sensorValue[1] = {0};
int checkoutTime[] = {0};
bool overdue[] {false};
time_t currentTime = 0;
int allotedTime = 50;


void setup() {
  // put your setup code here, to run once:
pinMode(D5, OUTPUT);
pinMode(A0, INPUT);
digitalWrite(D5, HIGH);
Serial.begin(9600);
}

void readSensor(){
         digitalWrite(pin[0], HIGH);
        delay(250);
        sensorValue[0] = analogRead(A0);  
        Serial.print( "SensorValue = ");
        Serial.println(sensorValue[0]);
}

void assignTimer() {
  checkoutTime[0] = now() + allotedTime;
  
}




/*See if it is time for the marker to be returned*/
void checkTimer(){
currentTime = now();
  if (checkoutTime[0] < currentTime) {
    overdue[0] = true;
    
  }
}

void compareSensor(){
  if(sensorValue[0] < lightThreashold[0]){
     if (checkoutTime[0] == 0) {
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

  Serial.print("overdue = ");
  Serial.println(overdue[0]);
  Serial.print("now = ");
  Serial.println(currentTime);
  Serial.print("checkoutTime = ");
  Serial.println(checkoutTime[0]);
  
  Serial.println();
  Serial.println();
  Serial.println();
  delay(10000);
}
