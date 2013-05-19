#ifndef _GUIMENU_H_
#define _GUIMENU_H_

#include "../GuiComponent.h"
#include "GuiList.h"
using namespace std;
using namespace Input;

//This is a very simple menu that is opened by pressing the Menu key.
class GuiMenu : public GuiComponent
{
public:
	GuiMenu(GuiComponent* parent);
	~GuiMenu();

	void onInput(InputButton button, bool keyDown);
	void onRender();

private:
	GuiComponent* mParent;
	GuiList<string>* mList;

	void populateList();

	bool mSkippedMenuClose;
};

#endif
