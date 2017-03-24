#include <Time.h>
#include <TimeLib.h>


int pin[] ={D5, D6, D7, D8};                    //digital output pins
int lightThreashold[] = {1000, 100, 100, 100};   //determines if marker is 'in' or not
int sensorValue[] = {0, 0, 0, 0};               //actual analog value from sensors
int checkoutTime[] = {0, 0, 0, 0};              //how long each marker is 'allowed' to be removed
bool overdue[] = {false, false, false, false};  //the flag for if the checkoutTime has surpassed how long it can be borrowed
bool markerDocked[] = {true, true, true, true}; //if each marker is in its home, then true
time_t currentTime = 0;                         //the time retrieved from the now() function
int allotedTime = 50;                           //how long a marker can be withdrawn for
int sensorIndex = 0;                            //which  sensor in the array


void setup() {
  
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

/*readSensor sets one digital pin (sensorIndex) to HIGH and reads the analog value that the corresponding IR sensor has. 
Then, it stores that value into the proper slot in sensorValue[] and returns the digital pin to LOW. */
void readSensor(){
  Serial.println("readSensor");
  Serial.print( "SensorValue[sensorIndex] = ");
  Serial.println(sensorValue[sensorIndex]);
  Serial.print("SensorValue[0] = ");
  Serial.println(sensorValue[0]);
  Serial.println("~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
        digitalWrite(pin[sensorIndex], HIGH);
        delay(250);
        sensorValue[sensorIndex] = analogRead(A0);  
        digitalWrite(pin[sensorIndex], LOW);        
        delay(250);
}

/*Find the time that the marker needs to be returned.*/
void assignTimer() {
  Serial.print("assignTimer");
  checkoutTime[sensorIndex] = now() + allotedTime;  
}

/*See if it is time for the marker to be returned*/
void checkTimer(){
  Serial.println("checkTimer");
currentTime = now();
  if (checkoutTime[sensorIndex] < currentTime) {
    overdue[sensorIndex] = true;    
  }
}

void compareSensor(){
  Serial.println("compareSensor!");
  Serial.print( "SensorValue[sensorIndex] = ");
  Serial.println(sensorValue[sensorIndex]);
  Serial.print("SensorValue[0] = ");
  Serial.println(sensorValue[0]);
  Serial.println("---");
  Serial.print("sensorIndex = ");
  Serial.println(sensorIndex);
  Serial.println();
  
  if(sensorValue[sensorIndex] < 100){
      markerDocked[sensorIndex] = false;
  }else{
      markerDocked[sensorIndex] = true;
  }
}

void loop() {
  for( sensorIndex=0; sensorIndex<4; sensorIndex++){
    
    readSensor();
    compareSensor();    
      if(markerDocked[sensorIndex] = false){      //if the marker is removed
          if (checkoutTime[sensorIndex] == 0) {   //and no checkout time is assigned
            assignTimer();                        //"checkout" the marker
          }                                       //BUT
          else {                                  //if the marker is already checked out
            checkTimer();                         //check the timer to make sure it's not overdue
          }
      }else{                                      //if the marker is in its home
           checkoutTime[sensorIndex] = 0;         //if so, reset the timer. It is returned
      }

     
        Serial.print( "SensorValue[0] = ");
  Serial.println(sensorValue[0]);
//  Serial.print( "SensorValue[1] = ");
//  Serial.println(sensorValue[1]);
//  Serial.print( "SensorValue[2] = ");
//  Serial.println(sensorValue[2]);
//  Serial.print( "SensorValue[3] = ");
//  Serial.println(sensorValue[3]);
  Serial.println("------------------------------------------");
  Serial.print("markerDocked[0] = ");
  Serial.println(markerDocked[0]);
  Serial.print("now = ");
  Serial.println(currentTime);
  Serial.print("checkoutTime[0] = ");
  Serial.println(checkoutTime[0]);
  Serial.print("Overdue[0] = ");
  Serial.println(overdue[0]);
  


  
  Serial.println();
  Serial.println();
  Serial.println();
  }

  delay(3000);
}
