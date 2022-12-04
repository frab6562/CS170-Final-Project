//Using Files 67 for Small and 39 for Large
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// This function reads in the desired file and then places all the appropiate data into their respective vectors.
void readFile(string file, vector<double> &Class, vector<vector<double>> &feature){
    ifstream fin;
    fin.open(file);
    if (!fin.is_open()){
        cout << "Could not open file: " << file << endl;
        return;
    }
    
    int row = 0;
    double data;
    fin >> data;
    Class.push_back(data);
    feature.push_back(vector<double>());
    
    int counter = 0;
    while(fin >> data){
        if(counter < 6){
            feature[row].push_back(data);
            ++counter;
        }
        else{
            counter = 0;
            ++row;
            Class.push_back(data);
            feature.push_back(vector<double>());
        }
    }
    fin.close();
}

int main() {
    string file;
    int algorithm;
    //This Matrix will hold all the features
    vector<vector<double>> features;
    //This Vector will hold all the Classifications
    vector<double> classes;
    
    cout << "Welcone to the best Feature Selection Algoithm!" << endl;
    cout << "Type the name of the file you want to run: " << endl;
    cin >> file;
    
    readFile(file, classes, features);
    
    cout << "Type the number of the Algoithm you want to run: \n" << "1) Foward Selection\n" << "2) Backward Elimination" << endl;
    cin >> algorithm;
    
    return 0;
}
