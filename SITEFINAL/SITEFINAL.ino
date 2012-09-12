
/***********
 * Journey Through Light Site
 * Authors: Ace Levenberg(llevenbe@ucsc.edu) and Aidan Seine (aseine@ucsc.edu)
 * 5/12/2012
 * See Color library for more information
 * This sketch uses the VirtualWire library radio transmission written by:
 * Mike McCauley (mikem@open.com.au) 
 ***************/

#include <VirtualWire.h>
#include <color.h>
/**************
 * //dont forget to change the id!!!!!
 * ids used = 
 **********
 */
 typedef struct{
  boolean seen;
  long int timeSeen;
  char color;
} orb;
 
const char id = '4';
int numberOfOrbs = 0;
int siteRate = 0;
int colors[7];
orb orbs[26];
long int prevOrbTime[26];
char highColor = '0';
int colorCounter = 0;
int sendCounter = random(0, 400);
int transPin = 4;
long int prevSendTime = 0;

void setup(){
  Serial.begin(9600);
   for(int i = 0; i < 26; i++){
   // prevOrbTime[i] = 0;
    orbs[i].seen = false;
  }
  for(int i = 0; i < 7; i++){
    colors[i] = 0;
  }
  pinMode(transPin, OUTPUT);
  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  //wait until we recieve a message
  checkTimeOut();
  //if we did get a message
  if(vw_wait_rx_max(1000)){
  if (vw_get_message(buf, &buflen)){ // Non-blocking
    //debug print of the id of the orb recieved
    Serial.print("Number of Orbs if got message  ");
    Serial.println(numberOfOrbs);
    Serial.print("id of orb = ");
    Serial.println(char(buf[2]) );
    //if the id has not been seen beofre increment the number of orbs seen
    // if(orbId[buf[2] - 97] == 0) numberOfOrbs++;
    //finds the highest color of all of the orbs present
   // highColor = findMax(buf[1], highColor);
    //sets the timeout time
    // orbId[buf[2] - 97] = 30000;
    //sets the time we have seen the orb
    //prevOrbTime[buf[2] - 97] = millis();
    
    
    int Orb = buf[2] - 97;
    if(!orbs[Orb].seen){
          orbs[Orb].seen = true;
          orbs[Orb].timeSeen = millis();
          orbs[Orb].color = buf[1];
          colors[buf[1]-48]++;
          for(int i = 0; i < 10; i++){
            //blink(color(buf[1]), 6);
            pulse(color(buf[1]), 1, 15);
          }
     }
     else {
        orbs[Orb].timeSeen = millis();
          if(orbs[Orb].color != buf[1]){
            colors[orbs[Orb].color-48]--;
            
            orbs[Orb].color = buf[1];
            colors[buf[1]-48]++;
          }
     }
  }

  }
   if (numberOfOrbs > 0){
    blink(getColor(), numberOfOrbs);
  }
}

color getColor(){
  while(1){
    colorCounter = (colorCounter >= 6 ) ? 0 : colorCounter + 1;
    if(colors[colorCounter]) {
      Serial.print("color counter = ");
      Serial.println(colorCounter);
      return color(colorCounter + 48);
    }
  }
}
/*
turns the transmitter on
 puts together the message needed to be sent by the site
 message is of length 4.  
 turns the transmitter off
 */

void siteSend(char Color, int peeps){
  digitalWrite(transPin, HIGH);
  char msg[4];
  char peepul = peeps + 48;
  msg[0] = 's';
  msg[1] = Color;
  msg[2] = peepul;
  msg[3] = id;
  //for (int i = 0; i < 14; i++){
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
 // }
  digitalWrite(transPin, LOW);
}




//blink, maps the rate based on the number of orbs and then pulse the correct color and rate
void blink(color hue, int rate){
  //color hue = color(Color);
  pulse(hue, map(rate, 0, 6, 40, 10), map(rate,0,6,5,10));
}




/*
pulses the color given with the given speed and fade value
 */
void pulse(color hue, int rate, int fadeRate){
  if(checkCounter()) siteSend(highColor, numberOfOrbs);
  for (int fadeValue = 245; fadeValue >= 0 ; fadeValue -= fadeRate){
    //map the value from the current red value - fadeValue, from the highest red value for lowest red value to the highest
    //to 0 to the highest fadevalue so the colors pulse correctly
    analogWrite(REDPIN, map(hue.red-fadeValue, hue.red-255, hue.red, 0, hue.red));
    analogWrite(GREENPIN, map(hue.green-fadeValue, hue.green-255, hue.green, 0, hue.green));
    analogWrite(BLUEPIN, map(hue.blue-fadeValue, hue.blue-255, hue.blue, 0, hue.blue));
    delay(rate);
    if(checkCounter()) siteSend(highColor, numberOfOrbs);

  }
  if(checkCounter()) siteSend(highColor, numberOfOrbs);
  for (int fadeValue = 0; fadeValue <= 245; fadeValue += fadeRate){
    analogWrite(REDPIN, map(hue.red-fadeValue, hue.red-255, hue.red, 0, hue.red));
    analogWrite(GREENPIN, map(hue.green-fadeValue, hue.green-255, hue.green, 0, hue.green));
    analogWrite(BLUEPIN, map(hue.blue-fadeValue, hue.blue-255, hue.blue, 0, hue.blue));
    delay(rate);
    if(checkCounter()) siteSend(highColor, numberOfOrbs);
  }
  analogWrite(REDPIN, 0);
  analogWrite(GREENPIN, 0);
  analogWrite(BLUEPIN, 0);
  if(checkCounter()) siteSend(highColor, numberOfOrbs);

}
/* 
 finds the max color of the orbs currently around
 old procedure, new procedure to be updated
 */
char findMax(char cur, char maxColor){
  if (cur > maxColor){
    return cur;
  }
  return maxColor;
}
/*
checks to see if the timer for sending is up
 if it is return true, if it is ot return false
 */
boolean checkCounter(){
  long int currTime = millis();
  if(currTime - prevSendTime >= sendCounter){
    sendCounter = random(0, 400);
    prevSendTime = millis();
    return true;
  }
  return false;
}

/*
checks to see if the any of the Orbs this Orb has seen
 have timed out.  Modifies the number of orbs to the needed
 value
 */
void checkTimeOut(){
  numberOfOrbs = 0;
  long int currTime = millis();
  for(int i = 0; i < 26 ; i++){
    //if(prevOrbTime[i] == 0) continue;
    if(orbs[i].seen == true){
    //if the certain value is greater than zero
      if(currTime - orbs[i].timeSeen < 40000) numberOfOrbs = (numberOfOrbs >= 7) ? 7 : numberOfOrbs+1;
      else {
        colors[orbs[i].color - 48]--;
        orbs[i].seen = false;
      }
    }
 }

}


