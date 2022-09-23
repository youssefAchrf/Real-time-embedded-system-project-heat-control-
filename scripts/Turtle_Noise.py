#! /usr/bin/env python 


# Imported Packages
import rospy 
import numpy as np 
import random
from turtlesim.msg import Pose




#Initialization
position = np.zeros((1,3))	#Identify array of six elements all initialized by zero
position_Noise = np.zeros((1,3)) #Create an array to have the noisy measurments

# Standard Deviation Inputs
SD = [0.1,0.1,0.1]

Pose_msg = Pose()
	



#Callback function which is called when a new message of type Odometry is received by the subscriber
def callback(Pose):
	  global position
	  global Pose_msg

	  # Extract the x,y and theta
	  x = round(Pose.x, 4)		
	  y = round(Pose.y, 4)	
	  Theta = round(Pose.theta, 4)	

	  #Save the positions in an array
	  position = [x, y, Theta] 

	  #Call the add noise function.
	  Position_Noise = add_Noise(position)
	
	  Pose_msg.x = round(Position_Noise[0],4)
	  Pose_msg.y = round(Position_Noise[1],4)
	  Pose_msg.theta = round(Position_Noise[2],4)

	  pub.publish(Pose_msg)	#Publish msg




def add_Noise(position):
	global SD
	global position_Noise

	x_noise = position[0] + random.uniform(-SD[0],SD[0])		#Noisy data calculated at x
	y_noise = position[1] + random.uniform(-SD[1],SD[1])		#Noisy data calculated at y
	theta_noise = position[2] + random.uniform(-SD[2],SD[2])	#Noisy data calculated at theta

	position_Noise = [x_noise, y_noise, theta_noise]   #Store the noisy position in array
 
	return(position_Noise)



if __name__ == '__main__':     # Main function that is executed 

	# Initialize ROS Node 
	rospy.init_node('Turtle_Noise', anonymous = True) #Identify Ros Node
	# Initialize The Subscriber
	sub = rospy.Subscriber('/turtle1/pose', Pose, callback) #Subscribe to the  topic "/turtle1/pose" to read the turtle coordinates
	# Initialize The Publisher
	pub = rospy.Publisher('/Turtle_Noise', Pose, queue_size=10) #Publish the New Noisy message
	rospy.spin()


