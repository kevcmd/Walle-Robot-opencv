//Kevin Ray 2019
#include "utils/robot.h"

Robot::Robot()
{
    uMotor.frameX = CAM_WIDTH/2;
    uMotor.frameY = CAM_HEIGHT/2;

    centerRobotHead();
    //uMotor.updateHeadData(55,75);
    headCentered = true;
    robotState = TRACK;
    //int stepscan = scanPath(5,1);
    //cDistance.resize(DIST_BUFFER);
    //heading.resize(POSITION_LIST);
}

Robot::~Robot()
{
    StopCamera();
};

void Robot::update()
{
    uVision.updateFrame();

    #ifdef ROBOT_SENSOR_DEBUG
	int dis = uSensor.readDistance();
	int head = uSensor.getHeading();
	std::cout << "Distance: " << dis << ".\n";
	std::cout << "Heading: " << head << ".\n";
	#endif

    switch(robotState) //Switch mode between Idle, Tracking or Explore
    {
        case IDLE:
            robotState = Idle();
            break;
        case TRACK:
            robotState = Track();
            break;
        case EXPLORE:
            robotState = Explore();
            break;
    }
}

int Robot::Idle()
{
    if(stateTimer.getTimer())
    {
        if(lookForObject())
        {
            stateTimer.resetTimer(true);
            return TRACK;
        }
    }
    else
    {
        stateTimer.resetTimer(true);
        return EXPLORE;
    }
    return IDLE;
}

int Robot::Track()
{
    if(stateTimer.getTimer())
    {
        if(trackObject())
            return TRACK;
        else
        {
            centerRobotHead();
            stateTimer.resetTimer(true);
            return EXPLORE;
        }
    }
    else
    {
        //centerRobotHead();
        stateTimer.resetTimer(true);
        //return EXPLORE;
    }
    return TRACK;
}

int Robot::Explore()
{
    if(stateTimer.getTimer(EXPLORE_TIME))
    {
        if(checkPath())//If new heading is not found, continue this path
            doPath(false);
        else//Else change path to match heading
            doPath(true);

        return EXPLORE;
    }
    else
    {
        stateTimer.resetTimer(true);
        return IDLE;
    }
    return IDLE;
}
/*
void Robot::doPath()
{
	//int dis = uSensor.readDistance();
	//cDistance.push_back(uSensor.readDistance());
	//if(timer.getTimer(HEAD_CENTER_TIME))
    //{
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
		cDistance.push_back(uSensor.readDistance());
        int d = accumulate(cDistance.begin(), cDistance.end(), 0)/ cDistance.size();

        if(d > MIN_DIST_THRESH)
        {
            return true;
            #ifdef ROBOT_DEBUG
            std::cout << d/10 << "Path is good" << ".\n";
            #endif
        }
        else if(d <= STOPPING_DIST)
        {
            uMotor.stopTracks();
            headCentered = false;
            timer.resetTimer(false);
            #ifdef ROBOT_DEBUG
            std::cout << d << "Stopped" << ".\n";
            #endif
        }
    //}
    if(cDistance.size() >= DIST_BUFFER)
		cDistance.clear();
}
*/
bool Robot::checkPath()
{
    if(headCentered)
    {
        if(timer.getTimer(HEAD_CENTER_TIME))
        {
            cDistance.push_back(uSensor.readDistance());
            int d = accumulate(cDistance.begin(), cDistance.end(), 0)/ cDistance.size();

            if(d > MIN_DIST_THRESH)
            {
                return true;
                #ifdef ROBOT_DEBUG
                std::cout << d/10 << "Path is good" << ".\n";
                #endif
            }
            else if(d <= STOPPING_DIST)
            {
                uMotor.stopTracks();
                headCentered = false;
                timer.resetTimer(false);
                #ifdef ROBOT_DEBUG
                std::cout << d/10 << "Stopped" << ".\n";
                #endif
            }
        }
        else
        {
            headCentered = false;
            timer.resetTimer(false);
        }
    }
    else
    {
        int b = rand()%2;
        int g = scanPath(5, b);
        int c = cDistance.back();
        if(g > c)
        {
            return addPath();
            #ifdef ROBOT_DEBUG
            std::cout << c << " - Center Distance " << ".\n";
            std::cout << g << " Scan: New Path Found " << b << ".\n";
            #endif
        }
        else
        {
            if(g <= STOPPING_DIST)
                uMotor.stopTracks();
            if(b) b = 0;
            else b = 1;

            int s = scanPath(5, b);

            if(s > c || s > g)
                addPath();
            //else if(s <= STOPPING_DIST)
        }
		centerRobotHead();
    }
    if(cDistance.size() >= DIST_BUFFER)
		cDistance.clear();
}

