/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
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

#include "ignition/math/Helpers.hh"
#include "ignition/math/Vector2.hh"

using namespace ignition;

/////////////////////////////////////////////////
TEST(Vector2Test, Vector2)
{
  {
    math::Vector2d v;
    EXPECT_DOUBLE_EQ(0, v.X());
    EXPECT_DOUBLE_EQ(0, v.Y());
  }

  // Constructor
  math::Vector2d v(1, 2);
  EXPECT_DOUBLE_EQ(1, v.X());
  EXPECT_DOUBLE_EQ(2, v.Y());

  // ::Distance
  EXPECT_TRUE(math::equal(2.236, v.Distance(math::Vector2d(0, 0)), 1e-2));

  // ::Normalize
  v.Normalize();
  EXPECT_TRUE(v == math::Vector2d(0.447214, 0.894427));

  // ::Set
  v.Set(4, 5);
  EXPECT_TRUE(v == math::Vector2d(4, 5));

  // ::operator=
  v = math::Vector2d(6, 7);
  EXPECT_TRUE(v == math::Vector2d(6, 7));

  // ::operator= int
  v = 5;
  EXPECT_TRUE(v == math::Vector2d(5, 5));

  // ::operator+
  v = v + math::Vector2d(1, 2);
  EXPECT_TRUE(v == math::Vector2d(6, 7));

  // ::operator +=
  v += math::Vector2d(5, 6);
  EXPECT_TRUE(v == math::Vector2d(11, 13));

  // ::operator -
  v = v - math::Vector2d(2, 4);
  EXPECT_TRUE(v == math::Vector2d(9, 9));

  // ::operator -=
  v.Set(2, 4);
  v -= math::Vector2d(1, 6);
  EXPECT_TRUE(v == math::Vector2d(1, -2));

  // ::operator /
  v.Set(10, 6);
  v = v / math::Vector2d(2, 3);
  EXPECT_TRUE(v == math::Vector2d(5, 2));

  // ::operator /=
  v.Set(10, 6);
  v /= math::Vector2d(2, 3);
  EXPECT_TRUE(v == math::Vector2d(5, 2));

  // ::operator / int
  v.Set(10, 6);
  v = v / 2;
  EXPECT_TRUE(v == math::Vector2d(5, 3));

  // ::operator /= int
  v.Set(10, 6);
  v /= 2;
  EXPECT_TRUE(v == math::Vector2d(5, 3));

  // ::operator * int
  v.Set(10, 6);
  v = v * 2;
  EXPECT_TRUE(v == math::Vector2d(20, 12));

  // ::operator *= int
  v.Set(10, 6);
  v *= 2;
  EXPECT_TRUE(v == math::Vector2d(20, 12));

  // ::operator * vector2i
  v.Set(10, 6);
  v = v * math::Vector2d(2, 4);
  EXPECT_TRUE(v == math::Vector2d(20, 24));

  // ::operator *= vector2i
  v.Set(10, 6);
  v *= math::Vector2d(2, 4);
  EXPECT_TRUE(v == math::Vector2d(20, 24));


  // ::IsFinite
  EXPECT_TRUE(v.IsFinite());

  // ::operator[]
  v.Set(6, 7);
  EXPECT_DOUBLE_EQ(6, v[0]);
  EXPECT_DOUBLE_EQ(7, v[1]);
}

/////////////////////////////////////////////////
TEST(Vector2Test, IndexException)
{
  math::Vector2d v(1, 2);
  EXPECT_NO_THROW(math::equal(v[0], 1.0));
  EXPECT_NO_THROW(math::equal(v[1], 2.0));

  EXPECT_THROW(math::equal(v[2], 1.0), math::IndexException);
}

/////////////////////////////////////////////////
TEST(Vector2Test, Dot)
{
  math::Vector2d v(1, 2);

  EXPECT_DOUBLE_EQ(v.Dot(math::Vector2d(3, 4)), 11.0);
  EXPECT_DOUBLE_EQ(v.Dot(math::Vector2d(0, 0)), 0.0);
  EXPECT_DOUBLE_EQ(v.Dot(math::Vector2d(1, 0)), 1.0);
  EXPECT_DOUBLE_EQ(v.Dot(math::Vector2d(0, 1)), 2.0);
}

/////////////////////////////////////////////////
TEST(Vector2Test, OperatorStreamOut)
{
  math::Vector2d v(0.1, 1.2);
  std::ostringstream stream;
  stream << v;
  EXPECT_EQ(stream.str(), "0.1 1.2");
}

/////////////////////////////////////////////////
TEST(Vector2dTest, Max)
{
  math::Vector2d vec1(0.1, 0.2);
  math::Vector2d vec2(0.2, 0.1);
  math::Vector2d vec3(0.1, 0.4);

  EXPECT_DOUBLE_EQ(vec1.Max(), 0.2);

  vec1.Max(vec2);
  EXPECT_EQ(vec1, math::Vector2d(0.2, 0.2));

  vec1.Max(vec3);
  EXPECT_EQ(vec1, math::Vector2d(0.2, 0.4));
}

/////////////////////////////////////////////////
TEST(Vector2dTest, Min)
{
  math::Vector2d vec1(0.1, 0.2);
  math::Vector2d vec2(0.2, 0.1);
  math::Vector2d vec3(0.05, 0.1);

  EXPECT_DOUBLE_EQ(vec1.Min(), 0.1);

  vec1.Min(vec2);
  EXPECT_EQ(vec1, math::Vector2d(0.1, 0.1));

  vec1.Min(vec3);
  EXPECT_EQ(vec1, math::Vector2d(0.05, 0.1));
}

/////////////////////////////////////////////////
TEST(Vector2dTest, Clamp)
{
  math::Vector2d vec1(0.1, 0.2);

  math::Vector2d min(0, 0);
  math::Vector2d max(1, 1);
  math::Vector2d result = vec1.Clamp(min, max);
  EXPECT_EQ(vec1, result);
  EXPECT_EQ(vec1, math::Vector2d(0.1, 0.2));

  min.Set(0.1, 0.2);
  max.Set(0.1, 0.2);
  result = vec1.Clamp(min, max);
  EXPECT_EQ(vec1, result);
  EXPECT_EQ(vec1, math::Vector2d(0.1, 0.2));

  vec1.Set(0.1, 0.2);
  min.Set(0.11, 0.21);
  max.Set(0.2, 0.4);
  result = vec1.Clamp(min, max);
  EXPECT_EQ(vec1, result);
  EXPECT_EQ(vec1, math::Vector2d(0.11, 0.21));

  vec1.Set(0.1, 0.2);
  min.Set(-1, -1);
  max.Set(0, 0.1);
  result = vec1.Clamp(min, max);
  EXPECT_EQ(vec1, result);
  EXPECT_EQ(vec1, math::Vector2d(0, 0.1));
  std::cout << "\n\nMax[" << vec1.Max(min) << "]\n\n";
  std::cout << "\n\nMin[" << vec1.Min(max) << "]\n\n";

  vec1.Set(0.1, 0.2);
  min.Set(1, 2);
  max.Set(-1, -2);
  result = vec1.Clamp(min, max);
  EXPECT_EQ(vec1, result);
  EXPECT_EQ(vec1, math::Vector2d(-1, -2));
}
