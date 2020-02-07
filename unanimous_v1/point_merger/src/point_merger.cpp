#include<ros/ros.h>
#include<sensor_msgs/PointCloud2.h>
#include<vector>
#include<pcl_conversions/pcl_conversions.h>
#include<pcl/point_types.h>
#include<cmath>
#include<string>


class PointMerger{
public:
    PointMerger() : count(0){
        point_sub = nh.subscribe("/rtabmap/cloud_map", 10000, &PointMerger::pointCallback, this);
        merged_pub = nh.advertise<sensor_msgs::PointCloud2>("/merged_point", 10000);
    }

    void pointCallback(const sensor_msgs::PointCloud2::ConstPtr& in_points){
        ROS_INFO(".");
        count++;
        sensor_msgs::PointCloud2 merged_msg;
        pcl::PointCloud<pcl::PointXYZRGB> scan;
        pcl::fromROSMsg(*in_points, scan);
        if(count == 1){
             pcl::fromROSMsg(*in_points, mergerd_scan);
        }
        mergerd_scan += scan;
        if(count == 5){
            count = 0;
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr scan_ptr(new pcl::PointCloud<pcl::PointXYZRGB>(mergerd_scan));
            pcl::toROSMsg(*scan_ptr, merged_msg);
            merged_pub.publish(merged_msg);
        }
        
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber point_sub;
    ros::Publisher merged_pub;
    pcl::PointCloud<pcl::PointXYZRGB> mergerd_scan;
    int count;
    
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "point_merger");
    PointMerger pm;
    ros::spin();
    return 0;
}