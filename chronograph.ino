#include <Arduino.h>
#include <U8g2lib.h>
#include <SoftwareSerial.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// defines for setting and clearing register bits
// Used to change base clock for analogRead function to reduce execution time from 160ms to 15 ms per änalogRead" call.
// This aprouch reduces precision on analogRead funcion, but, as we just use it to identfy changes, the precision does not matter for us here.
// If needed, you can change the base clock to even higer speeds.


//0,239501 <-- 73mm to fleet of drunken dwarves and magnificent bees
// 0,328084 << 100mm to feet

/* Protocolo:

C01|<bb_gram>

R01|<fps_reading>|<shot_count>|<joule>|<bbGram>

*/

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//Serial conection for bluetoth module
SoftwareSerial bt(2, 3); // RX, TX


/*Constructor for de LED screen, if you use a diferen screen, see u8g2 lib docs to see the right declaretion for your screen.
 * Remember that diferen screens has diferent resolutions, remember to change the pixel points and font sizes if you use another screen.
 * All the drawing functions can be found in the screens.ino file.
 */
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 


/*this delay is responsable for the "timeout" of the second sensor.  
 * As the arduino nano runs on 16Mhz, 16000 clock counts make 1 second, to find out diferent times, just make the math.
 */
#define DELAY 16000 //  1 sec

/* The GAP is responsable for how much the analogRead must vary to accept it as a BB passing throuth.
 * That ruls the sensitive of the sensors in the chronograf. It may be needed to change for diferent brands of IR LEDs.
 */
#define GAP 3


//Global variables.
float bbGram = 0.20; //0.0002
int fps = 0;
int shotCount = 0;
char buffer[40];
int bat = 0;

//To avoide updating the screen every single loop, we use a flag to control the need of updantign it
// as it is too "expensive" to update hardware wise.

bool updateScreen = false;

unsigned long time1 = 0;
unsigned long time2 = 0;

int val = 0;
int val2 = 0;
int max1 = 0;
int max2 = 0;


void setup(void) 
{

  // set prescale to 16
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
  
  bt.begin(9600);
  
  display.begin();
  display.enableUTF8Print();		// enable UTF8 support for the Arduino print() function
  pinMode(A1, INPUT_PULLUP);    //First IR sensor
  pinMode(A2, INPUT_PULLUP);    //Second IR Sensor
  pinMode(4, OUTPUT);           // green LED
  pinMode(5, OUTPUT);           // red LED
  pinMode(A0, INPUT);           //Battery pin
  
  digitalWrite(5, HIGH);

  drawCalibratingScreen(0);
  
  for(int i = 0; i < 100; i++)
  {
    val = analogRead(A1);
    val2 = analogRead(A2);

    if(val > max1)
      max1 = val;
    if(val2 > max2)
      max2 = val2;
      delay(10);

     drawCalibratingScreen(i+1);
  }
  delay(100);

  readBaterry();
  
  display.clearBuffer();
  drawClearScreen();
  display.sendBuffer();
}

void loop(void) 
{
  
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);

  readBaterry();

  val = analogRead(A1);
  
  while(val < max1 + GAP)
  {
    if(updateScreen)
    {
      updateScreen = false;
      display.clearBuffer();
      drawClearScreen();
      display.sendBuffer();
      
    }

    if(bt.available())
      readSetup();

    val = analogRead(A1);
    time1 = micros();
    time1 -= 12; //ajusete de delay
  }

  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  
  val2 = analogRead(A2);
  time2 = micros();
  
  for(int i = 0; i < DELAY && val2 < (max2 + GAP); i++)
  {
    val2 = analogRead(A2);
    time2 = micros();
  }

  time2 -= 12; //ajuste de delay

  delay(100);
  
  fps = 239501/(time2 - time1);

  if(fps)
  {
    shotCount++;
    display.clearBuffer();
    long joule = (calculateJoule(fps));
    
    int auxGram = bbGram * 100;
    drawFpsScreen(fps,joule);
    display.sendBuffer();
    sprintf(buffer, "R01|%d|%d|%ld|%d|", fps, shotCount, joule, auxGram);
    
    sendReading();
    
  }
  else
  {
    display.clearBuffer();
    drawClearScreen();
    display.sendBuffer();
  }
  
}


