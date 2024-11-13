float tempcelc;
int ldr_value;
int ldr_percent;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println("CLEARDATA");
Serial.println("LABEL,CLOCK,TEMPERATURE,test,LIGHT");

}

void loop() {
  // put your main code here, to run repeatedly:
float lm_value = analogRead(A0);
tempcelc = (lm_value*5/1023)*100;
// tempcelc = tempcelc/10;

ldr_value = analogRead(A1);
ldr_percent = map(ldr_value,0,1023,0,100);


Serial.print("DATA,TIME,");
Serial.print(tempcelc);
Serial.print(",");
Serial.print(",");

Serial.println(ldr_percent);
delay(1500);
}
