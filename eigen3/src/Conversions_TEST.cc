/*
 * Copyright (C) 2018 Open Source Robotics Foundation
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


#include <gtest/gtest.h>

#include <ignition/math/eigen3/Conversions.hh>

/////////////////////////////////////////////////
/// Check Vector3 conversions
TEST(EigenConversions, ConvertVector3)
{
  {
    ignition::math::Vector3d iVec, iVec2;
    Eigen::Vector3d eVec = ignition::math::eigen3::convert(iVec);
    EXPECT_DOUBLE_EQ(0, eVec[0]);
    EXPECT_DOUBLE_EQ(0, eVec[1]);
    EXPECT_DOUBLE_EQ(0, eVec[2]);
    iVec2 = ignition::math::eigen3::convert(eVec);
    EXPECT_EQ(iVec, iVec2);
  }

  {
    ignition::math::Vector3d iVec(100.5, -2.314, 42), iVec2;
    Eigen::Vector3d eVec = ignition::math::eigen3::convert(iVec);
    EXPECT_DOUBLE_EQ(iVec[0], eVec[0]);
    EXPECT_DOUBLE_EQ(iVec[1], eVec[1]);
    EXPECT_DOUBLE_EQ(iVec[2], eVec[2]);
    iVec2 = ignition::math::eigen3::convert(eVec);
    EXPECT_EQ(iVec, iVec2);
  }
}

/////////////////////////////////////////////////
/// Check Quaternion conversions
TEST(EigenConversions, ConvertQuaternion)
{
  {
    ignition::math::Quaterniond iQuat, iQuat2;
    Eigen::Quaterniond eQuat = ignition::math::eigen3::convert(iQuat);
    EXPECT_DOUBLE_EQ(1, eQuat.w());
    EXPECT_DOUBLE_EQ(0, eQuat.x());
    EXPECT_DOUBLE_EQ(0, eQuat.y());
    EXPECT_DOUBLE_EQ(0, eQuat.z());
    iQuat2 = ignition::math::eigen3::convert(eQuat);
    EXPECT_EQ(iQuat, iQuat2);
  }

  {
    ignition::math::Quaterniond iQuat(0.1, 0.2, 0.3), iQuat2;
    Eigen::Quaterniond eQuat = ignition::math::eigen3::convert(iQuat);
    EXPECT_DOUBLE_EQ(iQuat.W(), eQuat.w());
    EXPECT_DOUBLE_EQ(iQuat.X(), eQuat.x());
    EXPECT_DOUBLE_EQ(iQuat.Y(), eQuat.y());
    EXPECT_DOUBLE_EQ(iQuat.Z(), eQuat.z());
    iQuat2 = ignition::math::eigen3::convert(eQuat);
    EXPECT_EQ(iQuat, iQuat2);
  }
}

/////////////////////////////////////////////////
/// Check Matrix3 conversions
TEST(EigenConversions, ConvertMatrix3)
{
  {
    ignition::math::Matrix3d iMat, iMat2;
    Eigen::Matrix3d eMat = ignition::math::eigen3::convert(iMat);
    EXPECT_DOUBLE_EQ(0, eMat(0, 0));
    EXPECT_DOUBLE_EQ(0, eMat(0, 1));
    EXPECT_DOUBLE_EQ(0, eMat(0, 2));
    EXPECT_DOUBLE_EQ(0, eMat(1, 0));
    EXPECT_DOUBLE_EQ(0, eMat(1, 1));
    EXPECT_DOUBLE_EQ(0, eMat(1, 2));
    EXPECT_DOUBLE_EQ(0, eMat(2, 0));
    EXPECT_DOUBLE_EQ(0, eMat(2, 1));
    EXPECT_DOUBLE_EQ(0, eMat(2, 2));
    iMat2 = ignition::math::eigen3::convert(eMat);
    EXPECT_EQ(iMat, iMat2);
  }


  {
    ignition::math::Matrix3d iMat(1, 2, 3, 4, 5, 6, 7, 8, 9), iMat2;
    Eigen::Matrix3d eMat = ignition::math::eigen3::convert(iMat);
    EXPECT_DOUBLE_EQ(iMat(0, 0), eMat(0, 0));
    EXPECT_DOUBLE_EQ(iMat(0, 1), eMat(0, 1));
    EXPECT_DOUBLE_EQ(iMat(0, 2), eMat(0, 2));
    EXPECT_DOUBLE_EQ(iMat(1, 0), eMat(1, 0));
    EXPECT_DOUBLE_EQ(iMat(1, 1), eMat(1, 1));
    EXPECT_DOUBLE_EQ(iMat(1, 2), eMat(1, 2));
    EXPECT_DOUBLE_EQ(iMat(2, 0), eMat(2, 0));
    EXPECT_DOUBLE_EQ(iMat(2, 1), eMat(2, 1));
    EXPECT_DOUBLE_EQ(iMat(2, 2), eMat(2, 2));
    iMat2 = ignition::math::eigen3::convert(eMat);
    EXPECT_EQ(iMat, iMat2);
  }
}

/////////////////////////////////////////////////
/// Check Pose conversions
TEST(EigenConversions, ConvertPose3)
{
  {
    ignition::math::Pose3d iPose, iPose2;
    Eigen::Isometry3d ePose = ignition::math::eigen3::convert(iPose);
    Eigen::Vector3d eVec = ePose.translation();
    EXPECT_DOUBLE_EQ(0, eVec[0]);
    EXPECT_DOUBLE_EQ(0, eVec[1]);
    EXPECT_DOUBLE_EQ(0, eVec[2]);
    Eigen::Quaterniond eQuat(ePose.linear());
    EXPECT_DOUBLE_EQ(1, eQuat.w());
    EXPECT_DOUBLE_EQ(0, eQuat.x());
    EXPECT_DOUBLE_EQ(0, eQuat.y());
    EXPECT_DOUBLE_EQ(0, eQuat.z());
    iPose2 = ignition::math::eigen3::convert(ePose);
    EXPECT_EQ(iPose, iPose2);
  }

  {
    ignition::math::Pose3d iPose(105.4, 3.1, -0.34, 3.14/8, 3.14/16, -3.14/2);
    ignition::math::Pose3d iPose2;
    Eigen::Isometry3d ePose = ignition::math::eigen3::convert(iPose);
    Eigen::Vector3d eVec = ePose.translation();
    EXPECT_DOUBLE_EQ(iPose.Pos()[0], eVec[0]);
    EXPECT_DOUBLE_EQ(iPose.Pos()[1], eVec[1]);
    EXPECT_DOUBLE_EQ(iPose.Pos()[2], eVec[2]);
    Eigen::Quaterniond eQuat(ePose.linear());
    EXPECT_DOUBLE_EQ(iPose.Rot().W(), eQuat.w());
    EXPECT_DOUBLE_EQ(iPose.Rot().X(), eQuat.x());
    EXPECT_DOUBLE_EQ(iPose.Rot().Y(), eQuat.y());
    EXPECT_DOUBLE_EQ(iPose.Rot().Z(), eQuat.z());
    iPose2 = ignition::math::eigen3::convert(ePose);
    EXPECT_EQ(iPose, iPose2);
  }
}
