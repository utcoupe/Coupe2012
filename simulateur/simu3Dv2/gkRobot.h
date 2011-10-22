/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#ifndef GKBUGGY_H
#define GKBUGGY_H

#include "gkVehicle.h"

class gkBuggy : public gkVehicle
{
public:
	gkBuggy(gkScene* scene);
	~gkBuggy();

	virtual void load();

};

#define GK_RESOURCE_BUGGY_FILE    "media/robot.blend"
#define GK_RESOURCE_BUGGY_GROUP   "CarGroup"
#define GK_RESOURCE_BUGGY_PHYSOBJ "ChassisCollision"
#define GK_RESOURCE_BUGGY_CHASSIS "Chassis"
#define GK_RESOURCE_BUGGY_WHELLFL "TireFL"
#define GK_RESOURCE_BUGGY_WHELLFR "TireFR"
//#define GK_RESOURCE_BUGGY_WHELLRL "TireRL"
//#define GK_RESOURCE_BUGGY_WHELLRR "TireRR"

#endif // GKBUGGY_H
