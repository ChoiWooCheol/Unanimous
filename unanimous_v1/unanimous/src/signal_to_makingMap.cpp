#include<ros/ros.h>
#include"unanimous_msg/signal_msg.h"

int main(int argc, char** argv){
    ros::init(argc, argv, "signal");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<unanimous_msg::signal_msg>("signal_topic", 10);
    unanimous_msg::signal_msg msg;
    msg.value = true;
    while(ros::ok()){
        pub.publish(msg);
    }

    return 0;
}
