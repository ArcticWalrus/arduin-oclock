// Setting pins for shift register
int latch = 9;  //74HC595  pin 9 STCP
int clock = 10; //74HC595  pin 10 SHCP
int data = 8;   //74HC595  pin 8 DS

// Setting pins for controlling which digit is displayed
int minuteUnitPin = 7;
int minuteDecPin = 6;
int hourUnitPin = 5;
int hourDecPin = 4;

// Setting interrupt pins, these are the only two usable for interrupts
int minuteUptick = 2;
int hourUptick = 3;

// Setting initial values for time
volatile unsigned char minuteUnit = 0;
unsigned char minuteDec = 0;
volatile unsigned char hourUnit = 0;
unsigned char hourDec = 0;

// Time variables to avoid using delay
unsigned long current_time;
unsigned long last_time;
//unsigned int msPerMin = 1000 * 60;
unsigned int msPerMin = 250;

// Array that contains the display hex for 0 through f
unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

// Required functions
void displayTimeUnit(int outputPin, unsigned char value){
  // Need this ordering with delay in the middle to prevent bleeding between numbers
  (outputPin == hourUnitPin) ? displayNumberWithPeriod(value) : displayNumber(value);
  digitalWrite(outputPin, LOW);
  delay(1);
  digitalWrite(outputPin, HIGH);
}

void displayNumber(unsigned char num){
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
}

void displayNumberWithPeriod(unsigned char num){
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,(1 << 7) | table[num]);
  digitalWrite(latch,HIGH);
}

void updateTime(){
  current_time = millis();
  if (current_time - last_time >= msPerMin){
    minuteUnit++;
    last_time = current_time;
  }
  // Carry overs
  if (minuteUnit >= 10){
    minuteUnit = 0;
    minuteDec++;
    if (minuteDec >= 6){
      minuteDec = 0;
      hourUnit++;
      if (hourUnit >= 10){
        hourUnit = 0;
        hourDec++;
      }
    }
  }
  // Check for reset on hours due to interrupt
  if (hourUnit >= 10){
    hourUnit = 0;
    hourDec++;
  }
  // Check for reset at 24h
  if (hourUnit >= 4 && hourDec >= 2){
    minuteUnit = 0; 
    minuteDec = 0;
    hourUnit = 0;
    hourDec = 0;
  }
}

void turnOnBoardLEDISR(){
  digitalWrite(LED_BUILTIN, HIGH);
}

void increaseHourISR(){
  hourUnit++;
}

void increaseMinutesISR(){
  minuteUnit++;
}

void setup(){
  // Pins for shift register
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Pins for digit control on display
  pinMode(minuteUnitPin, OUTPUT);
  pinMode(minuteDecPin, OUTPUT);
  pinMode(hourUnitPin, OUTPUT);
  pinMode(hourDecPin, OUTPUT);

  // Attaching interrupts
  attachInterrupt(digitalPinToInterrupt(hourUptick), increaseHourISR, RISING);

  // Starting timer
  current_time = millis();
  last_time = current_time;
}

void loop(){
  updateTime();
  displayTimeUnit(minuteUnitPin, minuteUnit);
  displayTimeUnit(minuteDecPin, minuteDec);
  displayTimeUnit(hourUnitPin, hourUnit);
  displayTimeUnit(hourDecPin, hourDec);
}
