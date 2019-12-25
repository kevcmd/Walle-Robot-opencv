#include <Servo.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x04
#define CENTER_SERVOX   65
#define CENTER_SERVOY   65
#define NUM_DATA 2

byte data[NUM_DATA];
byte data_index = 0;
byte state = 1;

int tilt = 10;
int pan = 9;

int32_t posX = CENTER_SERVOX;
int32_t posY = CENTER_SERVOY;
char targetX = 0;
char targetY = 0;
int servoState = 0;
unsigned long lastTime;
unsigned long lastPulse;
unsigned long now;
int oldPosX = 0;
int oldPosY = 0;
Servo panServo;
Servo tiltServo;
void setup() {
  Wire.begin(SLAVE_ADDRESS);

Wire.onReceive(receiveData);
Wire.onRequest(sendData);
  pinMode(pan, OUTPUT);
  pinMode(tilt, OUTPUT);

  panServo.attach(pan);
  tiltServo.attach(tilt);
  panServo.write(posX);
  tiltServo.write(posY);
  delay(1000);
  panServo.detach();
  tiltServo.detach();
  delay(200);
  state = 1;

Serial.begin(9600); // Serial output
}

void startServo()
{
  if(servoState == 0)
  {
     panServo.attach(pan);
     tiltServo.attach(tilt);
     servoState = 1;
  }
}

void loop() {
now = millis();

  if(targetX != 0)
  {       
     if(targetX < 10)
     {
        if(targetX == 1)
          centerHead();
        else if(targetX == 2)
          lookDown();
        else
          posX = 10;
     }
     else if(targetX > 135)
        posX = 135;
     else
        posX = targetX;

     if(targetY != 0)
     {
        if(targetY < 40)
          posY = 40;
        else if(targetY > 125)
          posY = 125;
        else
          posY = targetY;
     }

     startServo();
     
     if(posX != oldPosX)
        panServo.write(posX);

     if(posY != oldPosY)
        tiltServo.write(posY);
     targetX = 0;
     targetY = 0;
     oldPosX = posX;
     oldPosY = posY;
  }
if((double)(now - lastTime) > 2000 && servoState != 0)
{
  panServo.detach();
  tiltServo.detach();
  delay(30);
  servoState = 0;
}
  delay(20);
  
}

void receiveData(int byteCount)
{
  state = 0;
while(Wire.available()) 
{
  data[data_index++] = Wire.read();
  if(data_index >= NUM_DATA){
    data_index = 0;
    targetX = data[0];
    targetY = data[1];
    state = 1;
}
}
    lastTime = now;
}

void sendData()
{
    Wire.write(state);
}

void centerHead()
{
  startServo();
  while(posX != CENTER_SERVOX && posY != CENTER_SERVOY)
  {
    if(posX != CENTER_SERVOX)
    {
        if(posX > CENTER_SERVOX)
        {
          if((posX-5) >= CENTER_SERVOX)
            posX -= 5;
          else
            posX = CENTER_SERVOX;
        }
        else
        {
          if((posX+5) <= CENTER_SERVOX)
            posX += 5;
          else
            posX = CENTER_SERVOX;
        }
        panServo.write(posX);
    }
    if(posY != CENTER_SERVOY)
    {
        if(posY > CENTER_SERVOY)
        {
          if((posY-5) >= CENTER_SERVOY)
            posY -= 5;
          else
            posY = CENTER_SERVOY;
        }
        else
        {
          if((posY+5) <= CENTER_SERVOY)
            posY += 5;
          else
            posY = CENTER_SERVOY;
        }
        tiltServo.write(posY);
    }
    delay(100);
  }
}

void lookDown()
{
  int downTarget = 85;
  while(posY < downTarget)
  {
    if((posY-5) <= downTarget)
      posY -= 5;
    else
      posY = downTarget;
    tiltServo.write(posY);
    delay(10);
  }
}
