#include<ros/ros.h>
#include"unanimous_msg/raspberry_msg.h"
#include<geometry_msgs/Twist.h>
#include<iostream>

using namespace std;


class TransVel{
public:
    TransVel() : trans_angular(65.0), trans_linear(65.0) {
        pub = nh.advertise<unanimous_msg::raspberry_msg>("wheel_topic", 10);
        sub = nh.subscribe("/cmd_vel", 10, &TransVel::telopCallback, this);
        //if(!nh.param("/velocity_param/trans_angular", trans_angular, 100.0)) throw std::runtime_error("set trans_angular");
        //if(!nh.param("/velocity_param/trans_linear", trans_linear, 50.0))   throw std::runtime_error("set trans_linear");
    }

    void telopCallback(const geometry_msgs::Twist::ConstPtr& ptr){
        unanimous_msg::raspberry_msg msg;
        double left = 0, right = 0;
        if(ptr->angular.z != 0){
            left = ptr->angular.z * -(trans_angular);
            right = ptr->angular.z * trans_angular;
        }

        left = left + ptr->linear.x * trans_linear;
        right = right + ptr->linear.x * trans_linear;

        msg.left_vel = left;
        msg.right_vel = right;
        pub.publish(msg);
    }

private:
    ros::NodeHandle nh;
    ros::Publisher pub;
    ros::Subscriber sub;
    double trans_angular, trans_linear;
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "trans_velocity");
    TransVel Tvel;
    ros::spin();
    return 0;
}
