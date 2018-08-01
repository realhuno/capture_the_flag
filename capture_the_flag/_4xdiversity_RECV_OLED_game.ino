/* 
 *  CHANGES: INT EXT ADC with NRF24l01
 *  TODO:    INT EXT ADC with two NRF24L01
 *  15.01.2018 HAINZ V3
 *  Added 3 pipes
 *  
 *  YourDuinoStarter Example: Simple nRF24L01 Receive
  - WHAT IT DOES: Receives simple fixed data with nRF24L01 radio
  - SEE the comments after "//" on each line below
   Start with radios about 4 feet apart.
  - SEE the comments after "//" on each line below
  - CONNECTIONS: nRF24L01 Modules See:
  http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
  Uses the RF24 Library by TMRH2o here:
  https://github.com/TMRh20/RF24
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED

   V1.02 02/06/2016
   Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
/*-----( Declare Constants and Pin Numbers )-----*/
//None yet
/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (7, 8); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
//ALT byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.
//const uint64_t pipes[3] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL };
const uint64_t pipes[6] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL , 0xF0F0F0F0E4LL , 0xF0F0F0F0E5LL, 0xF0F0F0F0E6LL};
//byte addresses[][6] = {"1Pipe","2Pipe","3Pipe","4Pipe","5Pipe","6Pipe"};

int dataReceived;  // Data that will be received from the transmitter

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x26, 20 , 4);

//OLED INIT
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define display_RESET 4
Adafruit_SSD1306 display(display_RESET);




// VideoSwitch Mapping
#define switchA_L1 3
#define switchA_L2 2
#define switchA_R1 5   //4  vorher
#define switchA_R2 4   //5  vorher

#define switchB_L1 9
#define switchB_L2 10
#define switchB_R1 A2
#define switchB_R2 A3




struct payload_t
{

  int id;
  int adc0;
  int adc1;
  int adc2;
  int adc3;
   



};

int rssiA_ext;
int rssiB_ext;
int rssiC_ext;
int rssiD_ext;

int rssiA_int;
int rssiB_int;
int rssiC_int;
int rssiD_int;

