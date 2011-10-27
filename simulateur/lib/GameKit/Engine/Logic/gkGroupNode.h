/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkGroupNode_h_
#define _gkGroupNode_h_

#include "gkLogicNode.h"

// execute x number of subtrees
class gkGroupNode : public gkLogicNode
{
public:
	typedef utList<gkLogicTree*> Groups;

	enum
	{
		EXECUTE,
		RETURN
	};

	DECLARE_SOCKET_TYPE(EXECUTE, bool);
	DECLARE_SOCKET_TYPE(RETURN, bool);

	gkGroupNode(gkLogicTree* parent, size_t id);
	virtual ~gkGroupNode() {}

	void update(gkScalar tick);
	bool evaluate(gkScalar tick);

	GK_INLINE void addTree(gkLogicTree* ltree)  {GK_ASSERT(ltree); m_groupList.push_back(ltree);}

private:
	Groups              m_groupList;
};

#endif//_gkGroupNode_h_
