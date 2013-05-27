#ifndef _SYSTEMDATA_H_
#define _SYSTEMDATA_H_

#include <vector>
#include <string>
#include "FolderData.h"
using namespace std;

class GameData;

class SystemData {
  public:
    SystemData(string name, string startPath, string extension, string command);
    ~SystemData();

    FolderData* getRootFolder();
    string getName();
    string getStartPath();
    string getExtension();
    bool hasGamelist();

    void launchGame(GameData* game);

    static void deleteSystems();
    static void loadConfig();
    static void writeExampleConfig();
    static string getConfigPath();

    static vector<SystemData*> sSystemVector;

  private:
    string mName;
    string mStartPath;
    string mSearchExtension;
    string mLaunchCommand;

    void populateFolder(FolderData* folder);

    FolderData* mRootFolder;
};

#endif
