/*
# MINX

Copyright (c) 2013-2014 Liam Middlebrook, Benjamin Pylko

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

> 1\. The origin of this software must not be misrepresented; you must not
> claim that you wrote the original software. If you use this software
> in a product, an acknowledgment in the product documentation would be
> appreciated but is not required
>
> 2\. Altered source versions must be plainly marked as such, and must not be
> misrepresented as being the original software
>
> 3\. This notice may not be removed or altered from any source
> distribution
>
 */

#ifndef MINX_GAMEPAD_HPP_
#define MINX_GAMEPAD_HPP_

#include <functional>

#include "../API.hpp"
#include "IGenericHID.hpp"
#include "../Game.hpp"

#define GENERIC_GAMEPAD 0
#define XBOX_360_GAMEPAD 1
//#ifdef XBOX_360_GAMEPAD
#define CONTROLLER_A 0
#define CONTROLLER_X 1
#define CONTROLLER_Y 2
#define CONTROLLER_O 3
#define CONTROLLER_LB 4
#define CONTROLLER_RB 5
#define CONTROLLER_BACK 6
#define CONTROLLER_START 7
#define CONTROLLER_LEFTSTICK 8
#define CONTROLLER_RIGHTSTICK 9
#define CONTROLLER_LEFTAXIS_X 0
#define CONTROLLER_LEFTAXIS_Y 1
#define CONTROLLER_LEFTTRIGGER 3:1
#define CONTROLLER_RIGHTTRIGGER 3:-1
#define CONTROLLER_RIGHTSTICK_Y 3
#define CONTROLLER_RIGHTSTICK_X 4
//#endif

namespace MINX
{
	namespace Input
	{
		/** A GamePad class that handles input from a USB gamepad
		*/
		class MINX_API GamePad : public IGenericHID
		{
			public:
				/** Creates a new GamePad instance
				 *	@param deviceIndex The Index of the GamePad to get an instance of
				 *	@param game A pointer to an instance of the game object
				 */
				GamePad(unsigned int deviceIndex, Game* game);
				
				/** Creates a new Gamepad instance
				 *	@param deviceIndex The Index of the GamePad to get an instance of
				 *	@param game A pointer to an instance of the game object
				 *	@param gamePadType The Type of GamePad that is being Instantiated
				 */
				GamePad(unsigned int deviceIndex, Game* game, unsigned int gamePadType);
				
				/** Returns the name of the GamePad
				 *	@return GLFW's stored name for the GamePad
				 */
				const char* GetName();
				
				/** Checks is the GamePad's state has changed, and saves the data properly
				 */
				void Update(GameTime* gameTime);
				
				/** Gets the Button at the specified index
				 * @param buttonID The id of the button
				 * @return The button
				 */
				Button GetButton(unsigned int buttonID);
				
				/** Gets the Axis at the specified index
				 * @param axisID The id of the axis
				 * @return The axis
				 */
				Axis GetAxis(unsigned int axisID);
				
				/** Returns whether or not the GamePad is connected
				 */
				bool IsConnected();
				
			private:
				bool isConnected;
				unsigned int deviceIndex;
				unsigned int gamePadType;
		};

	}

}
#endif
