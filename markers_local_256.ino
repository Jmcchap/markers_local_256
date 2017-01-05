

//IR version of markers local

#include <QTRSensors.h>
#include <Time.h>
#include <TimeLib.h>

    // Inputs 
    
  QTRSensorsAnalog qtra((unsigned char[]) {5, 6, 7, 8},4);  //pin locations of the IR sensors
  
  unsigned int sensor_value[4] = {0, 0, 0, 0};
  

  
  
    //Outputs. Buzzers that buzz when the markers have been 'checked out' too long. 
  const int buzzerA = 8;            //D5, Door-side buzzer
  const int buzzerB = 9;            //D6, Fab-side buzzer
  const int buzzerA_tone = 1000;    //Frequency buzzer A tones
  const int buzzerB_tone = 2000;    //Frequency buzzer B tones
  unsigned long buzzerB_duration = 1000;  //Buzzer B length of one tone
  unsigned long buzzerA_duration = 1000;  //Buzzer A length of one tone

   //Assorted variables used
   int      alloted_time = 300000;                       //how long the marker can be removed before the buzzer goes off (5 minutes)
   int      checkout_time[4] = {0, 0, 0, 0};             //how much time the user is has been borrowing the marker
   int      light_threashold[4] = {511, 511, 511, 511};   //to fine tune later. The value the sensors return that mean a maker is removed. 
   time_t   current_time = 0; 
   int      i=0;

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
   current_time = now();
   qtra.read(sensor_value, QTR_EMITTERS_ON);

   for (i=0; i<4; i++){
    if(light_threashold[i] < sensor_value[i]){
      checkout_time[i] = current_time + alloted_time;
    }
    if(checkout_time[i] > current_time){
      if(i<2){
        noTone(buzzerB);
        tone(buzzerA, buzzerA_tone,buzzerA_duration);
        i = 0;
      }
      else{
        i=0;
      }
      
      if(i>=2){
        noTone(buzzerA);
        tone(buzzerB, buzzerB_tone, buzzerB_duration);
        i = 0;
      }
     }else{
      i = 0;
     }
   }
}