int Robot::scanPath(int step, bool direction)
{
    //vector <int> scanVec;
    int l, x = 0;
    //int c = cDistance.back();
    //centerRobotHead();
    if(direction)
		x = 190;
	else
		x = step-uMotor.frameX;

    for(int k = 0; k <= SCAN_RANGE; k++)
    {
        uMotor.updateTracking(uMotor.frameX, x);//(x, uMotor.frameY);
        std::this_thread::sleep_for (std::chrono::milliseconds(50));
        //l = uSensor.readDistance();
        //scanVec.push_back(l);
    }
  /*  int tm = *max_element(scanVec.begin(), scanVec.end());
    int ti = *min_element(scanVec.begin(), scanVec.end());
    int p = this->getPercent(tm, ti);
    if(p < 40)
        p = accumulate(scanVec.begin(), scanVec.end(), 0)/scanVec.size();
    else
        p = ti;*/
    return 0;
}

bool Robot::addPath()
{
    int c, p = 0;
    int m = uSensor.getHeading();
    std::this_thread::sleep_for (std::chrono::milliseconds(5));
    if(m > 0)
    {
		centerRobotHead();
        if(heading.size() > 0)
        {
			c = heading.back();
			if(cDistance.size() >= POSITION_LIST)
				heading.clear();
            #ifdef ROBOT_DEBUG
            std::cout << c << " Heading to: " << m << ".\n";
            #endif
            if(c != m)
                heading.push_back(m);
            else
				return false;
        }
        else
            heading.push_back(m);
    }
    return true;
}

void Robot::doPath(bool switchPath)
{

}

int Robot::getPercent(int f, int t)
{
    return abs(((f-t) * 100) / t);
}

bool Robot::lookForObject()
{
    return true;
}

bool Robot::trackObject()
{
    if(trackingTimer.getTimer())
    {
	int ObjectArea = uVision.findObject();

    if(ObjectArea > 250 && ObjectArea < 90000)
    {
        int x = uVision.position.x;//uVision.roi[0].x + (uVision.roi[0].width / 2);
        int y = uVision.position.y;//uVision.roi[0].y + (uVision.roi[0].height / 2);
        /*if(oldX != 0 && oldY != 0)
        {
            int pX = getPercent(x, oldX);
            int pY = getPercent(y, oldY);
            if(pX > 30)
                x = oldX;
            if(pY > 30)
                y = oldY;
        }*/
        avX.push_back(x);
        avY.push_back(y);
        avArea.push_back(ObjectArea);
        int aX = accumulate(avX.begin(), avX.end(), 0)/avX.size();
        int aY = accumulate(avY.begin(), avY.end(), 0)/avY.size();
        int avAr = accumulate(avArea.begin(), avArea.end(), 0)/avArea.size();
        uMotor.updateTracking(x, y);
		
        #ifdef ROBOT_DEBUG
        std::cout << "X: " << aX << " Y: " << aY << " AREA: " << avAr << ".\n";
        #endif
        uMotor.followObject(avAr);
        oldX = x;
        oldY = y;
        //uMotor.updateState(1);
        if(avX.size() > 14)
            avX.clear();
        if(avY.size() > 14)
            avY.clear();
        if(avArea.size() > 4)
            avArea.clear();

        uVision.roi.clear();
	}
	else
        uMotor.stopTracks();
    }
    else
    {
		//uMotor.updateState(0);
		//centerRobotHead();
        trackingTimer.resetTimer(true);
        //return true;
    }
   std::this_thread::sleep_for (std::chrono::milliseconds(100));
   return true;
}



bool Robot::findLostObject()
{
return true;
}

void Robot::centerRobotHead()
{
	if(!headCentered)
	{
		uMotor.centerHead();
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
		headCentered = true;
	}
}
