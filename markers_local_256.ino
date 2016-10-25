
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
  const int checkout_time = 3600000;  //how long the marker can be removed before the buzzer goes off (1hr)
  const int countdown_time = 0;       //how much time has elapsed since the marker was pulled
  const bool markerValueA1;
  const bool markerValueA2;
  const bool markerValueB1;
  const bool maerkerValueB2;

void setup() {
  pinMode(marker_A1, INPUT);
  pinMode(marker_A2, INPUT);
  pinMode(marker_B1, INPUT); 
  pinMode(marker_B2, INPUT);
  pinMode(buzzerA, OUTPUT);
  pinMode(buzzerB, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:

 
  markerValueA1 = digitalRead(marker_A1);     //read the values of the markers 
  markerValueA2 = digitalRead(marker_A2);     //and store them for later use
  markerValueB1 = digitalRead(marker_B1);
  markerValueB2 = digitalRead(marker_B2);


  if ( markerValueA1 = LOW || markerValueA2 = LOW || markerValueB1 = LOW || markerValueB2  == LOW){      //IF a marker was removed
  
  while(countdown_time < checkout_time){                            //Timer countdown until buzzer
    countdown_time = countdown_time + 1000;
    
  }
  
}
}
