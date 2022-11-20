
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include "fmod_files/fmod.hpp"
#include "fmod_files/fmodlinux.h"

using std::cout;
using std::endl;
using std::cin;
using namespace boost::filesystem;
using std::string;
bool ERRSTATE = false; //the ERRSTATE variable, default false, cause the program is not in an error state

int DirectorySizer(const char* currPath){
  int dirSize = 0;
  bool done = false;
  directory_iterator dirSearch1(currPath);
  while (done == false) { //the loop will run until it hits a true condition
    if (dirSearch1 == directory_iterator()) {
      break; //as some extra insurance in case it wants to run again (it shouldn't)
    }
    dirSize++; //this run is to get the size of the directory, which I need for setting up my array
    dirSearch1++;
    if (dirSearch1 == directory_iterator()) {
      done = true;
    }
  }
  return dirSize;
}

void FileHandler(const char* currPath, string* fileList, string* nameList){
  bool done = false; //this function will use a directory iterator loop in order to
  int directorySize = 0; //this will be used to initialize the fileList and numSelections pointers to new arrays of their respective type
  int numDirs = 0;
  int numFiles = 0; //two useful integers will keep track of how many directories and how many files there are (I'll be quasi sorting them, not fully sorting, but quasi)

  //directory_iterator dirSearch1(currPath); moved to DirectorySizer
  directory_iterator dirSearch2(currPath);
  directory_iterator dirSearch3(currPath);
  directory_iterator dirSearch4(currPath);
  directory_iterator dirSearch5(currPath);
  directory_iterator dirSearch6(currPath);

  while (done == false) {
		//this loop will increment and count the number of subdirs vs files
		if (dirSearch2 == directory_iterator()) {
			break; //as some extra insurance in case it wants to run again (it shouldn't)
		}
		if (is_directory(dirSearch2->path())) {
			numDirs++;
		}
		if (!(is_directory(dirSearch2->path()))) {
			numFiles++;
		}
    dirSearch2++;
		if (dirSearch2 == directory_iterator()) {
			done = true;
		}
	}
  done = false;

	int listTracker = 0; //now to initialize the fileList and nameList arrays with their needed data//

	//now to set up fileList

	while (done == false) {
		if (dirSearch3 == directory_iterator()) {
			break;
		}
		if (is_directory(dirSearch3->path())) {
			fileList[listTracker] = dirSearch3->path().string();
			listTracker++;
		}
    dirSearch3++;
		if (dirSearch3 == directory_iterator()) {
			done == true;
		}
	}
  done = false;

  while (done == false) {
    if (dirSearch4 == directory_iterator()) {
      break;
    }
    if (!(is_directory(dirSearch4->path()))) {
      fileList[listTracker] = dirSearch4->path().string();
      listTracker++;
    }
    dirSearch4++;
    if (dirSearch4 == directory_iterator()) {
      done == true;
    }
  }
  done = false;

  //ditto for nameList
	listTracker = 0;

	while (done == false) {
		if (dirSearch5 == directory_iterator()) {
			break;
		}
		if (is_directory(dirSearch5->path())) {
			nameList[listTracker] = dirSearch5->path().filename().string();
			listTracker++;
		}
    dirSearch5++;
		if (dirSearch5 == directory_iterator()) {
			done == true;
		}
	}
	done = false;

	while (done == false) {
		if (dirSearch6 == directory_iterator()) {
			break;
		}
		if (!(is_directory(dirSearch6->path()))) {
			nameList[listTracker] = dirSearch6->path().filename().string();
			listTracker++;
		}
    dirSearch6++;
		if (dirSearch6 == directory_iterator()) {
			done == true;
		}
	}
}

void OutputProcessor(string currPath, string* nameList, int* numList, int maxSize);
bool checkDirecStatus(const char* rootDirec, string pather){
  directory_iterator input(rootDirec);
  bool done = false;
  while (!done){
    if (input->path().filename().string() == pather){
      if (is_directory(input->path())){
        return true;
      }
      else{
        return false;
      }
    }
    input++;
  }
}

