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
#include "ignition/math/Helpers.hh"
#include "ignition/math/Angle.hh"

using namespace ignition;
using namespace math;

const Angle Angle::Zero = math::Angle(0);
const Angle Angle::Pi = math::Angle(IGN_PI);
const Angle Angle::HalfPi = math::Angle(IGN_PI_2);
const Angle Angle::TwoPi = math::Angle(IGN_PI * 2.0);

//////////////////////////////////////////////////
Angle::Angle()
{
  this->value = 0;
}

//////////////////////////////////////////////////
Angle::Angle(const int _radian)
{
  this->value = static_cast<double>(_radian);
}

//////////////////////////////////////////////////
Angle::Angle(const double _radian)
{
  this->value = _radian;
}

//////////////////////////////////////////////////
Angle::Angle(const long double _radian)
{
  this->value = static_cast<double>(_radian);
}

//////////////////////////////////////////////////
Angle::Angle(const Angle &_angle)
{
  this->value = _angle.value;
}

//////////////////////////////////////////////////
Angle::~Angle()
{
}

//////////////////////////////////////////////////
void Angle::Radian(const double _radian)
{
  this->value = _radian;
}

//////////////////////////////////////////////////
void Angle::Degree(const double _degree)
{
  this->value = _degree * IGN_PI / 180.0;
}

//////////////////////////////////////////////////
double Angle::Radian() const
{
  return this->value;
}

//////////////////////////////////////////////////
double Angle::Degree() const
{
  return this->value * 180.0 / IGN_PI;
}

//////////////////////////////////////////////////
void Angle::Normalize()
{
  this->value = atan2(sin(this->value), cos(this->value));
}

//////////////////////////////////////////////////
Angle Angle::operator-() const
{
  return Angle(-this->value);
}

//////////////////////////////////////////////////
Angle Angle::operator+() const
{
  return Angle(+this->value);
}

//////////////////////////////////////////////////
Angle Angle::operator-(const Angle &angle) const
{
  return Angle(this->value - angle.value);
}

//////////////////////////////////////////////////
Angle Angle::operator-(const double _radians) const
{
  return Angle(this->value - _radians);
}

//////////////////////////////////////////////////
Angle Angle::operator+(const Angle &angle) const
{
  return Angle(this->value + angle.value);
}

//////////////////////////////////////////////////
Angle Angle::operator+(const double _radians) const
{
  return Angle(this->value + _radians);
}

//////////////////////////////////////////////////
Angle Angle::operator*(const Angle &angle) const
{
  return Angle(this->value * angle.value);
}

//////////////////////////////////////////////////
Angle Angle::operator*(const double _radians) const
{
  return Angle(this->value * _radians);
}

//////////////////////////////////////////////////
Angle Angle::operator/(const Angle &angle) const
{
  return Angle(this->value / angle.value);
}

//////////////////////////////////////////////////
Angle Angle::operator/(const double _radians) const
{
  return Angle(this->value / _radians);
}

//////////////////////////////////////////////////
Angle Angle::operator-=(const Angle &angle)
{
  this->value -= angle.value;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator-=(const double _radians)
{
  this->value -= _radians;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator+=(const Angle &angle)
{
  this->value += angle.value;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator+=(const double _radians)
{
  this->value += _radians;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator*=(const Angle &angle)
{
  this->value *= angle.value;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator*=(const double _radians)
{
  this->value *= _radians;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator/=(const Angle &angle)
{
  this->value /= angle.value;
  return *this;
}

//////////////////////////////////////////////////
Angle Angle::operator/=(const double _radians)
{
  this->value /= _radians;
  return *this;
}

//////////////////////////////////////////////////
bool Angle::operator==(const Angle &angle) const
{
  return equal(this->value, angle.value, 0.001);
}

//////////////////////////////////////////////////
bool Angle::operator==(const double _radians) const
{
  return equal(this->value, _radians, 0.001);
}

//////////////////////////////////////////////////
bool Angle::operator!=(const Angle &angle) const
{
  return !(*this == angle);
}

//////////////////////////////////////////////////
bool Angle::operator!=(const double _radians) const
{
  return !(*this == _radians);
}

//////////////////////////////////////////////////
bool Angle::operator<(const Angle &angle) const
{
  return this->value < angle.value;
}

//////////////////////////////////////////////////
bool Angle::operator<(const double _radians) const
{
  return this->value < _radians;
}

//////////////////////////////////////////////////
bool Angle::operator<=(const Angle &angle) const
{
  return this->value < angle.value || math::equal(this->value, angle.value);
}

//////////////////////////////////////////////////
bool Angle::operator<=(const double _radians) const
{
  return this->value < _radians || math::equal(this->value, _radians);
}

//////////////////////////////////////////////////
bool Angle::operator>(const Angle &angle) const
{
  return this->value > angle.value;
}

//////////////////////////////////////////////////
bool Angle::operator>(const double _radians) const
{
  return this->value > _radians;
}

//////////////////////////////////////////////////
bool Angle::operator>=(const Angle &angle) const
{
  return this->value > angle.value || math::equal(this->value, angle.value);
}

//////////////////////////////////////////////////
bool Angle::operator>=(const double _radians) const
{
  return this->value > _radians || math::equal(this->value, _radians);
}

//////////////////////////////////////////////////
Angle &Angle::operator=(const Angle &_angle)
{
  if (this != &_angle)
    this->value = _angle.value;

  return *this;
}

//////////////////////////////////////////////////
Angle &Angle::operator=(const double _radians)
{
  this->value = _radians;
  return *this;
}

//////////////////////////////////////////////////
double Angle::operator()() const
{
  return this->value;
}

//////////////////////////////////////////////////
ignition::math::Angle operator"" _ign_deg(long double _deg)
{
  return ignition::math::Angle(IGN_DTOR(_deg));
}

//////////////////////////////////////////////////
ignition::math::Angle operator"" _ign_rad(long double _rad)
{
  return ignition::math::Angle(_rad);
}
