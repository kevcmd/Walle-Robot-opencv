//Kevin Ray 2019
#include <stdio.h>
#include "utils/robot.h"

//Settings found in utils/constants.h

Robot walle;

int main(int argc, const char **argv)
{
	sleep(2);

	while(true)
	{
		walle.update();

		#ifdef SHOW_CV_WINDOW //Used for debug/test
		  if(cv::waitKey(30) >= 0) break;
		#endif
	}
	return 0;
}
