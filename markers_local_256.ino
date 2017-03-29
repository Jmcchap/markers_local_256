#include <Time.h>
#include <TimeLib.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*MQTT STUFF*/
const char* mqtt_host = "iot.256.makerslocal.org";
const char* outTopic = "ml256/info/markerslocal";

/*PINS*/
int pin[] ={D5, D6, D7, D8};                    //digital output pins

/*OTHER VARIABLES*/
int lightThreashold[] = {100, 100, 100, 100};   //determines if marker is 'in' or not
int sensorValue[] = {0, 0, 0, 0};               //actual analog value from sensors
int checkoutTime[] = {0, 0, 0, 0};              //how long each marker is 'allowed' to be removed
bool overdue[] = {0, 0, 0, 0};                   //the flag for if the checkoutTime has surpassed how long it can be borrowed
bool markerDocked[] = {1, 1, 1, 1};              //if each marker is in its home, then true
time_t currentTime = 0;                         //the time retrieved from the now() function
int allotedTime = ;                           //how long a marker can be withdrawn for
int sensorIndex = 0;                            //which  sensor in the array

/*WIFI CONNECTION STUFF*/
//That block I don't fully understand again
WiFiClient espClient;
PubSubClient client(espClient);

//connect to WiFi network
void setup_wifi() {
   Serial.println("Starting wireless.");
  WiFiManager wifiManager; //Load the Wi-Fi Manager library.
  wifiManager.setTimeout(300); //Give up with the AP if no users gives us configuration in this many secs.
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }

  Serial.println("");
  Serial.print("Successfully connected to ");
  Serial.println(WiFi.localIP());
}

//reconnect if wifi is lost
void reconnect(){
 Serial.println("Oh no! Lost connection!"); 
 
 while(!client.connected()) {
  Serial.print("One moment, please...");
  
  if(client.connect("q2w3e4r5")){
    Serial.println("There we go!");              //If there is a successful reconnection
    client.publish(outTopic, "Here I am!");      //Publish an announcement
    
    
    
  }else {
    Serial.print("Well, drat. The issue seems to be: ");
    Serial.println(client.state());
    Serial.println("Trying again in a jiffy");
    //Wait a jiffy seconds and retry connecting
    delay(3340);
  }
 }
}






/*SENSOR DATA STUFF*/

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
    overdue[sensorIndex] = 1;    
  }
}


//Determine if the marker is in the holder or not
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
  
  if(sensorValue[sensorIndex] < 200){
    
      markerDocked[sensorIndex] = 0;
  }else{
      markerDocked[sensorIndex] = 1;
  }
}

void buzzer() {
  switch(sensorIndex){
    case 0: 
    case 1: Serial.println("Please returnt he marker to the left side of Table 1");
            client.publish(outTopic, R"({"source":"Markers Local", "message":"Please return the marker to the left side of Table 1"})");
            break;
    case 2: 
    case 3: Serial.println("Please returnt he marker to the right side of Table 1");
            client.publish(outTopic, R"({"source":"Markers Local", "message":"Please return the marker to the right side of Table 1"})");
            break;
  }
}


void setup() {  
  Serial.begin(9600);

  setup_wifi();
  Serial.println("Markers Local Begin!");
  client.setServer(mqtt_host, 1883);
  
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(A0, INPUT);
  
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
}

void loop() {
  
  if(!client.connected()){
    reconnect();
   }
   client.loop();
      Serial.println("made it past client.loop()!");
 
  for( sensorIndex=0; sensorIndex<4; sensorIndex++){
    
    readSensor();
    compareSensor();    
      if(markerDocked[sensorIndex] == 0){      //if the marker is removed
          if (checkoutTime[sensorIndex] == 0) {   //and no checkout time is assigned
            assignTimer();                        //"checkout" the marker
          }                                       //BUT
          else {                                  //if the marker is already checked out
            checkTimer();                         //check the timer to make sure it's not overdue
          }
      }else{                                      //if the marker is in its home
           checkoutTime[sensorIndex] = 0;         //if so, reset the timer. It is returned
           overdue[sensorIndex] = 0;              //also, since it's returned it isn't overdue anymore
      }

      if (overdue[sensorIndex] = true) {
     buzzer();
    }

     
  Serial.print( "SensorValue[0] = ");
  Serial.println(sensorValue[0]);
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

  delay(10000);
}
