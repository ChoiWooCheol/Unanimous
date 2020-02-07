#include<ros/ros.h>
#include<wiringPi.h>
#include<softPwm.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>
#include"unanimous_msg/raspberry_msg.h"
#include<iostream>

#define ENA 29
#define IN1 28
#define IN2 27
#define IN3 25
#define IN4 24
#define ENB 23

class MotorTest{
public:
    MotorTest(){
        sub = nh.subscribe("wheel_topic", 10, &MotorTest::Motor_Callback, this);
        if(wiringPiSetup() == -1){ ROS_ERROR("WIRING_PI_SETUP ERROR!!"); exit(1); }
        Initial_Setting();

    }

    void Initial_Setting(){
        pinMode(ENA, OUTPUT);
        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);
        pinMode(IN3, OUTPUT);
        pinMode(IN4, OUTPUT);
        pinMode(ENB, OUTPUT);
        softPwmCreate(ENA, 0, 255);
        softPwmCreate(ENB, 0, 255);
    }

    void Motor_Callback(const unanimous_msg::raspberry_msg::ConstPtr& ptr){
        digitalWrite(IN1, 0); // left wheel
        digitalWrite(IN2, 1); // left wheel
        digitalWrite(IN3, 1); // right wheel
        digitalWrite(IN4, 0); // right wheel
        softPwmWrite(ENA, ptr->left_vel); // left wheel speed
        softPwmWrite(ENB, ptr->right_vel); // right wheel speed
    }

    void E_stop(){
        softPwmWrite(ENA, 0);
        softPwmWrite(ENB, 0);
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber sub;
    
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "motor");
    MotorTest mt;

    ros::spin();
    return 0;
}