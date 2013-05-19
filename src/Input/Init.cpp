#include "Init.h"
#include <map>
#include <string>
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Input.h"
using namespace std;
using namespace Input;

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

    string cfgLinesPrefix = "input_player";
    string path = getConfigPath();
    ifstream file(path.c_str());

    cout << "Reading input config file...\n";
    while (file.good()) {
      string line;
      getline(file, line);

      // Only read lines that start with cfgLinesPrefix
      if (cfgLinesPrefix != line.substr(0, cfgLinesPrefix.size())) {
        cout << "\tUnknwn line: " << line << "\n";
        continue;
      } else {
        cout << "\tConfig line: " << line << "\n";
      }

      istringstream stream(line);

      string tokens[3];
      int tokNum = 0;

      // Break config line into the various tokens
      while (getline(stream, tokens[tokNum], ' '))
        tokNum++;

      if (tokens[0] == "BUTTON") {
        joystickButtonMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISPOS") {
        joystickAxisPosMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISNEG") {
        joystickAxisNegMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      }
    }
  }

  void initJoysticks() {
    SDL_JoystickOpen(0);
    SDL_JoystickOpen(1);
  }
}
