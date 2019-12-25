//Robot class settings
#define ROBOT_DEBUG
//#define ROBOT_SENSOR_DEBUG
#define DIST_BUFFER      5
//Distance in mm
//Time in seconds
#define STOPPING_DIST    300
#define MIN_DIST_THRESH  500
#define MIN_DIST_PERCENT 25
#define HEAD_CENTER_TIME 3
#define SCAN_RANGE       30
#define POSITION_LIST    4
#define EXPLORE_TIME     30

#define IDLE             1
#define TRACK            2
#define EXPLORE          3
//Vision class settings
//Face detect (LBP) found in model folder
#define CAM_WIDTH        640
#define CAM_HEIGHT       480
#define CAM_FPS          25
#define FACE_DETECT      true

//Sensor class settings
//Current sensors include - VL53L0X distance sensor and GY-85 IMU sensor
#define USE_IMU          true
#define DISTANCE_SENSOR  true


//Motor class settings
//Motor pin configuration and settings found in motor.h
