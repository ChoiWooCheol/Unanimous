#include<ros/ros.h>
#include<geometry_msgs/PoseWithCovarianceStamped.h>
#include<cstdlib>
int main(int argc, char** argv)
{
	if(argc < 5) {
		ROS_ERROR("set argv1 argv2 argv3 argv4!");
		ROS_ERROR("argv[1] = x(position) ");
		ROS_ERROR("argv[2] = y(position) ");
		ROS_ERROR("argv[3] = z(orientation)");
		ROS_ERROR("argv[4] = w(orientation)");
		return 0;		
	}
	double x, y, z, w;
	x = atof(argv[1]);
	y = atof(argv[2]);
	z = atof(argv[3]);
	w = atof(argv[4]);
	ROS_INFO("x = %f", x);
	ROS_INFO("y = %f", y);
	ROS_INFO("z = %f", z);
	ROS_INFO("w = %f", w);
	ros::init(argc, argv, "fake_init");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 1);
	int pub_count = 0;
	ros::Rate loop_rate(1);
	while(ros::ok()){
		if(pub_count == 2) break;
		geometry_msgs::PoseWithCovarianceStamped msg;
		msg.header.seq = 0;
		msg.header.stamp = ros::Time::now();
		msg.header.frame_id = "map";

		msg.pose.pose.position.x = x;
		msg.pose.pose.position.y = y;
		msg.pose.pose.position.z = 0.0;
		msg.pose.pose.orientation.x = 0.0;
		msg.pose.pose.orientation.y = 0.0;
		msg.pose.pose.orientation.z = z;
		msg.pose.pose.orientation.w = w;
		pub.publish(msg);
		pub_count++;
		loop_rate.sleep();
		ROS_INFO("count = %d", pub_count);
		
	}
	ROS_INFO("sending fake initalpose( /initalpose )...");
	ROS_INFO("done!");
	return 0;
}
