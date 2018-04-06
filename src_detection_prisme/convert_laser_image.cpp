#include "ros/ros.h"
#include "std_msgs/String.h"
#include <math.h>
#include <iostream>
#include "std_msgs/Bool.h"
#include <sstream>
#include "sensor_msgs/LaserScan.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector> 
//#include <utility>
#include <time.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace cv;

using namespace std;


vector<double> dist;


void chatterCallback(const sensor_msgs::LaserScanConstPtr& msg){
	dist.clear();
	for(int i=0; i<msg->ranges.size(); i++){
		if(msg->ranges[i] > 20)
			dist.push_back(0);
		else
			dist.push_back(msg->ranges[i]);
	}
}

int main(int argc, char **argv){

	//namedWindow("Video", CV_WINDOW_AUTOSIZE);
	
	ros::init(argc, argv, "convert_laser_image");
	ros::NodeHandle n;
	
	ros::Subscriber sub = n.subscribe("/front/scan", 1, chatterCallback);

	ros::Rate loop_rate(10);
	vector<Point> pose;	
	double x, y;
	int xpixel, ypixel;
	
	float j = -2.3561899662;
	vector<double> angle;
	float pas = 0.0065540750511;
	
	Mat imgprec2;
	imgprec2 = Mat::zeros(800, 800, CV_8U);

	while (ros::ok()){
		
		
		//imshow("Video", imgprec2);
	
		ros::spinOnce();
		
		//cout << "taille : " << dist.size() << endl;

		for(int i=0; i<dist.size(); i++){
			angle.push_back(j);
			j = j + pas;
			//cout << "dist : " << dist[i];
			if(dist[i] > 0 && dist[i] < 1*10^2){
				x = dist[i]*cos(angle[i]);
				xpixel = int(x*100) + 400;
				y = dist[i]*sin(angle[i]);
				ypixel = int(y*100) + 400;
				pose.push_back(Point(xpixel,ypixel));
				//cout << "x : " << pose[i].x;
				//cout << "y : " << pose[i].y;
				//cout << "xcalcul : " << x << endl;
				//imgprec2.at<uchar>(pose[i].x, pose[i].y) == 255 ;
				  circle( imgprec2,
      pose[i],
      10,
      Scalar( 0, 255, 0 ),
      -1 );
			}
			// creer tableau de type point avec opencv
		}
		
		imshow("Video 2", imgprec2);	
		
		cout << "/////////////" << endl << endl;
		
		
		// Creer une image vide noir
		// on remplis de pixels blancs l'image correspodant au points
		// Avec opencv, fair une analyse de cette image pour detecter un triangle de géométrie connue
		// Donner sa positon en x,y (par exemple la millieu du triangle isocele)
		// Convertir cette position comme si x=0 et y=O etait au centre de l'image
		// On obtient donc la distance en x,y entre le prisme et le robot, avec le robot comme repere de base
		// Creer un topic et publié dessus ses distances
		
		//for(int i=dist.size()-1; i>= 0; i--){
			 
		//}
		waitKey(0);
		
		//a finir, laura
	}

	 destroyAllWindows();
    sleep(1);
	loop_rate.sleep();
	
	return 1;

}














