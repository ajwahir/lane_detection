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

Mat imageR;
Mat imageC;
int cnt = 0;
Point2f points[10];

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

	while(cnt<10)
	{
		imshow("SelectPoints",imageC);
		waitKey(10);
	}

	imshow("SelectPoints",imageC);

    /*float sumy1=0,sumy2=0;
    for(int i=0; i<4; i++)
    {
        sumy1+=points[i].y;
        sumy2+=points[4+i].y;
    }
    sumy1/=4;
    sumy2/=4;
    for(int i=0; i<4; i++)
    {
        points[i].y=sumy1;
        points[4+i].y=sumy2;
    }*/

	return points;
}





int main()
{
	Mat src = imread("lanes.jpg", CV_LOAD_IMAGE_UNCHANGED);
	while(src.data)
	{
	float cx = src.cols/2, cy = src.rows/2;
	float cw = src.cols/4, ch = src.cols/4;
	selectPoint(src);
	Point2f srcq[4], dstq[4];
	Mat finalout(src.rows, src.cols, src.type()), out1, out2, out3, out4;




    //CENTRE-LEFT IMAGE
    srcq[0] = points[1];
    srcq[1] = points[2];
    srcq[2] = points[7];
    srcq[3] = points[8];

	dstq[0] = Point2f(cx-cw, cy-ch/2);	//this is for a rectangle with centre to immediate left of centre of screen
	dstq[1] = Point2f(cx, cy-ch/2);
	dstq[2] = Point2f(cx, cy+ch/2);
	dstq[3] = Point2f(cx-cw, cy+ch/2);


	Mat H1 = getPerspectiveTransform(srcq, dstq), H2, H3, H4;	//gives the transformation matrix for getting the focussed object as a perfect rectangle at screeen centre(left)
	cout <<"\nFor Centre Left Image: "<< H1	<<endl;
	for(int i = 0; i < 4; i++)
		draw_circle(finalout, dstq[i], 4);	//drawing circles at the points where it was selected




	//CENTRE-RIGHT IMAGE
    srcq[0] = points[2];
    srcq[1] = points[3];
    srcq[2] = points[6];
    srcq[3] = points[7];

	dstq[0] = Point2f(cx, cy-ch/2);	//this is for a rectangle with centre to immediate right of centre of screen
	dstq[1] = Point2f(cx+cw, cy-ch/2);
	dstq[2] = Point2f(cx+cw, cy+ch/2);
	dstq[3] = Point2f(cx, cy+ch/2);

	H2 = getPerspectiveTransform(srcq, dstq);	//gives the transformation matrix for getting the focussed object as a perfect rectangle at screeen centre(right)
	cout <<"\nFor Centre Right Image: "<< H2<<endl;
	for(int i = 0; i < 4; i++)
		draw_circle(finalout, dstq[i], 4);	//drawing circles at the points where it was selected




	//far left IMAGE
    srcq[0] = points[0];
    srcq[1] = points[1];
    srcq[2] = points[8];
    srcq[3] = points[9];


	dstq[0] = Point2f(cx-2.0*cw, cy-ch/2);	//this is for a rectangle with centre at far left of screen
	dstq[1] = Point2f(cx-1.0*cw, cy-ch/2);
	dstq[2] = Point2f(cx-1.0*cw, cy+ch/2);
	dstq[3] = Point2f(cx-2.0*cw, cy+ch/2);

	H3 = getPerspectiveTransform(srcq, dstq);	//gives the transformation matrix for getting the focussed object as a perfect rectangle at the far left of the screeen centre
	cout << "\nFar Left Image: "<<H3<<endl;
	for(int i = 0; i < 4; i++)
		draw_circle(finalout, dstq[i], 4);	//drawing circles at the points where it was selected




	//FAR RIGHT IMAGE
    srcq[0] = points[3];
    srcq[1] = points[4];
    srcq[2] = points[5];
    srcq[3] = points[6];

	dstq[0] = Point2f(cx+cw, cy-ch/2);	//this is for a rectangle with centre at far right of screen
	dstq[1] = Point2f(cx+2.0*cw, cy-ch/2);
	dstq[2] = Point2f(cx+2.0*cw, cy+ch/2);
	dstq[3] = Point2f(cx+cw, cy+ch/2);

	H4 = getPerspectiveTransform(srcq, dstq);	//gives the transformation matrix for getting the focussed object as a perfect rectangle at the far right of the screeen centre
	cout <<"\nRight Image: "<< H4<<endl;
	for(int i = 0; i < 4; i++)
		draw_circle(finalout, dstq[i], 4);	//drawing circles at the points where it was selected



    warpPerspective(src, out1, H1, Size(src.cols, src.rows));	//using the homography matrix gives the orthographic view
	warpPerspective(src, out2, H2, Size(src.cols, src.rows));
	warpPerspective(src, out3, H3, Size(src.cols, src.rows));
	warpPerspective(src, out4, H4, Size(src.cols, src.rows));

    float xshift1=cx-cw, xshift3=cx-2.0*cw, xshift4=cx+cw;






	/*//EXPERIMENTAL CODE...@Arvind: Using elements of H to manually calculate only the requisite pixels' transformation
	 double tempx, tempy;
	 //invert(H,H); // Inverts the matrix as the below fomula is applicable only if you are carrying out dst-->src
	 for(float i=cx-cw/2; i<xlim; i++)
	 {
	    for(float j=cy-ch/2; j<ylim;j++)
	    {
	        tempx=(H.at<double>(0,0)*j + H.at<double>(0,1)*i + H.at<double>(0,2))/(H.at<double>(2,0)*j + H.at<double>(2,1)*i + H.at<double>(2,2));
	        tempy=(H.at<double>(1,0)*j + H.at<double>(1,1)*i + H.at<double>(1,2))/(H.at<double>(2,0)*j + H.at<double>(2,1)*i + H.at<double>(2,2));
	        out2.at<Vec3b>(j,i)[0]=src.at<Vec3b>(tempy,tempx)[0];
	        out2.at<Vec3b>(j,i)[1]=src.at<Vec3b>(tempy,tempx)[1];
	        out2.at<Vec3b>(j,i)[2]=src.at<Vec3b>(tempy,tempx)[2];
	        cout<<"\n"<<i<<"\t"<<j;
	    }
	 }*/






	for(int i=0; i<cw; i++)
	{
		for(int j=cy-ch/2; j<cy+ch/2; j++)
		{

			finalout.at<Vec3b>(j,xshift1+i)[0]=out1.at<Vec3b>(j,xshift1+i)[0];
			finalout.at<Vec3b>(j,xshift1+i)[1]=out1.at<Vec3b>(j,xshift1+i)[1];
			finalout.at<Vec3b>(j,xshift1+i)[2]=out1.at<Vec3b>(j,xshift1+i)[2];



			finalout.at<Vec3b>(j,cx+i)[0]=out2.at<Vec3b>(j,cx+i)[0];
			finalout.at<Vec3b>(j,cx+i)[1]=out2.at<Vec3b>(j,cx+i)[1];
			finalout.at<Vec3b>(j,cx+i)[2]=out2.at<Vec3b>(j,cx+i)[2];



			finalout.at<Vec3b>(j,xshift3+i)[0]=out3.at<Vec3b>(j,xshift3+i)[0];
			finalout.at<Vec3b>(j,xshift3+i)[1]=out3.at<Vec3b>(j,xshift3+i)[1];
			finalout.at<Vec3b>(j,xshift3+i)[2]=out3.at<Vec3b>(j,xshift3+i)[2];



			finalout.at<Vec3b>(j,xshift4+i)[0]=out4.at<Vec3b>(j,xshift4+i)[0];
			finalout.at<Vec3b>(j,xshift4+i)[1]=out4.at<Vec3b>(j,xshift4+i)[1];
			finalout.at<Vec3b>(j,xshift4+i)[2]=out4.at<Vec3b>(j,xshift4+i)[2];
		}
	}


	imshow("output", finalout);
	waitKey(0);
	}
	return 0;
}
