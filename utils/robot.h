#include <math.h>
#include<vector>
#include<algorithm>
#include <thread>
#include <chrono>
#include <iostream>
#include <numeric>
#include "constants.h"
#include "vision.h"
#include "sensor.h"
#include "motor.h"
#include "timeUtil.h"



using std::vector;
using namespace std;
class Robot
{
public:
    Robot();
    ~Robot();
    void update();
    int Idle();
    int Track();
    int Explore();
    bool checkPath();
    bool addPath();
    void doPath(bool switchPath);
    int scanPath(int step, bool direction);
    bool lookForObject();
    bool trackObject();
    bool findLostObject();
    void centerRobotHead();
    //void followObject(int objArea);

private:
    std::vector <int> cDistance;//{DIST_BUFFER};
    std::vector <int> heading;//{POSITION_LIST};
    std::vector <int> avX;
    std::vector <int> avY;
    std::vector <int> avArea;
    int getPercent(int c, int p);
    int robotState;
    int oldX;
    int oldY;
    bool headCentered = false;
    Sensor uSensor{USE_IMU, DISTANCE_SENSOR};
    Vision uVision{CAM_WIDTH, CAM_HEIGHT, CAM_FPS, FACE_DETECT};
    Motor uMotor;
    TimeUtil timer;
    TimeUtil stateTimer;
    TimeUtil trackingTimer;
};
