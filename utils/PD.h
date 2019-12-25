#include <math.h>
#include <chrono>
class PD
{
public:

    PD(double proportional, double derivative, int minP, int maxP, int setPos)
    {
        centerPos = setPos;
        pos = setPos;
        p = proportional;
        d = derivative;
        prevError = 0;
        minPOS = minP;
        maxPOS = maxP;
    }


    void update(int error)
    {
		double sum = 0;
		t = std::chrono::system_clock::now();
        if (prevError!=0)
        {
			double dt = std::chrono::duration_cast<std::chrono::microseconds>(t-prev_time_).count() * 0.0001;
			double derivativeT = (error - prevError) / dt;
            //pos += (error*p + (error - prevError)*d);
			//pos += (error*p) + (derivativeT*d);
			pos += ((error*p) + (derivativeT*d))*-1;
			
			/*if (sum>35)
                sum = 35;
            else if (sum < -35)
                sum = -35;
                
			pos += sum;*/
			
            if (pos>maxPOS)
                pos = maxPOS;
            else if (pos<minPOS)
                pos = minPOS;
        }
        prevError = error;
        prev_time_ = t;
    }

    void reset()
    {
        pos = centerPos;
        prevError = 0;
    }

	int pos;

private:
	std::chrono::time_point<std::chrono::system_clock> prev_time_;
	std::chrono::time_point<std::chrono::system_clock> t;
	int prevError;
	double p;
	double d;
	int centerPos;
	int minPOS;
	int maxPOS;
};

