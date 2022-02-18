//
// Created by cc on 2020/8/5.
//

#include <ros/ros.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>
#include <Eigen/Eigen>
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <nav_msgs/Odometry.h>

using namespace Eigen;

Vector3d current_p;
mavros_msgs::State current_state;
ros::Publisher pva_pub;

// void positionCallback(const geometry_msgs::PoseStamped::ConstPtr &msg)
void positionCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    /// ENU frame to NWU
    current_p << msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z;
}

void stateCallback(const mavros_msgs::State::ConstPtr &msg)
{
    current_state = *msg;
}


void setPVA(Eigen::Vector3d p, Eigen::Vector3d v, Eigen::Vector3d a, double yaw=0.0)
{
    trajectory_msgs::JointTrajectoryPoint pva_setpoint;

    pva_setpoint.positions.push_back(p(0)); //x
    pva_setpoint.positions.push_back(p(1)); //y
    pva_setpoint.positions.push_back(p(2)); //z
    pva_setpoint.positions.push_back(yaw);

    pva_setpoint.velocities.push_back(v(0));
    pva_setpoint.velocities.push_back(v(1));
    pva_setpoint.velocities.push_back(v(2));

    pva_setpoint.accelerations.push_back(a(0));
    pva_setpoint.accelerations.push_back(a(1));
    pva_setpoint.accelerations.push_back(a(2));

    pva_pub.publish(pva_setpoint);

//    ROS_INFO_THROTTLE(1.0, "P x=%f, y=%f, z=%f", pva_setpoint.positions[0], pva_setpoint.positions[1], pva_setpoint.positions[2]);
//    ROS_INFO_THROTTLE(1.0, "V x=%f, y=%f, z=%f", pva_setpoint.velocities[0], pva_setpoint.velocities[1], pva_setpoint.velocities[2]);
//    ROS_INFO_THROTTLE(1.0, "A x=%f, y=%f, z=%f", pva_setpoint.accelerations[0], pva_setpoint.accelerations[1], pva_setpoint.accelerations[2]);

//    ROS_INFO("P x=%f, y=%f, z=%f", pva_setpoint.positions[0], pva_setpoint.positions[1], pva_setpoint.positions[2]);
//    ROS_INFO("V x=%f, y=%f, z=%f", pva_setpoint.velocities[0], pva_setpoint.velocities[1], pva_setpoint.velocities[2]);
//    ROS_INFO("A x=%f, y=%f, z=%f", pva_setpoint.accelerations[0], pva_setpoint.accelerations[1], pva_setpoint.accelerations[2]);
}



int main(int argc  , char** argv )
{

    ros::init(argc, argv, "straight_line");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 1, stateCallback);
    // ros::Subscriber pose_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, positionCallback);
    ros::Subscriber pose_sub = nh.subscribe<nav_msgs::Odometry>("/quadrotor_pose", 1, positionCallback);

    pva_pub = nh.advertise<trajectory_msgs::JointTrajectoryPoint>("/pva_setpoint", 1);


    const int LOOPRATE = 40;
    ros::Rate loop_rate(LOOPRATE);


    double delt_t = 1.0 / LOOPRATE;

    double yaw_target= 3.1415926/9;
    Vector3d recorded_p = current_p;

    double offset=0;
    while(ros::ok())
    {
        if(current_state.mode != "OFFBOARD" )
        {
            setPVA(current_p, Vector3d::Zero(), Vector3d::Zero(), yaw_target);
	        recorded_p = current_p;
        }
        else
        {
            Eigen::Vector3d goal_p=recorded_p;
            goal_p(0)+=offset;
            setPVA(goal_p, Vector3d::Zero(), Vector3d::Zero(), yaw_target);
            ROS_INFO_THROTTLE(0.5,"GOAL: %f %f %f",goal_p(0),goal_p(1),goal_p(2));
        }
        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}