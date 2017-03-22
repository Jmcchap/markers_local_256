//#include <QTRSensors.h>

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Time.h>
#include <TimeLib.h>

/* PINS */
int pins[] = {D5, D6, D7, D8};              //Digital pins

/*MQTT STUFF*/
const char* mqtt_host = "iot.256.makerslocal.org";
const char* outTopic = "ml256/info/markerslocal";

/*CONSTANTS*/
int allotedTime = 5000;                        //How much time the user is allowed to check out the pen (5 seconds)
int lightThreshold[] = {511, 511, 511, 511};   //The value in which the pen is considered removed. NEEDS TRIAL


/*OTHER VARIABLES*/
int checkoutTime[] = {0, 0, 0, 0};              //The time in which each pen was removed
int sensorValue[] = {0, 0, 0, 0};               //The analog values returned by sensor
time_t currentTime = 0;                         //The time retrieved from now()
int sensorQTY = 4;                              //The number of sensors - 1
int sensorIndex = 0;                            //what sensor is currently being worked on
bool overdue[] = {false, false, false, false};  //Pretty much determines if the conditions are right for the buzzer to buzz

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

void setup() {
  Serial.begin(9600);
  
  
  setup_wifi();
  Serial.println("Markers Local Begin!");
  client.setServer(mqtt_host, 1883);

  pinMode(A0, INPUT);
  
  for (int i = 0; i < sensorQTY; i++) {
    pinMode(i, OUTPUT);
   digitalWrite(i, LOW);
 }
}

/*read the sensor that is highlighted (sensorIndex) by setting the Digital output to high for the sensor
  and reading it from the Analog sensor A0.*/
void readSensors(int sensorIndex){
  digitalWrite(sensorIndex, HIGH);
  delay(250);
  sensorValue[sensorIndex] = analogRead(A0);
  
  Serial.print( "SensorValue = [");
  Serial.print(sensorIndex);
  Serial.print( "] = " );
  Serial.println(sensorValue[sensorIndex]);
}

/*See if it is time for the marker to be returned*/
void checkTimer(int sensorIndex){
currentTime = now();
  if (checkoutTime[sensorIndex] > currentTime) {
    overdue[sensorIndex] = true;
  }
}


/*Record the time that the marker should be returned*/
void assignTimer(int sensorIndex) {
  checkoutTime[sensorIndex] = now() + allotedTime;
}


/*Compare the values of the sensor that is read against the light threashold that is set. If it is determined
  that the marker is removed, then a timer would be assigned*/
void compareSensor(int sensorIndex) {
  if (sensorValue[sensorIndex] <= lightThreshold[sensorIndex]) {
    if (checkoutTime[sensorIndex] == 0) {
      assignTimer(sensorIndex);
    }
    else {
      checkTimer(sensorIndex);
    }
  }
}

void buzzer(int sensorIndex) {
  switch(sensorIndex){
    case 0: 
    case 1: Serial.println("Please returnt he marker to the left side of Table 1");
            client.publish(outTopic, "Please return the marker to the left side of Table 1");
            break;
    case 2: 
    case 3: Serial.println("Please returnt he marker to the right side of Table 1");
            client.publish(outTopic, "Please return the marker to the right side of Table 1");
            break;
  }
}


void loop() {

if(!client.connected()){
  reconnect();
 }

client.loop();
Serial.println("made it past client.loop()!");
  
for (sensorIndex = 0; sensorIndex < sensorQTY; sensorIndex++) {
    readSensors(sensorIndex);
    Serial.println("readSensors");
    compareSensor(sensorIndex);
    Serial.println("compareSensors");

    if (overdue[sensorIndex] = true) {
     buzzer(sensorIndex);
    }
  }

}
