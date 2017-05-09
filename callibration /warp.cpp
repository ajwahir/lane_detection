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

Mat imageR;
Mat imageC;
int cnt = 0;
Point2f points[4];

void draw_circle( Mat img, Point2f center, int radius )	 //to draw a green colored circle given the circle centre, radius
{
	circle( img,center,radius,cvScalar(0x00,0xff,0x00),CV_FILLED );
}

static void onMouseSelect( int event, int x, int y, int, void* )	//for recording the mouse select activity
{
	switch( event ){
		case CV_EVENT_LBUTTONDOWN:	//stores the value of the coordinates where the mouse pointer hits
		{
			points[cnt] = Point2f((float)x, (float)y);
			break;
		}
		case CV_EVENT_LBUTTONUP:	//corresponding to above coordinates the circle is drawn using the function
		{
			draw_circle(imageC, Point2f(x, y), 4);
			cnt++;
			break;
		}
	}
}

Point2f* selectPoint(Mat src)	//creates a new image from original enables to select 4 points and returns coordinates of those points
{
	if( src.empty() )
	{
		printf("Image empty\n");
		return NULL;
	}

	src.copyTo(imageC);

	namedWindow( "SelectPoints", 1 );
	cnt = 0;
	setMouseCallback( "SelectPoints", onMouseSelect, 0 );

	while(cnt<4)
	{
		imshow("SelectPoints",imageC);
		waitKey(10);
	}

	imshow("SelectPoints",imageC);

	printf("\nReturning\n");
	return points;
}

int main(int argc, char** argv)
{
	Mat src = imread(argv[1]);
	float cx = src.cols/2, cy = src.rows/2;
	float cw = src.cols/4, ch = src.rows/4;

	selectPoint(src);

	Point2f srcq[4], dstq[4];
	for(int i=0; i<4; i++)
		srcq[i] = points[i];	//srcq now has the initial points clicked on the image

	dstq[0] = Point2f(cx-cw/2, cy-ch/2);	//this is for a rectangle with centre at centre of screen
	dstq[1] = Point2f(cx+cw/2, cy-ch/2);
	dstq[2] = Point2f(cx+cw/2, cy+ch/2);
	dstq[3] = Point2f(cx-cw/2, cy+ch/2);

	Mat H = getPerspectiveTransform(srcq, dstq);	//gives the transformation matrix for getting the focussed object as a perfect rectangle at screeen centre
	cout << H<<endl;

	Mat out;
	warpPerspective(src, out, H, Size(src.cols, src.rows));	//using the homography matrix gives the orthographic view

	for(int i = 0; i < 4; i++)
		draw_circle(out, dstq[i], 4);	//drawing circles at the points where it was selected

	imshow("output", out);
	/*VideoCapture capture(1);
	Mat frame,frameout;
	while(true)
	{
		capture>>frame;
		imshow("invideo",frame);
		warpPerspective(frame, frameout, H, Size(frame.cols, frame.rows));
		cout<<frameout.cols<<"    "<<frameout.rows;
		imshow("video", frameout);
    	cvWaitKey(30);

    //if (waitKey(30) == 'q')
      //  break;
	}*/
	imwrite("output.jpg",out);
	waitKey(0);

	return 0;
}
