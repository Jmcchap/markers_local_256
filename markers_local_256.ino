#include <dummy.h>

#include <ESP8266WiFi.h>
//#include <WifiManager.h>
#include <QTRSensors.h>
#include <Time.h>
#include <TimeLib.h>

/* INPUTS */
int pins[] = {D5, D6, D7, D8};              //Digital pins

/* OUTPUTS */

/*CONSTANTS*/
int allotedTime = 5000;                          //How much time the user is allowed to check out the pen (5 seconds)
int lightThreshold[] = {511, 511, 511, 511};   //The value in which the pen is considered removed. NEEDS TRIAL


/*OTHER VARIABLES*/
int checkoutTime[] = {0, 0, 0, 0};              //The time in which each pen was removed
int sensorValue[] = {0, 0, 0, 0};               //The analog values returned by sensor
time_t currentTime = 0;                            //The time retrieved from now()
int sensorQTY = 4;                              //The number of sensors - 1
int sensorIndex = 0;                            //what sensor is currently being worked on
bool overdue[] = {false, false, false, false};  //Pretty much determines if the conditions are right for the buzzer to buzz


//connect to WiFi network
void setup_wifi(){

  Serial.println("Starting wireless.");
  WiFiManager wifiManager; //Load the Wi-Fi Manager library.
  wifiManager.setTimeout(300); //Give up with the AP if no users gives us configuration in this many secs.
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }
  
  Serial.println("");
  Serial.print("Successfully connected to ");
  Serial.println(WiFi.localIP());
}

void setup() {
  setup_wifi();
 
  for(i=0; i < sensorQTY; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

/*read the sensor that is highlighted (sensorIndex) by setting the Digital output to high for the sensor
and reading it from the Analog sensor A0.*/
readSensors(sensorIndex){
  digitalWrite(sensorIndex, HIGH);
  delay(250);
  sensorValue[sensorIndex] = analogRead(A0);
  digitalWrite(sensorIndex, LOW);
}

checkTimer(sensorIndex){
  currentTime = now();
  if(checkoutTime[sensorIndex] > currentTime){
    overdue[sensorIndex] = TRUE;
  }
}

assignTimer(sensorIndex){
  checkoutTime[sensorIndex] = now() + allotedTime;
}


/*Compare the values of the sensor that is read against the light threashold that is set. If it is determined
that the marker is removed, then a timer would be assigned*/
compareSensor(sensorIndex){
  if(sensorValue[sensorIndex] >= lightThreshold[sensorIndex]){
    if(checkoutTime[sensorIndex] == 0);{
      assignTimer(sensorIndex);
    }
    else{
      checkTimer(sensorIndex);
    }
  }
}

buzzer(sensorIndex){
  
}


void loop() {
  for(sensorIndex = 0; sensorIndex < sensorQTY; sensorIndex++){
    readSensors(sensorIndex);
    compareSensors(sensorIndex);
    
    if (overdue[sensorIndex] = true){
      buzzer(sensorIndex);
      }
    }

}
