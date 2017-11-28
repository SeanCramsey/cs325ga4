#include "./cnf2sat.hpp"

#include <stdio.h>

//#include <iostream>
//#include <string>
//#include <sstream>
//#include <fstream>

using namespace std;

int main() {

  int numSwitches,      //Total number of switches
      numLights,        //Total number of lights
      *initialState,    //The initial state of each light, on or off
                        //each light's connections
      *a,               //1st connection
      *b,               //2nd connection
      *c;               //num connections a light has

  ifstream iFile("./input.txt");
  if (!iFile){
    cout << "Could not read input file\n";
    exit(EXIT_FAILURE);
  }
  ofstream oFile("./output.txt");
  if (!oFile){
    cout << "Could not read output file\n";
    exit(EXIT_FAILURE);
  }
  string line = "";
  string word = "";
  //get numSwitches and numLights, the first line
  getline(iFile,line);
  stringstream vars(line);
  getline(vars, word, ',');
  numSwitches = atoi(word.c_str());
  getline(vars, word);
  numLights = atoi(word.c_str());
  //cout << numSwitches << ", " << numLights << "\n";
  //get the initial states of all lights, the second line
  getline(iFile,line);
  stringstream state(line);
  initialState = new int [numLights];
  for (int i = 0; i < numLights; i++){
    getline(state, word, ',');
    initialState[i] = atoi(word.c_str());
  }
  cout << "\n";
  //get switches and connections
  int switchID = 0;
  a = new int[numLights];
  b = new int[numLights];
  c = new int[numLights];
  while(getline(iFile, line)){
    stringstream connect(line);
    int lightID;
    while(getline(connect, word, ',')){
      lightID = atoi(word.c_str()) - 1;
      if (c[lightID] == 1){ //if the light already has a connection
        b[lightID] = switchID;
      } else {              //add first switch connection to light
        a[lightID] = switchID;
        c[lightID]++;
      }
      /*//DEBUG TEST, per step display
      for (int i = 0; i < numLights; i++){
        cout << "STATE: " << initialState[i] << " CONNECTIONS: " << a[i] << "," << b[i] << "\n";;
      } cout << "\n"; /**/
    }
    switchID++;
  }
  free(c);



//DEBUG final display from input file
  //initial vars,
  cout <<"SWITCHES: "<< numSwitches << " LIGHTS: " << numLights << "\n";
  //show initial state of each light and its connections
  for (int i = 0; i < numLights; i++){
    cout << "STATE: " << initialState[i] << " CONNECTIONS: " << a[i] << "," << b[i] << "\n";;
  }
//

  iFile.close();
  oFile.close();
  /**/
  return 0;
}
/* tests from provided file
  int numClauses = 2;
  int a[] = {-2, 2};
  int b[] = {-2, 2};
  vector<pair<int, int> > test;
  for(int i = 0; i < numClauses; i++) {
      test.push_back(make_pair(a[i],b[i]));
  }
  cout << "Expected: 0\nActual: " << satisfiable(test) << endl;
  test.clear();

  numClauses = 7;
  int c[] = {1, -2, -1, 3, -3, -4, -3};
  int d[] = {2, 3, -2, 4, 5, -5, 4};

  for(int i = 0; i < numClauses; i++) {
      test.push_back(make_pair(c[i],d[i]));
  }
  cout << "\nExpected: 1\nActual: " << satisfiable(test) << endl;
*/
