# Unanimous
 * 숭실대학교 스마트시스템소프트웨어학과 재학생끼리 계획한 통합실내지도 솔루션을 제공하기위한 프로젝트 입니다.

# Unanimous v1
 * unanimous 초기버전
 * rtabmap_ros, realsense_ros 패키지와 라이브러리가 추가로 필요하다.
 
## Hardware
 * raspberry pi 4
 * turtle - 2wd mobile platform
 * Li-op battery
 * realsense D435
 * rplidar a1
 
## Explanation (요약)
 1. 2륜 mobile robot을 이용하여 자율주행 시스템을 탑재한다.
 2. 자율주행 시스템을 위한 slam알고리즘 구동을 위한 센서들을 부착한다.
 3. 로봇에 장착된 realsense D435를 이용하여 point cloud map을 만든다.
 4. 모든 센서와 로봇간의 TF정보를 설정하고, odometry를 이용하여 값을 보정한다.
 5. 최종적으로 만들어진 point cloud map을 사용자들에게 제공하고, 경로안내 시스템을 구현한다.
 
## Result


# Unanimous v2
```sh
$ roslaunch lidar_detect qt_detect_launch.launch --screen
``` 
# Reference
https://www.youtube.com/watch?v=rWDjQ4hLrtE&t=14s
