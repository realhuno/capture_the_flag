// COM 7
#include <IRremote.h>

int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define ZERO 360
#define ONE  650

#define TOLERANCE 200

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+

#include <nRF24L01.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/

#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/

const int pinCE = 9; //This pin is used to set the nRF24 to standby (0) or active mode (1)

const int pinCSN = 10; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out


RF24 radio(pinCE, pinCSN); // Create your nRF24 object or wireless SPI connection

#define WHICH_NODE 3     // must be a number from 1 - 6 identifying the PTX node                         //<--------------------CHANGE ME

const uint64_t wAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};

const uint64_t PTXpipe = wAddress[ WHICH_NODE - 1 ];   // Pulls the address from the above array for this node's pipe

byte counter = 1; //used to count the packets sent

bool done = false; //used to know when to stop sending packets


void setup(){
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(115200);   //start serial to communicate process

 
  radio.begin();            //Start the nRF24 module

  radio.setPALevel(RF24_PA_LOW);  // "short range setting" - increase if you want more range AND have a good power supply
  radio.setChannel(108);          // the higher channels tend to be more "open"


  Serial.begin(115200);
  Serial.println("started transmitter");
}

void loop(){
  
     
  if (irrecv.decode(&results)) {
     
      
    Serial.print("got data: ");
    Serial.print(results.rawlen-1,DEC);
    Serial.println(" bits");
    
    int codeType = results.decode_type;
    unsigned char received_value = 0;
    unsigned int set_bits = 0;
    for(int i = 1; i <= results.rawlen -1; i++){
      unsigned int signal_length = results.rawbuf[i]*USECPERTICK;
      
      Serial.print(i);
      Serial.print(":");
      Serial.print(signal_length,DEC);

      if(set_bits < 6){
        if(signal_length > (ONE - TOLERANCE)){ // ONE
          BIT_SET(received_value,6 - i);
          Serial.println(" ONE");
        }else{ // ZERO
          BIT_CLEAR(received_value,6-i);
          Serial.println(" ZERO");
        }
        set_bits++;
      }else{
        Serial.println("");
      }
    }
    Serial.print("value: ");
    Serial.println(received_value,DEC);
   byte randNumber = (byte)received_value; //generate random guess between 0 and 10

     radio.openWritingPipe(PTXpipe);        //open writing or transmit pipe

     radio.write( &randNumber, 1 );



      
    irrecv.resume(); // resume receiver
  }
}
