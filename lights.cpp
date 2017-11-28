#include "./cnf2sat.hpp"

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
  //get the initial states of all lights, the second line
  getline(iFile,line);
  stringstream state(line);
  initialState = new int [numLights];
  for (int i = 0; i < numLights; i++){
    getline(state, word, ',');
    initialState[i] = atoi(word.c_str());
  }
  //get switches and connections
  int switchID = 1;
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
      /*//DEBUG: per step display
      for (int i = 0; i < numLights; i++){
        cout << "STATE: " << initialState[i] << " CONNECTIONS: " << a[i] << "," << b[i] << "\n";;
      } cout << "\n"; /**/
    }
    switchID++;
  }
  delete [] c; //no longer need c array, de-allocate

/*//DEBUG: final display from input file
  //initial vars,
  cout <<"SWITCHES: "<< numSwitches << " LIGHTS: " << numLights << "\n";
  //show initial state of each light and its connections
  for (int i = 0; i < numLights; i++){
    cout << "STATE: " << initialState[i] << " CONNECTIONS: " << a[i] << "," << b[i] << "\n";;
  }
/**/

  //construct clauses
  int numClauses = 0;
  vector<pair < int, int > > test;
  for (int i = 0; i < numLights; i++){
    if (initialState[i] == 0){
      //light is off
      //full clause is:            (x ^ y) v (!x ^ !y)
      //full clause in 2-SAT form: (x v !x) ^ (y v !y) ^ (x v !y) ^ (!x v y)
      test.push_back(make_pair(a[i], a[i] * (-1)));
      test.push_back(make_pair(b[i], b[i] * (-1)));
      test.push_back(make_pair(a[i], b[i] * (-1)));
      test.push_back(make_pair(a[i] * (-1), b[i]));
      numClauses += 4;
    } else {
      //light is on
      //full clause is:           (x v y) ^ (-x v -y)
      test.push_back(make_pair(a[i], b[i]));
      test.push_back(make_pair(a[i] * (-1), b[i] * (-1)));
      numClauses += 2;
    }
  }

  //cout << "CLAUSES: " << numClauses << "\n"

  if(satisfiable(test) == 1){
    cout << "yes\n";
    oFile << "yes";
  } else {
    cout << "no\n";
    oFile << "no";
  }
  test.clear();

  delete [] a;
  delete [] b;
  delete [] initialState;

  iFile.close();
  oFile.close();
  return 0;
}
