#ifndef _GUITHEME_H_
#define _GUITHEME_H_

#include "../GuiComponent.h"
#include "../pugiXML/pugixml.hpp"
#include "GuiBox.h"
#include "../Font.h"

//This class loads an XML-defined list of GuiComponents.
class GuiTheme : public GuiComponent
{
public:
	GuiTheme(std::string path = "");
	~GuiTheme();

	void readXML(std::string path);

	unsigned int getPrimaryColor();
	unsigned int getSecondaryColor();
	unsigned int getSelectorColor();
	unsigned int getSelectedTextColor();
	unsigned int getDescColor();
	unsigned int getFastSelectColor();
	bool getHeaderHidden();
	bool getDividersHidden();
	bool getListCentered();

	float getListOffsetX();
	float getListTextOffsetX();

	float getGameImageOffsetX();
	float getGameImageOffsetY();
	float getGameImageWidth();
	float getGameImageHeight();
	float getGameImageOriginX();
	float getGameImageOriginY();

	GuiBoxData getBoxData();

	std::string getImageNotFoundPath();

	Font* getListFont();
	Font* getDescriptionFont();
private:
	void setDefaults();
	void deleteComponents();
	void createComponentChildren(pugi::xml_node node, GuiComponent* parent);
	GuiComponent* createElement(pugi::xml_node data, GuiComponent* parent);

	//utility functions
	std::string expandPath(std::string path);
	float resolveExp(std::string str, float defaultVal = 0.0);
	unsigned int resolveColor(std::string str, unsigned int defaultColor = 0x000000FF);
	void splitString(std::string str, char delim, std::string* before, std::string* after);
	float strToFloat(std::string str, float defaultVal = 0.0f);
	Font* resolveFont(pugi::xml_node node, std::string defaultPath, unsigned int defaultSize);

	std::vector<GuiComponent*> mComponentVector;
	std::string mPath;

	unsigned int mListPrimaryColor, mListSecondaryColor, mListSelectorColor, mListSelectedColor, mDescColor, mFastSelectColor;
	bool mHideHeader, mHideDividers, mListCentered;
	float mListOffsetX, mListTextOffsetX, mGameImageOffsetX, mGameImageOffsetY, mGameImageWidth, mGameImageHeight, mGameImageOriginX, mGameImageOriginY;
	GuiBoxData mBoxData;
	std::string mImageNotFoundPath;
	Font* mListFont;
	Font* mDescFont;
};

#endif
