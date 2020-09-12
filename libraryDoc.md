# Use of shiftOut() function

The most common method of using 74CH595 shift register
latch->LOW : Begin transmitting signals.
shiftOut(dataPin, clockPin, bitOrder, value)
dataPin: the pin on which to output each bit. Allowed data types: int.
clockPin: the pin to toggle once the dataPin has been set to the correct value. Allowed data types: int.
bitOrder: which order to shift out the bits; either MSBFIRST or LSBFIRST. (Most Significant Bit First, or, Least Significant Bit First).
value: the data to shift out. Allowed data types: byte. 
latch->HIch : The end of the transmission signal.
