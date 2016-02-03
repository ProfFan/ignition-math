/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>

#include "ignition/math/Rand.hh"
#include "ignition/math/Vector3.hh"
#include "ignition/math/Helpers.hh"

using namespace ignition;

/////////////////////////////////////////////////
// Test a few function in Helpers
TEST(HelpersTest, Helpers)
{
  EXPECT_EQ(12345, math::parseInt("12345"));
  EXPECT_EQ(-12345, math::parseInt("-12345"));
  EXPECT_EQ(-12345, math::parseInt("    -12345"));
  EXPECT_EQ(0, math::parseInt("    "));

  EXPECT_EQ(math::NAN_I, math::parseInt(""));
  EXPECT_EQ(math::NAN_I, math::parseInt("?"));
  EXPECT_EQ(math::NAN_I, math::parseInt("23ab67"));

  EXPECT_DOUBLE_EQ(12.345, math::parseFloat("12.345"));
  EXPECT_DOUBLE_EQ(-12.345, math::parseFloat("-12.345"));
  EXPECT_DOUBLE_EQ(-12.345, math::parseFloat("    -12.345"));
  EXPECT_DOUBLE_EQ(0.0, math::parseFloat("    "));
  EXPECT_TRUE(math::equal(123.45, math::parseFloat("1.2345e2"), 1e-2));
  EXPECT_TRUE(math::equal(123.45, math::parseFloat("1.2345e+2"), 1e-2));
  EXPECT_TRUE(math::equal(123.45, math::parseFloat("1.2345e+002"), 1e-2));
  EXPECT_TRUE(math::equal(.012345, math::parseFloat("1.2345e-2"), 1e-2));
  EXPECT_TRUE(math::equal(.012345, math::parseFloat("1.2345e-002"), 1e-2));
  EXPECT_TRUE(math::equal(1.2345, math::parseFloat("1.2345e+"), 1e-2));
  EXPECT_TRUE(math::equal(1.2345, math::parseFloat("1.2345e-"), 1e-2));
  EXPECT_DOUBLE_EQ(1.2345, math::parseFloat("1.2345e+0"));

  EXPECT_TRUE(math::isnan(math::parseFloat("")));
  EXPECT_TRUE(math::isnan(math::parseFloat("?")));
  EXPECT_TRUE(math::isnan(math::parseFloat("23ab67")));

  EXPECT_EQ(1u, math::roundUpPowerOfTwo(0));
  EXPECT_EQ(1u, math::roundUpPowerOfTwo(1));
  EXPECT_EQ(2u, math::roundUpPowerOfTwo(2));
  EXPECT_EQ(2048u, math::roundUpPowerOfTwo(1025));
}

/////////////////////////////////////////////////
// Test Helpers::precision
TEST(HelpersTest, Precision)
{
  EXPECT_DOUBLE_EQ(0, math::precision(0.0, 1));
  EXPECT_DOUBLE_EQ(0.1, math::precision(0.1, 1));
  EXPECT_DOUBLE_EQ(0.1, math::precision(0.14, 1));
  EXPECT_DOUBLE_EQ(0.2, math::precision(0.15, 1));
  EXPECT_DOUBLE_EQ(0.15, math::precision(0.15, 2));

  EXPECT_DOUBLE_EQ(1, math::precision(1.4, 0));
  EXPECT_EQ(0, math::precision(0, 0));
}

/////////////////////////////////////////////////
// Test Helpers::isPowerOfTwo
TEST(HelpersTest, PowerOfTwo)
{
  EXPECT_FALSE(math::isPowerOfTwo(0));
  EXPECT_FALSE(math::isPowerOfTwo(3));

  EXPECT_TRUE(math::isPowerOfTwo(1));

  EXPECT_TRUE(math::isPowerOfTwo(2));
  EXPECT_TRUE(math::isPowerOfTwo(4));
}

// MSVC report errors on division by zero
#ifndef _MSC_VER
/////////////////////////////////////////////////
// Test Helpers::fixnan functions
TEST(HelpersTest, FixNaN)
{
  EXPECT_DOUBLE_EQ(math::fixnan(1.0 / 0.0), 0.0);
  EXPECT_DOUBLE_EQ(math::fixnan(-1.0 / 0.0), 0.0);
  EXPECT_DOUBLE_EQ(math::fixnan(0.0 / 0.0), 0.0);

  EXPECT_DOUBLE_EQ(math::fixnan(42.0), 42.0);
  EXPECT_DOUBLE_EQ(math::fixnan(-42.0), -42.0);

  EXPECT_FLOAT_EQ(math::fixnan(1.0f / 0.0f), 0.0f);
  EXPECT_FLOAT_EQ(math::fixnan(-1.0f / 0.0f), 0.0f);
  EXPECT_FLOAT_EQ(math::fixnan(0.0f / 0.0f), 0.0f);

  EXPECT_FLOAT_EQ(math::fixnan(42.0f), 42.0f);
  EXPECT_FLOAT_EQ(math::fixnan(-42.0f), -42.0f);
}
#endif

