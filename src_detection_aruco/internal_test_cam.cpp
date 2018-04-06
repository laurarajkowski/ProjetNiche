#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

class cam_test{
	public:
		cam_test(){

			//On capture l'image de la caméra
   			VideoCapture cap(CV_CAP_ANY);
         	if(!cap.isOpened()){
	   			cout << "cannot open the video cam" << endl;
	  		}
	  
			//On récupère les dimensions de l'image
 	 		double dwidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	  		double dweigth = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	  		cout << "Frame size : " << dwidth << " x " << dweigth << endl;

	  		namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);

			//On initialise les matrices de calibration de la caméra. Ces matrices sont calculées préamlablement 
			//à l'aide d'un node présent dans ROS. 
	  		Mat cameraMatrix, distCoeffs;
	  		cameraMatrix = (Mat_<float>(3,3)<<513.216209, 0, 326.382673, 0, 512.924656, 247.839066, 0, 0, 1);
	  		distCoeffs = (Mat_<float>(1,5)<<0.09482299999999999, -0.183796, 0.000904, 0.003045, 0);

	  		float markerLength = 2.3; //Longueur du code aruco utilisé (en centimetre)
	  		float xdiff; //Différence sur l'axe x entre le centre de l'image et la position du code aruco.
	  
			//On génère la bibliothèque des codes aruco de type 6x6 250.
      	 	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);

	  		while(1){

	     		Mat image, imageCopy;
	     		bool bSuccess = cap.read(image);

	     		if(!bSuccess){
	        		cout << "cannot read a frame from video stream" << endl;
		 			break;
	     		}

	     		image.copyTo(imageCopy); //On copie l'image filmée, c'est sur cette copie qu'on fera les traitements	
	
				//Initialisation de paramètres
            	vector<int> ids;
    	    	vector<vector<Point2f> > corners;
    	    	aruco::detectMarkers(image, dictionary, corners, ids);

	     		if (ids.size() > 0){

                	aruco::drawDetectedMarkers(imageCopy, corners, ids);//Fonction qui détecte les codes aruco
   					vector<Vec3d> rvecs, tvecs; //Initialisation des vecteurs qui vont nous permettre de récupérer la position du marker
					aruco::estimatePoseSingleMarkers(corners, markerLength, cameraMatrix, distCoeffs, rvecs, tvecs);

					for(int i=0; i<ids.size(); i++){
						aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 20);
						xdiff = tvecs[i][0];//Le premier élements correspond à la position selon x. Sachant que x=O est au centre de l'image.
						cout << "xdiff: "<< xdiff << endl;
					}
	    		}

            	imshow("MyVideo", imageCopy);
	     		if (waitKey(30) == 27){	
					cout << "key problem" << endl;
					break;
	     		}
	  		}
		}

		~cam_test(){
		cvDestroyWindow("Camera_Output");
	}
};


int main(int argc, char** argv){

	ros::init(argc, argv, "internal_test_cam");
 	cam_test cam_object;
	ROS_INFO("cam tested");
 	return 0;
}

