#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.hpp"
#include "opencv2/opencv.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

using namespace std::chrono_literals;

class ColorDetector : public rclcpp::Node
{
public:
  ColorDetector()
  : Node("color_detector")
  {
    declare_parameter("debug", debug_);
    get_parameter("debug", debug_);

    declare_parameter("min_positive", min_positive);
    get_parameter("min_positive", min_positive);

    declare_parameter("red_hsv_ranges", red_hsv_filter_ranges_);
    get_parameter("red_hsv_ranges", red_hsv_filter_ranges_);

    declare_parameter("green_hsv_ranges", green_hsv_filter_ranges_);
    get_parameter("green_hsv_ranges", green_hsv_filter_ranges_);

    declare_parameter("blue_hsv_ranges", blue_hsv_filter_ranges_);
    get_parameter("blue_hsv_ranges", blue_hsv_filter_ranges_);

    image_sub_ = image_transport::create_subscription(
      this, "input_image", std::bind(&ColorDetector::image_callback, this, _1), "compressed",
      rclcpp::SensorDataQoS().get_rmw_qos_profile());
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  image_transport::Subscriber image_sub_;

  // HSV ranges for detection [h,s,v H,S,V]
  std::vector<double> red_hsv_filter_ranges_{0, 0, 0, 180, 255, 255};
  std::vector<double> green_hsv_filter_ranges_{0, 0, 0, 180, 255, 255};
  std::vector<double> blue_hsv_filter_ranges_{0, 0, 0, 180, 255, 255};
  // number of pixels required to classify a correct color detection
  uint8_t min_positive = 200;
  bool debug_ = false;

  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr & msg)
  {
    const float & red_h = red_hsv_filter_ranges_[0];
    const float & red_s = red_hsv_filter_ranges_[1];
    const float & red_v = red_hsv_filter_ranges_[2];
    const float & red_H = red_hsv_filter_ranges_[3];
    const float & red_S = red_hsv_filter_ranges_[4];
    const float & red_V = red_hsv_filter_ranges_[5];

    const float & green_h = green_hsv_filter_ranges_[0];
    const float & green_s = green_hsv_filter_ranges_[1];
    const float & green_v = green_hsv_filter_ranges_[2];
    const float & green_H = green_hsv_filter_ranges_[3];
    const float & green_S = green_hsv_filter_ranges_[4];
    const float & green_V = green_hsv_filter_ranges_[5];

    const float & blue_h = blue_hsv_filter_ranges_[0];
    const float & blue_s = blue_hsv_filter_ranges_[1];
    const float & blue_v = blue_hsv_filter_ranges_[2];
    const float & blue_H = blue_hsv_filter_ranges_[3];
    const float & blue_S = blue_hsv_filter_ranges_[4];
    const float & blue_V = blue_hsv_filter_ranges_[5];

    cv_bridge::CvImagePtr cv_ptr;
    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    } catch (cv_bridge::Exception & e) {
      RCLCPP_ERROR(get_logger(), "cv_bridge exception: %s", e.what());
    }
    ;

    cv::Mat img_hsv;
    cv::cvtColor(cv_ptr->image, img_hsv, cv::COLOR_BGR2HSV);

    cv::Mat1b filtered;

    // RED color detection
    cv::inRange(
      img_hsv, cv::Scalar(red_h, red_s, red_v), cv::Scalar(red_H, red_S, red_V), filtered);
    auto numPositive = cv::countNonZero(filtered);
    if (numPositive >= min_positive) {
      RCLCPP_INFO(this->get_logger(), "Detected red: %d", min_positive);
      return;
    }

    // GREEN color detection
    cv::inRange(
      img_hsv, cv::Scalar(green_h, green_s, green_v), cv::Scalar(green_H, green_S, green_V),
      filtered);
    numPositive = cv::countNonZero(filtered);
    if (numPositive >= min_positive) {
      RCLCPP_INFO(this->get_logger(), "Detected green: %d", min_positive);
      return;
    }

    // BLUE color detection
    cv::inRange(
      img_hsv, cv::Scalar(blue_h, blue_s, blue_v), cv::Scalar(blue_H, blue_S, blue_V), filtered);
    numPositive = cv::countNonZero(filtered);
    if (numPositive >= min_positive) {
      RCLCPP_INFO(this->get_logger(), "Detected blue: %d", min_positive);
      return;
    }

    RCLCPP_INFO(this->get_logger(), "Detected none");
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ColorDetector>());
  rclcpp::shutdown();
  return 0;
}
