#include "Init.h"
#include "Input.h"
#include "../Util/StringParsing.h"
#include <map>
#include <string>
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace Input;
using namespace StringParsing;

namespace Input {
  string getConfigPath() {
    string home = getenv("HOME");
    home += "/.retroarch/retroarch.cfg";
    return home;
  }

  void initConfig() {
    // Clear any old config
    joystickButtonMap.clear();
    joystickAxisPosMap.clear();
    joystickAxisNegMap.clear();

    // Init the stuff we need to catch config options
    string inputConfigLinePrefix = "input_player";
    string path = getConfigPath();
    ifstream file(path.c_str());
    int uiActionIndex = 0; // UNKNOWN

    // Reading input config file from retroarch
    while(file.good()) {
      string line;
      getline(file, line);
      string inputConfigLinePrefix = "input_player";

      // If the line starts with the specified prefix, then attempt to handle it
      if (startsWith(line, inputConfigLinePrefix)) {
        vector<string> configLineElems;
        configLineElems = split(line, ' ', configLineElems);
        string configOption = configLineElems[0];
        string configValue = stripQuotationMarks(configLineElems[2]);

        uiActionIndex = 0;

        // An ugly, piece of crap set of if statements
        if (configOption == "input_player1_up_axis")
          uiActionIndex = 1;
        else if (configOption == "input_player1_down_axis")
          uiActionIndex = 2;
        else if (configOption == "input_player1_left_axis")
          uiActionIndex = 3;
        else if (configOption == "input_player1_right_axis")
          uiActionIndex = 4;
        else if (configOption == "input_player1_a_btn")
          uiActionIndex = 5;
        else if (configOption == "input_player1_b_btn")
          uiActionIndex = 6;
        else if (configOption == "input_player1_start_btn")
          uiActionIndex = 7;
        else if (configOption == "input_player1_select_btn")
          uiActionIndex = 8;
        
        // The actual button mapping
        if (startsWith(configValue, "+"))
          joystickAxisPosMap[atoi(configValue.c_str())] = (InputButton)uiActionIndex;
        else if (startsWith(configValue, "-"))
          joystickAxisNegMap[atoi(configValue.c_str())] = (InputButton)uiActionIndex;
        else
          joystickButtonMap[atoi(configValue.c_str())] = (InputButton)uiActionIndex;

        cout << "\tMapping: (cfgFile): " << configValue << " => (glint-ui mapping) " << uiActionIndex << "\n";
      } else {
        continue;
      }
    } // while
  }

  void initJoysticks() {
    SDL_JoystickOpen(0);
    SDL_JoystickOpen(1);
  }
}

