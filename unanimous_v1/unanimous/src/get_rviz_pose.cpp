#include<ros/ros.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/PoseWithCovarianceStamped.h>

void goalPoseCB(const geometry_msgs::PoseStamped::ConstPtr& ptr){
    ROS_INFO("====================2D Nav Goal====================");
    ROS_INFO("goalPose x (position)    = %f", ptr->pose.position.x);
    ROS_INFO("goalPose y (position)    = %f", ptr->pose.position.y);
    ROS_INFO("goalPose z (orientation) = %f", ptr->pose.orientation.z);
    ROS_INFO("goalPose w (orientation) = %f", ptr->pose.orientation.w);
    ROS_INFO("===================================================");
}

void initalPoseCB(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& ptr){
    ROS_INFO("=====================InitialPose=====================");
    ROS_INFO("initPose x (position)    = %f", ptr->pose.pose.position.x);
    ROS_INFO("initPose y (position)    = %f", ptr->pose.pose.position.y);
    ROS_INFO("initPose z (orientation) = %f", ptr->pose.pose.orientation.z);
    ROS_INFO("initPose w (orientation) = %f", ptr->pose.pose.orientation.w);
    ROS_INFO("=====================================================");
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "get_rviz_pose");
    ros::NodeHandle nh;
    ros::Subscriber goal_sub = nh.subscribe("/move_base_simple/goal", 1, &goalPoseCB);
    ros::Subscriber init_sub = nh.subscribe("/initialpose", 1, &initalPoseCB);
    ros::spin();
    return 0;
}