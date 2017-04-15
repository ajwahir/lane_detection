#include <iostream>
#include <cmath>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/LaserScan.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	Mat src = imread("lane_test.png");
	int size_src[3]={src.rows,src.cols,3};
	// Mat dst(3,size_src,CV_8UC3,Scalar::all(0));
	Mat dst;
	double sr=13,sp=13;
	int maxLevel;
	TermCriteria termcrit=TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS,5,0.5);
	pyrMeanShiftFiltering(src,dst,sp,sr,maxLevel=2,termcrit);
	// cout << "dst = " << endl << " " << dst << endl << endl;
	imshow("Original",src);
	imshow("filtered",dst);

	waitKey(0);
}
