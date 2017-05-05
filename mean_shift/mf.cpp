#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

#define pi 3.14159265359

Mat MatchFilterAndGaussDerKernel(float sigma,int YLength, float theta, int type)
{

  if(type==1)
  {

    Mat matchFilterKernel;
    int widthOfTheKernel = ceil(pow((pow((6*ceil(sigma)+1),2) + pow(YLength,2)),0.5));
    if((widthOfTheKernel%2) == 0)
      widthOfTheKernel = widthOfTheKernel + 1;

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
        matchFilterKernel.at<double>(row,col) = 0;
        else if (abs(yPrime)> (YLength-1) / 2)
          matchFilterKernel.at<double>(row,col) = 0;
        else
          matchFilterKernel.at<double>(row,col) = -exp(-0.5*pow((xPrime/sigma),2))/((pow((2*pi),0.5)*sigma));

        col = col + 1;
      }
      row = row + 1;
    }
    float mean = sum(matchFilterKernel)/sum(matchFilterKernel < 0);

    for(int i=0;i<widthOfTheKernel;i++)
     for(int j=0;j<widthOfTheKernel;j++)
       if(matchFilterKernel.at<double>(i,j)<0)
        matchFilterKernel.at<double>(i,j) = matchFilterKernel.at<double>(i,j) - mean;
      
   return matchFilterKernel;
    }
  
 
   else
  {

    Mat GaussDerivativeKernel;
    int widthOfTheKernel = ceil(pow((pow((6*ceil(sigma)+1),2) + pow(YLength,2)),0.5));
    if((widthOfTheKernel%2) == 0)
      widthOfTheKernel = widthOfTheKernel + 1;

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
        GaussDerivativeKernel.at<double>(row,col) = 0;
        else if (abs(yPrime)> (YLength-1) / 2)
          GaussDerivativeKernel.at<double>(row,col) = 0;
        else
          GaussDerivativeKernel.at<double>(row,col) = -exp(-0.5*pow((xPrime/sigma),2))/((pow((2*pi),0.5)*pow(sigma,3)));

        col = col + 1;
      }
      row = row + 1;
    }
    
   return GaussDerivativeKernel;
    } 






  }



int main( int argc, char** argv )
{
	Mat src = imread("opmeanshift2p0image-233fullsize.png");
	
	
	waitKey(0);
}

