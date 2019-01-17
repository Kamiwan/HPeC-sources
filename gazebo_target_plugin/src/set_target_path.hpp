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
 * Created on:  January 17, 2019
 * 
 * Main file header of gazebo target model plugin.
 *  This program allow to move a model by setting its coordinates by following a script
 *************************************************************************************/
#include <gazebo/common/Plugin.hh>

namespace gazebo
{
  class GAZEBO_VISIBLE SetStaticPose : public ModelPlugin
  {
    // Documentation inherited
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Main loop to update the pose
    private: void OnUpdate();

    /// \brief All the event connections.
    private: std::vector<event::ConnectionPtr> connections;

    /// \brief Pointer to the model
    private: physics::ModelPtr model;
  };
}

