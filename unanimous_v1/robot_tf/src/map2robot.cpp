#include<ros/ros.h>
#include<tf/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>

class RobotTF
{
public:
    RobotTF() {
        odom_sub = nh.subscribe("/odom", 1, &RobotTF::Tf_Callback, this);
    }
    void Tf_Callback(const nav_msgs::Odometry::ConstPtr& ptr){
        double x, y, theta;

        theta = ptr->pose.pose.orientation.z;
        x = ptr->pose.pose.position.x;
        y = ptr->pose.pose.position.y;

        tf::Transform transform;
        transform.setOrigin(tf::Vector3(x, y, 0.0));
        tf::Quaternion q;
        q.setRPY(0, 0, theta);
        transform.setRotation(q);

        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "map", "odom"));
    }
private:
    ros::NodeHandle nh;
    tf::TransformBroadcaster br;
    ros::Subscriber odom_sub;
    
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "map2robot");
    RobotTF R;
 
    return 0;
}