//EmulationStation, a graphical front-end for ROM browsing. Created by Alec "Aloshi" Lofquist.

#include <iostream>
#include "Renderer.h"
#include "components/GuiGameList.h"
#include "SystemData.h"
#include <boost/filesystem.hpp>
#include "components/GuiInputConfig.h"
#include <SDL.h>
#include "platform.h"

#ifdef _RPI_
  #include <bcm_host.h>
#endif

#include <sstream>

bool DRAWFRAMERATE = true;
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
void initRendererOrDie(int width, int height);
void ensureConfigDirectoryExists();
void createDemoConfig();
bool configSettingsDoNotExist();
bool configExistsButIsEmpty();


// Start app
int main(int argc, char* argv[]) {
  init(argc, argv);
  while (keepRunning) { runFrontEnd(); }
  shutdown();
  
	return 0;
}

void init(int argc, char* argv[]) {
  std::cout << "Processing command line args...";
  processCmdLineArgs(argc, argv);
  std::cout << "OK\n";
  
  std::cout << "Starting renderer...";
  initRendererOrDie(width, height);
  std::cout << "OK\n";
  
  std::cout << "Enabling joystick...";
  enableJoystick();
  std::cout << "OK\n";
  
  std::cout << "Checking for existence of config directory and files...";
  ensureConfigDirectoryExists();
  std::cout << "OK\n";
  
  std::cout << "Processing config files...";
  processConfigOrDie();
  std::cout << "OK\n";
  
  std::cout.flush();
}

void enableJoystick() {
  SDL_JoystickEventState(SDL_ENABLE);
}

void processConfigOrDie() {
  if (configSettingsDoNotExist())	{
    createDemoConfig();
	  keepRunning = false;

	} else if (configExistsButIsEmpty()) {
		std::cerr << "A system config file in " << SystemData::getConfigPath() << " was found, but contained no systems.\n";
		std::cerr << "Does at least one system have a game presesnt?\n";
		keepRunning = false;
	} else {
	  //choose which GUI to open depending on Input configuration
		if(fs::exists(InputManager::getConfigPath()))
		{
			//an input config already exists - load it and proceed to the gamelist as usual.
			InputManager::loadConfig();
			GuiGameList::create();
		}else{
			//if no input.cfg is present, but a joystick is connected, launch the input config GUI
			if(SDL_NumJoysticks() > 0)
				new GuiInputConfig();
			else
				GuiGameList::create();

		}
	}
}

void shutdown() {
	Renderer::deleteAll();
	Renderer::deinit();
	SystemData::deleteSystems();

	std::cout << "EmulationStation cleanly shutting down...\n";

	SDL_Quit();
}

void runFrontEnd() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
  	switch(event.type) {
  		case SDL_JOYHATMOTION:
  		case SDL_JOYAXISMOTION:
  		case SDL_JOYBUTTONDOWN:
  		case SDL_JOYBUTTONUP:
  		case SDL_KEYDOWN:
  		case SDL_KEYUP:
  			InputManager::processEvent(&event);
        std::cout << "Input captured (" << event.type << ")\n";
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
		std::stringstream ss;
		ss << framerate;
		std::string fps;
		ss >> fps;
		Renderer::drawText(fps, 50, 50, 0x00FF00FF, Renderer::getDefaultFont(Renderer::MEDIUM));
	}

	Renderer::swapBuffers();
}

void processCmdLineArgs(int argc, char* argv[]) {
  if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-w") == 0) {
				width = atoi(argv[i + 1]);
				i++;
			} else if(strcmp(argv[i], "-h") == 0) {
				height = atoi(argv[i + 1]);
				i++;
			} else if(strcmp(argv[i], "--draw-framerate") == 0) {
				DRAWFRAMERATE = true;
			} else if(strcmp(argv[i], "--help") == 0) {
				std::cout << "EmulationStation, a graphical front-end for ROM browsing.\n";
				std::cout << "Command line arguments:\n";
				std::cout << "-w [width in pixels]		set screen width\n";
				std::cout << "-h [height in pixels]		set screen height\n";
				std::cout << "--draw-framerate		display the framerate\n";
				std::cout << "--help				summon a sentient, angry tuba\n\n";
				std::cout << "More information available in README.md.\n";
			}
		}
	}
}

void initRendererOrDie(int width, int height) {
  bool renderInitSuccessful = Renderer::init(width, height);
	if(!renderInitSuccessful) {
		std::cerr << "Error initializing renderer!\n";
		exit(1);
	}
}

void ensureConfigDirectoryExists() {
  std::string home = getenv("HOME");
	std::string configDir = home + "/.emulationstation";
	if(!fs::exists(configDir)) {
		std::cout << "Creating config directory \"" << configDir << "\"\n";
		fs::create_directory(configDir);
	}
}

void createDemoConfig() {
  if (configSettingsDoNotExist()) {
    std::cerr << "A system config file in " << SystemData::getConfigPath() << " was not found. An example will be created.\n";
    SystemData::writeExampleConfig();
    std::cerr << "Set it up, then re-run glint-es.\n";
  }
}

bool configSettingsDoNotExist() {
  return !fs::exists(SystemData::getConfigPath());
}

bool configExistsButIsEmpty() {
  SystemData::loadConfig();
  return (SystemData::sSystemVector.size() == 0);
}