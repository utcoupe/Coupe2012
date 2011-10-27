/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
	Force rebuild when settings change
*/
#ifndef _gkSettings_h_
#define _gkSettings_h_

#define OGREKIT_USE_LUA 1
#define OGREKIT_COMPILE_OGRE_SCRIPTS 1
#define OGREKIT_DEBUG_ASSERT 1
/* #undef OGREKIT_OPENAL_SOUND */
#define OGRE_BUILD_RENDERSYSTEM_GL 1
/* #undef OGRE_BUILD_RENDERSYSTEM_GLES */
/* #undef OGREKIT_BUILD_D3D9RS */
/* #undef OGREKIT_BUILD_D3D11RS */
/* #undef OGREKIT_BUILD_IPHONE */
#define OGREKIT_USE_NNODE 1
/* #undef OGREKIT_COMPILE_RECAST */
/* #undef OGREKIT_COMPILE_OPENSTEER */
/* #undef OGERKIT_USE_RTSHADER_SYSTEM */

#ifdef OGREKIT_DEBUG_ASSERT
#define UT_DEBUG_ASSERT 1
#endif


#endif//_gkSettings_h_
