//Kevin Ray 2019
#include "utils/motor.h"

Motor::Motor()
{
    wiringPiSetupGpio();
    //Setup pins for the L298 Motor Drive Board
    pinMode(d1Pin, OUTPUT);
    pinMode(d2Pin, OUTPUT);
    pinMode(d1Pin2, OUTPUT);
    pinMode(d2Pin2, OUTPUT);
    pinMode(en1, PWM_OUTPUT);
    pinMode(en2, PWM_OUTPUT);
    updateState(0);
    softPwmCreate(en1, 100, 100) ;
    softPwmCreate(en2, 100, 100) ;

    head.openHead("/dev/i2c-1");
    int copen = head.headAddressSet(HEAD_ADDRESS);
    #ifdef ROBOT_MOTOR_DEBUG
    if(copen == 0)
        std::cout << "Failed to open head port" << ".\n";
    #endif
}

void Motor::updateTracking(int objectX, int objectY)
{
    pdX.update(frameX-objectX);
    pdY.update(frameY-objectY);
    updateHeadData(pdX.pos, pdY.pos);
    #ifdef ROBOT_MOTOR_DEBUG
    std::cout << "Error X: " << objectX << " Error Y: " << objectY << ".\n";
    std::cout << "New X: " << pdX.pos << " New Y: " << pdY.pos << ".\n";
    #endif
}

void Motor::centerHead()
{
    updateHeadData(1,0);
    pdX.reset();
    pdY.reset();
}

void Motor::lookDown()
{
    updateHeadData(2,0);
}

void Motor::updateState(int state)
{
    motorState = state;

    switch(state)
    {
    case 0:
        digitalWrite(d1Pin, LOW);//tracks off
        digitalWrite(d2Pin, LOW);
        digitalWrite(d1Pin2, LOW);
        digitalWrite(d2Pin2, LOW);
        break;
    case 1:
        digitalWrite(d2Pin, HIGH);//tracks forward
        digitalWrite(d1Pin2, HIGH);
        break;
    case 2:
        digitalWrite(d1Pin, HIGH);//tracks reverse
        digitalWrite(d2Pin2, HIGH);
        break;
    case 3:
        digitalWrite(d1Pin, HIGH);//left turn
        digitalWrite(d1Pin2, HIGH);
        break;
    case 4:
        digitalWrite(d2Pin, HIGH);//right turn
        digitalWrite(d2Pin2, HIGH);
        break;
    }
}

void Motor::stopTracks()
{
    if(motorState != 0)
    {
        updateState(0);
        softPwmWrite(en1, 0);
        softPwmWrite(en2, 0);
    }
}

void Motor::setSpeed(int leftMotor, int rightMotor)
{
    softPwmWrite(en1, leftMotor);
    softPwmWrite(en2, rightMotor);
    #ifdef ROBOT_MOTOR_DEBUG
    std::cout << "Left Motor Speed: " << leftMotor << " - Right Motor Speed: " << rightMotor << ".\n";
    #endif
}

void Motor::followObject(int objArea)
{
    int32_t followError = CENTER_HEAD_POSX - pdX.pos;
    //objArea -= objArea >> 3;
    int forwardSpeed = constrain(100 - (objArea/50), -100, 100);

    #ifdef ROBOT_DEBUG
    std::cout << "Forward Speed: " << forwardSpeed << ".\n";
    #endif

	int32_t differential = (followError + (followError * forwardSpeed))>>8;

	int leftSpeed = constrain(forwardSpeed + differential, -100, 100);
	int rightSpeed = constrain(forwardSpeed - differential, -100, 100);

	#ifdef ROBOT_DEBUG
    std::cout << "Left Speed: " << leftSpeed << " - Right Speed: " << rightSpeed << ".\n";
    #endif

    //uMotor.setSpeed(leftSpeed, rightSpeed);
}

int Motor::receiveHeadData()
{
    return head.headReadByte();
}

void Motor::updateHeadData(int hx, int hy)
{

    int state = receiveHeadData();
    #ifdef ROBOT_MOTOR_DEBUG
    std::cout << "Receive state is: " << state << ".\n";
    #endif
    if(state == 1)
    {
        if(hx != 0)
        {
            head.headWriteByte(hx);
        //if(hy != 0 && hy != oldhY)
            head.headWriteByte(hy);
		}
        //oldhX = hx;
        //oldhY = hy;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(30));
}
