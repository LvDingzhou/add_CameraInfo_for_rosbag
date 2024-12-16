cmake_minimum_required(VERSION 3.0.2)
project(camera_info)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  rosbag
)
find_package(OpenCV REQUIRED)


catkin_package(

)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_LIBRARIES}
)

add_executable(camera_info cam_info_node.cpp)
target_link_libraries(camera_info ${catkin_LIBRARIES}  ${OpenCV_LIBRARIES} )

