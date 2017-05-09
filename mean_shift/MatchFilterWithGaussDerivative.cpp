#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "mf.h"
#define pi 3.14159265359

using namespace cv;
using namespace std;




int main( int argc, char** argv )
{
	// cout<<"here";
	Mat im = imread("opmeanshift2p0image-171fullsize.png");
  int rows=im.rows;
  int cols=im.cols;
  Mat maskForGDRange = Mat::zeros(rows,cols, CV_8UC1);
  maskForGDRange.convertTo(maskForGDRange,CV_32F);
	Mat channel[3];
	split(im,channel);
	Mat_<float> fm;
  channel[0].convertTo(fm,CV_32F);
  Mat vess1 = MatchFilterWithGaussDerivative(1,fm,1.5,1.5,9,5,41,201,8,maskForGDRange,3.0,40);
	imshow("filtered",vess1);
	imwrite("lane_out.jpg",vess1);
	waitKey(0);

  return 0;
}
