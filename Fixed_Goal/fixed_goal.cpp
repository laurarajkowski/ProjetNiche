#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
 
	// on crée le node fixed_goal

  ros::init(argc, argv, "fixed_goal"); 

  ros::NodeHandle n; 

	// ce node va être en mode publisher, il va publier sur le topic "move_base_simple/goal"
	// la message est de type "geometry_msgs::PoseStamped"
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);

  ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    
		// Variable que l'on va envoyer au topic ci-dessus
    geometry_msgs::PoseStamped pose_to_send;	
		
		

    ROS_INFO("%s", msg.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
