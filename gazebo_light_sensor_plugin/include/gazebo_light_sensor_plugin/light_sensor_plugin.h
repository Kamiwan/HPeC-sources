/* 
 * This file is part of the HPeC distribution (https://github.com/Kamiwan/HPeC-sources).
 * Copyright (c) 2019 Lab-STICC Laboratory.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*************************************************************************************
 * Author(s) :  Erwan Moréac, erwan.moreac@univ-ubs.fr (EM)
 * Created on:  January 8, 2019
 * 
 * Main header file of light sensor plugin.
 * Create a light sensor class which inherits of Camera sensor class to compute illuminance
 *    
 * Plugin derived from this tutorial: 
 * https://github.com/SMARTlab-Purdue/ros-tutorial-gazebo-simulation/wiki/Sec.-4:-Creating-a-light-sensor-plugin
 *************************************************************************************/
    #ifndef GAZEBO_ROS_LIGHT_SENSOR_HH
    #define GAZEBO_ROS_LIGHT_SENSOR_HH
    
    #include <string>
    
    // library for processing camera data for gazebo / ros conversions
    #include <gazebo/plugins/CameraPlugin.hh>
    
    #include <gazebo_plugins/gazebo_ros_camera_utils.h>
    
    namespace gazebo
    {
      class GazeboRosLight : public CameraPlugin, GazeboRosCameraUtils
      {
        /// \brief Constructor
        /// \param parent The parent entity, must be a Model or a Sensor
        public: GazeboRosLight();
    
        /// \brief Destructor
        public: ~GazeboRosLight();
    
        /// \brief Load the plugin
        /// \param take in SDF root element
        public: void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);
    
        /// \brief Update the controller
        protected: virtual void OnNewFrame(const unsigned char *_image,
        unsigned int _width, unsigned int _height,
        unsigned int _depth, const std::string &_format);
    
        ros::NodeHandle _nh;
        ros::Publisher _sensorPublisher;
    
        double _fov;
        double _range;
      };
    }
    #endif