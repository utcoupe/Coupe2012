/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
/*  Win32 native support added for OgreKit by Charlie C.
	http://gamekit.googlecode.com/
*/
#ifndef OIS_WIN32NATIVEINPUTMANAGER_H
#define OIS_WIN32NATIVEINPUTMANAGER_H

#include "OISInputManager.h"
#include "OISFactoryCreator.h"
#include "win32Native/Win32NativePrereqs.h"


namespace OIS
{
	/**
		Native Win32 input backend
	*/
	class Win32NativeInputManager : public InputManager, public FactoryCreator
	{
	public:

		Win32NativeInputManager();
		virtual ~Win32NativeInputManager();

		/** @copydoc InputManager::_initialize */
		void _initialize( ParamList &paramList );

		/** @copydoc FactoryCreator::deviceList */
		DeviceList freeDeviceList();

		/** @copydoc FactoryCreator::totalDevices */
		int totalDevices(Type iType);

		/** @copydoc FactoryCreator::freeDevices */
		int freeDevices(Type iType);

		/** @copydoc FactoryCreator::vendorExist */
		bool vendorExist(Type iType, const std::string & vendor);

		/** @copydoc FactoryCreator::createObject */
		Object* createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor = "");

		/** @copydoc FactoryCreator::destroyObject */
		void destroyObject(Object* obj);


		/** callback for the windows procedure */
		LRESULT WINAPI _proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	protected:

		/** Old windows procedure, 
			Used by creating a chain with the current
			procedure (old) to the new one defined here 
		 */
		WNDPROC mOldProc;
		HWND mHandle;


		/** Keyboard wrapper*/
		class Win32NativeKeyboard *mKeyboard;

		/** Mouse wrapper*/
		class Win32NativeMouse	*mMouse;	
        bool mGrab, mHide;
		bool mPass;
	};
}

#endif//OIS_WIN32NATIVEINPUTMANAGER_H
