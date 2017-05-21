#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
// #include "opencv2/nonfree/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <math.h>
#define PI 3.14159

using namespace cv;
using namespace std;

int cnt = 0;
Point2f points[10];

Mat imageR;
Mat imageC;

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


Point2f* selectPoint(Mat src)	//creates a new image from original enables to select 8 points and returns coordinates of those points
{
	if( src.empty() )
	{
		cout<<"Image empty\n";
		return NULL;
	}

	src.copyTo(imageC);

	namedWindow( "SelectPoints", 1 );
	cnt = 0;
	setMouseCallback( "SelectPoints", onMouseSelect, 0 );

	while(cnt<2)
	{
		imshow("SelectPoints",imageC);
		waitKey(10);
	}

	imshow("SelectPoints",imageC);
	return points;
}

int main(int argc, char** argv)
{
	Mat src = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	Mat finalout(src.rows, src.cols, src.type());
	while(src.data)
	{

	selectPoint(src);
	cout<<points[0]<<endl<<points[1]<<endl;
	// for(int i = 0; i < 4; i++)
	// 	draw_circle(finalout, points[i], 2);

  	waitKey(0);
  	}
  	return 0;
  }
