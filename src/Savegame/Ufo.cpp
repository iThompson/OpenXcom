/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Ufo.h"
#include <cmath>
#include <sstream>
#include "../Engine/Language.h"
#include "../Ruleset/RuleUfo.h"

/**
 * Initializes a UFO of the specified type.
 * @param rules Pointer to ruleset.
 */
Ufo::Ufo(RuleUfo *rules) : MovingTarget(), _rules(rules), _id(0), _damage(0), _altitude(0), _direction(STR_NORTH), _detected(false), _daysCrashed(0)
{
}

/**
 *
 */
Ufo::~Ufo()
{
	delete _dest;
}

/**
 * Returns the ruleset for the UFO's type.
 * @return Pointer to ruleset.
 */
RuleUfo *const Ufo::getRules() const
{
	return _rules;
}

/**
 * Returns the UFO's unique ID. If it's 0,
 * this UFO has never been detected.
 * @return Unique ID.
 */
int Ufo::getId() const
{
	return _id;
}

/**
 * Changes the UFO's unique ID.
 * @param id Unique ID.
 */
void Ufo::setId(int id)
{
	_id = id;
}

/**
 * Returns the UFO's unique identifying name.
 * @param lang Language to get strings from.
 * @return Full name.
 */
std::string Ufo::getName(Language *lang) const
{
	std::stringstream name;
	if (!isCrashed())
	{
		name << lang->getString(STR_UFO_) << _id;
	}
	else
	{
		name << lang->getString(STR_CRASH_SITE_) << _id;
	}
	return name.str();
}

/**
 * Returns the amount of damage this UFO has taken.
 * @return Amount of damage.
 */
int Ufo::getDamage() const
{
	return _damage;
}

/**
 * Changes the amount of damage this UFO has taken.
 * @param damage Amount of damage.
 */
void Ufo::setDamage(int damage)
{
	_damage = damage;
	if (_damage < 0)
	{
		_damage = 0;
	}
}

/**
 * Returns whether this UFO has been detected by radars.
 * @return Detection status.
 */
bool Ufo::getDetected() const
{
	return _detected;
}

/**
 * Changes whether this UFO has been detected by radars.
 * @param detected Detection status.
 */
void Ufo::setDetected(bool detected)
{
	_detected = detected;
}

/**
 * Returns the amount of days the UFO has been crashed for.
 * @return Amount of days.
 */
int Ufo::getDaysCrashed() const
{
	return _daysCrashed;
}

/**
 * Changes the amount of days the UFO has been crashed for.
 * @param days Amount of days.
 */
void Ufo::setDaysCrashed(int days)
{
	_daysCrashed = days;
}

/**
 * Returns the current direction the UFO is heading in.
 * @return Direction.
 */
LangString Ufo::getDirection() const
{
	return _direction;
}

/**
 * Returns the current altitude of the UFO.
 * @return Altitude.
 */
LangString Ufo::getAltitude() const
{
	return STR_HIGH;
}

/**
 * Returns if this UFO took enough damage
 * to cause it to crash.
 * @return Crashed status.
 */
bool Ufo::isCrashed() const
{
	return (_damage >= _rules->getMaxDamage() / 2);
}

/**
 * Returns if this UFO took enough damage
 * to cause it to crash.
 * @return Crashed status.
 */
bool Ufo::isDestroyed() const
{
	return (_damage >= _rules->getMaxDamage());
}

/**
 * Calculates the direction for the UFO based
 * on the current raw speed and destination.
 */
void Ufo::calculateSpeed()
{
	MovingTarget::calculateSpeed();
	if (_speedLon > 0)
	{
		if (_speedLat > 0)
		{
			_direction = STR_SOUTH_EAST;
		}
		else if (_speedLat < 0)
		{
			_direction = STR_NORTH_EAST;
		}
		else
		{
			_direction = STR_EAST;
		}
	}
	else if (_speedLon < 0)
	{
		if (_speedLat > 0)
		{
			_direction = STR_SOUTH_WEST;
		}
		else if (_speedLat < 0)
		{
			_direction = STR_NORTH_WEST;
		}
		else
		{
			_direction = STR_WEST;
		}
	}
	else
	{
		if (_speedLat > 0)
		{
			_direction = STR_SOUTH;
		}
		else if (_speedLat < 0)
		{
			_direction = STR_NORTH;
		}
	}
}

/**
 * Moves the UFO to its destination.
 */
void Ufo::think()
{
	if (!isCrashed())
	{
		setLongitude(_lon + _speedLon);
		setLatitude(_lat + _speedLat);
		if (finishedRoute())
		{
			_lon = _dest->getLongitude();
			_lat = _dest->getLatitude();
			setSpeed(0);
		}
	}
}