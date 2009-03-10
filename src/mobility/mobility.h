/**
 * \addtogroup mobility Mobility
 *
 * The mobility support includes:
 *  - a set of mobility models which are used to track and maintain
 *    the "current" cartesian position and speed of an object.
 *
 *  - a "course change notifier" trace which can be used to register
 *    listeners to the course changes of a mobility model
 *
 * The mobility models themselves are:
 *   - ns3::ConstantPositionMobilityModel: a model which maintains a constant position
 *     until it is changed by the user.
 *
 *   - ns3::ConstantVelocityMobilityModel: a model which maintains a constant speed
 *     until it is changed by the user.
 *
 *   - ns3::HierarchicalMobilityModel: a model which calculates the current 
 *     absolute position from a "reference" (parent) mobility model
 *     and a "relative" (child) mobility model. This allows users to
 *     compose mobility models.
 *
 *   - ns3::RandomWalk2dMobilityModel: a 2d "brownian" motion mobility model
 *     where the bounds of the mobility area are a rectangle.
 *
 *   - ns3::RandomWaypointMobilityModel: a 3d random waypoint mobility model.
 *
 *   - ns3::RandomDirection2dMobilityModel: a 2d random direction mobility
 *     model where the bounds of the mobility are are a rectangle.
 *
 */
