//This is *actually* part of the GuiList header and not meant to be compiled.

#include "GuiList.h"
#include <iostream>
using namespace Input;

template <typename listType>
GuiList<listType>::GuiList(int offsetX, int offsetY, Font* font) {
  mSelection = 0;
  mScrollDir = 0;
  mScrolling = 0;
  mScrollAccumulator = 0;

  setOffsetX(offsetX);
  setOffsetY(offsetY);

  mTextOffsetX = 0;

  mFont = font;
  mSelectorColor = 0x000000FF;
  mSelectedTextColorOverride = 0xFFFFFFFF;
  mDrawCentered = false;

  registerComponent(this);
}

template <typename listType>
GuiList<listType>::~GuiList() {
  unregisterComponent(this);
}

template <typename listType>
void GuiList<listType>::onRender() {
  const int cutoff = getOffsetY();
  const int entrySize = mFont->getHeight() + 5;

  int startEntry = 0;

  //number of entries that can fit on the screen simultaniously
  int screenCount = (Renderer::getScreenHeight() - cutoff) / entrySize;
  screenCount -= 1;

  if((int)mRowVector.size() >= screenCount) {
    startEntry = mSelection - (screenCount * 0.5);
    if(startEntry < 0)
      startEntry = 0;
    if(startEntry >= (int)mRowVector.size() - screenCount)
      startEntry = mRowVector.size() - screenCount;
  }

  int y = cutoff;

  if(mRowVector.size() == 0) {
    Renderer::drawText("The list is empty.", 20, y, 0xFF0000FF, mFont);
    return;
  }

  int listCutoff = startEntry + screenCount;
  if(listCutoff > (int)mRowVector.size())
    listCutoff = mRowVector.size();

  for(int i = startEntry; i < listCutoff; i++) {
    //draw selector bar
    if(mSelection == i) {
      Renderer::drawRect(getOffsetX(), y, Renderer::getScreenWidth(), mFont->getHeight(), mSelectorColor);
    }

    ListRow row = mRowVector.at((unsigned int)i);

    if(mDrawCentered)
      Renderer::drawCenteredText(row.name, getOffsetX(), y, (mSelection == i) ? mSelectedTextColorOverride : row.color, mFont);
    else
      Renderer::drawText(row.name, getOffsetX() + mTextOffsetX + 20, y, (mSelection == i) ? mSelectedTextColorOverride : row.color, mFont);

    y += entrySize;
  }
}

template <typename listType>
void GuiList<listType>::onInput(InputButton button, bool keyDown) {
  if (mRowVector.size() > 0) {
    if (keyDown) {
      if (button == AXIS_DOWN) {
        mScrollDir = 1;
        scroll();
      }

      if (button == AXIS_UP) {
        mScrollDir = -1;
        scroll();
      }
      if (button == BTN_PAGEDOWN) {
        mScrollDir = 10;
        scroll();
      }

      if (button == BTN_PAGEUP) {
        mScrollDir = -10;
        scroll();
      }
    } else {
      if((button == AXIS_DOWN && mScrollDir > 0) || (button == BTN_PAGEDOWN && mScrollDir > 0) || (button == AXIS_UP && mScrollDir < 0) || (button == BTN_PAGEUP && mScrollDir < 0)) {
        stopScrolling();
      }
    }
  }
}

template <typename listType>
void GuiList<listType>::stopScrolling() {
  mScrollAccumulator = 0;
  mScrolling = false;
  mScrollDir = 0;
}

template <typename listType>
void GuiList<listType>::onTick(int deltaTime) {
  if(mScrollDir != 0) {
    mScrollAccumulator += deltaTime;

    if(!mScrolling) {
      if(mScrollAccumulator >= SCROLLDELAY) {
        mScrollAccumulator = SCROLLTIME;
        mScrolling = true;
      }
    }

    if(mScrolling) {
      mScrollAccumulator += deltaTime;

      while(mScrollAccumulator >= SCROLLTIME) {
        mScrollAccumulator -= SCROLLTIME;

        scroll();
      }
    }
  }
}

template <typename listType>
void GuiList<listType>::scroll() {
  mSelection += mScrollDir;

  if(mSelection < 0) {
    if(mScrollDir < -1)
      mSelection = 0;
    else
      mSelection += mRowVector.size();
  }
  if(mSelection >= (int)mRowVector.size()) {
    if(mScrollDir > 1)
      mSelection = (int)mRowVector.size() - 1;
    else
      mSelection -= mRowVector.size();
  }

}

//list management stuff
template <typename listType>
void GuiList<listType>::addObject(std::string name, listType obj, unsigned int color) {
  ListRow row = {name, obj, color};
  mRowVector.push_back(row);
}

template <typename listType>
void GuiList<listType>::clear() {
  mRowVector.clear();
  mSelection = 0;
}

template <typename listType>
std::string GuiList<listType>::getSelectedName() {
  if((int)mRowVector.size() > mSelection)
    return mRowVector.at(mSelection).name;
  else
    return "";
}

template <typename listType>
listType GuiList<listType>::getSelectedObject() {
  if((int)mRowVector.size() > mSelection)
    return mRowVector.at(mSelection).object;
  else
    return NULL;
}

template <typename listType>
int GuiList<listType>::getSelection() {
  return mSelection;
}

template <typename listType>
bool GuiList<listType>::isScrolling() {
  return mScrollDir != 0;
}

template <typename listType>
void GuiList<listType>::onPause() {
  unregisterComponent(this);
}

template <typename listType>
void GuiList<listType>::onResume() {
  registerComponent(this);
}

template <typename listType>
void GuiList<listType>::setSelectorColor(unsigned int selectorColor) {
  mSelectorColor = selectorColor;
}

template <typename listType>
void GuiList<listType>::setSelectedTextColor(unsigned int selectedColor) {
  mSelectedTextColorOverride = selectedColor;
}

template<typename listType>
void GuiList<listType>::setCentered(bool centered) {
  mDrawCentered = centered;
}

template<typename listType>
void GuiList<listType>::setTextOffsetX(int textoffsetx) {
  mTextOffsetX = textoffsetx;
}

template <typename listType>
int GuiList<listType>::getObjectCount() {
  return mRowVector.size();
}

template <typename listType>
listType GuiList<listType>::getObject(int i) {
  return mRowVector.at(i).object;
}

template <typename listType>
void GuiList<listType>::setSelection(int i) {
  mSelection = i;
}

template <typename listType>
void GuiList<listType>::setFont(Font* font) {
  mFont = font;
}