/////////////////////////////////////////////////
// Even test
TEST(HelpersTest, Even)
{
  int i = 1;
  signed s = 1;
  signed int si = 1;
  unsigned u = 1;
  unsigned int ui = 1;

  EXPECT_FALSE(math::isEven(i));
  EXPECT_FALSE(math::isEven(s));
  EXPECT_FALSE(math::isEven(si));
  EXPECT_FALSE(math::isEven(u));
  EXPECT_FALSE(math::isEven(ui));

  i = -1;
  s = -1;
  si = -1;

  EXPECT_FALSE(math::isEven(i));
  EXPECT_FALSE(math::isEven(s));
  EXPECT_FALSE(math::isEven(si));

  i = 4;
  s = 4;
  si = 4;
  u = 4;
  ui = 4;

  EXPECT_TRUE(math::isEven(i));
  EXPECT_TRUE(math::isEven(s));
  EXPECT_TRUE(math::isEven(si));
  EXPECT_TRUE(math::isEven(u));
  EXPECT_TRUE(math::isEven(ui));

  i = -2;
  s = -2;
  si = -2;

  EXPECT_TRUE(math::isEven(i));
  EXPECT_TRUE(math::isEven(s));
  EXPECT_TRUE(math::isEven(si));

  i = 0;
  s = 0;
  si = 0;
  u = 0;
  ui = 0;

  EXPECT_TRUE(math::isEven(i));
  EXPECT_TRUE(math::isEven(s));
  EXPECT_TRUE(math::isEven(si));
  EXPECT_TRUE(math::isEven(u));
  EXPECT_TRUE(math::isEven(ui));
}

/////////////////////////////////////////////////
// Odd test
TEST(HelpersTest, Odd)
{
  int i = 1;
  signed s = 1;
  signed int si = 1;
  unsigned u = 1;
  unsigned int ui = 1;

  EXPECT_TRUE(math::isOdd(i));
  EXPECT_TRUE(math::isOdd(s));
  EXPECT_TRUE(math::isOdd(si));
  EXPECT_TRUE(math::isOdd(u));
  EXPECT_TRUE(math::isOdd(ui));

  i = -1;
  s = -1;
  si = -1;

  EXPECT_TRUE(math::isOdd(i));
  EXPECT_TRUE(math::isOdd(s));
  EXPECT_TRUE(math::isOdd(si));

  i = 4;
  s = 4;
  si = 4;
  u = 4;
  ui = 4;

  EXPECT_FALSE(math::isOdd(i));
  EXPECT_FALSE(math::isOdd(s));
  EXPECT_FALSE(math::isOdd(si));
  EXPECT_FALSE(math::isOdd(u));
  EXPECT_FALSE(math::isOdd(ui));

  i = -2;
  s = -2;
  si = -2;

  EXPECT_FALSE(math::isOdd(i));
  EXPECT_FALSE(math::isOdd(s));
  EXPECT_FALSE(math::isOdd(si));

  i = 0;
  s = 0;
  si = 0;
  u = 0;
  ui = 0;

  EXPECT_FALSE(math::isOdd(i));
  EXPECT_FALSE(math::isOdd(s));
  EXPECT_FALSE(math::isOdd(si));
  EXPECT_FALSE(math::isOdd(u));
  EXPECT_FALSE(math::isOdd(ui));
}

/////////////////////////////////////////////////
TEST(HelpersTest, Sort)
{
  {
    int a = 2;
    int b = -1;
    math::sort2(a, b);
    EXPECT_LE(a, b);
  }

  {
    int a = 0;
    int b = 1;
    math::sort2(a, b);
    EXPECT_LE(a, b);
  }

  {
    int a = 2;
    int b = -1;
    int c = 0;
    math::sort3(a, b, c);
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);
  }

  {
    unsigned int a = 2;
    unsigned int b = 1;
    math::sort2(a, b);
    EXPECT_LE(a, b);
  }

  {
    unsigned int a = 2;
    unsigned int b = 1;
    unsigned int c = 0;
    math::sort3(a, b, c);
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);
  }
  {
    unsigned int a = 0;
    unsigned int b = 1;
    unsigned int c = 2;
    math::sort3(a, b, c);
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);
  }


  {
    float a = 2.1f;
    float b = -1.1e-1f;
    math::sort2(a, b);
    EXPECT_LE(a, b);
  }

  {
    float a = 34.5f;
    float b = -1.34f;
    float c = 0.194f;
    math::sort3(a, b, c);
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);
  }

  {
    double a = 2.1;
    double b = -1.1e-1;
    math::sort2(a, b);
    EXPECT_LE(a, b);
  }

  {
    double a = 34.5;
    double b = -1.34;
    double c = 0.194;
    math::sort3(a, b, c);
    EXPECT_LE(a, b);
    EXPECT_LE(b, c);
  }
}

