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

#ifndef MOVING_AVERAGE_STATISTICS__TYPES_HPP_
#define MOVING_AVERAGE_STATISTICS__TYPES_HPP_

#include <cmath>
#include <sstream>
#include <string>

namespace moving_average_statistics
{

/**
 *  A container for statistics data results for a set of recorded observations.
 */
struct StatisticData
{
  double average = std::nan("");
  double min = std::nan("");
  double max = std::nan("");
  double standard_deviation = std::nan("");
  uint64_t sample_count = 0;
};

/**
 * Function which pretty prints the contents of a StatisticData struct.
 *
 * @param results the StatisticData to pretty print
 * @return std::string formatted struct contents in an easily readable format, e.g.,
 * /"avg=1, min=2, max=3, std_dev=4, count=5/"
 */
std::string statisticsDataToString(const StatisticData & results);

}  // namespace moving_average_statistics

#endif  // MOVING_AVERAGE_STATISTICS__TYPES_HPP_
