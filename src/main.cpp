#include "motor_ros2/motor_cfg.h"
#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <unistd.h>

class MotorControlSample : public rclcpp::Node {
public:
    MotorControlSample()
        : Node("motor_control_set_node"),
          motor(RobStrideMotor("can0", 0xFD, 0x02, 1))
    {
        motor.Get_RobStrite_Motor_parameter(0x7005);
        usleep(1000);
        motor.enable_motor();
        usleep(1000);

        // 10ms 定时器，50Hz 更新位置
        timer_ = create_wall_timer(
            std::chrono::milliseconds(20),
            std::bind(&MotorControlSample::control_loop, this));
    }

private:
    void control_loop() {
        motor.RobStrite_Motor_PosPP_control(1,1,1);  // 2 rad/s 速度限制
        RCLCPP_INFO(get_logger(), "Sent angle:1", 1);
    }

    RobStrideMotor motor;
    rclcpp::TimerBase::SharedPtr timer_;
    double angle_ = 0.0;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MotorControlSample>());
    rclcpp::shutdown();
    return 0;
}