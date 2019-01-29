#include "madara/knowledge/containers/NativeDoubleVector.h"
#include "gams/pose/Euler.h"
#include "multirotor.h"

#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>

#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"

// factory class for creating a multirotor
gams::platforms::BasePlatform *
platforms::multirotorFactory::create(
    const madara::knowledge::KnowledgeMap &args,
    madara::knowledge::KnowledgeBase *knowledge,
    gams::variables::Sensors *sensors,
    gams::variables::Platforms *platforms,
    gams::variables::Self *self)
{
  return new multirotor(knowledge, sensors, self);
}

// Constructor
platforms::multirotor::multirotor(
    madara::knowledge::KnowledgeBase *knowledge,
    gams::variables::Sensors *sensors,
    gams::variables::Self *self)
    : gams::platforms::BasePlatform(knowledge, sensors, self)
{

  vehicle_name_ = "agent." + std::to_string(self_->id.to_integer());
  std::cout << "vehicle_name: " << vehicle_name_ << std::endl;

  client_ = new msr::airlib::MultirotorRpcLibClient();

  // as an example of what to do here, create a coverage sensor
  if (knowledge && sensors)
  {
    // set the data plane for the threader
    threader_.set_data_plane(*knowledge);

    // create a coverage sensor
    gams::variables::Sensors::iterator it = sensors->find("coverage");
    if (it == sensors->end()) // create coverage sensor
    {
      // get origin
      gams::pose::Position origin(gams::pose::gps_frame());
      madara::knowledge::containers::NativeDoubleArray origin_container;
      origin_container.set_name("sensor.coverage.origin", *knowledge, 3);
      origin.from_container(origin_container);

      // establish sensor
      gams::variables::Sensor *coverage_sensor =
          new gams::variables::Sensor("coverage", knowledge, 2.5, origin);
      (*sensors)["coverage"] = coverage_sensor;
    }
    (*sensors_)["coverage"] = (*sensors)["coverage"];
    status_.init_vars(*knowledge, get_id());

    // create threads
    // end create threads

    client_->confirmConnection();

    client_->enableApiControl(true, vehicle_name_);

    client_->armDisarm(true, vehicle_name_);
    client_->takeoffAsync(5, vehicle_name_);
    std::this_thread::sleep_for(std::chrono::duration<double>(5));
    client_->enableApiControl(true, vehicle_name_);

    status_.movement_available = 1;
  }
}

// Destructor
platforms::multirotor::~multirotor()
{
  threader_.terminate();
  threader_.wait();
  delete client_;
}

// Polls the sensor environment for useful information. Required.
int platforms::multirotor::sense(void)
{
  auto location = client_->simGetVehiclePose(vehicle_name_).position;

  madara_logger_ptr_log(gams::loggers::global_logger.get(),
                        gams::loggers::LOG_MAJOR,
                        "SIMBOTIC position: %f,%f,%f\n", location.x() / 100.0, location.y() / 100.0,  location.z() / 100.0);

  gams::pose::Position loc(get_frame(), location.x() / 100.0, location.y() / 100.0, 0);

  loc.to_container(self_->agent.location);
  self_->agent.orientation.set(0, 0.0);
  self_->agent.orientation.set(1, 0.0);
  self_->agent.orientation.set(2, 0.0);

  status_.waiting = 0;
  status_.moving = 1;

  return gams::platforms::PLATFORM_OK;
}

// Analyzes platform information. Required.
int platforms::multirotor::analyze(void)
{
  return gams::platforms::PLATFORM_OK;
}

// Gets the name of the platform. Required.
std::string
platforms::multirotor::get_name(void) const
{
  return "multirotor";
}

// Gets the unique identifier of the platform.
std::string
platforms::multirotor::get_id(void) const
{
  return "multirotor";
}

// Gets the position accuracy in meters. Optional.
double
platforms::multirotor::get_accuracy(void) const
{
  // will depend on your localization capabilities for robotics
  return 5.0;
}

// Gets sensor radius. Optional.
double
platforms::multirotor::get_min_sensor_range(void) const
{
  // should be in square meters
  return 10.0;
}

// Gets move speed. Optional.
double
platforms::multirotor::get_move_speed(void) const
{
  // should be in meters/s
  return 3.0;
}

// Instructs the agent to return home. Optional.
int platforms::multirotor::home(void)
{
  /**
   * Movement functions work in a polling manner. In a real implementation,
   * we would want to check a finite state machine, external thread or
   * platform status to determine what to return. For now, we will simply
   * return that we are in the process of moving to the final pose.
   **/
  client_->goHomeAsync(Utils::max<float>(), vehicle_name_);
  return gams::platforms::PLATFORM_IN_PROGRESS;
}

// Instructs the agent to land. Optional.
int platforms::multirotor::land(void)
{
  /**
   * Movement functions work in a polling manner. In a real implementation,
   * we would want to check a finite state machine, external thread or
   * platform status to determine what to return. For now, we will simply
   * return that we are in the process of moving to the final pose.
   **/
  client_->landAsync(60, vehicle_name_);
  return gams::platforms::PLATFORM_IN_PROGRESS;
}

// Moves the platform to a location. Optional.
int platforms::multirotor::move(
    const gams::pose::Position &location,
    const gams::platforms::PositionBounds &bounds)
{
  /**
   * Movement functions work in a polling manner. In a real implementation,
   * we would want to check a finite state machine, external thread or
   * platform status to determine what to return. For now, we will simply
   * return that we are in the process of moving to the final pose.
   **/
  client_->enableApiControl(true, vehicle_name_);
  client_->moveToPositionAsync(location.x(), location.y(), -40.0, 10,
                               Utils::max<float>(), msr::airlib::DrivetrainType::MaxDegreeOfFreedom,
                               YawMode(), -1, 1, vehicle_name_);
  // client_->hoverAsync(vehicle_name_)->waitOnLastTask();
  return BasePlatform::move(location, bounds);
}

// Rotates the platform to match a given angle. Optional.
int platforms::multirotor::rotate(
    const gams::pose::Orientation &target,
    double epsilon)
{
  /**
   * Movement functions work in a polling manner. In a real implementation,
   * we would want to check a finite state machine, external thread or
   * platform status to determine what to return. For now, we will simply
   * return that we are in the process of moving to the final pose.
   **/
  gams::pose::euler::EulerXYZ e(target);
  client_->rotateToYawAsync(e.c(), Utils::max<float>(), 5, vehicle_name_);
  return gams::platforms::PLATFORM_MOVING;
}

// Pauses movement, keeps source and dest at current values. Optional.
void platforms::multirotor::pause_move(void)
{
}

// Set move speed. Optional.
void platforms::multirotor::set_move_speed(const double &speed)
{
}

// Stops movement, resetting source and dest to current location. Optional.
void platforms::multirotor::stop_move(void)
{
}

// Instructs the agent to take off. Optional.
int platforms::multirotor::takeoff(void)
{
  client_->takeoffAsync(20, vehicle_name_)->waitOnLastTask();
  return gams::platforms::PLATFORM_OK;
}

const gams::pose::ReferenceFrame &
platforms::multirotor::get_frame(void) const
{
  // For cartesian, replace with gams::pose::default_frame()
  return gams::pose::gps_frame();
}

