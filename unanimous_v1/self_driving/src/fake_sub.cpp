#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/PoseWithCovarianceStamped.h>


void callback(const geometry_msgs::PoseStamped::ConstPtr& ptr){
	ROS_INFO("============goalpose============");
	ROS_INFO("x : %f", ptr->pose.position.x);
	ROS_INFO("y : %f", ptr->pose.position.y);
	ROS_INFO("z : %f", ptr->pose.orientation.z);
	ROS_INFO("w : %f", ptr->pose.orientation.w);
}

void callback2(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& ptr2){
	ROS_INFO("============initialpose============");
	ROS_INFO("x : %f", ptr2->pose.pose.position.x);
	ROS_INFO("y : %f", ptr2->pose.pose.position.y);
	ROS_INFO("z : %f", ptr2->pose.pose.orientation.z);
	ROS_INFO("w : %f", ptr2->pose.pose.orientation.w);
}


int main(int argc, char** argv){
	ros::init(argc, argv, "fake_sub");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/move_base_simple/goal", 1, &callback);	
	ros::Subscriber sub2 = nh.subscribe("/initialpose", 1, &callback2);	
	ros::spin();
	return 0;
}
