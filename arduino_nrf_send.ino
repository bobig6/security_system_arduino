// MasterSwapRoles

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



#define CE_PIN  7// D3
#define CSN_PIN 8//D0

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
const byte masterAddress[5] = {'T', 'X', 'a', 'a', 'a'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char txNum = '0';
char dataReceived[10] = "empty"; // to hold the data from the slave - must match replyData[] in the slave
bool newData = false;
char modeAS[1] = "";
bool pressed = true;
int butPin = 3;
int mode = 1;
int led = 2;
char c;
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

//============

void setup() {

  Serial.begin(115200);
  pinMode(butPin, INPUT);
  pinMode(led, OUTPUT);
  Serial.println("MasterSwapRoles Starting");
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );

  radio.openWritingPipe(slaveAddress);
  radio.openReadingPipe(1, masterAddress);

  radio.setRetries(3, 5); // delay, count
  send(); // to get things started
  prevMillis = millis(); // set clock
}

//=============

void loop() {
  if (But() && pressed == true)
  {
    mode++;
    if(mode > 2)
    {
      mode = 1;
    }
    for(int k = 0; k<mode; k++)
    {
      delay(200);
      Serial.println("Blink");
      digitalWrite(led, HIGH);
      delay(200);
      digitalWrite(led, LOW);
    }
    c = mode;
    modeAS[0] = c;
    pressed = false;
    send();

  }
  /*if (RightBut() && pressed == true)
  {
    butPressed[0] = 'R';
    pressed = false;
    send();
  }*/
  if (analogRead(butPin) < 100)
  {
    pressed = true;
  }
  //Serial.println(analogRead(butPin));
  /*
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
      send();
      prevMillis = millis();
    }*/
  getData();
  showData();

}

//====================

void send() {


  radio.stopListening();
  bool rslt;

  Serial.print(mode);
  rslt = radio.write( &modeAS, sizeof(modeAS) );
  modeAS[0] = ' ';


  radio.startListening();
  Serial.print("Data Sent ");


  if (rslt) {
    Serial.println("  Acknowledge received");
  }
  else {
    Serial.println("  Tx failed");
  }
}

//================

void getData() {

  if ( radio.available() ) {
    radio.read( &dataReceived, sizeof(dataReceived) );
    newData = true;
  }
}

//================

void showData() {
  if (newData == true) {
    Serial.print("Data received ");
    Serial.print(dataReceived);
    Serial.print(", ");
    Serial.println(dataReceived);
    Serial.println();
    newData = false;
  }
}

//================


bool But()
{
  int button = analogRead(butPin);
  if (button >= 900 && button <= 940)
  {
      return true;

  }
  return false;

}
/*bool RightBut()
{
  int button = analogRead(butPin);
  if (button >= 750 && button <= 850)
  {
    return true;
  }
  return false;
}*/
