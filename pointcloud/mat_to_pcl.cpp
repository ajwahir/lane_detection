#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "mf.h"
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>

using namespace sensor_msgs;
using namespace std;
using namespace cv;

#define height_of_cam_from_ground -0.9
#define blind_spot 0.4
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

int main(int argc, char** argv)
{
  float x_pix=183.33;
  float y_pix=103.33;

  Mat trans;
  Mat frame;

  double H1[3][3]={-0.9194494538490644, 3.029211141257473, 1199.216754562135,
    -0.002781469435773509, 2.843461629191059, 99.21405495098099,
    -1.030173865101134e-05, 0.004728215172708329, 1};

  cv::Mat H(3,3,CV_64F,H1);

  VideoCapture capture(0);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT,720,CV_CAP_PROP_FRAME_WIDTH,1280);
  while(true)
	{
	  capture >> frame;
		Rect roi;
		roi.x = 420;
    roi.y = 217;
    roi.width = 440;
    roi.height = 271;
		warpPerspective(frame, trans, H, Size(frame.cols, frame.rows));
		trans = trans(roi);
    // imshow("original",frame);

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


  ros::init (argc, argv, "pub_pcl");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<PointCloud> ("points2", 1);

  PointCloud2Ptr points_msg = boost::make_shared<PointCloud2>();
  points_msg->header = ...; // TODO fill in header
  points_msg->height = frame.rows; // if this is a "full 3D" pointcloud, height is 1; if this is Kinect-like pointcloud, height is the vertical resolution
  points_msg->width  = frame.cols;
  points_msg->is_bigendian = false;
  points_msg->is_dense = false; // there may be invalid points

  sensor_msgs::PointCloud2Modifier pcd_modifier(*points_msg);
  // this call also resizes the data structure according to the given width, height and fields
  pcd_modifier.setPointCloud2FieldsByString(2, "xyz", "rgb");

  sensor_msgs::PointCloud2Iterator<float> iter_x(*points_msg, "x");
  sensor_msgs::PointCloud2Iterator<float> iter_y(*points_msg, "y");
  sensor_msgs::PointCloud2Iterator<float> iter_z(*points_msg, "z");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_r(*points_msg, "r");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_g(*points_msg, "g");
  sensor_msgs::PointCloud2Iterator<uint8_t> iter_b(*points_msg, "b");

  for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
    {
      for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z, ++iter_r, ++iter_g, ++iter_b)
      {
        if(frame.at<float>(Point(i,j))==1)
        {
        // TODO fill in x, y, z, r, g, b local variables
        *iter_x = (j-(cols/2))*x_pix;
        *iter_y = (rows-i)*y_pix+blind_spot;
        *iter_z = height_of_cam_from_ground;
        *iter_r = 255;
        *iter_g = 255;
        *iter_b = 255;
        }
        else
        {
          *iter_x = (j-(cols/2))*x_pix;
          *iter_y = (rows-i)*y_pix+blind_spot;
          *iter_z = height_of_cam_from_ground;
          *iter_i = 0 ;
        }
      }
    }
  //
  // PointCloud::Ptr msg (new PointCloud);
  // msg->header.frame_id = "camera";
  // msg->height = rows*y_pix;
  // msg->width = cols*x_pix;
  //
  // for(int i=0;i<rows;i++)
  //   for(int j=0;j<cols;j++)
  //   {
  //     if(frame.at<float>(Point(i,j))==1)
  //     {
  //       y= (rows-i)*y_pix+blind_spot;
  //       x= (j-(cols/2))*x_pix;
  //       msg->points.push_back (pcl::PointXYZ(x, y, height_of_cam_from_ground));
  //     }
  //   }

  ros::Rate loop_rate(4);
  while (nh.ok())
  {
    msg->header.stamp = ros::Time::now().toNSec();
    pub.publish (points_msg);
    ros::spinOnce ();
    loop_rate.sleep ();
  }
}
