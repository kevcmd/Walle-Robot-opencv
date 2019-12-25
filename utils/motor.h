#include <thread>
#include <chrono>
#include <iostream>
#include <wiringPi.h>
#include "lib/I2Cdev.h"
#include "PD.h"
#include <softPwm.h>
#define d1Pin   27
#define d2Pin   22
#define d1Pin2  5
#define d2Pin2  6
#define en1     21
#define en2     20
#define defaultPWM  75

#define CENTER_HEAD_POSX 65
#define CENTER_HEAD_POSY 65

#define HEAD_ADDRESS 0x04
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define ROBOT_MOTOR_DEBUG

using namespace std;
class Motor
{
public:
    Motor();
    void updateTracking(int objectX, int objectY);
    void centerHead();
    void lookDown();
    void updateState(int state);
    void stopTracks();
    void setSpeed(int leftMotor, int rightMotor);
    int frameX;
    int frameY;
    void followObject(int objArea);
    int oldhX;
    int oldhY;
    void updateHeadData(int hx, int hy);
private:
    PD pdX = PD(0.03,0.03, 25, 115, CENTER_HEAD_POSX);
    PD pdY = PD(0.03,0.03, 40, 115, CENTER_HEAD_POSY);
    void sendHeadData();
    int receiveHeadData();
    int motorState;
    I2Cdev head;
};

