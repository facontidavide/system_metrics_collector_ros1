cmake_minimum_required(VERSION 3.5)
project(system_metrics_collector)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Enable strict compiler flags if possible.
include(CheckCXXCompilerFlag)
set(FLAGS -pedantic -Wno-long-long -Wall -Wextra -Wcast-qual -Wformat -Wwrite-strings -Wcast-align
  -Wno-error=cast-align -Wmissing-declarations)
foreach(FLAG ${FLAGS})
  check_cxx_compiler_flag(${FLAG} R${FLAG})
  if(${R${FLAG}})
    set(WARNING_CXX_FLAGS "${WARNING_CXX_FLAGS} ${FLAG}")
  endif()
endforeach()

if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

# find dependencies
find_package(catkin REQUIRED COMPONENTS
   std_msgs
   metrics_statistics_msgs
   roscpp)

include_directories(${catkin_INCLUDE_DIRS})

add_library(system_metrics_collector
  src/moving_average_statistics/moving_average.cpp
  src/moving_average_statistics/types.cpp
  src/moving_average_statistics/types.hpp
#  src/system_metrics_collector/metrics_message_publisher.cpp
  src/system_metrics_collector/collector.cpp
  src/system_metrics_collector/linux_cpu_measurement_node.cpp
  src/system_metrics_collector/linux_memory_measurement_node.cpp
  src/system_metrics_collector/linux_process_memory_measurement_node.cpp
#  src/system_metrics_collector/periodic_measurement_node.cpp
  src/system_metrics_collector/proc_cpu_data.cpp
  src/system_metrics_collector/utilities.cpp
)

#add_executable(main src/system_metrics_collector/main.cpp)
#target_link_libraries(main system_metrics_collector)
