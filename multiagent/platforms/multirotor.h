
#ifndef   _PLATFORM_MULTIROTOR_H_
#define   _PLATFORM_MULTIROTOR_H_

#define MADARA_NO_THREAD_LOCAL

#include "gams/platforms/BasePlatform.h"
#include "gams/platforms/PlatformFactory.h"
#include "madara/threads/Threader.h"
#include "gams/pose/GPSFrame.h"
#include "gams/pose/CartesianFrame.h"

namespace msr {
    namespace airlib {
        class MultirotorRpcLibClient;
    }
}

namespace platforms
{        
  /**
  * A custom platform generated by gpc.pl
  **/
  class multirotor : public gams::platforms::BasePlatform
  {
  public:
    /**
     * Constructor
     * @param  knowledge  context containing variables and values
     * @param  sensors    map of sensor names to sensor information
     * @param  self       self referencing variables for the agent
     **/
    multirotor (
      madara::knowledge::KnowledgeBase * knowledge = 0,
      gams::variables::Sensors * sensors = 0,
      gams::variables::Self * self = 0);

    /**
     * Destructor
     **/
    virtual ~multirotor ();

    /**
     * Polls the sensor environment for useful information. Required.
     * @return number of sensors updated/used
     **/
    virtual int sense (void);

    /**
     * Analyzes platform information. Required.
     * @return bitmask status of the platform. @see PlatformAnalyzeStatus.
     **/
    virtual int analyze (void);

    /**
     * Gets the name of the platform. Required.
     **/
    virtual std::string get_name () const;

    /**
     * Gets the unique identifier of the platform. This should be an
     * alphanumeric identifier that can be used as part of a MADARA
     * variable (e.g. vrep_ant, autonomous_snake, etc.) Required.
     * @return the id of the platform to use in factory methods
     **/
    virtual std::string get_id () const;

    /**
     * Gets the position accuracy in meters. Optional.
     * @return position accuracy
     **/
    virtual double get_accuracy (void) const;

    /**
     * Gets sensor radius. Optional.
     * @return minimum radius of all available sensors for this platform
     **/
    virtual double get_min_sensor_range (void) const;

    /**
     * Gets move speed. Optional.
     * @return speed in meters per second
     **/
    virtual double get_move_speed (void) const;

    /**
     * Instructs the agent to return home. Optional.
     * @return the status of the home operation, @see PlatformReturnValues
     **/
    virtual int home (void);

    /**
     * Instructs the agent to land. Optional.
     * @return the status of the land operation, @see PlatformReturnValues
     **/
    virtual int land (void);

    virtual int move (const gams::pose::Position & location,
      const gams::platforms::PositionBounds &bounds);

    /**
     * Rotates the platform to match a given angle. Optional.
     * @param   target    the rotation to move to
     * @param   epsilon   approximation value
     * @return the status of the rotate, @see PlatformReturnValues
     **/
    virtual int rotate (const gams::pose::Orientation & target,
      double epsilon = M_PI/16);

    // /**
    //  * Moves the platform to a pose (location and rotation). Optional.
    //  *
    //  * This default implementation calls move and rotate with the
    //  * Location and Rotation portions of the target Pose. The return value
    //  * is composed as follows: if either call returns ERROR (0), this call
    //  * also returns ERROR (0). Otherwise, if BOTH calls return ARRIVED (2),
    //  * this call also returns ARRIVED (2). Otherwise, this call returns
    //  * MOVING (1)
    //  *
    //  * Overrides might function differently.
    //  *
    //  * @param   target        the coordinates to move to
    //  * @param   loc_epsilon   approximation value for the location
    //  * @param   rot_epsilon   approximation value for the rotation
    //  * @return the status of the operation, @see PlatformReturnValues
    //  **/
    // virtual int pose (const gams::pose::Pose & target,
    //   double loc_epsilon = 0.1, double rot_epsilon = M_PI/16);

    /**
     * Pauses movement, keeps source and dest at current values. Optional.
     **/
    virtual void pause_move (void);

    /**
     * Set move speed. Optional.
     * @param speed new speed in meters/second
     **/
    virtual void set_move_speed (const double& speed);

    /**
     * Stops movement, resetting source and dest to current location.
     * Optional.
     **/
    virtual void stop_move (void);

    /**
     * Instructs the agent to take off. Optional.
     * @return the status of the takeoff, @see PlatformReturnValues
     **/
    virtual int takeoff (void);
    
    /**
     * Returns the world reference frame for the platform (e.g. GPS or cartesian)
     **/
    virtual const gams::pose::ReferenceFrame & get_frame (void) const;
    
  private:

    msr::airlib::MultirotorRpcLibClient* client_;
    std::string vehicle_name_;

    // a threader for managing platform threads
    madara::threads::Threader threader_;    
  }; // end multirotor class
    

  /**
   * A factory class for creating multirotor platforms
   **/
  class multirotorFactory : public gams::platforms::PlatformFactory
  {
  public:
    /**
     * Creates a multirotor platform.
     * @param   args      no arguments are necessary for this platform
     * @param   knowledge the knowledge base. This will be set by the
     *                    controller in init_vars.
     * @param   sensors   the sensor info. This will be set by the
     *                    controller in init_vars.
     * @param   platforms status inform for all known agents. This
     *                    will be set by the controller in init_vars
     * @param   self      self-referencing variables. This will be
     *                    set by the controller in init_vars
     **/
    virtual gams::platforms::BasePlatform * create (
      const madara::knowledge::KnowledgeMap & args,
      madara::knowledge::KnowledgeBase * knowledge,
      gams::variables::Sensors * sensors,
      gams::variables::Platforms * platforms,
      gams::variables::Self * self);
  };
    
} // end platforms namespace

#endif // _PLATFORM_MULTIROTOR_H_