/////////////////////////////////////////////////
TEST(HelpersTest, Volume)
{
  EXPECT_DOUBLE_EQ(IGN_SPHERE_VOLUME(1.0), 4.0*IGN_PI*std::pow(1, 3)/3.0);
  EXPECT_DOUBLE_EQ(IGN_SPHERE_VOLUME(0.1), 4.0*IGN_PI*std::pow(.1, 3)/3.0);
  EXPECT_DOUBLE_EQ(IGN_SPHERE_VOLUME(-1.1), 4.0*IGN_PI*std::pow(-1.1, 3)/3.0);

  EXPECT_DOUBLE_EQ(IGN_CYLINDER_VOLUME(0.5, 2.0), 2 * IGN_PI * std::pow(.5, 2));
  EXPECT_DOUBLE_EQ(IGN_CYLINDER_VOLUME(1, -1), -1 * IGN_PI * std::pow(1, 2));

  EXPECT_DOUBLE_EQ(IGN_BOX_VOLUME(1, 2, 3), 1 * 2 * 3);
  EXPECT_DOUBLE_EQ(IGN_BOX_VOLUME(.1, .2, .3),
                   IGN_BOX_VOLUME_V(math::Vector3d(0.1, 0.2, 0.3)));
}

/////////////////////////////////////////////////
TEST(HelpersTest, Pair)
{
  uint32_t maxA = IGN_UINT32_MAX;
  uint32_t maxB = IGN_UINT32_MAX;
  uint32_t maxC, maxD;

  // Maximum parameters should generate a maximum key
  uint64_t maxKey = math::Pair(maxA, maxB);
  EXPECT_EQ(maxKey, IGN_UINT64_MAX);

  std::tie(maxC, maxD) = math::Unpair(maxKey);
  EXPECT_EQ(maxC, maxA);
  EXPECT_EQ(maxD, maxB);

  uint32_t minA = IGN_UINT32_MIN;
  uint32_t minB = IGN_UINT32_MIN;
  uint32_t minC, minD;

  // Minimum parameters should generate a minimum key
  uint64_t minKey = math::Pair(minA, minB);
  EXPECT_EQ(minKey, IGN_UINT64_MIN);

  std::tie(minC, minD) = math::Unpair(minKey);
  EXPECT_EQ(minC, minA);
  EXPECT_EQ(minD, minB);

  // Max key != min key
  EXPECT_TRUE(minKey != maxKey);

  // Just a simple test case
  {
    int a = 10;
    int b = 20;
    uint32_t c, d;

    auto key = math::Pair(a, b);
    EXPECT_EQ(key, 410);
    EXPECT_TRUE(key != maxKey);
    EXPECT_TRUE(key != minKey);

    std::tie(c, d) = math::Unpair(key);
    EXPECT_EQ(c, a);
    EXPECT_EQ(d, b);
  }

  if (0)
  {
    uint32_t c, d;
    std::set<uint64_t> set;

    // Iterate of range of pairs, and check for unique keys.
    for (uint16_t a = IGN_UINT16_MIN; a < IGN_UINT16_MAX - 200;
         a+=math::Rand::IntUniform(50, 200))
    {
      for (uint16_t b = IGN_UINT16_MIN; b < IGN_UINT16_MAX - 200;
         b += math::Rand::IntUniform(50, 200))
      {
        uint64_t key = math::Pair(a, b);
        std::tie(c, d) = math::Unpair(key);
        EXPECT_EQ(a, c);
        EXPECT_EQ(b, d);
        EXPECT_TRUE(set.find(key) == set.end());
        EXPECT_TRUE(key != maxKey);
        set.insert(key);
      }
    }

    // Iterate over large numbers, and check for unique keys.
    for (uint32_t a = IGN_UINT32_MAX-5000; a < IGN_UINT32_MAX; a++)
    {
      for (uint32_t b = IGN_UINT32_MAX-5000; b < IGN_UINT32_MAX; b++)
      {
        uint64_t key = math::Pair(a, b);
        std::tie(c, d) = math::Unpair(key);
        EXPECT_EQ(a, c);
        EXPECT_EQ(b, d);
        EXPECT_TRUE(set.find(key) == set.end());
        EXPECT_TRUE(key != minKey);
        set.insert(key);
      }
    }
  }
}
