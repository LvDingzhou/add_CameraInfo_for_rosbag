#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

// 获取左相机的CameraInfo
sensor_msgs::CameraInfo getCameraInfoLeft(const sensor_msgs::Image::ConstPtr& image_msg) {
    sensor_msgs::CameraInfo cam;

    std::vector<double> D{-0.288105327549552, 0.074578284234601, 7.784489598138802e-04, -2.277853975035461e-04, 0.000000};
    boost::array<double, 9> K = {4.250258563372763e+02, 0.000000, 3.860151866550880e+02,
                                 0.000000, 4.267976260903337e+02, 2.419130336743440e+02,
                                 0.000000, 0.000000, 1.000000};

    boost::array<double, 12> P = {4.250258563372763e+02, 0.000000, 3.860151866550880e+02, 0.000000,
                                  0.000000, 4.267976260903337e+02, 2.419130336743440e+02, 0.000000,
                                  0.000000, 0.000000, 1.000000, 0.000000};

    boost::array<double, 9> R = {1, 0, 0, 0, 1, 0, 0, 0, 1};

    cam.width = 752;
    cam.height = 480;
    cam.distortion_model = "plumb_bob";
    cam.D = D;
    cam.K = K;
    cam.P = P;
    cam.R = R;
    cam.binning_x = 0;
    cam.binning_y = 0;
    cam.header.frame_id = "camera";
    cam.header.stamp = image_msg->header.stamp;

    return cam;
}

// 获取右相机的CameraInfo
sensor_msgs::CameraInfo getCameraInfoRight(const sensor_msgs::Image::ConstPtr& image_msg) {
    sensor_msgs::CameraInfo cam;

    std::vector<double> D{-0.300267420221178, 0.090544063693053, 3.330220891093334e-05, 8.989607188457415e-05, 0};
    boost::array<double, 9> K = {4.313364265799752e+02, 0.000000, 3.548956286992647e+02,
                                 0.000000, 4.327527965378035e+02, 2.325508916495161e+02,
                                 0.000000, 0.000000, 1.000000};

    boost::array<double, 12> P = {4.313364265799752e+02, 0.000000, 3.548956286992647e+02, -112.14,
                                  0.000000, 4.327527965378035e+02, 2.325508916495161e+02, 0.000000,
                                  0.000000, 0.000000, 1, 0.000000};

    boost::array<double, 9> R = {0.9997, 0.0094, 0.0225, -0.0094, 1, 0.000792, -0.0225, -0.001, 0.9997};

    cam.width = 752;
    cam.height = 480;
    cam.distortion_model = "plumb_bob";
    cam.D = D;
    cam.K = K;
    cam.P = P;
    cam.R = R;
    cam.binning_x = 0;
    cam.binning_y = 0;
    cam.header.frame_id = "camera";
    cam.header.stamp = image_msg->header.stamp;

    return cam;
}

int main(int argc, char** argv) {
    // 初始化ROS节点
    ros::init(argc, argv, "camera_info_writer");
    ros::NodeHandle nh;

    // 打开输入rosbag文件进行读取
    rosbag::Bag input_bag;
    input_bag.open("nya_01.bag", rosbag::bagmode::Read);

    // 打开输出rosbag文件进行写入
    rosbag::Bag output_bag;
    output_bag.open("Nnya_01_with_camera_info.bag", rosbag::bagmode::Write);

    // 创建视图来读取所有话题
    rosbag::View view(input_bag);

    // 遍历rosbag消息
    for (rosbag::MessageInstance const m : view) {
        // 检查是否是左相机图像
        // if (m.getTopic() == "/NTU/left/image_raw") {
        if (m.getTopic() == "/left/image_raw") { //如果没有改rostopic，请取消注释，并注释上一行
            sensor_msgs::Image::ConstPtr img_msg = m.instantiate<sensor_msgs::Image>();
            if (img_msg != nullptr) {
                sensor_msgs::CameraInfo left_info_msg = getCameraInfoLeft(img_msg);
                output_bag.write("/NTU/left/camera_info", img_msg->header.stamp, left_info_msg);
                output_bag.write("/NTU/left/image_raw", img_msg->header.stamp, img_msg); //如果没有改rostopic，请取消注释
                output_bag.write(m.getTopic(), img_msg->header.stamp, img_msg);
            }
        }
        // 检查是否是右相机图像
        // else if (m.getTopic() == "/NTU/right/image_raw") {
        else if (m.getTopic() == "/right/image_raw") { //如果没有改rostopic，请取消注释，并注释上一行
            sensor_msgs::Image::ConstPtr img_msg = m.instantiate<sensor_msgs::Image>();
            if (img_msg != nullptr) {
                sensor_msgs::CameraInfo right_info_msg = getCameraInfoRight(img_msg);
                output_bag.write("/NTU/right/camera_info", img_msg->header.stamp, right_info_msg);
                output_bag.write("/NTU/right/image_raw", img_msg->header.stamp, img_msg); //如果没有改rostopic，请取消注释
                output_bag.write(m.getTopic(), img_msg->header.stamp, img_msg);
            }
        }
        // 如果不是图像消息，直接写入输出bag
        else {
            output_bag.write(m.getTopic(), m.getTime(), m);
        }
    }

    // 关闭rosbag文件
    input_bag.close();
    output_bag.close();

    return 0;
}
