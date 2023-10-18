#include <math.h>

int red = 4;
int yellow = 5;
int green = 6;
int buzzer = 10;
int echo = 2;
int trig = 3;
double duration = 0;
double distance = 0;
int buzzer_delay = 500;
int lim1 = 60;
int lim2 = 40;
int lim3 = 20;
double previousTime = 0;
double currentTime = 0;

/*
  The variable isDebug does so the program does not handle the piezo buzzer when debugging when isDebug is set to true. When using the product
  the isDebug is set to false so the program can handle the piezo buzzer and the light emmitting diodes.
*/
bool isDebug = false;

void setup() {
  /*this does so that everything written out can be seen in the serial monitor*/
  Serial.begin(9600);
  
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  currentTime = millis();

  /*a pulse is sent to the ultrasonic sensor*/
  digitalWrite(trig, HIGH);  
  
  /*the pulse is on for 10 microseconds to trigger the ultrasonic sensor*/
  delayMicroseconds(10);
  
  /*the pulse is turned off so the ultrasonic sensor emmits ultrasound*/
  digitalWrite(trig, LOW);

  /*the time the sound takes from travelling to the target and back is stored*/
  duration = pulseIn(echo, HIGH);

  /*
    The distance is calculated using d=vt. The time is converted to seconds from milli seconds
    so the distance becomes in (m). The distance is divided by 2 to get the distance since the stored distance is 
    the distance stored is for the sound to go from the ultrasonic sensor to the target and back.
    The distance is multiplied by 100 to get it in (cm) instead of (m).
  */
  
  distance = (340*(duration*0.000001)/2)*100;
  distance = round(distance);
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (!isDebug) {
    if (distance >= lim1){
      /*green: off | yellow: off | red: off*/   
      handleLED(LOW, LOW, LOW);
      digitalWrite(buzzer, LOW);
    } else if (distance < lim1 && distance >= lim2){
      buzzer_delay = 500;
      /*green: on | yellow: off | red: off*/   
      handleLED(HIGH, LOW, LOW);
      buzzerSound(buzzer_delay);
    } else if (distance < lim2 && distance >= lim3){
      buzzer_delay = 300;
      /*green: on | yellow: on | red: off*/   
      handleLED(HIGH, HIGH, LOW);
      buzzerSound(buzzer_delay);
    } else if (distance < lim3){
      buzzer_delay = 100;
      /*green: on | yellow: on | red: on*/
      handleLED(HIGH, HIGH, HIGH);
      buzzerSound(buzzer_delay);
    } 
  }
  /*this does so the ultrasonic sensor does not measure the distance 0 every other time, since the sensor needs a delay*/
  delay(8);
}

/*this function is used to make the piezo buzer emmit sound where the time for the pulse is given*/
void buzzerSound(int buzzer_delay){
  if (currentTime - previousTime >= buzzer_delay){
    previousTime = currentTime;
    /*the piezo buzzer is either turned on or off*/
    if (digitalRead(buzzer) == LOW){
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    }
  }
}

/*this function turns either on or off the LED depending on what mode for the LED is given is*/
void handleLED(int greenStatus, int yellowStatus, int redStatus){
  digitalWrite(green, greenStatus);
  digitalWrite(yellow, yellowStatus);
  digitalWrite(red, redStatus);
}
