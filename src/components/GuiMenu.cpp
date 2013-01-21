#include "GuiMenu.h"
#include "GuiInputConfig.h"
#include <iostream>

GuiMenu::GuiMenu(GuiComponent* parent)
{
	mParent = parent;
	parent->pause();

	mList = new GuiList<std::string>(0, Renderer::getDefaultFont(Renderer::MEDIUM)->getHeight() + 2, Renderer::getDefaultFont(Renderer::MEDIUM));
	mList->setSelectedTextColor(0xFFFFFFFF);
	populateList();
	addChild(mList);

	mSkippedMenuClose = false;

	Renderer::registerComponent(this);
	InputManager::registerComponent(this);
}

GuiMenu::~GuiMenu()
{
	Renderer::unregisterComponent(this);
	InputManager::unregisterComponent(this);

	delete mList;
	mParent->resume();
}

void GuiMenu::onInput(InputManager::InputButton button, bool keyDown)
{
	if(button == InputManager::MENU && !keyDown)
	{
		if(!mSkippedMenuClose)
		{
			mSkippedMenuClose = true;
		}else{
			delete this;
			return;
		}
	}

	if(button == InputManager::BUTTON1 && keyDown)
	{
		if(system(mList->getSelectedObject().c_str()) != 0)
		{
			std::cout << "(menu command terminated with a nonzero errorcode)\n";
		}
	}
}

void GuiMenu::populateList()
{
	mList->clear();

	//if you want to add your own commands to the menu, here is where you need to change!
	//commands added here are called with system() when selected (so are executed as shell commands)
	//the method is GuiList::addObject(std::string displayString, std::string commandString, unsigned int displayHexColor);
	//the list will automatically adjust as items are added to it, this should be the only area you need to change
  mList->addObject("Setup controllers", "rm ~/.glint-es/es_input.cfg && . ~/.glint-es/restart-glint-es.sh", 0xFFFFFFFF);
  mList->addObject("Reboot glint", "sudo shutdown -r now", 0xFFFFFFFF);
  mList->addObject("Shutdown glint", "sudo shutdown -hP now", 0xFFFFFFFF);
	mList->addObject("Exit", "pkill glint-es", 0xFFFFFFFF);
}

void GuiMenu::onRender()
{
	Renderer::drawRect(0, 0, Renderer::getScreenWidth(), Renderer::getScreenHeight(), 0x999999FF);
}
