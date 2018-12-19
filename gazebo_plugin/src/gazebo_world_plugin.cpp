#include <gazebo/common/Plugin.hh>
#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"
#include <ros/ros.h>

#include <chrono>
#include <thread>

namespace gazebo
{
class WorldPluginTutorial : public WorldPlugin
{
  public:
    WorldPluginTutorial() : WorldPlugin()
    {
    }

    void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
    {
        // Make sure the ROS node for Gazebo has already been initialized
        if (!ros::isInitialized())
        {
            ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
                             << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
            return;
        }

        ROS_INFO("Hello World FROM GAZEBO WORLD PLUGIN!");

        //std::this_thread::sleep_for(std::chrono::seconds(10));

        ROS_INFO("Hello World FROM GAZEBO WORLD PLUGIN!");

        // Option 3: Insert model from file via message passing.
        // Create a new transport node
        transport::NodePtr node(new transport::Node());

        // Initialize the node with the world name
        node->Init(_world->Name());

        // Create a publisher on the ~/factory topic
        transport::PublisherPtr factoryPub =
            node->Advertise<msgs::Factory>("~/factory");

        // Create the message
        msgs::Factory msg;

        // Model file to load
        msg.set_sdf_filename("model:///Gazebo/Cylinder");

        // Pose to initialize the model to
        msgs::Set(msg.mutable_pose(),
                  ignition::math::Pose3d(
                      ignition::math::Vector3d(2, 2, 0),
                      ignition::math::Quaterniond(0, 0, 0)));

        // Send the message
        factoryPub->Publish(msg);

        transport::PublisherPtr windPub =
            node->Advertise<msgs::Wind>("~/wind");
        msgs::Wind msg_wind;

        msgs::Set(msg_wind.mutable_linear_velocity(),
                ignition::math::Vector3d(0, 5, 10)
        );
        windPub->Publish(msg_wind);

        // Create a publisher on the ~/light/modify
        transport::PublisherPtr lightPub =
        node->Advertise<msgs::Light>("~/light/modify");
        msgs::Light msg_light;
        msg_light.set_name("sun");
        msgs::Set(msg_light.mutable_diffuse(),
               common::Color(1, 0.0, 0.0, 1));
        lightPub->Publish(msg_light);
      
    }

  private:

};
    GZ_REGISTER_WORLD_PLUGIN(WorldPluginTutorial)
} // namespace gazebo