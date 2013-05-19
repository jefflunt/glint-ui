using namespace std;
#include "InputManager.h"
#include "GuiComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace input {
  std::vector<GuiComponent*> inputVector;
  SDL_Event* lastEvent = NULL;

  std::map<int, InputButton> joystickButtonMap, joystickAxisPosMap, joystickAxisNegMap;
  std::map<int, int> axisState;
  InputButton hatState = UNKNOWN;

  int deadzone = 28000;

  void registerComponent(GuiComponent* comp) {
    inputVector.push_back(comp);
  }

  void unregisterComponent(GuiComponent* comp) {
    for(unsigned int i = 0; i < inputVector.size(); i++) {
      if(inputVector.at(i) == comp) {
        inputVector.erase(inputVector.begin() + i);
        break;
      }
    }
  }

  void processEvent(SDL_Event* event) {
    bool keyDown = false;
    InputButton button = UNKNOWN;

    lastEvent = event;

    //keyboard events
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
      if(event->key.state == SDL_PRESSED)
        keyDown = true;

      switch (event->key.keysym.sym) {
        case SDLK_LEFT:
          button = LEFT;
          break;
        case SDLK_RIGHT:
          button = RIGHT;
          break;
        case SDLK_UP:
          button = UP;
          break;
        case SDLK_DOWN:
          button = DOWN;
          break;
        case SDLK_PAGEUP:
          button = PAGEUP;
          break;
        case SDLK_PAGEDOWN:
          button = PAGEDOWN;
          break;
        case SDLK_RETURN:
          button = BUTTON1;
          break;
        case SDLK_ESCAPE:
          button = BUTTON2;
          break;
        case SDLK_F1:
          button = MENU;
          break;
        case SDLK_F2:
          button = SELECT;
          break;

        default:
          button = UNKNOWN;
          break;
      }

      //catch emergency quit event
      if (event->key.keysym.sym == SDLK_F4 && keyDown) {
        //I have no idea if SDL will delete this event, but we're quitting, so I don't think it really matters
        SDL_Event* quit = new SDL_Event();
        quit->type = SDL_QUIT;
        SDL_PushEvent(quit);
        std::cout << "Pushing quit event\n";
      }
    } else {
      if (event->type == SDL_JOYBUTTONDOWN || event->type == SDL_JOYBUTTONUP) {
        if(event->type == SDL_JOYBUTTONDOWN)
          keyDown = true;

        button = joystickButtonMap[event->jbutton.button];
      } else {
        if (event->type == SDL_JOYHATMOTION) {
          int hat = event->jhat.value;

          // if centered
          if (hat == 0) {
            //we need to send a keyUp event for the last hat
            //keyDown is already false
            button = hatState;
          } else {
            keyDown = true;
          }

          if (hat & SDL_HAT_LEFT)
            button = LEFT;
          if (hat & SDL_HAT_RIGHT)
            button = RIGHT;

          if (hat & SDL_HAT_UP)
            button = UP;
          if (hat & SDL_HAT_DOWN)
            button = DOWN;

          if (button == hatState && keyDown) {
            //ignore this hat event since the user most likely just made it a diagonal (but it still is using the old direction)
            button = UNKNOWN;
          } else {
            if (hatState != UNKNOWN && keyDown) {
              //this will occur if the user went down -> downLeft -> Left or similar
              button = hatState;
              keyDown = false;
              hatState = UNKNOWN;
              processEvent(event);
            } else {
              if(!keyDown)
                hatState = UNKNOWN;
              else
                hatState = button;
            }
          }
        } else {
          if (event->type == SDL_JOYAXISMOTION) {
            int axis = event->jaxis.axis;
            int value = event->jaxis.value;

            // if this axis was previously not centered, it can only keyUp
            if (axisState[axis] != 0) {
              if(abs(value) < deadzone) {
                if(axisState[axis] > 0)
                  button = joystickAxisPosMap[axis];
                else
                  button = joystickAxisNegMap[axis];
                axisState[axis] = 0;
              }
            } else {
              if (value > deadzone) {
                //axisPos keyDown
                axisState[axis] = 1;
                keyDown = true;
                button = joystickAxisPosMap[axis];
              } else if(value < -deadzone) {
                axisState[axis] = -1;
                keyDown = true;
                button = joystickAxisNegMap[axis];
              }
            }
          }
        }
      }
    }

    for (unsigned int i = 0; i < inputVector.size(); i++) {
      inputVector.at(i)->onInput(button, keyDown);
    }
  }

  // Loads the input (controller) config file and assigns buttons
  void loadConfig() {
    //clear any old config
    joystickButtonMap.clear();
    joystickAxisPosMap.clear();
    joystickAxisNegMap.clear();

    std::string cfgLinesPrefix = "input_player";
    std::string path = getConfigPath();
    std::ifstream file(path.c_str());

    std::cout << "Reading input config file...\n";
    while (file.good()) {
      std::string line;
      std::getline(file, line);

      // Only read lines that start with cfgLinesPrefix
      if (cfgLinesPrefix != line.substr(0, cfgLinesPrefix.size())) {
        std::cout << "\tUnknwn line: " << line << "\n";
        continue;
      } else {
        std::cout << "\tConfig line: " << line << "\n";
      }

      std::istringstream stream(line);

      std::string tokens[3];
      int tokNum = 0;

      // Break config line into the various tokens
      while (std::getline(stream, tokens[tokNum], ' '))
        tokNum++;

      if (tokens[0] == "BUTTON") {
        joystickButtonMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISPOS") {
        joystickAxisPosMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      } else if(tokens[0] == "AXISNEG") {
        joystickAxisNegMap[atoi(tokens[1].c_str())] = (InputButton)atoi(tokens[2].c_str());
      }
    }

    // Open the first two joysticks (there may be only 1 or 0 joysticks - in this case we ignore that fact)
    SDL_JoystickOpen(0);
    SDL_JoystickOpen(1);
  }

  std::string getConfigPath() {
    std::string home = getenv("HOME");
    home += "/.retroarch/retroarch.cfg";
    return home;
  }
} // namespace
