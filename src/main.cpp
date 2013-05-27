// glint-ui, a graphical front-end for ROM browsing. Created by Alec "Aloshi" Lofquist and modified by Jeff Lunt

#include <iostream>
#include "Renderer.h"
#include "components/GuiGameList.h"
#include "SystemData.h"
#include <boost/filesystem.hpp>
#include "components/GuiInputConfig.h"
#include <SDL.h>
#include "platform.h"
#include "Input/Input.h"
#include "Input/Init.h"
using namespace std;

#ifdef _RPI_
#include <bcm_host.h>
#endif

#include <sstream>

bool DRAWFRAMERATE = false;
bool keepRunning = true;
int lastTime = 0;
unsigned int width = 0;
unsigned int height = 0;

// Function declarations
void init(int argc, char* argv[]);
namespace fs = boost::filesystem;
void enableJoystick();
void processConfigOrDie();
void shutdown();
void runFrontEnd();
void processCmdLineArgs(int argc, char* argv[]);
void initRendererOrDie();
void ensureConfigDirectoryExists();
bool configSettingsDoNotExist();

// Start app
int main(int argc, char* argv[]) {
  init(argc, argv);
  while (keepRunning) { runFrontEnd(); }
  shutdown();

  return 0;
}

void init(int argc, char* argv[]) {
  bcm_host_init();

  cout << "Starting renderer...";
  initRendererOrDie();
  cout << "OK\n";

  cout << "Enabling joystick...";
  SDL_JoystickEventState(SDL_ENABLE);
  cout << "OK\n";

  cout << "Checking for existence of config directory and files...";
  ensureConfigDirectoryExists();
  cout << "OK\n";

  cout << "Processing config files...";
  processConfigOrDie();
  cout << "OK\n";

  cout.flush();
}

void shutdown() {
  Renderer::deleteAll();
  Renderer::deinit();
  SystemData::deleteSystems();
  bcm_host_deinit();

  cout << "glint-ui cleanly shutting down...\n";

  SDL_Quit();
}

void processConfigOrDie() {
  SystemData::loadConfig();
  Input::initConfig();
  GuiGameList::create();
}

void runFrontEnd() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_JOYHATMOTION:
      case SDL_JOYAXISMOTION:
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        processEvent(&event);
        break;

      case SDL_QUIT:
        keepRunning = false;
        break;
    }
  }

  int curTime = SDL_GetTicks();
  int deltaTime = curTime - lastTime;
  lastTime = curTime;

  GuiComponent::processTicks(deltaTime);
  Renderer::render();

  if (DRAWFRAMERATE) {
    float framerate = 1/((float)deltaTime)*1000;
    stringstream ss;
    ss << framerate;
    string fps;
    ss >> fps;
    Renderer::drawText(fps, 50, 50, 0x00FF00FF, Renderer::getDefaultFont(Renderer::MEDIUM));
  }

  Renderer::swapBuffers();
}

void initRendererOrDie() {
  bool renderInitSuccessful = Renderer::init();
  if(!renderInitSuccessful) {
    cerr << "Error initializing renderer!\n";
    exit(1);
  }
}

void ensureConfigDirectoryExists() {
  string home = getenv("HOME");
  string configDir = home + "/.glint-ui";
  if(!fs::exists(configDir)) {
    cout << "Creating config directory \"" << configDir << "\"\n";
    fs::create_directory(configDir);
  }
}

bool configSettingsDoNotExist() {
  return !fs::exists(SystemData::getConfigPath());
}

