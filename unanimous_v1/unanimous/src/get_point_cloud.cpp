#include<ros/ros.h>
#include<sensor_msgs/PointCloud.h>
#include<geometry_msgs/Point.h>
#include"unanimous_msg/signal_msg.h"
#include<vector>
#include<set>
#include<fstream>
#include<algorithm>
#include<string>

using namespace std;

class Point_Data{
public:
    int x;
    int y;
    int z;

    bool operator<(const Point_Data& pd) const {
        if(x < pd.x || y < pd.y || z < pd.z) return true;
        else return false;
    }

     bool operator>(const Point_Data& pd) const {
        if(x > pd.x || y > pd.y || z > pd.z) return true;
        else return false;
    }
};

// int binarySearch(vector< Point_Data >& pvec, int l, int r, Point_Data pd) { // 매개변수 : 배열이름, 배열 시작 인덱스, 배열 끝 인덱스, 찾으려는 값 
//    if (r >= l)
//    { 
//         int mid = l + (r - l)/2; // 중간 값 선택
//         if (pvec[mid] == pd) return mid; // 해당 인덱스 반환
//         if (pvec[mid] > pd) return binarySearch(pvec, l, mid-1, pd); // 좌측 배열 탐색
//         return binarySearch(pvec, mid+1, r, pd); // 우측 배열 탐색
//    }
//    return -1;
// }


class Get_Point{
public:
    Get_Point(){
        point_sub = nh.subscribe("slam_cloud", 1000 , &Get_Point::Point_Data_CallBack, this);
        point_map_server = nh.subscribe("signal_topic", 10, &Get_Point::make_point_file, this);
        total_points = 0;
        recive_count = 0;

        if(!nh.getParam("get_point_cloud/sampling_number", sampling_number)) throw std::runtime_error("give me a sampling_number!");
        if(!nh.getParam("get_point_cloud/path", path))                       throw std::runtime_error("give me a path!");

        state_print();
    }

    void state_print(){
        ROS_INFO("--------------------------------------------------");
        ROS_INFO("sampling_number   =   %d", sampling_number);
        ROS_INFO("path              =   %s", path.c_str());
        ROS_INFO("--------------------------------------------------");
    }

    void Down_Sapmling(set<Point_Data>& pset, vector<Point_Data> pvec, int& total_point_count){
        vector<Point_Data> clear_vec;
        set<Point_Data> clear_set;

        for(int i = 0; i < pvec.size(); ++i){
            pset.insert(pvec[i]);
        }
//-------------------------------- STL MEMORY CLEAR ---------------------------------//        
        pvec.swap(clear_vec);
        clear_vec.clear();
        pvec.clear();
//-----------------------------------------------------------------------------------//


        Down_Sampling_vec.assign(pset.begin(), pset.end());


//-------------------------------- STL MEMORY CLEAR ---------------------------------//
        pset.swap(clear_set);
        clear_set.clear();
        pset.clear();
//-----------------------------------------------------------------------------------//

        for(int i = 0; i < Down_Sampling_vec.size(); ++i){
            pset.insert(Down_Sampling_vec[i]);
        }

//-------------------------------- STL MEMORY CLEAR ---------------------------------//
        Down_Sampling_vec.swap(clear_vec);
        clear_vec.clear();
        Down_Sampling_vec.clear();
//-----------------------------------------------------------------------------------//


        Down_Sampling_vec.assign(pset.begin(), pset.end());

        pset.swap(clear_set);
        clear_set.clear();
        pset.clear();

        ROS_INFO("Down_Sampling_vec = %d", Down_Sampling_vec.size());
        int Down_Sampling_count = Down_Sampling_vec.size();
        total_point_count = Down_Sampling_count;
    }

    void Point_Data_CallBack(const sensor_msgs::PointCloud::ConstPtr& ptr){
        int x_, y_, z_;
        int point_count = ptr->points.size();
        for(int i = 0; i < point_count; ++i){
//------------------------------------------------------------------------------------//
            x_ = ptr->points[i].x * sampling_number;
            y_ = ptr->points[i].y * sampling_number;
            z_ = ptr->points[i].z * sampling_number;
            PD.x = x_; 
            PD.y = y_;
            PD.z = z_;
//-------------down sampling & const variable -> nomal variable-----------------------//

            point_vec.push_back(PD);
        }
        //total_vec.push_back(point_vec);
        //point_vec.resize(0);
    }

    void Making_Fixed_sample(){
        int sampling_count = point_vec.size();;
        int count = 0, fix_vec_size;
        
        while(1){
            if(count == 0){
                Down_Sapmling(point_set, point_vec, total_points);
                if(sampling_count == Down_Sampling_vec.size()){
                    break;
                }
                sampling_count = Down_Sampling_vec.size();
            }
            else{
                Down_Sapmling(point_set, Down_Sampling_vec, total_points);
                if(sampling_count == Down_Sampling_vec.size()){
                    break;
                }
                sampling_count = Down_Sampling_vec.size();
            }
            count++;
        }
        fix_vec_size = Down_Sampling_vec.size();
        int index;
        for(int i = 0; i < fix_vec_size; ++i){
            for(int j = i + 1; j < fix_vec_size; ++j){
                if(Down_Sampling_vec[i].x == Down_Sampling_vec[j].x && Down_Sampling_vec[i].y == Down_Sampling_vec[j].y){
                    Down_Sampling_vec.erase(Down_Sampling_vec.begin() + j);
                    --j;
                    --total_points;
                    --fix_vec_size;
                }
            }
        }
        ROS_INFO("FIXED_SAMPLE_SIZE = %d", total_points);
        
    }

    void make_point_file(const unanimous_msg::signal_msg::ConstPtr& msg){
        if(recive_count == 0){
            ROS_INFO("start map making...");
            ROS_INFO("point_vec = %d", point_vec.size());
            Making_Fixed_sample();
            ofstream of(path.c_str());
            of << "POINTS " << total_points << endl;
            for(int i = 0; i < Down_Sampling_vec.size(); ++i){
                of << static_cast<float>(Down_Sampling_vec[i].x) / sampling_number << " ";
                of << static_cast<float>(Down_Sampling_vec[i].y) / sampling_number << " ";
                of << static_cast<float>(Down_Sampling_vec[i].z) / sampling_number << endl;
            }
            ROS_INFO("DONE!");
            of.close();
            recive_count++;
        }
    }
    
private:
    ros::NodeHandle nh;
    ros::Subscriber point_sub;
    ros::Subscriber point_map_server;
    set<Point_Data> point_set;
    vector<Point_Data> point_vec;
    vector<Point_Data> Down_Sampling_vec;
    Point_Data PD;
    int total_points;
    int recive_count;
    int sampling_number;
    string path;
};


int main(int argc, char** argv){
    ros::init(argc, argv, "get_point_cloud");	
    Get_Point GP;
    ROS_INFO("wating 'rosrun map_server map_saver ...'");
	ros::spin();
    return 0;
}