//Using Files 67 for Small dataset and 39 for Large dataset
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// This function reads in the desired file and then places all the appropiate data into their respective vectors.
bool readFile(string file, vector<double> &Class, vector<vector<double>> &feature){
    ifstream fin;
    fin.open(file);
    if (!fin.is_open()){
        cout << "ERROR: File does no Exist" << file << endl;
        return false;
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
    return true;
}



int main() {
    string file;
    int algorithm;
    vector<vector<double>> features; //This Matrix will hold all the features
    vector<double> classes;  //This Vector will hold all the Classifications
    
    cout << "Welcone to the best Feature Selection Algoithm!" << endl;
    cout << "Type the name of the file you want to run: " << endl;
    cin >> file;
    
    if(!readFile(file, classes, features))
        return 0;
    
    cout << "Type the number of the Algoithm you want to run: \n" << "1) Foward Selection\n" << "2) Backward Elimination" << endl;
    cin >> algorithm;
    
    cout << "The dataset has " << features[0].size() << " features (not including class attrubites), with " << features.size() << " instances" << endl;
    cout << "Beginning Search... " << endl;
    
    
    return 0;
}
