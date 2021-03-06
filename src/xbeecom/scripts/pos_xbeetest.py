#!/usr/bin/env python
# license removed for brevity
import rospy
import math
# from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped

POS_REF = PoseStamped()

def pos_talker():
    pub = rospy.Publisher('/mocap/pose', PoseStamped, queue_size=10)
    rospy.init_node('mocap_pose', anonymous=True)
    rate = rospy.Rate(10) # 20hz

    # while not rospy.is_shutdown():
    #     POS_REF.pose.position.x = 0
    #     POS_REF.pose.position.y = 0
    #     POS_REF.pose.position.z = 2

    ii = 0
    while not rospy.is_shutdown():
        now = rospy.get_rostime()
        POS_REF.header.stamp.secs = now.secs
        POS_REF.header.stamp.nsecs = now.nsecs
        if(ii<10):
            POS_REF.pose.position.x = 0
            POS_REF.pose.position.y = 0
            POS_REF.pose.position.z = 1
        elif(ii<20):
            POS_REF.pose.position.x = 3
            POS_REF.pose.position.y = 3
            POS_REF.pose.position.z = 3
        elif(ii<30):
            POS_REF.pose.position.x = 0
            POS_REF.pose.position.y = 6
            POS_REF.pose.position.z = 1
        elif(ii<40):
            POS_REF.pose.position.x = -3
            POS_REF.pose.position.y = 3
            POS_REF.pose.position.z = 3
        else:
            ii = 0

        ii = ii + 1

    # count=0
    # while not rospy.is_shutdown():
    #     if count <= 100:
    #         POS_REF.pose.position.x = 2
    #         POS_REF.pose.position.y = 0
    #         POS_REF.pose.position.z = 2
    #     # elif count < 1200:
    #     #     POS_REF.pose.position.x = 2 * math.cos(0.05 * (count - 500))
    #     #     POS_REF.pose.position.y = 2 * math.sin(0.05 * (count - 500))
    #     #     POS_REF.pose.position.z = 2
    #     else:
    #         POS_REF.pose.position.x = 2 * math.cos(0.05 * (count - 100) / 10)
    #         POS_REF.pose.position.y = 2 * math.sin(0.05 * (count - 100) / 10)
    #         POS_REF.pose.position.z = 2
    #
    #     count = count + 1

        # if count > 1000:
        #     count = 0


        rospy.loginfo(POS_REF)
        pub.publish(POS_REF)
        rate.sleep()

if __name__ == '__main__':
    try:
        pos_talker()
    except rospy.ROSInterruptException:
        pass