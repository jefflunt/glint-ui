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
    //clear any old config
    joystickButtonMap.clear();
    joystickAxisPosMap.clear();
    joystickAxisNegMap.clear();

    string inputConfigLinePrefix = "input_player";
    string path = getConfigPath();
    ifstream file(path.c_str());

    cout << "Reading input config file...\n";
    while(file.good()) {
      string line;
      getline(file, line);
      string inputConfigLinePrefix = "input_player";

      if (startsWith(line, inputConfigLinePrefix)) {
        vector<string> configLineElems;
        configLineElems = split(line, ' ', configLineElems);
        string configOption = configLineElems[0];
        string configValue = configLineElems[2];
        
        cout << "\tConfig line: " << line << "\n";
        cout << "\t\tOpt: " << configOption << "\n";
        cout << "\t\tVal: " << configValue  << "\n";
      } else {
        cout << "\tUnknwn line: " << line << "\n";
        continue;
      }
    }

/*
    while (file.good()) {
      if (tokens[0] == "BUTTON") {
        joystickButtonMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISPOS") {
        joystickAxisPosMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISNEG") {
        joystickAxisNegMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      }
    }
*/
  }

  void initJoysticks() {
    SDL_JoystickOpen(0);
    SDL_JoystickOpen(1);
  }
}

