#!/usr/bin/env python
# license removed for brevity
import rospy
import math
# from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped
from mavros_msgs.msg import AttitudeTarget

POS_REF = AttitudeTarget()

def pos_talker():
    pub = rospy.Publisher('/cmd/attitudeTarget', AttitudeTarget, queue_size=10)
    rospy.init_node('mocap_attitudeTarget', anonymous=True)
    rate = rospy.Rate(1) # 20hz

    while not rospy.is_shutdown():
        POS_REF.body_rate.x = 0.1
        POS_REF.body_rate.y = 0.2
        POS_REF.body_rate.z = 0.3

    # ii = 0
    # while not rospy.is_shutdown():
    #     if(ii<10):
    #         POS_REF.pose.position.x = 0
    #         POS_REF.pose.position.y = 0
    #         POS_REF.pose.position.z = 1
    #     elif(ii<20):
    #         POS_REF.pose.position.x = 3
    #         POS_REF.pose.position.y = 3
    #         POS_REF.pose.position.z = 3
    #     elif(ii<30):
    #         POS_REF.pose.position.x = 0
    #         POS_REF.pose.position.y = 6
    #         POS_REF.pose.position.z = 1
    #     elif(ii<40):
    #         POS_REF.pose.position.x = -3
    #         POS_REF.pose.position.y = 3
    #         POS_REF.pose.position.z = 3
    #     else:
    #         ii = 0
    #
    #     ii = ii + 1

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