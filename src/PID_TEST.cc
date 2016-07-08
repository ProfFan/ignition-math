/*
 * Copyright (C) 2016 Open Source Robotics Foundation
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

#include "ignition/math/PID.hh"
#include "ignition/math/Helpers.hh"

using namespace ignition;

/////////////////////////////////////////////////
TEST(PidTest, ConstructorDefault)
{
  const math::PID pid;
  EXPECT_DOUBLE_EQ(0.0, pid.PGain());
  EXPECT_DOUBLE_EQ(0.0, pid.IGain());
  EXPECT_DOUBLE_EQ(0.0, pid.DGain());
  EXPECT_DOUBLE_EQ(0.0, pid.IMax());
  EXPECT_DOUBLE_EQ(0.0, pid.IMin());
  EXPECT_DOUBLE_EQ(-1.0, pid.CmdMax());
  EXPECT_DOUBLE_EQ(0.0, pid.CmdMin());
  EXPECT_DOUBLE_EQ(0.0, pid.Cmd());

  double pe, ie, de;
  pid.Errors(pe, ie, de);
  EXPECT_DOUBLE_EQ(pe, 0.0);
  EXPECT_DOUBLE_EQ(ie, 0.0);
  EXPECT_DOUBLE_EQ(de, 0.0);
}

/////////////////////////////////////////////////
TEST(PidTest, SetValues)
{
  const math::PID pid2(1.0, 2.1, -4.5, 10.5, 1.4, 45, -35);
  EXPECT_DOUBLE_EQ(1.0,  pid2.PGain());
  EXPECT_DOUBLE_EQ(2.1,  pid2.IGain());
  EXPECT_DOUBLE_EQ(-4.5, pid2.DGain());
  EXPECT_DOUBLE_EQ(10.5, pid2.IMax());
  EXPECT_DOUBLE_EQ(1.4,  pid2.IMin());
  EXPECT_DOUBLE_EQ(45,   pid2.CmdMax());
  EXPECT_DOUBLE_EQ(-35,  pid2.CmdMin());
  EXPECT_DOUBLE_EQ(0.0,  pid2.Cmd());

  // Test Set*() functions
  {
    const double cmd = 10.4;
    math::PID pid;
    pid.SetPGain(pid2.PGain());
    pid.SetIGain(pid2.IGain());
    pid.SetDGain(pid2.DGain());
    pid.SetIMax(pid2.IMax());
    pid.SetIMin(pid2.IMin());
    pid.SetCmdMax(pid2.CmdMax());
    pid.SetCmdMin(pid2.CmdMin());
    pid.SetCmd(cmd);

    EXPECT_DOUBLE_EQ(pid.PGain(), pid2.PGain());
    EXPECT_DOUBLE_EQ(pid.IGain(), pid2.IGain());
    EXPECT_DOUBLE_EQ(pid.DGain(), pid2.DGain());
    EXPECT_DOUBLE_EQ(pid.IMax(), pid2.IMax());
    EXPECT_DOUBLE_EQ(pid.IMin(), pid2.IMin());
    EXPECT_DOUBLE_EQ(pid.CmdMax(), pid2.CmdMax());
    EXPECT_DOUBLE_EQ(pid.CmdMin(), pid2.CmdMin());
    EXPECT_DOUBLE_EQ(pid.Cmd(), cmd);
  }

  // Assignment operator
  {
    math::PID pid = pid2;
    EXPECT_DOUBLE_EQ(pid.PGain(), pid2.PGain());
    EXPECT_DOUBLE_EQ(pid.IGain(), pid2.IGain());
    EXPECT_DOUBLE_EQ(pid.DGain(), pid2.DGain());
    EXPECT_DOUBLE_EQ(pid.IMax(), pid2.IMax());
    EXPECT_DOUBLE_EQ(pid.IMin(), pid2.IMin());
    EXPECT_DOUBLE_EQ(pid.CmdMax(), pid2.CmdMax());
    EXPECT_DOUBLE_EQ(pid.CmdMin(), pid2.CmdMin());
    EXPECT_DOUBLE_EQ(pid.Cmd(), pid2.Cmd());
  }
}

/////////////////////////////////////////////////
TEST(PidTest, EqualOperatorCornerCase)
{
  math::PID pid(1.0, 2.1, -4.5, 10.5, 1.4, 45, -35);
  EXPECT_NEAR(pid.PGain(), 1.0, 1e-6);
  EXPECT_NEAR(pid.IGain(), 2.1, 1e-6);
  EXPECT_NEAR(pid.DGain(), -4.5, 1e-6);
  EXPECT_NEAR(pid.IMax(), 10.5, 1e-6);
  EXPECT_NEAR(pid.IMin(), 1.4, 1e-6);
  EXPECT_NEAR(pid.CmdMax(), 45.0, 1e-6);
  EXPECT_NEAR(pid.CmdMin(), -35.0, 1e-6);
  EXPECT_NEAR(pid.Cmd(), 0.0, 1e-6);

  pid = pid;

  EXPECT_NEAR(pid.PGain(), 1.0, 1e-6);
  EXPECT_NEAR(pid.IGain(), 2.1, 1e-6);
  EXPECT_NEAR(pid.DGain(), -4.5, 1e-6);
  EXPECT_NEAR(pid.IMax(), 10.5, 1e-6);
  EXPECT_NEAR(pid.IMin(), 1.4, 1e-6);
  EXPECT_NEAR(pid.CmdMax(), 45.0, 1e-6);
  EXPECT_NEAR(pid.CmdMin(), -35.0, 1e-6);
  EXPECT_NEAR(pid.Cmd(), 0.0, 1e-6);
}

/////////////////////////////////////////////////
TEST(PidTest, Update)
{
  math::PID pid;
  pid.Init(1.0, 0.1, 0.5, 10, 0, 20, -20);

  double result = pid.Update(5.0, std::chrono::duration<double>(0.0));
  EXPECT_NEAR(result, 0.0, 1e-6);

  result = pid.Update(5.0, std::chrono::duration<double>(10.0));
  EXPECT_NEAR(result, -10.25, 1e-6);

  double pe, ie, de;
  pid.Errors(pe, ie, de);
  EXPECT_NEAR(pe, 5, 1e-6);
  EXPECT_NEAR(ie, 50, 1e-6);
  EXPECT_NEAR(de, 0.5, 1e-6);

  // Test max integral term
  pid.SetIMax(0.2);
  pid.SetIGain(10.0);
  result = pid.Update(5.0, std::chrono::duration<double>(10.0));
  EXPECT_NEAR(result, -5.2, 1e-6);
  pid.Errors(pe, ie, de);
  EXPECT_NEAR(pe, 5, 1e-6);
  EXPECT_NEAR(ie, 0.02, 1e-6);
  EXPECT_NEAR(de, 0.0, 1e-6);

  // Test min integral term
  pid.SetIMax(20);
  pid.SetIMin(1.4);
  pid.SetIGain(0.01);
  result = pid.Update(5.0, std::chrono::duration<double>(10.0));
  EXPECT_NEAR(result, -6.4, 1e-6);
  pid.Errors(pe, ie, de);
  EXPECT_NEAR(pe, 5, 1e-6);
  EXPECT_NEAR(ie, 140, 1e-6);
  EXPECT_NEAR(de, 0.0, 1e-6);
}

/////////////////////////////////////////////////
TEST(PidTest, ZeroGains)
{
  // controller with zero gains, no command limits
  // should output only zero
  math::PID pid;
  EXPECT_DOUBLE_EQ(0, pid.Update(0,  std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(0, pid.Update(1,  std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(0, pid.Update(-1, std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(0, pid.Update(0,  std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(0, pid.Update(1,  std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(0, pid.Update(-1, std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(0, pid.Update(0,  std::chrono::duration<double>(-1)));
  EXPECT_DOUBLE_EQ(0, pid.Update(1,  std::chrono::duration<double>(-1)));
  EXPECT_DOUBLE_EQ(0, pid.Update(-1, std::chrono::duration<double>(-1)));

  // CmdMax defaults to -1.0
  // setting CmdMin to -10.0, means output should now be -1.0
  // when time is non-zero
  pid.SetCmdMin(-10.0);
  EXPECT_DOUBLE_EQ(-10.0, pid.CmdMin());
  EXPECT_DOUBLE_EQ(0, pid.Update(0,  std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(0, pid.Update(1,  std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(0, pid.Update(-1, std::chrono::duration<double>(0)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(0,  std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(1,  std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(-1, std::chrono::duration<double>(1)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(0,  std::chrono::duration<double>(-1)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(1,  std::chrono::duration<double>(-1)));
  EXPECT_DOUBLE_EQ(-1, pid.Update(-1, std::chrono::duration<double>(-1)));
}
