#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <chrono>
#include<vector>
#include "camera/camera.h"

using namespace cv;
using std::vector;

//Uncomment the below lines to display OpenCV windows (for debug/testing)
//#define SHOW_CV_WINDOW
//#define SHOW_CV_CTHRESHOLD_WINDOW
//#define SHOW_CV_FACE_WINDOW

//Uncomment the below lines to detect object color ranges (for debug/testing)
#define RED1_TH
//#define RED2_TH
//#define BLUE_TH
//#define GREEN_TH
//#define YELLOW_TH
class Vision
{
public:
    Vision(int camWidth = 640, int camHeight = 480, int camFPS = 30, bool facialDetect = true);
    ~Vision();
    void updateFrame();
    int getFace();
    int findContour();
    void colorThreshold();
    int findObject();
    Point position;
    std::vector <Rect> roi;

private:
    CCamera* cam;
    Mat frame;
    Mat roi_bin;
    Mat gray;
    Mat bgrFrame;
    //std::vector <Rect> roi;
    Ptr <BackgroundSubtractor> pBgSub;
    CascadeClassifier faceDetect;
    const void* frame_data;
    int frame_sz;
    int camW;
    int camH;
    //const Scalar HSV_MIN{15, 210, 20};//h - h_threshold
    //const Scalar HSV_MAX{35, 255, 255};//h + h_threshold
};
