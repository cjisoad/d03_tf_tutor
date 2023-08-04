#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"

using geometry_msgs::msg::TransformStamped;
// ros2 run ** x y z roll pitch yaw parents child
class tf2_static_bro : public rclcpp::Node
{
private:
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_bro;
    void send_transform(char * argv[])
    {
        geometry_msgs::msg::TransformStamped transform_msgs;
        // 设置头消息
        transform_msgs.header.frame_id = argv[7];
        transform_msgs.header.stamp = this->now(); //时间
        transform_msgs.child_frame_id= argv[8];

        transform_msgs.transform.translation.x = atof(argv[1]);
        transform_msgs.transform.translation.y = atof(argv[2]);
        transform_msgs.transform.translation.y = atof(argv[3]);

        tf2::Quaternion qtn;
        qtn.setRPY(atof(argv[4]),atof(argv[5]),atof(argv[6]));
        transform_msgs.transform.rotation.x = qtn.getX();
        transform_msgs.transform.rotation.y = qtn.getY();
        transform_msgs.transform.rotation.z = qtn.getZ();
        transform_msgs.transform.rotation.w = qtn.getW();

        static_bro->sendTransform(transform_msgs);
        RCLCPP_INFO(rclcpp::get_logger("static_tf"),"坐标系发送成功！");
    }
    
public:
    tf2_static_bro(std::string name,char * argv[]):Node(name)
    {
        RCLCPP_INFO(rclcpp::get_logger("static_tf"),"坐标系服务器启动");
        static_bro = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        send_transform(argv);
    }
    ~tf2_static_bro(){
        RCLCPP_WARN(rclcpp::get_logger("static_tf"),"坐标系服务器关闭");
    }
};

int main(int argc, char *argv[])
{
    if (argc!= 9)
    {
        RCLCPP_ERROR(rclcpp::get_logger("static_tf"),"参数非法！");
    }
    RCLCPP_INFO(rclcpp::get_logger("static_tf"),"参数接收成功！！");
    
    rclcpp::init(argc,argv);
    auto sta_node = std::make_shared<tf2_static_bro>("static_node_tf",argv);
    rclcpp::spin(sta_node);
    rclcpp::shutdown();
    return 0;
}
