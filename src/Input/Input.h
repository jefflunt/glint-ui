#ifndef _INPUT_INPUT_H_
#define _INPUT_INPUT_H_

#include <vector>
#include <SDL/SDL.h>
#include <map>
#include <string>
using namespace std;

class GuiComponent;

// The input takes native system input and abstracts it into InputButtons.
// GuiComponents can be registered to receive onInput() events.
namespace Input {
  // Defines the input deadzone, or joystick centered size
  #define DEADZONE 28000

  void registerComponent(GuiComponent* comp);
  void unregisterComponent(GuiComponent* comp);

  //enum for identifying input, regardless of configuration
  enum InputButton {
    UNKNOWN,
    AXIS_UP,
    AXIS_DOWN,
    AXIS_LEFT,
    AXIS_RIGHT,
    BTN_1,
    BTN_2,
    BTN_MENU,
    BTN_SELECT,
    BTN_PAGEUP,
    BTN_PAGEDOWN
  };

  // Handle input events
  void processEvent(SDL_Event* event);

  // A vector of GUI components that accept input events
  extern vector<GuiComponent*> inputVector;

  // The last event triggered
  extern SDL_Event* lastEvent;

  // The list of button => joystick mappings
  extern map<int, InputButton> joystickButtonMap;

  // The list of axis => joystick mappings
  extern map<int, InputButton> joystickAxisPosMap, joystickAxisNegMap;

  // The current state of the axes
  extern map<int, int> axisState;

  // The current state of the hat
  extern InputButton hatState;
} // namespace

#endif
