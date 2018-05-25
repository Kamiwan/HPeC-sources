/** @ingroup drone_move_planner
 */

/**
 * @file
 * 
 * \file droneMovePlannerClass.h
 * \brief 
 * \author D. Wallach
 * \version 0.1
 * \date 19 février 2016
 */
#ifndef DRONEMOVEPLANNERCLASS_H
#define DRONEMOVEPLANNERCLASS_H 

#include "communication/path_msg.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>

#include <ros/time.h>

/** @ingroup drone_move_planner
 */
class droneMovePlannerClass
{
public:
    /*!
	*  \brief Constructeur par défaut
	 */
	 droneMovePlannerClass();
    /*!
	*  \brief Modifie l'état du système
	* @param a_mode état du système (PATROL_IN_ZONE, PATROL_OUT_OF_ZONE, WAYPOINT, ANTICOL ou STANDBY)
	*/		
    bool setMode(std::string a_mode);
    /*!
	*  \brief Met à jour le WP donné par la base
	* @param a_waypoint coordonnées globales (UTM) du waypoint
	*/		
    //bool setWaypointAndAction (communication::goal_and_action a_goalAndAction);
    bool setWaypoint (geometry_msgs::Pose a_waypoint);
    
    /*!
	*  \brief Met à jour la liste de WP donné par la base
	* @param a_waypointlist coordonnées globales (UTM) des waypoints
	*/
    bool setWaypointList (std::vector<geometry_msgs::Pose> a_waypointlist);
    /*!
	*  \brief Met à jour l'action à effectuer une fois le wp atteint
	* @param a_nextAction action à effectuer (STANDBY ou PATROL)
	*/		
    bool pathSent ();
    /*!
	*  \brief change haspathchanged à false
	*/	
    bool setNextAction (std::string a_nextAction);
    /*!
	*  \brief Met à jour la position du drone
	* @param a_odom position
	*/		
    bool setPose(const nav_msgs::Odometry::ConstPtr& a_odom); 
    /*!
	*  \brief Met à jour la position du drone
	* @param a_wp position
	*/		
    bool setPosition(geometry_msgs::Pose a_wp);
    
    /*!
	*  \brief Set orientation (attribut m_orientation, en radians)
	* @param a_orientation orientation donnée par la boussole, en degrés entre le Nord et l'axe du robot
    */      
    int setCurrentOrientation(double a_orientation);
    
    bool setAnticol(int8_t dir);
    
    bool setPrevMode(std::string a_mode);
    
    /*!
	*  \brief Un nouveau chemin a-t-il été calculé ?
	* @return true si un nouveau chemin a été calculé et devrait être publié, false sinon
	*/	    
    bool hasPathChanged();
    
    /*!
	*  \brief Renvoie le chemin à suivre
	* @return chemin à suivre (coordonnées de chacun des points, et étiquette disant si le chemin a changé)
	*/    
    communication::path_msg getPathMsg(); 
    
     /*!
	*  \brief Met à jour tous les paramètres de la classe (chemin à suivre, but à atteindre, etc) en fonction de l'état du système
    * @param a_sytem_state état du système
	*/    
    bool update(std::string a_sytem_state);
    
protected:
    
    float m_altRef ;
    
    geometry_msgs::PoseStamped m_pose ; /*!< Position du drone en coordonnées globales */
    double m_orientation ; /*!< angle en radians entre le nord et le drone (sens des aiguilles d'une montre) */
    
    //geometry_msgs::Pose m_computedGoal; /*!< Waypoint à atteindre calculé par le noeud, en coordonnées globales */
    geometry_msgs::PoseStamped m_goal; /*!< Waypoint à atteindre, en coordonnées globales */
    std::vector<geometry_msgs::Pose> m_list;

    std::vector< geometry_msgs::PoseStamped > m_path ; /*!< chemin calculé par m_rosGlobalPlanner*/
    bool m_hasPathChanged ; /*!< le chemin a-t-il changé depuis le dernier calcul*/
    bool m_isMoving ;
    bool m_isFlying ;
    std::string m_prevMode;
    ros::Time m_lastAnticol;
    /*!
    * \brief Le but a-t-il été atteint ?
    * @return true si le but a été atteint, false sinon
    */    
    bool isGoalReached();
    /*!
    * \brief Calcule le chemin à parcourir pour atteindre un waypoint, et le stocke dans la variable m_path
    * @param a_waypoint point à atteindre en coordonnées globales
    */ 
    bool computePathToWaypoint(geometry_msgs::Pose a_waypoint);    
    /*!
    * \brief Réinitialise m_goal et m_path
    */     
    bool resetGoalAndPath() ;
    /*!
    * \brief la variable m_goal a-t-elle une valeur ? 
    * @return true si on a une valeur valide, false sinon
    */     
    bool isGoalSet();
    
};

#endif
