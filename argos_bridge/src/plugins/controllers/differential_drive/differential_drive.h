/*********************************************************************************************
 * Differential drive robot class
 * Designed to be inhereted into other ros robot controllers
 * Author: Peter Nikopoulos -> peter@nikopoulos.net
 *********************************************************************************************/

#ifndef DIFFERENTIAL_DRIVE_H
#define DIFFERENTIAL_DRIVE_H

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_sensor.h>

#include <ros/ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>

#include <math.h>

#define WHEEL_NUM 2

#define LEFT 0
#define RIGHT 1

#define DEG2RAD(x) (x * 0.01745329252) // *PI/180
#define RAD2DEG(x) (x * 57.2957795131) // *180/PI

using namespace argos;

class CDifferentialDrive : public CCI_Controller
{

public:
    CDifferentialDrive();
    virtual ~CDifferentialDrive() {}

    /**
     * @brief
     * This function initializes the controller.
     * The 't_node' variable points to the <parameters> section in the XML
     * file in the <controllers><footbot_ccw_wander_controller> section.
     */
    virtual void Init(TConfigurationNode &t_node);

    /**
     * @brief
     * This function is called once every time step.
     * The length of the time step is set in the XML file.
     */
    virtual void ControlStep();

    /**
     * @brief
     * This function resets the controller to its state right after the
     * Init().
     * It is called when you press the reset button in the GUI.
     * In this example controller there is no need for resetting anything,
     * so the function could have been omitted. It's here just for
     * completeness.
     */
    virtual void Reset() {}

    /**
     * @brief
     * Called to cleanup what done by Init() when the experiment finishes.
     * In this example controller there is no need for clean anything up,
     * so the function could have been omitted. It's here just for
     * completeness.
     */
    virtual void Destroy() {}

    /**
     * @brief Starts rosnode for robot controller
     * 
     * @return void *
     */
    void InitROS();

    /**
     * @brief Callback method for cmd velocity topic
     *
     * @param twist callback msg from /cmd_vel topic
     * @return * void
     */
    void CommandVelocityCallback(const geometry_msgs::Twist &twist);

    /**
     * @brief Reads latest velocity info from motor encoders
     *
     * @return float* wheel_vel[WHEEL_NUM]
     */
    float *ReadMotorEncoders();

    /**
     * @brief Updates the odometry arrays (odom_position, odom_velocity)
     *
     * @param timeDiff elapsed time from last update
     * @return * void
     */
    void CalculateOdometry(double timeDiff);

    /**
     * @brief  Updates the odometry message object with the latest odom data
     *
     * @param odom_msg pointer to the odometry message object
     * @return * void
     */
    void UpdateOdometryMessage(const nav_msgs::Odometry &odom_msg);

    /**
     * @brief Updates the odometry tf object with the latest odom data
     *
     * @param odom_tf pointer to odom tf transform
     * @return * void
     */
    void UpdateOdometryTF(const geometry_msgs::TransformStamped &odom_tf);

    /**
     * @brief Inits all the odom messages
     *
     * @return * void
     */
    void InitOdometry();

private:
    /*********************************************************************
     * ROS NodeHandle
     ********************************************************************/
    ros::NodeHandle nh;
    ros::Time current_time;
    ros::Time previous_time;

    /*********************************************************************
     * ROS Parameters
     ********************************************************************/
    char odom_header_frame_id[30];
    char odom_child_frame_id[30];
    
    /*********************************************************************
     * ROS Subscribers
     ********************************************************************/
    ros::Subscriber cmd_vel_sub;
    
    /*********************************************************************
     * ROS Publishers
     ********************************************************************/
    nav_msgs::Odometry odom_msg;
    ros::Publisher odom_pub;

    /*********************************************************************
     * Transform Broadcasters
     ********************************************************************/
    geometry_msgs::TransformStamped odom_tf;
    tf::TransformBroadcaster tf_broadcaster;

    /*********************************************************************
     * Params for SLAM and Naviagation
     ********************************************************************/
    float odom_pose[3] = {0.0, 0.0, 0.0};
    double odom_vel[3] = {0.0, 0.0, 0.0};
    double  last_velocity[WHEEL_NUM]  = {0.0, 0.0};

    /*********************************************************************
     * ARGOS Sensors and Actuators
     ********************************************************************/
    /* Pointer to the differential steering actuator */
    CCI_DifferentialSteeringActuator* m_pcWheels;
    /* Pointer to the differential steering sensor */ 
    CCI_DifferentialSteeringSensor* m_pcEncoder;
    
    /*********************************************************************
     * MISC
     ********************************************************************/
    std::string controller_name;
};

#endif