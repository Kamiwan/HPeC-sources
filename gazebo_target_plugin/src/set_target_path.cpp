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
 * Main file of gazebo target model plugin.
 *  This program allow to move a model by setting its coordinates by following a script
 *************************************************************************************/

#include <ignition/math/Pose3.hh>
#include <ignition/math/Rand.hh>
#include <gazebo/physics/physics.hh>
#include "set_target_path.hpp"

using namespace gazebo;

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(set_target_plugin)

/////////////////////////////////////////////////
void SetStaticPose::Load(physics::ModelPtr _model, sdf::ElementPtr /*_sdf*/)
{
  this->model = _model;

  this->connections.push_back(event::Events::ConnectWorldUpdateEnd(
      std::bind(&SetStaticPose::OnUpdate, this)));
}

/////////////////////////////////////////////
void SetStaticPose::OnUpdate()
{
  // Get the desired pose, here giving a random offset
  ignition::math::Pose3d pose = this->model->GetWorldPose().Ign();

  pose += ignition::math::Pose3d(ignition::math::Rand::DblUniform(-0.01, 0.01),
                                 ignition::math::Rand::DblUniform(-0.01, 0.01),
                                 ignition::math::Rand::DblUniform(-0.01, 0.01),
                                 ignition::math::Rand::DblUniform(-0.01, 0.01),
                                 ignition::math::Rand::DblUniform(-0.01, 0.01),
                                 ignition::math::Rand::DblUniform(-0.01, 0.01));

  // Don't let it go far under the gound
  pose.Pos().Z() = pose.Pos().Z() < 0.5 ? 0.5 : pose.Pos().Z();

  this->model->SetWorldPose(pose);
}