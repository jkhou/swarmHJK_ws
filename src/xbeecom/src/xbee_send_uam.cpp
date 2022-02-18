#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <sstream>
#include <stdio.h>
#include "serial.h"

int initsecs;
int fd;
int count =0;
static short data[16];
int secs[2];

void pose_callback(geometry_msgs::PoseStamped msg)
{
    data[0]=(short)(msg.pose.position.x*1000);
    data[1]=(short)(msg.pose.position.y*1000);
    data[2]=(short)(msg.pose.position.z*1000);

}
void vel_callback(geometry_msgs::TwistStamped msg)
{
    data[3]=(short)(msg.twist.linear.x*1000);
    data[4]=(short)(msg.twist.linear.y*1000);
    data[5]=(short)(msg.twist.linear.z*1000);
}
void pose_callback_tiger(geometry_msgs::PoseStamped msg)
{
    data[6]=(short)(msg.pose.position.x*1000);
    data[7]=(short)(msg.pose.position.y*1000);
    data[8]=(short)(msg.pose.position.z*1000);
    // data[9]=(short)(msg.pose.position.z*1000);
	// ROS_INFO_STREAM(data[6]);

}
void vel_callback_tiger(geometry_msgs::TwistStamped msg)
{
    data[9]=(short)(msg.twist.linear.x*1000);
    data[10]=(short)(msg.twist.linear.y*1000);
    data[11]=(short)(msg.twist.linear.z*1000);
	// std::cout<<"tiger_vel_x:\t"<<data[9]/1000<<std::endl;

}

int main(int argc, char **argv) {
	ros::init(argc, argv, "xbee_send_uam");
	ros::NodeHandle n;
	ros::Rate rate(100); 
    fd = serial_open_file((char*)"/dev/ttyUSB0", 57600);
    ROS_INFO("Open Serial: [%d]", fd);
    // int secs[2];
    secs[0]=-1;
    secs[1]=-1;
	double rost;
    // memcpy(&data[12],&secs,sizeof(secs));

	initsecs = (int)(ros::Time::now().toSec());
	ros::Subscriber pose_sub = n.subscribe("/mocap/pose", 1, pose_callback);
	ros::Subscriber vel_sub = n.subscribe("/mocap/vel", 1, vel_callback);
	ros::Subscriber tiger_sub = n.subscribe("/mocap/pose_tiger", 1, pose_callback_tiger);
	ros::Subscriber vel_tiger_sub = n.subscribe("/mocap/vel_tiger", 1, vel_callback_tiger);
	while (ros::ok())
	{
		ros::spinOnce();
		rate.sleep();

		ros::spinOnce();
		rate.sleep();

		ros::spinOnce();
		rate.sleep();

	    if(fd<1)
		{
			fd = serial_open_file((char*)"/dev/ttyUSB0", 57600);    //upboard 的波特率为19200
		}
		ROS_INFO("xbee send successfully! pose: %f %f %f",data[0]/1000.0,data[1]/1000.0,data[2]/1000.0);
		rost = ros::Time::now().toSec();
		memcpy(&data[12],&rost,sizeof(rost));
		sendmessage(fd,data);
	
		rate.sleep();
	}
	return 0;
}