int main() {
	//m3player player; // initiate the mp3 player object [currently under construction]
	int* numList;
	int upperBound = 0;
	string* fileList;
	string* nameList;
	int nextCommand;
  string currPath;
  string playPath;
  bool fileCheck; //not super indicative of what it does, but I don't want to mess up anything I've done, so we'll just do that
	bool firstRun = true; //a boolean value to determine whether the program is in first run mode
	bool exitState = false;

  //the mp3player parts
  FMOD::System *playsys;
  FMOD::Sound *sound;
  FMOD::Channel *aChannel;
  FMOD_RESULT result;
  std::string commandWord;
  bool pauseState;

	//time for the greeting
	cout << "Welcome to the M3Player program." << endl;
	cout << "This program is designed to play your typical MP3 file." << endl;
	//more instructions/messages might be added later, to help the user understand the commands

	while (!exitState) {

		if (ERRSTATE) {
			delete[] fileList;
			delete[] nameList;
			delete[] numList;
			firstRun = true;
			ERRSTATE = false;  //these two commands will do the bulk of the error handling, basically if one of the functions detects an error, it will turn on the ERRSTATE flag and immediately return ERRSTATE being true will stop any further functions from being called and kick the program out of the main loop below
			                   //then, this if statement will essentially reset the program into "first run" mode, and approach as if the program is being run for the first time
		}

		if (!ERRSTATE) {
			if (firstRun == true) {
        upperBound = DirectorySizer("./");
        upperBound += 2;
        numList = new int[upperBound];
        for (int i = 0; i < upperBound; i++) {
      		numList[i] = i + 1; //initializes numSelections with numbers, (remember tht numSelections[0] is 1, not 0)
      	}
        fileList = new string[(upperBound - 2)];
        nameList = new string[(upperBound - 2)];
				FileHandler("./", fileList, nameList);
        currPath = "./";
				firstRun = false; //from now on, the program will be using other strings in order to
			}
			else {
        upperBound = DirectorySizer(currPath.c_str());
        upperBound += 2;
        numList = new int[upperBound];
        for (int i = 0; i < upperBound; i++) {
      		numList[i] = i + 1; //initializes numSelections with numbers, (remember tht numSelections[0] is 1, not 0)
      	}
        fileList = new string[(upperBound - 2)];
        nameList = new string[(upperBound - 2)];
				FileHandler(currPath.c_str(), fileList, nameList);
			}
    }

    OutputProcessor(currPath, nameList, numList, upperBound);

    cin >> nextCommand;

    if(nextCommand == upperBound){
      exitState = true;
    }
    else if (nextCommand == 1){
      currPath += "../";
    }
    else{
      fileCheck = checkDirecStatus(currPath.c_str(), nameList[nextCommand - 2]);
      if (fileCheck){
        currPath += nameList[nextCommand -2];
        currPath += "/";
      }
      else{
        playPath = currPath;
        playPath += nameList[nextCommand - 2];
        int checkMP3;
        bool validFile = false;
        checkMP3 = playPath.length();
        if (playPath[checkMP3 - 4] == '.'){
          if (playPath[checkMP3 - 3] == 'm'){
            if (playPath[checkMP3 - 2] == 'p'){
              if (playPath[checkMP3 -1] == '3'){
                validFile = true;
              }
            }
          }
        }
        if (validFile == true){
          cout << "valid mp3 file selected" << endl;
          cout << "Commands:" << endl << "pause -> pause song" << endl << "play (while paused) -> continue song" << endl << "stop -> stop song, go back to selections" << endl;
          result = FMOD::System_Create(&playsys);
          result = playsys->init(1, FMOD_INIT_NORMAL, 0);
          result = playsys->createSound(playPath.c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &sound);
          result =playsys->playSound(FMOD_CHANNEL_FREE, sound, false, &aChannel);
          while(1){
            std::cin >> commandWord;
            if (commandWord == "pause"|| commandWord == "Pause" || commandWord == "p" || commandWord == "P"){
              aChannel->getPaused(&pauseState);
              if (!pauseState){
                aChannel->setPaused(true);
              }
            }
            if (commandWord == "play"|| commandWord == "Play" || commandWord == "c" || commandWord == "C"){
              aChannel->getPaused(&pauseState);
              if(pauseState){
                aChannel->setPaused(false);
              }
            }
            if (commandWord == "stop"|| commandWord == "Stop" || commandWord == "s" || commandWord == "S"){
              break;
            }
          }
          sound->release();
          playsys->close();
          playsys->release();
        }
        else{
          cout << endl << "Error: Not a valid mp3 file!" << endl << endl;
        }
      }
    }
    delete[] fileList;
    delete[] nameList;
    delete[] numList; //these need to be re-used in the loop, so I'll delete

    if (exitState == true){
      break;
    }
  }
  cout << endl << endl << "Thank you for using this program." << endl;
  cout << "Goodbye" << endl;
	return 0;
}

void OutputProcessor (string currPath, string* nameList, int* numList, int maxSize){
  cout << "The current directory is: " << currPath << endl;
  cout << "Available Options: " << endl << endl;
  for (int i = 0; i < maxSize; i++){
    cout << "[" << numList[i] << "]";
    if (i == 0){
      cout << "\t" << ".." << endl;
    }
    else if (i == (maxSize - 1)){
      cout << "\t" << "Exit Program" << endl;
    }
    else{
      cout << "\t" << nameList[i - 1] << endl;
    }
  }
  cout << endl;
  cout << "Please select option using the numbered labels: ";
  cout << endl;
}
