#ifndef _INPUT_INIT_H_
#define _INPUT_INIT_H_

#include <map>
#include <string>
#include <SDL/SDL.h>
using namespace std;

// Reads the input config file and maps buttons to glint-ui action
namespace Input {
  // Returns the path to the config file
  string getConfigPath();

  // Reads the configuration file
  void initConfig();

  // Initializes the first two joysticks available to the system
  void initJoysticks();
}

#endif
