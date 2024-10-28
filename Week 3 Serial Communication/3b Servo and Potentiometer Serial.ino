#include <Servo.h>

Servo myservo;  
int potPin = A0; 
int pos = 0;
char incomingByte;

void setup() {
  myservo.attach(5);  
  Serial.begin(9600);
}

void loop() {
  
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 'q') {
      while (true); 
    }
  }

  int potValue = analogRead(potPin);
  pos = map(potValue, 0, 1023, 0, 180);
  myservo.write(pos); 
  Serial.println(pos); 

  delay(100); 
}
