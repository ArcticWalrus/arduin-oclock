// Setting pins for shift register
int latch = 9;  //74HC595  pin 9 STCP
int clock = 10; //74HC595  pin 10 SHCP
int data = 8;   //74HC595  pin 8 DS

// Setting pins for controlling which digit is displayed
int minuteUnitPin = 7;
int minuteDecPin = 6;
int hourUnitPin = 5;
int hourDecPin = 4;

// Array that contains the display hex for 0 through f
unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

void setup(){
  // Pins for shift register
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);

  // Pins for digit control on display
  pinMode(minuteUnitPin, OUTPUT);
  pinMode(minuteDecPin, OUTPUT);
  pinMode(hourUnitPin, OUTPUT);
  pinMode(hourDecPin, OUTPUT);

  digitalWrite(minuteUnitPin, HIGH);
  digitalWrite(minuteDecPin, LOW);
  digitalWrite(hourUnitPin, HIGH);
  digitalWrite(hourDecPin, HIGH);
}

/*   The most common method of using 74CH595 shift register
 *   latch->LOW : Begin transmitting signals.
 *   shiftOut(dataPin, clockPin, bitOrder, value)
 *   dataPin: the pin on which to output each bit. Allowed data types: int.
 *   clockPin: the pin to toggle once the dataPin has been set to the correct value. Allowed data types: int.
 *   bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit First, or, Least Significant Bit First).
 *   value: the data to shift out. Allowed data types: byte. 
 *   latch->HIch : The end of the transmission signal.
*/
void displayTime(unsigned char num){
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
}

void loop(){
  for(int i = 0; i < 17; i++){
    displayTime(i);
    delay(500);
  }
}
