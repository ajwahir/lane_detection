#include <iostream>
#include <cmath>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/LaserScan.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#define pi 3.14159265359

using namespace cv;
using namespace std;

Mat MatchFilterWithGaussDerivative(int num,Mat im,float sigmaForMF,float sigmaForGD,float yLengthForMF,float yLengthForGD,int tForMatchfilterRes,int tForGaussDerRes,int numOfDirections,float c_value,float t)
{
	int rows = im.rows;
	int cols = im.cols;
	int sz[3] = {rows,cols,numOfDirections};
	Mat MatchFilterRes(3,sz, CV_8UC(1), Scalar::all(0));
	Mat GaussDerivativeRes(3,sz, CV_8UC(1), Scalar::all(0));
	int K2=tForMatchfilterRes;
	Mat S2 = Mat::ones(K2, K2, CV_8UC1);
	S2=S2/(K2*K2);
  Mat matchFilterKernel;
	Mat gaussDerivativeFilterKernel;
	Mat dst;
	Mat channel_MatchFilterRes[numOfDirections];
	Mat channel_GaussDerivativeRes[numOfDirections];
	Mat RDF;
  split(MatchFilterRes, channel_MatchFilterRes);
	split(GaussDerivativeRes, channel_GaussDerivativeRes);

	for(int i=0;i<numOfDirections;i++)
	{
		matchFilterKernel = MatchFilterAndGaussDerKernel(sigmaForMF,yLengthForMF, pi/numOfDirections*i,0);
		gaussDerivativeFilterKernel = MatchFilterAndGaussDerKernel(sigmaForGD,yLengthForGD, pi/numOfDirections*i,1);
		filter2D(im, channel_MatchFilterRes[i], -1 , matchFilterKernel, Point(-1, -1), 0, BORDER_DEFAULT );
		filter2D(im, RDF, -1 , gaussDerivativeFilterKernel, Point(-1, -1), 0, BORDER_DEFAULT );
		filter2D(RDF, channel_GaussDerivativeRes[i], -1 , S2, Point(-1, -1), 0, BORDER_DEFAULT );
		channel_GaussDerivativeRes[i]=abs(channel_GaussDerivativeRes[i]);
	}
	Mat maxMatchFilterRes = Mat::zeros(rows,cols, CV_8UC1);
  for(int j=0;j<numOfDirections;j++)
		max(maxMatchFilterRes,channel_MatchFilterRes[j],maxMatchFilterRes);
	Mat accoGaussDerivativeRes = Mat::zeros(rows,cols, CV_8UC1);
	  for(j=0;j<numOfDirections;j++)
			min(accoGaussDerivativeRes,channel_GaussDerivativeRes[j],accoGaussDerivativeRes);
  filter2D(accoGaussDerivativeRes,averageGD, -1 , S2, Point(-1, -1), 0, BORDER_DEFAULT );
	Scalar tempVal = mean(averageGD);
	float meanGD = tempVal.val[0];
	Mat normalGD = averageGD / meanGD; //check if the mat elements are of float type
	threshold(normalGD,big,5,1,THRESH_BINARY)
	threshold(normalGD,small,5,1,THRESH_BINARY_INV)
  multipy(normalGD,small,normalGD)
	normalGD = normalGD + big*5;
	Mat W = c_value + normalGD / 2;
	// bitwise_not (maskForGDRange, maskForGDRange_not );
	// multipy(W,maskForGDRange_not,W);
  // W = W + maskForGDRange * c_value;
	W = W + c_value;
	K1 = tForGaussDerRes;
	Mat S1 = Mat::ones(K1, K1, CV_8UC1);
	S1=S1/(K1*K1);
	filter2D(maxMatchFilterRes,averageMF, -1 , S1, Point(-1, -1), 0, BORDER_DEFAULT );
	multipy(W,averageMF,out1);
  vess = threshold((maxMatchFilterRes - out1),vess,0,1,THRESH_BINARY);

	return vess

}


int main( int argc, char** argv )
{
	Mat im = imread("lane_test.png");
  Mat vess1 = MatchFilterWithGaussDerivative(1,im,1.5,1.5,9,5,41,201,8,3,40);

}
