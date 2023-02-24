int Count = 0;
int Count1 = 0;
int pin = 2; //define interrupt pin to 2
int x = 0;
int flag = 0;
int flag2 = 0;
int flag3 =0; 
unsigned long StopdialMillis = 0;

int endCount =0;

void setup() {
Serial.begin(9600);
Serial.println (" HI");
pinMode(pin, INPUT_PULLUP);
for(int i=3; i < 14; i++){
 pinMode(i, OUTPUT); 
}
attachInterrupt(digitalPinToInterrupt(pin), Flag, RISING);
}

void Flag(){ //ISR function
  flag=1;
}
void loop() {
   for(int i=3; i < Count+2; i++){ // turn off lower leds
      digitalWrite (i, LOW); // (i, LOW);
  }
  if( Count > Count1 ){ 
  Serial.println (Count);
  //digitalWrite (Count+2, HIGH); // highest led lit ATTN: don't execute here because circuit opens and no higher count will be registered than the calling line number
  }
  Count1 = Count;
    if (flag == 1 ){
      delay(20); // for debounce
    count();
    flag=0;
  }
  
  if (millis() - StopdialMillis > 2000 && Count > 0 && Count <7){
    endCount = Count;
   Serial.println (endCount);
   detachInterrupt(digitalPinToInterrupt(pin)); // avoid interrupts here
    Ring(); // at this point, Count +2 is highest pin and is low, line RING position
   attachInterrupt(digitalPinToInterrupt(pin), Flag, RISING);
    }
  }

void count(){ //count function
      
        x = digitalRead(pin); //first thing phone is off hook
        if (x == 1){
        Count++; //count after every while
        StopdialMillis= millis();        
        Serial.println(Count);
}
}

void Ring(){
  
  while(endCount != 0){
    digitalWrite( Count+2, HIGH); // switch line from DC to AC ring.
    delay(200);
    digitalWrite ( 13, HIGH); // Ring power on (DC off)
    delay(1000); // Ringing 2 seconds
    digitalWrite( 13, LOW); // Ring power off (DC on)
    delay(100);
    digitalWrite( Count+2, LOW); // switch line from AC to DC
    unsigned long stMillis = millis();
    while(millis() -stMillis < 2000 ){
      int z = analogRead( Count + 13); // Pin 3 is associated with A0 (pin 14)
      if (z < 700){ //off hook (answer) is 614 newer phone, and 483 candlestick phone [3.00v vs. 2.36v] This using INA210, not 6N317
        Count = 0;
        endCount = 0;
        Serial.println(z);
      }
    }
  }
 }


