#include "GuiInputConfig.h"
#include "GuiGameList.h"
#include <iostream>
#include <fstream>

std::string GuiInputConfig::sConfigPath = "./input.cfg";
std::string GuiInputConfig::sInputs[] = { "UNKNOWN", "UP", "DOWN", "LEFT", "RIGHT", "BUTTON A (Action)", "BUTTON B (Back)", "START (Show menu)", "SELECT (Jump-to-letter)", "PAGE UP", "PAGE DOWN" }; //must be same order as InputManager::InputButton enum; only add to the end to preserve backwards compatibility
int GuiInputConfig::sInputCount = 9;

GuiInputConfig::GuiInputConfig()
{
	mInputNum = 1;
	mDone = false;
	mLastAxis = -1;

	Renderer::registerComponent(this);
	InputManager::registerComponent(this);

	if(SDL_NumJoysticks() < 1)
	{
		std::cerr << "Error - GuiInputConfig found no SDL joysticks!\n";
		mJoystick = NULL;
		mDone = true;
		return;
	} else {
    std::cout << "Opening joystick \"" << SDL_JoystickName(0) << "\" for configuration...\n";
		mJoystick = SDL_JoystickOpen(0);
	}
}

GuiInputConfig::~GuiInputConfig()
{
	Renderer::unregisterComponent(this);
	InputManager::unregisterComponent(this);
}

void GuiInputConfig::onRender()
{
	Renderer::drawRect(0, 0, Renderer::getScreenWidth(), Renderer::getScreenHeight(), 0xFFFFFFFF);

	Font* font = Renderer::getDefaultFont(Renderer::SMALL);

	int height = font->getHeight() + 6;


	Renderer::drawText("Configure controller #1:", 20, 20, 0x000000FF, font);

	if(!mDone)
		Renderer::drawText("Press " + sInputs[mInputNum], 20, height * 5, 0x00C000FF, font);
}

void GuiInputConfig::onInput(InputManager::InputButton button, bool keyDown)
{
	if(mDone)
	{
//		if(InputManager::lastEvent->type == SDL_KEYUP)
//  	{
			writeConfig();

			if(mJoystick)
				SDL_JoystickClose(mJoystick);

			InputManager::loadConfig();
			delete this;
			GuiGameList::create();
//		}
		return;
	}

	SDL_Event* event = InputManager::lastEvent;
	if(event->type == SDL_KEYUP)
	{
		//keyboard key pressed; skip and continue
		mInputNum++;
	}

	if(event->type == SDL_JOYBUTTONDOWN)
	{
		mButtonMap[event->jbutton.button] = (InputManager::InputButton)mInputNum;
		std::cout << "	Mapping " << sInputs[mInputNum] << " to button " << (int)event->jbutton.button << "\n";
		mInputNum++;
	}

	if(event->type == SDL_JOYAXISMOTION)
	{
		//std::cout << "motion on axis " << event->jaxis.axis << " to value " << event->jaxis.value << "\n";

		if(event->jaxis.axis == mLastAxis)
		{
			if(event->jaxis.value < InputManager::deadzone && event->jaxis.value > -InputManager::deadzone)
				mLastAxis = -1;
			return;
		}
		if(event->jaxis.value > InputManager::deadzone)
		{
			mAxisPosMap[event->jaxis.axis] = (InputManager::InputButton)mInputNum;
			mInputNum++;
			mLastAxis = event->jaxis.axis;
			std::cout << "	Mapping " << sInputs[mInputNum - 1] << " to axis+ " << mLastAxis << "\n";
		}else if(event->jaxis.value < -InputManager::deadzone)
		{
			mAxisNegMap[event->jaxis.axis] = (InputManager::InputButton)mInputNum;
			mInputNum++;
			mLastAxis = event->jaxis.axis;
			std::cout << "	Mapping " << sInputs[mInputNum - 1] << " to axis- " << mLastAxis << "\n";
		}
	}

	if(mInputNum >= sInputCount)
	{
		mDone = true;
		return;
	}
}

void GuiInputConfig::writeConfig()
{
	std::string path = InputManager::getConfigPath();

	std::ofstream file(path.c_str());

	if(SDL_JoystickName(0))
		file << "JOYNAME " << SDL_JoystickName(0) << "\n";

	typedef std::map<int, InputManager::InputButton>::iterator it_type;
	for(it_type iter = mButtonMap.begin(); iter != mButtonMap.end(); iter++)
	{
		file << "BUTTON " << iter->first << " " << iter->second << "\n";
	}

	for(it_type iter = mAxisPosMap.begin(); iter != mAxisPosMap.end(); iter++)
	{
		file << "AXISPOS " << iter->first << " " << iter->second << "\n";
	}

	for(it_type iter = mAxisNegMap.begin(); iter != mAxisNegMap.end(); iter++)
	{
		file << "AXISNEG " << iter->first << " " << iter->second << "\n";
	}

	file.close();
}
