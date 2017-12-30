# string_encoding

This is the B2-11 Project github page for our programming mini-project. 

### Introduction to concept
For our P1 project, we worked with data transfer using radio transmitters, for sending files such as images to and from our robot. The radio transmitters gave us long range when compared to a wifi network, and allowed our robot to function without using a central wifi ‘hub’ as the only connection to its operator. The encoding we used for sending this data was very inefficient, resulting in each file being eight times bigger when transmitted, compared to its size when stored. We have chosen to work with Base64 for this programming project, because it would have been a useful feature for our robot, that we hope to apply to future projects.


### Building
Using ROS kinetic,

1) Using a valid catkin workspace folder, place this repository into the src folder
2) run `catkin_make` on the base folder. 
3) cd to the devel folder and run `source setup.bash`
4) use `rosrun string_encoded pub_string` to run the publisher and `rosrun string_encoded sup_string` to run the subscriber