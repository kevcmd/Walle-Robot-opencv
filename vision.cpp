//Kevin Ray 2019
#include "utils/vision.h"



Vision::Vision(int camWidth, int camHeight, int camFPS, bool facialDetect)
{
    this->cam = StartCamera(camWidth, camHeight, camFPS,1,true);
    camW = camWidth;
    camH = camHeight;

    if(facialDetect)
        faceDetect.load("utils/model/frontalface.xml");
}

Vision::~Vision()
{
};

void Vision::updateFrame()
{
    if(cam->BeginReadFrame(0,frame_data,frame_sz))
    {
        frame = cv::Mat(camH, camW, CV_8UC4, (void*)frame_data);
        cam->EndReadFrame(0);
        cv::cvtColor(frame, bgrFrame, COLOR_RGB2BGR);
    }
}


int Vision::getFace()
{
    int oArea = 0;
	cv::cvtColor(bgrFrame, gray, COLOR_BGR2GRAY);
	//cv::resize(gray, gray, cv::Size(), 0.50, 0.50);
    faceDetect.detectMultiScale(gray, roi, 1.1, 5, 0 | 1, Size(20, 20), Size(600, 600));

    if (roi.size() > 0)
    {
        oArea = roi[0].width * roi[0].height;
        #ifdef SHOW_CV_FACE_WINDOW
        for (int b = 0; b < roi.size(); b++)
        {
            rectangle(bgrFrame, roi[b].tl(), roi[b].br(), Scalar(0, 0, 255), 2, 8, 0);
        }
        imshow("threshold", bgrFrame);
        #endif
	}
	return oArea;
}

int Vision::findContour()
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //std::vector <Point> positions;
    int area, oArea;
    findContours(roi_bin, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    Rect rect;
    for (size_t i=0; i<contours.size(); i++)
    {
        if(contourArea(contours.at(i)) < 200)
            continue;
        rect = boundingRect(contours.at(i));
        area = rect.width * rect.height;

        //Point objectP;
        if(oArea < area)
        {
            oArea = area;
            position.x = rect.x + (rect.width / 2);
            position.y = rect.y + (rect.height / 2);
            //positions.push_back(objectP);
        }
    }
   // if(oArea > 250)
    //{
    //    roi.push_back(rect);
    //}
    return oArea;
}


void Vision::colorThreshold()
{
    Mat roi_hsv(Size(frame.cols, frame.rows), CV_8UC4);
    cvtColor(bgrFrame, roi_hsv, COLOR_BGR2HSV);
    #ifdef RED1_TH
    inRange(roi_hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), roi_bin);
    #endif
    #ifdef RED2_TH
    inRange(roi_hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), roi_bin);
    #endif
    #ifdef BLUE_TH
    inRange(roi_hsv, Scalar(110,50,50), Scalar(130,255,255), roi_bin);
    #endif
    #ifdef GREEN_TH
    inRange(roi_hsv, Scalar(50,50,50), Scalar(70,255,255), roi_bin);
    #endif
    #ifdef YELLOW_TH
    inRange(roi_hsv, Scalar(15, 210, 20), Scalar(35, 255, 255), roi_bin);
    #endif
    cv::erode(roi_bin, roi_bin, 0);
    //cv::dilate(roi_bin, roi_bin, 0);
    #ifdef SHOW_CV_CTHRESHOLD_WINDOW
        imshow("threshold", roi_bin);
    #endif
}

int Vision::findObject()
{
    int foundArea = 0;
    //std::vector <Rect> roiFace;
	//foundArea = getFace();
	//objectP.x += rect.x + (rect.width / 2);
    //objectP.y += rect.y + (rect.height / 2);
    //positions.push_back(objectP);
    colorThreshold();
    std::this_thread::sleep_for (std::chrono::milliseconds(3));
    foundArea = findContour();
    //foundArea = positions[0].x * positions[0].y;
    return foundArea;
}
