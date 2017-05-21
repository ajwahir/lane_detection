

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
// #include <QDebug>
#include <ctime>
#include "../mean_shift/mf.h"
#define PI 3.14159

using namespace cv;
using namespace std;

void flush(VideoCapture& camera)
{
    int delay = 0;
    int framesWithDelayCount = 0;

    while (framesWithDelayCount <= 1)
    {
			clock_t start = clock();
      camera.grab();
			clock_t finish = clock();


      delay = (finish - start) / CLOCKS_PER_SEC;
      cout<<delay<<endl;
      if(delay > 0)
      {
        framesWithDelayCount++;
      }
    }
}

int main(int argc, char** argv)
{


  //cam height=90cm
  //grid depth=347cm

	Mat trans;
	Mat frame;

	double H1[3][3]={-0.9194494538490644, 3.029211141257473, 1199.216754562135,
	  -0.002781469435773509, 2.843461629191059, 99.21405495098099,
	  -1.030173865101134e-05, 0.004728215172708329, 1};


	cv::Mat H(3,3,CV_64F,H1);
	VideoCapture capture(0);

	while(true)
	{

	   capture >> frame;
		//  frame = capture->GetLatestFrame();
    //  cout<<frame.rows<<endl<<frame.cols<<endl;
	  //  imshow("original",frame);
		Rect roi;
		roi.x = 420;
    roi.y = 217;
    roi.width = 440;
    roi.height = 271;
		warpPerspective(frame, trans, H, Size(frame.cols, frame.rows));
		// trans = trans(roi);
imshow("original",frame);
///lane detection part starts here

		Mat im = trans;
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

//lane detection part ends here




    	  //  imshow("video", trans);
    	   if (waitKey(30) == 'q')
        	break;

 	   // Use the Homography Matrix to warp the images


 	}
	   //imshow("original",frame);

	   //imshow("transformed",trans);

	cv::waitKey();

	return 0;
}
