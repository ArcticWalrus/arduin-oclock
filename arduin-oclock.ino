// Defines for alarm
#define ALARM_ON 1
#define ALARM_OFF 0

// Setting pins for shift register
int latch = 9;
int clock = 10;
int data = 8;

// Setting pins for controlling which digit is displayed
int minuteUnitPin = 7;
int minuteDecPin = 6;
int hourUnitPin = 5;
int hourDecPin = 4;

// Setting output pin for passive buzzer
int buzzer = 0;

// Setting interrupt pins, these are the only two usable for interrupts on the uno
int minuteUptick = 2;
int hourUptick = 3;

// Setting initial values for time
volatile unsigned char minuteUnit = 0;
unsigned char minuteDec = 0;
volatile unsigned char hourUnit = 7;
unsigned char hourDec = 0;

// Setting time for alarm
unsigned char alarmMinuteUnit = 0;
unsigned char alarmMinuteDec = 0;
unsigned char alarmHourunit = 8;
unsigned char alarmHourDec = 0;

// Variable that keeps the state of the alarm
unsigned char alarmStatus = 0;

// Time variables to avoid using delay
unsigned long current_time;
unsigned long last_time;
//unsigned int msPerMin = 1000 * 60;
unsigned int msPerMin = 50;

// Array that contains the display hex for 0 through f
unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

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
  // Counting minutes
  current_time = millis();
  if (current_time - last_time >= msPerMin){
    minuteUnit++;
    last_time = current_time;
  }
  // Carry overs
  if (minuteUnit >= 10){
    minuteUnit = 0;
    minuteDec++;
  }
  // Check for reset on decimal position on minutes
  if (minuteDec >= 6){
    minuteDec = 0;
    hourUnit++;
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

void checkAlarm(){
  if (minuteUnit == alarmMinuteUnit && minuteDec == alarmMinuteDec && hourUnit == alarmHourunit && hourDec == alarmHourDec){
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    alarmStatus = 1;
  }
  // The hourUnit == 9 will be replace with the motion sensor from the ultrasonic
  if (alarmStatus == ALARM_ON && hourUnit >= 9){
    alarmStatus = 0;
    digitalWrite(buzzer, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// For testing that buttons are set up correctly.
void turnOnBoardLEDISR(){
  digitalWrite(LED_BUILTIN, HIGH);
}

void increaseHourISR(){
  hourUnit++;
}

void increaseMinuteISR(){
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

  // Setting pin for buzzer
  pinMode(buzzer, OUTPUT);

  // Attaching interrupts
  attachInterrupt(digitalPinToInterrupt(hourUptick), increaseHourISR, RISING);
  attachInterrupt(digitalPinToInterrupt(minuteUptick), increaseMinuteISR, RISING);

  // Starting timer
  current_time = millis();
  last_time = current_time;
}

void loop(){
  updateTime();
  checkAlarm();
  displayTimeUnit(minuteUnitPin, minuteUnit);
  displayTimeUnit(minuteDecPin, minuteDec);
  displayTimeUnit(hourUnitPin, hourUnit);
  displayTimeUnit(hourDecPin, hourDec);
}
