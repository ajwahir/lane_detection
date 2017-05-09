

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
// #include "opencv2/nonfree/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <math.h>
#include <math.h>
#define PI 3.14159

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{


  //cam height=90cm
  //grid depth=347cm

	Mat trans;
	Mat frame;

	double H1[3][3]={-0.9292908790222717, 2.341752523940345, 607.1565948371357,
    0.009553765572285489, 2.931989216501558, 59.38005900465606,
    1.907968257994046e-05, 0.007313675675649619, 1};




	int i,j;
  	for(i=0;i<3;i++)
    	for(j=0;j<3;j++)
    	{
      		cout<<H1[i][j]<<endl;
    	}
	cv::Mat H(3,3,CV_64F,H1);

	cout<<H<<endl;
	cout<<sizeof(H)<<endl<<endl;

	VideoCapture capture(1);

	/*   if (!capture.isOpened())
	{
	    cout << "Cannot open video device or file!" << endl;
	    return -1;
	}
	*/

	//namedWindow("video", CV_WINDOW_AUTOSIZE);

	while(true)
	{
	   capture >> frame;
    //  cout<<frame.rows<<endl<<frame.cols<<endl;
	   imshow("original",frame);
	   warpPerspective(frame, trans, H, Size(frame.cols, frame.rows));
    	   imshow("video", trans);
    	   if (waitKey(30) == 'q')
        	break;

 	   // Use the Homography Matrix to warp the images


 	}
	   //imshow("original",frame);

	   //imshow("transformed",trans);

	cv::waitKey();

	return 0;
}
