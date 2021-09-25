void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
  doEncoder 02 Interrupt on A & B pin
  doEncoder 4x              .  [-] [+]  +2 [+]  .   -2 [-] [-]  -2  .  [+]  +2 [+] [-]  .
  doEncoder 2x              .   .  [+]  .   .   .   .  [-] [-]  .   .   .   .  [+]  .   .
  doEncoder 1x              .   .  [+]  .   .   .   .   .  [-]  .   .   .   .   .   .   .
      A old state       A*  0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1
      B old state       B*  0   0   0   0   1   1   1   1   0   0   0   0   1   1   1   1
      A state           A   0   0   1   1   0   0   1   1   0   0   1   1   0   0   1   1
      B state           B   0   1   0   1   0   1   0   1   0   1   0   1   0   1   0   1
  Encoder State table       .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
  char EncoderState[16] = { 0, -1,  1,  0,  1,  0,  0, -1, -1,  0,  0,  1,  0,  1, -1,  0}; //2i4x
  char EncoderState[16] = { 0,  0,  1,  0,  0,  0,  0, -1, -1,  0,  0,  0,  0,  1,  0,  0}; //2i2x
  char EncoderState[16] = { 0,  0,  1,  0,  0,  0,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0}; //2i1x
  ________________________________________________________________________________________________
  doEncoder use only one Interrupt, on A or B pin
  doEncoder 2x              .   .   .  [-]  .   .  [+]  .   .  [+]  .   .  [-]  .   .   .
  doEncoder 1x              .   .   .  [-]  .   .  [+]  .   .   .   .   .   .   .   .   .
      A old state       A*  0   0   0   0   0   0   0   0   1   1   1   1   1   1   1   1
      B old state       B*  0   0   0   0   1   1   1   1   0   0   0   0   1   1   1   1
      A state           A   0   0   1   1   0   0   1   1   0   0   1   1   0   0   1   1
      B state           B   0   1   0   1   0   1   0   1   0   1   0   1   0   1   0   1
  Encoder State table       .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
  char EncoderState[16] = { 0,  0,  0, -1,  0,  0,  1,  0,  0,  1,  0,  0, -1,  0,  0,  0}; //1i2x
  char EncoderState[16] = { 0,  0,  0, -1,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0}; //1i1x

  http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
*/

char EncoderState[16] = { 0,  0,  1,  0,  0,  0,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0}; //2i1x
char EncoderVal       = 0;
long EncoderCount;   //encoder count is stored here

void setupEncoder()
{
  // Arduino with Atmega328 mCU
  // pin 2 -> PORTD 2 -> INT0
  // pin 3 -> PORTD 3 -> INT1
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE); //RISING FALLING CHANGE
  attachInterrupt(1, doEncoder, CHANGE); //RISING FALLING CHANGE
}
void doEncoder()
{
  /*
    //Read pin time for one 3.6uS
    char A = bitRead(PIND, 2);
    char B = bitRead(PIND, 3);
    //Read encoder state A* B* A  B
    EncoderVal = ((A << 1) | B | (EncoderVal << 2)) & 0x0f;
  */
  
  //Read port time for one 3.3uS
  //Read encoder state A* B* A  B
  EncoderVal = (((PIND >> 2) & B00000011) | (EncoderVal << 2)) & B00001111;
  EncoderCount += EncoderState[EncoderVal];
}
