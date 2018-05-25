/** @ingroup px4_comm
 */


/**
 * @file
 *
 * \file px4_comm_class.h
 * \brief
 * \author JJ. ESTIMBRE
 * \version 0.1
 * \date 16 Août 2016
 */

#ifndef PX4_COMM_CLASS_H
#define PX4_COMM_CLASS_H

#include <mavros_msgs/Waypoint.h>
#include <communication/path_msg.h>

/** @addtogroup px4_comm
* @{ */


 /*!@class px4_comm
 */

class px4_comm_class
{
    public:
    px4_comm_class();
    ~px4_comm_class();
        
    /*!
	*  \brief crée le wp du but à atteindre pour eviter une collision, qui sera envoyé vers l'autopilote
	*/
    //mavros_msgs::Waypoint goal_wp();
    mavros_msgs::Waypoint goal_wp();
    
    /*!
	*  \brief crée le vector des wps du chemin a suivre, qui sera envoyé vers l'autopilote
	*/
    std::vector<mavros_msgs::Waypoint> path_wp_vector();
    //mavros_msgs::Waypoint path_wp();
    
    /*!
	*  \brief envoy le vector des wps du chemin a suivre, qui sera envoyé vers l'autopilote
	*  @param a_path chemin a suivre en coordonées UTM
	*/
    void setPosesFromPath(std::vector<geometry_msgs::PoseStamped> a_poses);
    
    /*!
	*  \brief envoy le wp du but à atteindre pour eviter une collision, qui sera envoyé vers l'autopilote
	*  @param a_goal but à atteindre en coordonées UTM
	*/
    void setPoseFromGoal(geometry_msgs::Pose a_goal);
    
    private:
    
    mavros_msgs::Waypoint wp; //WP a envoyer
    std::vector<mavros_msgs::Waypoint> wpv_path; //WP for the path to follow
    mavros_msgs::Waypoint wpv_goal; //Wp for collision avoidance
    
    
};
#endif
