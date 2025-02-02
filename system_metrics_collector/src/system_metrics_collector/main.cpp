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


#include <functional>

#include <chrono>
#include <memory>
#include <string>

#include <ros/ros.h>

#include "../../src/system_metrics_collector/linux_cpu_measurement_node.hpp"
#include "../../src/system_metrics_collector/linux_memory_measurement_node.hpp"
#include "../../src/system_metrics_collector/linux_process_memory_measurement_node.hpp"

namespace
{
constexpr const char STATISTICS_TOPIC_NAME[] = "system_metrics";
}

/**
 * This is current a "test" main in order to manually test the measurement nodes.
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char ** argv)
{
  ros::init(argc, argv);

  using namespace std::chrono_literals;
  const auto cpu_node = std::make_shared<system_metrics_collector::LinuxCpuMeasurementNode>(
    "linuxCpuCollector",
    1000ms,
    STATISTICS_TOPIC_NAME,
    60s);

  const auto mem_node = std::make_shared<system_metrics_collector::LinuxMemoryMeasurementNode>(
    "linuxMemoryCollector",
    1000ms,
    STATISTICS_TOPIC_NAME,
    60s);

  const auto process_mem_node =
    std::make_shared<system_metrics_collector::LinuxProcessMemoryMeasurementNode>(
    "linuxProcessMemoryCollector",
    1000ms,
    STATISTICS_TOPIC_NAME,
    60s);

  rclcpp::executors::MultiThreadedExecutor ex;
  cpu_node->start();
  mem_node->start();
  process_mem_node->start();

  {
    const auto r =
      rcutils_logging_set_logger_level(cpu_node->get_name(), RCUTILS_LOG_SEVERITY_DEBUG);
    if (r != 0) {
      ROS_ERROR_NAMED("main", "Unable to set debug logging for the cpu node: %s\n",
        rcutils_get_error_string().str);
    }
  }
  {
    const auto r =
      rcutils_logging_set_logger_level(mem_node->get_name(), RCUTILS_LOG_SEVERITY_DEBUG);
    if (r != 0) {
      ROS_ERROR_NAMED("main", "Unable to set debug logging for the memory node: %s\n",
        rcutils_get_error_string().str);
    }
  }
  {
    const auto r = rcutils_logging_set_logger_level(
      process_mem_node->get_name(), RCUTILS_LOG_SEVERITY_DEBUG);

    if (r != 0) {
      ROS_ERROR_NAMED("main",
        "Unable to set debug logging for the process memory node: %s\n",
        rcutils_get_error_string().str);
    }
  }

  ex.add_node(cpu_node);
  ex.add_node(mem_node);
  ex.add_node(process_mem_node);
  ex.spin();

  ros::shutdown();

  cpu_node->stop();
  mem_node->stop();
  process_mem_node->stop();

  return 0;
}
