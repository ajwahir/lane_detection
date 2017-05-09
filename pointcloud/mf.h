#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

#define pi 3.14159265359

#define pi 3.14159265359

using namespace cv;
using namespace std;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
Mat MatchFilterAndGaussDerKernel(float sigma,int YLength, float theta, int type)
{

  if(type==0)
  {

    int widthOfTheKernel = ceil(pow((pow((6*ceil(sigma)+1),2) + pow(YLength,2)),0.5));
    if((widthOfTheKernel%2) == 0)
      widthOfTheKernel = widthOfTheKernel + 1;
    Mat matchFilterKernel = Mat::zeros(widthOfTheKernel,widthOfTheKernel, CV_32FC1);
    int halfLength = (widthOfTheKernel - 1) / 2;
    int row = 0;
    for(int y=halfLength;y>=-halfLength;y--)
    {
     int col=0;
     for(int x=-halfLength;x<=halfLength;x++)
     {
      int xPrime = x * cos(theta) + y * sin(theta);
      int yPrime = y * cos(theta) - x * sin(theta);
      if(abs(xPrime)>3.5*ceil(sigma))
        matchFilterKernel.at<float>(Point(row,col)) = 0;
        else if (abs(yPrime)> (YLength-1) / 2)
          matchFilterKernel.at<float>(Point(row,col)) = 0;
        else
					matchFilterKernel.at<float>(Point(row,col)) = -exp(-0.5*pow((xPrime/sigma),2))/((pow((2*pi),0.5)*sigma));
        col = col + 1;
      }
      row = row + 1;
    }
    transpose(matchFilterKernel,matchFilterKernel);
		// Mat out;
		Scalar tempVal1 = sum(matchFilterKernel);
		// cout<<matchFilterKernel<<endl;
		int negative_sum=0;
		for (int i = 0; i < matchFilterKernel.cols; i++ )
        for (int j = 0; j < matchFilterKernel.rows; j++)
            if (matchFilterKernel.at<float>(j, i)<0)
                negative_sum+= 1;
		// threshold(matchFilterKernel,out,0,1,THRESH_TOZERO_INV);
		// Scalar tempVal2 = sum(out);
		float mean = tempVal1.val[0]/negative_sum;
    for(int i=0;i<widthOfTheKernel;i++)
     for(int j=0;j<widthOfTheKernel;j++)
       if(matchFilterKernel.at<float>(Point(i,j))<0)
        matchFilterKernel.at<float>(Point(i,j)) = matchFilterKernel.at<float>(Point(i,j)) - mean;
   return matchFilterKernel;
    }


   else
  {


    int widthOfTheKernel = ceil(pow((pow((6*ceil(sigma)+1),2) + pow(YLength,2)),0.5));
    if((widthOfTheKernel%2) == 0)
      widthOfTheKernel = widthOfTheKernel + 1;
    int halfLength = (widthOfTheKernel - 1) / 2;
		Mat GaussDerivativeKernel = Mat::zeros(widthOfTheKernel,widthOfTheKernel, CV_32FC1);
    int row = 0;
    for(int y=halfLength;y>=-halfLength;y--)
    {
     int col=0;
     for(int x=-halfLength;x<=halfLength;x++)
     {
      int xPrime = x * cos(theta) + y * sin(theta);
      int yPrime = y * cos(theta) - x * sin(theta);
      if(abs(xPrime)>3.5*ceil(sigma))
        GaussDerivativeKernel.at<float>(Point(row,col)) = 0;
      else if (abs(yPrime)> (YLength-1) / 2)
        GaussDerivativeKernel.at<float>(Point(row,col)) = 0;
      else
        GaussDerivativeKernel.at<float>(Point(row,col)) = -exp(-0.5*pow((xPrime/sigma),2))*xPrime/((pow((2*pi),0.5)*pow(sigma,3)));

        col = col + 1;
      }
      row = row + 1;
    }
   transpose(GaussDerivativeKernel,GaussDerivativeKernel);
   return GaussDerivativeKernel;
    }
  }

Mat MatchFilterWithGaussDerivative(int num,Mat im,float sigmaForMF,float sigmaForGD,int yLengthForMF,int yLengthForGD,int tForMatchfilterRes,int tForGaussDerRes,int numOfDirections,Mat maskForGDRange,float c_value,int t)
{
	int rows = im.rows;
	int cols = im.cols;
	// cout<<"here";
	int sz[3] = {rows,cols,numOfDirections};
	Mat MatchFilterRes(3,sz, CV_32FC(1), Scalar::all(0));
	Mat GaussDerivativeRes(3,sz, CV_32FC(1), Scalar::all(0));
	int K2=tForMatchfilterRes;
	Mat S2 = Mat::ones(K2, K2, CV_32FC1);
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
	maxMatchFilterRes.convertTo(maxMatchFilterRes,CV_32F);
	maxMatchFilterRes=channel_MatchFilterRes[0];
  for(int j=0;j<numOfDirections;j++)
		max(maxMatchFilterRes,channel_MatchFilterRes[j],maxMatchFilterRes);
	Mat accoGaussDerivativeRes = Mat::zeros(rows,cols, CV_8UC1);
	accoGaussDerivativeRes.convertTo(accoGaussDerivativeRes,CV_32F);
	accoGaussDerivativeRes=channel_GaussDerivativeRes[0];
	for(int j=0;j<numOfDirections;j++)
		min(accoGaussDerivativeRes,channel_GaussDerivativeRes[j],accoGaussDerivativeRes);
	// cout<<maxMatchFilterRes<<endl;
  Mat averageGD;
  filter2D(accoGaussDerivativeRes,averageGD, -1 , S2, Point(-1, -1), 0, BORDER_DEFAULT );
	Scalar tempVal = mean(averageGD);
	float meanGD = tempVal.val[0];
	// cout<<meanGD<<endl;
	Mat normalGD = averageGD / meanGD; //check if the mat elements are of float type
	Mat big,small;
	int th=5;
	threshold(normalGD,big,th,1,THRESH_BINARY);
	threshold(normalGD,small,th,1,THRESH_BINARY_INV);
  multiply(normalGD,small,normalGD);
	normalGD = normalGD + big*th;
	Mat W = c_value + normalGD / 2;
	// cout<<W<<endl;
  Mat maskForGDRange_not=Mat::ones(rows, cols, CV_32FC1);
  maskForGDRange_not=maskForGDRange_not-maskForGDRange;
	multiply(W,maskForGDRange_not,W);
  W = W + maskForGDRange_not * c_value;
	// W = W + c_value;
	int K1 = tForGaussDerRes;
	Mat S1 = Mat::ones(K1, K1, CV_32FC1);
	S1=S1/(K1*K1);
	Mat averageMF;
	Mat out1;
	filter2D(maxMatchFilterRes,averageMF, -1 , S1, Point(-1, -1), 0, BORDER_DEFAULT );
	multiply(W,averageMF,out1);
	// cout<<out1<<endl;
	Mat diff_max=maxMatchFilterRes - out1;
	// cout<<maxMatchFilterRes<<endl;
  Mat vess;
  threshold(diff_max,vess,0,1,THRESH_BINARY);
  // cout<<vess<<endl;
	return vess;
	// return im;

}
