# HPeC project

The HPeC project aims at demonstrating the relevancy of self-adaptive hardware architectures to respond to the growing demands of high performance computing, in an increasing class of embedded systems that also have demanding footprint and energy efficiency constraints.

This github repository aims at improving the teamwork in the project. It helps by providing the same files at the whole team in order to avoid version and compliance issues.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

* Ubuntu 16.04 (Xenial) or equivalent
* [Git](https://git-scm.com/book/fr/v1/D%C3%A9marrage-rapide-Installation-de-Git#Installation-sur-Linux)
* [ROS kinetic](http://wiki.ros.org/kinetic/Installation)
  * Gazebo 7, provided with kinetic full install
  * MAVLINK, provided with kinetic full install
* [SITL ardupilot](http://ardupilot.org/dev/docs/setting-up-sitl-on-linux.html)
* [MAVROS](https://dev.px4.io/en/ros/mavros_installation.html)

### Installing

This section aims at creating a catkin workspace and include sources from this repository. The environment configuration is introduced. Then, compiling and execution of the project are explained. 

Please contact me if you encounter an unexpected error at erwan.moreac@univ-ubs.fr.

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
The -j 1 option is to only use one core to compile since catkin can have some errors due to compile dependancies separately. 

The command should end with: 

```
[100%] Built target px4_comm_node_copter
```

ds


## Running the tests

**/!\ Work in progress**

```

```

```javascript
function test() {
 console.log("look ma’, no spaces");
}
```

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



