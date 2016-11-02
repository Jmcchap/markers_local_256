
#include <Time.h>
#include <TimeLib.h>

    // Inputs. Hall sensors that go to LOW when a marker is removed
  const int marker_A1 = 4;   //D1, Door-side Left-most marker slot
  const int marker_A2 = 5;   //D2, Door-side Right-most marker slot
  const int marker_B1 = 6;   //D3, Fab-side Left-most marker slot
  const int marker_B2 = 7;   //D4, Fab-side Right-most marker slot
  
  
    //Outputs. Buzzers that buzz when the markers have been 'checked out' too long. 
  const int buzzerA = 8;            //D5, Door-side buzzer
  const int buzzerB = 9;            //D6, Fab-side buzzer
  const int buzzerA_tone = 1000;    //Frequency buzzer A tones
  const int buzzerB_tone = 2000;    //Frequency buzzer B tones

   //Assorted variables used
   int checkout_time = 3600000;  //how long the marker can be removed before the buzzer goes off (1hr)
   int countdown_time[4] = {0, 0, 0, 0};       //how much time has elapsed since each marker was pulled
   time_t   

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
