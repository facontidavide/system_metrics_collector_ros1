// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef SYSTEM_METRICS_COLLECTOR__PERIODIC_MEASUREMENT_NODE_HPP_
#define SYSTEM_METRICS_COLLECTOR__PERIODIC_MEASUREMENT_NODE_HPP_

#include <chrono>
#include <string>

#include <ros/ros.h>

#include "collector.hpp"
#include "metrics_message_publisher.hpp"


namespace system_metrics_collector
{

/**
 * Class which makes periodic measurements, using a ROS timer.
 */
class PeriodicMeasurementNode : public system_metrics_collector::Collector,
  public system_metrics_collector::MetricsMessagePublisher, public ros::NodeHandle
{
public:
  /**
   * Construct a PeriodicMeasurementNode.
   *
   * @param name the name of this node
   * @param topic the topic for publishing data
   * @param measurement_period
   * @param publish_period the window of active measurements
   * @param clear_measurements_on_publish whether all measurements should be cleared
   *        between subsequent publishing windows
   */
  PeriodicMeasurementNode(
    const std::string & name,
    const std::chrono::milliseconds measurement_period,
    const std::string & topic,  // todo @dbbonnie think about a default topic
    const std::chrono::milliseconds publish_period);

  virtual ~PeriodicMeasurementNode() = default;

  /**
   * Return a pretty printed status representation of this class
   *
   * @return a string detailing the current status
   */
  std::string getStatusString() const override;

protected:
  /**
   * Create ROS2 timers and a publisher for periodically triggering measurements
   * and publishing MetricsMessages
   *
   * @return if setup was successful
   */
  bool setupStart() override;

  /**
   * Stop the ROS2 timers that were created by setupStart()
   *
   * @return if teardown was successful
   */
  bool setupStop() override;

  /**
   * Track the starting time of the statistics
   */
  ros::Time window_start_;

  ros::Publisher publisher_;

private:
  /**
   * Override this method to perform a single measurement. This is called via performPeriodicMeasurement
   * with the period defined in the constructor.
   *
   * @return the measurement made to be aggregated for statistics
   */
  virtual double periodicMeasurement() = 0;

  /**
   * Called via a ROS2 timer per the measurement_period_. This calls periodicMeasurement
   * and adds the resulting output via Collector::acceptData(double data);
   */
  virtual void performPeriodicMeasurement();

  /**
   * Publish the statistics derived from the collected measurements (this is to be called via a
   * ROS2 timer per the publish_period)
   */
  void publishStatisticMessage() override;

  /**
   * Topic used for publishing
   */
  const std::string publishing_topic_;
  /**
   * The period used to take a single measurement
   */
  const std::chrono::milliseconds measurement_period_;
  /**
   * The period used to publish measurement data
   */
  const std::chrono::milliseconds publish_period_;

  ros::Timer measurement_timer_;
  ros::Timer publish_timer_;
};

}  // namespace system_metrics_collector

#endif  // SYSTEM_METRICS_COLLECTOR__PERIODIC_MEASUREMENT_NODE_HPP_
