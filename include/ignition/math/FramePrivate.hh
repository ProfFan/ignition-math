/*
 * Copyright (C) 2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _IGNITION_FRAMEPRIVATE_HH_
#define _IGNITION_FRAMEPRIVATE_HH_

#include <string>
#include <ignition/math/Pose3.hh>

namespace ignition
{
  namespace math
  {
    class FramePrivate
    {
      public: FramePrivate() {}
      public: FramePrivate(const std::string &_name, const Pose3d &_pose)
             :name(_name), pose(_pose) {}

      public: std::string name;
      public: Pose3d pose;
    };
  }
}
#endif