void setup()   /****** SETUP: RUNS ONCE ******/
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // Use the serial Monitor (Symbol on far right). Set speed to 115200 (Bottom Right)
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Simple Receive data Test"));
  Serial.println(F("Questions: terry@yourduino.com"));
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  myRadio.setPALevel(RF24_PA_MIN);
  //  myRadio.setPALevel(RF24_PA_MAX);  // Uncomment for more power

  //alt myRadio.openReadingPipe(1, addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  myRadio.openReadingPipe(0, pipes[0]);
  myRadio.openReadingPipe(1, pipes[1]);
  myRadio.openReadingPipe(2, pipes[2]);
  myRadio.openReadingPipe(3, pipes[3]);
  myRadio.openReadingPipe(4, pipes[4]);
  myRadio.openReadingPipe(5, pipes[5]);
  
  //myRadio.enableDynamicPayloads();
  myRadio.startListening();
  //pinMode(A0,INPUT);
  //pinMode(A1,INPUT);
  //pinMode(A6,INPUT);
  //pinMode(A7,INPUT);
  
  pinMode(switchA_L1,OUTPUT);
  pinMode(switchA_L2,OUTPUT);
  pinMode(switchA_R1,OUTPUT);
  pinMode(switchA_R2,OUTPUT);

  pinMode(switchB_L1,OUTPUT);
  pinMode(switchB_L2,OUTPUT);
  pinMode(switchB_R1,OUTPUT);
  pinMode(switchB_R2,OUTPUT);




  
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{


  if ( myRadio.available()) // Check for incoming data from transmitter
  {
    while (myRadio.available())  // While there is data ready
    {
       payload_t payload;
      myRadio.read( &payload, sizeof(payload) ); // Get the data payload (You must have defined that already!)
        Serial.print(payload.id);
        Serial.print("|");
        Serial.print(payload.adc0);
        Serial.print("|");
        Serial.println(payload.adc1);
         
         
         
  //rssiA_ext=payload.adc0;
  //rssiB_ext=payload.adc1*1.2;
  //rssiC_ext=payload.adc2;
  //rssiD_ext=payload.adc3*1.2;
 switch (payload.id) {
  case 0:
  rssiA_ext=payload.adc0;
  rssiB_ext=payload.adc1;
  rssiC_ext=payload.adc2;
  rssiD_ext=payload.adc3;
    break;
  case 1:
   
  rssiA_int=payload.adc0;
  rssiB_int=payload.adc1;
  rssiC_int=payload.adc2;
  rssiD_int=payload.adc3;

    break;
  default:
  break;
    // statements
}
   

    }
    // DO something with the data, like print it
    //Serial.print("Data received = ");
    
  } //END Radio available

  //int rssiA_int=analogRead(A0);
  //int rssiB_int=analogRead(A1);
  //int rssiC_int=analogRead(A6);
  //int rssiD_int=analogRead(A7);
   display.setTextSize(1);
  display.setTextColor(WHITE);

  
  //LCD STUFF
  lcd.clear();
  display.setCursor(0,0);
  lcd.print(rssiA_int);
  display.print(rssiA_int);
  if(rssiA_int<rssiA_ext){digitalWrite(switchA_L1,HIGH);digitalWrite(switchA_L2,LOW); lcd.print("<"); display.print("<");}else{ digitalWrite(switchA_L1,LOW);digitalWrite(switchA_L2,HIGH); lcd.print(">"); display.print(">");}
  lcd.print(rssiA_ext);
  display.print(rssiA_ext);

  display.setCursor(0,8);
  display.print(rssiB_int);
  lcd.setCursor(0, 1);
  lcd.print(rssiB_int);
  if(rssiB_int<rssiB_ext){digitalWrite(switchA_R1,HIGH);digitalWrite(switchA_R2,LOW); lcd.print("<"); display.print("<");}else{ digitalWrite(switchA_R1,LOW);digitalWrite(switchA_R2,HIGH); lcd.print(">"); display.print(">");}
  lcd.print(rssiB_ext);
  display.print(rssiB_ext);

   display.setCursor(0,16);
  display.print(rssiC_int);
  lcd.setCursor(0, 2);
  lcd.print(rssiC_int);
  if(rssiC_int<rssiC_ext){digitalWrite(switchB_L1,HIGH);digitalWrite(switchB_L2,LOW); lcd.print("<"); display.print("<"); }else{ digitalWrite(switchB_L1,LOW);digitalWrite(switchB_L2,HIGH); lcd.print(">");  display.print(">");}
  lcd.print(rssiC_ext);
  display.print(rssiC_ext);

  display.setCursor(0,24);
  display.print(rssiD_int);
  lcd.setCursor(0, 3);
  lcd.print(rssiD_int);
  if(rssiD_int<rssiD_ext){digitalWrite(switchB_R1,HIGH);digitalWrite(switchB_R2,LOW); lcd.print("<");display.print("<"); }else{ digitalWrite(switchB_R1,LOW);digitalWrite(switchB_R2,HIGH); lcd.print(">");  display.print(">");}
  lcd.print(rssiD_ext);
display.print(rssiD_ext);
//OLED STUFF
//int rssi_3=analogRead(A0);

//Serial.print(rssiA_int);
//Serial.print(";");
//Serial.print(rssiA_ext);
//Serial.print(";");
//Serial.println(rssi_3);
  
  /*
  display.display();

  
  display.print(analogRead(1));
  display.display();

  

  display.print(analogRead(2));
  display.display();

 
  display.print(analogRead(3));
  display.display();
  delay(500);
  display.clearDisplay();
*/









 display.display();
  
  delay(100);  
    display.clearDisplay();

  

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//None yet
//*********( THE END )***********

