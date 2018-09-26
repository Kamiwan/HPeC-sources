# HPeC project

The HPeC project aims at demonstrating the relevancy of self-adaptive hardware architectures to respond to the growing demands of high performance computing, in an increasing class of embedded systems that also have demanding footprint and energy efficiency constraints.

This github repository aims at improving the teamwork in the project. It helps by providing the same files at the whole team in order to avoid version and compliance issues.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

Please contact me if you encounter an unexpected error at erwan.moreac@univ-ubs.fr.

### **1. Prerequisites**

* Ubuntu 16.04 (Xenial) or equivalent (Debian Jessie)

There is no guarantee that the following project runs in another distro than Ubuntu 16.04. 

You need to install the following applications: 

* [Git](https://git-scm.com/book/fr/v1/D%C3%A9marrage-rapide-Installation-de-Git#Installation-sur-Linux)
* [ROS kinetic](http://wiki.ros.org/kinetic/Installation)
  * Gazebo 7, provided with kinetic full install
  * MAVLINK, provided with kinetic full install
* [SITL ardupilot](http://ardupilot.org/dev/docs/setting-up-sitl-on-linux.html)
* [MAVROS](https://dev.px4.io/en/ros/mavros_installation.html)
* [Plugin ardupilot_gazebo](https://github.com/khancyr/ardupilot_gazebo)
  * If you want to work with Gazebo 8 to get _movable objects_, please follow the [part 2](###-**2.-Upgrading-to-Gazebo-8**) of Getting Started before installing this plugin.

* Additional packages:

1. We need octomap packages to compile the project
```
$ sudo apt-get install ros-kinetic-octomap ros-kinetic-octomap-mapping ros-kinetic-octomap-msgs ros-kinetic-octomap-ros ros-kinetic-octomap-rviz-plugins ros-kinetic-octomap-server
```

2. In order to run gazebo and ros this package is required:
```
$ sudo apt-get install ros-kinetic-gazebo-ros-pkgs
```

3. Geographic lib is mandatory to execute MAVROS, to install the dataset, run the script at the following location:
```
/opt/ros/kinetic/lib/mavros/install_geographiclib_datasets.sh
```

As soon as all programs have been installed, we configure the terminal environment by editing the .bashrc. First, we source ROS to get all commands: 
```
$ echo 'source /opt/ros/kinetic/setup.bash' >> ~/.bashrc
```
Second, we source gazebo setup with:
```
$ echo 'source /usr/share/gazebo/setup.sh' >> ~/.bashrc
```
Now, reload your .bashrc in your terminal:
```
$ source ~/.bashrc
```
### **2. Upgrading to Gazebo 8**
This section is not mandatory but strongly recommended in order to enjoy all features of the project. The first step is to remove the current gazebo 7 installed with ROS kinetic full install: 

```
$ sudo apt-get purge gazebo7
```

We install gazebo 8 thereafter: 
```
$ sudo apt-get install gazebo8
```
A next step is to get gazebo-ros packages for gazebo 8:
```
$ sudo apt-get install ros-kinetic-gazebo8-ros-pkgs
```
As well as for the package ros-control:
```
$ sudo apt-get install ros-kinetic-gazebo8-ros-control
```
Once it's done, you can check gazebo version by typing **$ gazebo --verbose**.
To complete the upgrade, we **MUST** install the [Plugin ardupilot_gazebo](https://github.com/khancyr/ardupilot_gazebo) (or reinstall if you used gazebo 7). 

### **3. Installing the project**

This section aims at creating a catkin workspace and include sources from this repository. The environment configuration is introduced. Then, compiling and execution of the project are explained. 

First step, create a catkin workspace directory and go inside it.

```
$ mkdir -p  ./catkin_ws/src
$ cd ./catkin_ws/
```

Execute catkin_make to setup the directory

```
$ catkin_make
```
It creates the folder build/ and devel/ and the CMake top level in src/. 

Clone the git repository in catkin_ws/.

```
$ git clone https://github.com/Kamiwan/HPeC-sources.git
```
Move source files from HPeC-sources/ to src/

```
$ mv ./HPeC-sources/* ./HPeC-sources/.* ./src/
```
It worked but you will get a message error such as **"mv: cannot move './HPeC-sources/.' to './src/.'"** because this command tries to move the "." directory pointer. 

Then, remove the empty HPeC-sources directory

```
$ rm -r HPeC-sources/
```
Then, check if the repo is properly in place.
```
$ cd ./src/
$ git status
```
You should get something like **"Your branch is up-to-date with 'origin/master'. Nothing to validate, the working copy is clean."**

Now we configure the project to compile it.

Replace the path of *image_transport* lib by yours in the CMakeLists.txt of following packages:
* emergency_landing
* generic_task_wrapper
* rosnode_model
* harris_detector
* detection_tracking

in the include directories
```bash 
## Specify additional locations of header files
include_directories(
  ${catkin_INCLUDE_DIRS} 
  /Your/Path/image_transport
  ${OpenCV_INCLUDE_DIRS}
)
```
and to link executable library. Image_transport include directory and binary file are supposed to come from your ROS kinetic install path with something like **/opt/ros/kinetic/[...]**.
```bash
## Specify libraries to link a library or executable target against
 target_link_libraries(the_actual_node
   ${catkin_LIBRARIES}
    /Your/Path/libimage_transport.so
   ${OpenCV_LIBRARIES}
 )
```
Once every paths are updated you can compile the whole project with the following command at the root of your catkin workspace:
```
$ cd ../
$ catkin_make -j 1
```
The -j 1 option is to only use one core to compile since catkin can have some errors due to compile dependencies separately. 

The command should end with: 

```
[100%] Built target px4_comm_node_copter
```

Now your project directory is ready to run current installed packages and also to create new ones. :)


## Running the tests

### **1. Configuration**

Before to start, we configure paths in order to make Gazebo find HPeC models and worlds. Please adapt the path to your own setup. The first environment variable we need to update is GAZEBO_MODEL_PATH,
```
$ echo 'export GAZEBO_MODEL_PATH=/Your/Path/catkin_ws/src/mybot_gazebo/models:${GAZEBO_MODEL_PATH}' >> ~/.bashrc
```
and the other one is GAZEBO_RESOURCE_PATH,
```
$ echo 'export GAZEBO_RESOURCE_PATH=/Your/Path/catkin_ws/src/mybot_gazebo/models:/Your/Path/catkin_ws/src/mybot_gazebo/worlds:${GAZEBO_RESOURCE_PATH}' >> ~/.bashrc
```
Reload your .bashrc file.
```
$ source ~/.bashrc
```
Each time you compile your catkin_ws with _$ catkin_make -j 1_, you need to source your project:
```
$ source ./devel/setup.bash
```
We are finally ready for our first try!

### **2. Execution**

#### Run roscore and gazebo
```
$ ./src/scripts/run_server_gazebo.sh
```
This command launches the roscore and gazebo with the world **HPeC_sandbox.world**. You should see in gazebo GUI a drone with sensors (range in blue) and some objects and surfaces.

#### Execute and use a node of the project
If you have at least launched a ROS Master with **$ roscore** or used the script **run_server_gazebo.sh**, you can in another terminal launch every nodes compiled by the HPeC project. In this example we will use the node  of _rosnode_model_ package.

First of all, go to your catkin_ws and source it with **$ source ./devel/setup.bash**. Then, you should be able to run:
```
$ rosrun rosnode_model rosnode_model_node 
```
If it worked the terminal printed this **[INFO] [1528472534.171299944]: [TASK WRAPPER][RUNNING]**.  

In the HPeC project, most of the nodes created can be run in several modes. Indeed, they can be executed in their software version or in the hardware one. Thus, when you launch the node with rosrun, it does nothing until you tell it in which mode start.

To control the node mode, you have to send him an integer in the right ROS Topic. To do so, you can check which topic is using the node in its source files or by using **$ rostopic list**.

If you only run the roscore and the rosnode_model_node, rostopic list gives this output:
```
/rosnode_model_mgt_topic
/rosnode_model_notification_topic
/rosout
/rosout_agg
```
This is the topic **/rosnode_model_mgt_topic** where we have to publish a message. On another terminal run:

```
$ rostopic pub  -1  /rosnode_model_mgt_topic std_msgs/Int32 "1"
```
- 0 to stop the application
- 1 to start the software version
- 2 to start the "hardware" version

The terminal where the node has been launched should print this kind of output,
```
[ERROR] [1528474077.010573417]: [THREAD][RUNNING][SW]: rosnode_model SOFTWARE VERSION 
[ERROR] [1528474077.039458231]: Could not read rosnode_model activation rate. Setting 1 Hz
Hello World!
SOFTWARE rosnode_model Processing time : 0.051
Hello World!
SOFTWARE rosnode_model Processing time : 0.027
...
```
Note that in this case the hardware version is not really a hardware mode, just another version of the program to give a ROS node model.


#### Run the simulation Loop ROS + GAZEBO + SITL + MAVROS
In order to run the complete simulation loop, we need at least 3 terminals. In each terminal is used one specific part. Follow commands for each one of them.

##### Terminal 1: ROS+Gazebo
```
$ cd /Your/Path/catkin_ws
$ source ./devel/setup.bash
$ ./src/scripts/run_nodes_gazebo.sh
```
It run the ROS Master, gazebo and several nodes which are essential to run the first scenario.

##### Terminal 2: SITL
```
$ cd /Your/Path/SITL/ardupilot/ArduCopter
$ python ../Tools/autotest/sim_vehicle.py -v ArduCopter -f gazebo-iris -m --mav10 --map --console -I0
```
This command should create 3 windows:
- xterm   Terminal output of the autopilot
- console GUI with drone parameters
- map     GUI map that locates the drone

/!\ IT IS VERY IMPORTANT TO WAIT THAT APM finishes the drone setup before to start MAVROS. It requires around one minute. The information is displayed in the console window and the following messages indicate when the configuration is over: 
<div style="background-color:green;">APM: EKF2 IMU0 is using GPS </div>
<div style="background-color:green;">APM: EKF2 IMU1 is using GPS</div>

##### Terminal 3: MAVROS

```
$ roslaunch mavros px4.launch fcu_url:="udp://:14550@127.0.0.1:14551"
```
This application finishes the loop and allow ROS nodes to communicate navigation orders to the emulated autopilot SITL.


Now you should see the drone flying in the virtual world created by gazebo. :)

## Troubleshooting

**/!\ Work in progress**

### **1. catkin_ws**
#### *CMake Error,* error: could not find svn for checkout of opentld

You need to install subversion:

```
$ sudo apt-get install subversion
```
Then compile again with 
```
$ catkin_make -j 1
```

### **2. Gazebo**
#### Missing libArduPilotPlugin.so ... etc
In case you see this message when you launch gazebo with demo worlds, check you have no error after sudo make install the plugin ardupilot_gazebo.
If no error use "ls" on the install path given to see if the plugin is really here.
If this is correct, check with "cat /usr/share/gazebo/setup.sh" the variable GAZEBO_PLUGIN_PATH. It should be the same as the install path. If not use "cp" to copy the lib to right path.

For Example

```
sudo cp -a /usr/lib/x86_64-linux-gnu/gazebo-7.0/plugins/ /usr/lib/x86_64-linux-gnu/gazebo-7/
```
path mismatch is confirmed as ROS's glitch. It'll be fixed.

### **3. MAVROS**

### **4. SITL**

## Additional links to learn how to use Git

### Github link
https://github.com/

### Online tutorials
[Try github](https://try.github.io/levels/1/challenges/1)

[Gitreal tutorial](http://gitreal.codeschool.com/enroll)

### A really good French git lesson
[Openclassroom git](https://openclassrooms.com/courses/gerez-vos-codes-source-avec-git)
### Git documentation
[Using branches](https://www.atlassian.com/git/tutorials/using-branches)

[How to merge your branch](https://www.atlassian.com/git/tutorials/merging-vs-rebasing)

### Good practices with git 
[Best practices](https://www.git-tower.com/learn/git/ebook/en/command-line/appendix/best-practices)

[Writing good commits](https://github.com/erlang/otp/wiki/writing-good-commit-messages)

### Git command summary
[Git Cheat sheet](https://www.atlassian.com/git/tutorials/atlassian-git-cheatsheet)

