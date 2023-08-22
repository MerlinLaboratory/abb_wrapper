/* TASK SEQUENCER - Contains all recepies for grasping, moving and other tasks
Authors: George Jose Pollayil - Mathew Jose Pollayil - Stefano Angeli
Email: gpollayil@gmail.com, mathewjosepollayil@gmail.com, stefano.angeli@ing.unipi.it */

// Basic Includes
#include "ros/ros.h"
#include "Eigen/Dense"
#include <eigen_conversions/eigen_msg.h>

#include <tf/transform_listener.h>
#include <geometry_msgs/Pose.h>
#include <tf_conversions/tf_eigen.h>

// ROS Service and Message Includes
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include "std_srvs/SetBool.h"

// MoveIt
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

// Custom Includes
#include "Abb_client.h"

//
#include <abb_wrapper_control/ArmControl_trial.h>
// Other Includes

// Defines
#define     DEBUG   1       // Prints out additional stuff
#define     VISUAL          // Publishes visual info on RViz

class TaskSequencer {

    /// public variables and functions ------------------------------------------------------------
	public:
		TaskSequencer(ros::NodeHandle& nh_);

        ~TaskSequencer();

        // get last joint position values

        std::vector<double> getLastJointPosValues(trajectory_msgs::JointTrajectory& traj);

        // Parameters parsing
        bool parse_task_params();

        // Convert xyzrpy vector to geometry_msgs Pose
        geometry_msgs::Pose convert_vector_to_pose(std::vector<double> input_vec);

        // Callback for simple grasp task service
        bool call_simple_grasp_task(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);
        
        // Callback for simple home task service
        bool call_simple_home_task(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res);
    /// private variables -------------------------------------------------------------------------
	private:
        
        ArmControlTrial* actionClient_;
		ros::NodeHandle nh;

        // The Abb Client
        AbbClient abb_client;

        // Service names
        std::string grasp_task_service_name;
        std::string home_task_service_name;

        // Service Servers
        ros::ServiceServer grasp_task_server;
        ros::ServiceServer home_task_server;

        // Parsed task sequence variables
        std::vector<double> home_joints;
        std::vector<double> joint_pos_A;
        std::vector<double> grasp_transform;
        geometry_msgs::Pose grasp_T;
        std::vector<double> pre_grasp_transform;
        geometry_msgs::Pose pre_grasp_T;
 
        // MoveIt stuff and functions for FK and IK
        std::string group_name;
        std::string end_effector_name;
        std::shared_ptr<robot_model_loader::RobotModelLoader> robot_model_loader_ptr;
        robot_model::RobotModelPtr kinematic_model;
        robot_state::RobotStatePtr kinematic_state;

        // FK and IK Functions which makes use of MoveIt
        geometry_msgs::Pose performFK(std::vector<double> joints_in);
        bool performIK(geometry_msgs::Pose pose_in, double timeout, std::vector<double>& joints_out);

        // Other execution vars
        ros::Duration waiting_time;
        trajectory_msgs::JointTrajectory tmp_traj;
        trajectory_msgs::JointTrajectory tmp_traj_arm;
        std::vector<double> null_joints; 
        std::vector<double> now_joints;   
        std::vector<moveit::core::JointModel *> number_of_active_joints;                        // null joints in order to make joint plan from present joints     
};