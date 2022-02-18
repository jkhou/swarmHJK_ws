#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/PoseStamped.h> 
#include <nav_msgs/Odometry.h> 
#include <geometry_msgs/TwistStamped.h> 
#include <sstream>
#include "sys/time.h"
#include <stdio.h>
#include "serial.h"
#include <iomanip>
#include <iostream>
using namespace std;

double inittime = 0;

double getTime()
{
    struct timeval t;
    gettimeofday(&t,NULL);
    double rt=t.tv_usec;
    rt=rt/1000000+t.tv_sec;
    return rt;
}


int main(int argc, char **argv) {
	ros::init(argc, argv, "xbee_receive_uam");
	ros::NodeHandle n;
	ros::Publisher uam_base_pub1 = n.advertise<nav_msgs::Odometry>("quadrotor_pose", 1); 
	ros::Publisher uam_base_pub2 = n.advertise<geometry_msgs::TwistStamped>("quadrotor_vel", 1);
	ros::Publisher uam_base_pub3 = n.advertise<nav_msgs::Odometry>("tiger_odom", 1);

	ros::Rate loop_rate(25);
	short data_out[14];
	int secs[2];
	double sendTime = 0.0;
	static int fd;

	nav_msgs::Odometry quad_pose;
	geometry_msgs::TwistStamped vel;
	nav_msgs::Odometry pose_tiger;
//	double ltime,ntime;
//	ltime=getTime();
//	ntime=ltime;

	fd = serial_open_file((char*)"/dev/xbee", 57600);
	ROS_INFO("Open Serial %d", fd);

	cout.setf(ios_base::fixed, ios_base::floatfield);

	while (ros::ok()) {
		// ntime=getTime();
		// ltime=ntime;

		if(fd<1)
		{

			fd = serial_open_file((char*)"/dev/xbee", 57500);
		}

		if(readmessage (fd, data_out)<1)
		{
			std::cout<<"error"<<std::endl;
			loop_rate.sleep();
		}

		// memcpy(&rostt,&data_out[10],sizeof(rostt)); //from int to short
		quad_pose.header.stamp = ros::Time::now();
		quad_pose.pose.pose.position.x = (float)data_out[0]/1000;
		quad_pose.pose.pose.position.y = (float)data_out[1]/1000;
		quad_pose.pose.pose.position.z = (float)data_out[2]/1000;

		vel.header.stamp = ros::Time::now();
		vel.twist.linear.x = (float)data_out[3]/1000;
		vel.twist.linear.y = (float)data_out[4]/1000;
		vel.twist.linear.z = (float)data_out[5]/1000;

		sendTime = data_out[10] + 1643274000.0;
		sendTime += data_out[11]/10000.0;
		// pose_tiger.pose.pose.position.x = (float)data_out[6]/1000;
		// pose_tiger.pose.pose.position.y = (float)data_out[7]/1000;
		// pose_tiger.pose.pose.position.z = (float)data_out[8]/1000;

        // ROS_INFO_STREAM(quad_pose.pose.pose.position);
		// ROS_INFO_STREAM(vel.twist.linear);

		// ROS_INFO_STREAM(pose_tiger.pose.pose.position.x);
        uam_base_pub1.publish(quad_pose);
		uam_base_pub2.publish(vel);
		uam_base_pub3.publish(pose_tiger);

		std::cout << std::endl;
		std::cout << "send time:" << sendTime << std::endl;
	    std::cout << "delay time:" << ros::Time::now().toSec()-sendTime << std::endl;
		// std::cout<<"ros time:"<<fixed<<setprecision(14)<<ros::Time::now().toSec()<<std::endl;
/*
		if(inittime<1){
			inittime=ros::Time::now().toSec()-secs[0]-secs[1]/1000000000.0f;
		}
		
		std::cout<<"send time:"<<secs[0]+secs[1]/1000000000.0f<<std::endl;
	    std::cout<<"delay time:"<<inittime<<std::endl;
		std::cout<<"ros time:"<<fixed<<setprecision(14)<<ros::Time::now().toSec()<<std::endl;
		*/
//		double t=(inittime+secs[0]+secs[1]/1000000000.0f);
//		msg.header.stamp.sec=(int)t;
//		msg.header.stamp.nsec=(int)((t-msg.header.stamp.sec)*1000000000);//secs[1];
//		pub.publish(msg);

		//ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
