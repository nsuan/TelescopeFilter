// Include the AccelStepper Library
#include <AccelStepper.h>

// Define step constant
#define MotorInterfaceType 4

#define FILTER_ON 510
#define FILTER_OFF 0
 
// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);
bool enable = false;
void filterOn();
void filterOff();
void filterPos();
void filterPosPer();
bool initFilter();
bool sendStatus = false;
int cnt = 1; 
void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(500);
  myStepper.setSpeed(990);
  //TODO: Get position here first, then run moveTo  
  //myStepper.moveTo(2038);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  enable = initFilter();
  while (Serial.available() != 0) {
     delay(1);
  }
  Serial.print(">");
  Serial.flush();
}


void loop() {
  if(enable) {
    int incomingByte = 0; // for incoming serial data
    char cmd; 
      // read the incoming byte:
      incomingByte = Serial.read();
      cmd = incomingByte;
      // say what you got:
      if(incomingByte != -1 && incomingByte != 13 && incomingByte != 10) {
         Serial.println(cmd);
        switch (cmd) {
            case 'C': 
               //Filter on (cover)
               filterOn();
               break;
            case 'S':
               //Get Pos 
               filterPos();
             break;
             case 'U':
               //Filter on (uncover)
               filterOff();
             break;
        }
        Serial.print(">");
        Serial.flush();
      }
  }
  if(sendStatus) {
    filterPosPer();
    sendStatus = false;
    //Serial.println(cnt);

  }
}

ISR(TCB2_INT_vect){
  TCB2.INTFLAGS = TCB_CAPT_bm;
 if  (cnt % 2000 == 0)
    sendStatus = true;
    cnt++;
}

bool initFilter(){
TCB2.CCMP = 0x1F3F; // 1ms with TCB_CLKSEL_CLKDIV2_gc 
TCB2.CTRLB = 0; // interrupt mode
TCB2.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm;
//TCB2.CTRLA = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm;  // use TCA 250kHz clock
TCB2.INTCTRL =  TCB_CAPT_bm; 

  //Only init filter if the sensor does not detect positions
  if (0) {
      for(int i = 0; i < 2; i++) {
    
        myStepper.moveTo(500);
        while (myStepper.currentPosition() != 300) // Full speed up to 300
          myStepper.run();
      myStepper.runToNewPosition(0); // Cause an overshoot then back to 0
      while (myStepper.currentPosition() != 0) // Full speed up to 300
          myStepper.run();
    }
    // Put on filter
    myStepper.moveTo(FILTER_ON);
    while (myStepper.currentPosition() != FILTER_ON) // Put on filter
          myStepper.run();
  }
  return true;
}
void filterPosPer() {
  filterPos();
  Serial.println(" >");
}
void filterPos() {
  Serial.print("P: ");
  switch (myStepper.currentPosition()) {
     case FILTER_ON: 
      Serial.print("F");
      break;
     case FILTER_OFF: 
      Serial.print("N");
      break;
     default: 
      Serial.print("U");  
      break;
  }
  Serial.print(" ");
  Serial.print(myStepper.currentPosition());
}
void filterOn() {
    myStepper.moveTo(FILTER_ON);
    while (myStepper.currentPosition() != FILTER_ON) // Full speed up to 300
        myStepper.run();
   if (true) //check filter position
      Serial.println("F");
}

void filterOff() {
    myStepper.moveTo(FILTER_OFF);
    while (myStepper.currentPosition() != FILTER_OFF) // Full speed up to 300
        myStepper.run();
      Serial.println("N");
}
