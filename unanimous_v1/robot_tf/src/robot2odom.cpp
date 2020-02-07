#include<ros/ros.h>
#include<nav_msgs/Odometry.h>
#include<tf/transform_broadcaster.h>
#include<string>
#include<cmath>
#include"unanimous_msg/raspberry_msg.h"
#include<geometry_msgs/Twist.h>
#include<vector>
#include<iostream>

#define radi 3.14159265359 / 180

using namespace std;

class UnaniOdom{
public:
    UnaniOdom():seq(0),x(0.1), y(0.0), theta(0.0), last(ros::Time::now()){
        sub = nh.subscribe("/wheel_topic", 100, &UnaniOdom::calcVelocityCB, this);
        odom_sub = nh.subscribe("/match_odom", 100, &UnaniOdom::pubTF, this);
        odomPub = nh.advertise<nav_msgs::Odometry>("/odom",100);
        base_link_id = "base_link";
        odom_link_id = "odom";
    }

    void calcVelocityCB(const unanimous_msg::raspberry_msg::ConstPtr& ptr){
        Vl  = ptr->left_vel;
        Vr = ptr->right_vel;
        V  = (Vl + Vr) / 2.0;
    }

    void boradcastTransform(){

        tf::Transform transform;
        transform.setOrigin(tf::Vector3(x,y,0.0));
        tf::Quaternion q;
        q.setRPY(0, 0, theta);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), odom_link_id.c_str(), base_link_id.c_str())); // linking base_link
    }

    void pubTF(const nav_msgs::Odometry::ConstPtr& ptr){
        theta = ptr->pose.pose.orientation.z;
        x = ptr->pose.pose.position.x;
        y = ptr->pose.pose.position.y;

        nav_msgs::Odometry odom;
        odom.header.seq             = seq++;
        odom.header.stamp           = ptr->header.stamp;
        odom.header.frame_id        = odom_link_id;
        odom.child_frame_id         = base_link_id;
        odom.pose.pose.position.x   = x;
        odom.pose.pose.position.y   = y;
        odom.pose.pose.position.z   = ptr->pose.pose.position.z;
        odom.pose.pose.orientation  = tf::createQuaternionMsgFromYaw(theta);
        odom.twist.twist.linear.x   = ptr->twist.twist.linear.x;
        odom.twist.twist.linear.y   = ptr->twist.twist.linear.y;
        odom.twist.twist.angular.z  = ptr->twist.twist.angular.z;

        odomPub.publish(odom);
        boradcastTransform();
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber sub, odom_sub;
    ros::Publisher odomPub;
    ros::Time last;
    string base_link_id, odom_link_id;
    double x, y, theta;
    int seq;
    tf::TransformBroadcaster br;
    double Vl, Vr, V;
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "robot2odom");
    UnaniOdom Odom;
    ros::spin();

    return 0;
}