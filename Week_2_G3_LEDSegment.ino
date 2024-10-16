#define BUTTON_PIN 10
int segmentA = 2;
int segmentB = 3;
int segmentC = 4;
int segmentD = 5;
int segmentE = 6;
int segmentF = 7;
int segmentG = 8;

int i = 0;
int j = 0;

int Arduino_Pins[7] = {segmentA, segmentB, segmentC, segmentD, segmentE, segmentF, segmentG};
int Segment_Display[10][7] = {{0, 0, 0, 0, 0, 0, 1}, //display number 0
                              {1, 0, 0, 1, 1, 1, 1}, //display number 1
                              {0, 0, 1, 0, 0, 1, 0}, //display number 2
                              {0, 0, 0, 0, 1, 1, 0}, //display number 3
                              {1, 0, 0, 1, 1, 0, 0}, //display number 4
                              {0, 1, 0, 0, 1, 0, 0}, //display number 5
                              {0, 1, 0, 0, 0, 0, 0}, //display number 6
                              {0, 0, 0, 1, 1, 1, 1}, //display number 7
                              {0, 0, 0, 0, 0, 0, 0}, //display number 8
                              {0, 0, 0, 0, 1, 0, 0}, //display number 9
};

void setup() {
  // put your setup code here, to run once:
  pinMode(segmentA, OUTPUT);
  pinMode(segmentB, OUTPUT);
  pinMode(segmentC, OUTPUT);
  pinMode(segmentD, OUTPUT);
  pinMode(segmentE, OUTPUT);
  pinMode(segmentF, OUTPUT);
  pinMode(segmentG, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
   

  //to display number according to segment pin
  for (j = 0; j<7; j++)
  {
    digitalWrite(Arduino_Pins[j], Segment_Display[i][j]);
  }


  //if button is pushed, number increase
  if(digitalRead(BUTTON_PIN)==0)
  {
    while(digitalRead(BUTTON_PIN)==0)
    {
      
    }
    i++; //increase counter with each push button
  }
  

  //start count back to 0
  if(i == 10)
  {
    i = 0;
  }
  delay(100);
}
