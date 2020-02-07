#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
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
	ros::init(argc, argv, "fake_goal");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1);
	ROS_INFO("x = %f", x);
	ROS_INFO("y = %f", y);
	ROS_INFO("z = %f", z);
	ROS_INFO("w = %f", w);
	int pub_count = 0;
	ros::Rate loop_rate(1);
	while(ros::ok()){
		if(pub_count == 2) break;
		geometry_msgs::PoseStamped msg;
		msg.header.seq = 0;
		msg.header.stamp = ros::Time::now();
		msg.header.frame_id = "map";

		msg.pose.position.x = x;
		msg.pose.position.y = y;
		msg.pose.position.z = 0.0;
		msg.pose.orientation.x = 0.0;
		msg.pose.orientation.y = 0.0;
		msg.pose.orientation.z = z;
		msg.pose.orientation.w = w;
		pub.publish(msg);
		pub_count++;
		loop_rate.sleep();
		ROS_INFO("count = %d", pub_count);
		
	}
	ROS_INFO("sending fake goal( /move_base_simple/goal )...");
	ROS_INFO("done!");
	return 0;
}
