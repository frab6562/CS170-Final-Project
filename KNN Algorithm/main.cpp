//Using Files 67 for Small dataset and 39 for Large dataset
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

// This function reads in the desired file and then places all the appropiate data into their respective vectors.
bool readFile(string file, vector<double> &classes, vector<vector<double>> &features){
    ifstream fin;
    fin.open(file);
    if (!fin.is_open()){
        cout << "ERROR: File does not Exist: " << file << endl;
        return false;
    }
    
    int row = 0;
    double data;
    fin >> data;
    classes.push_back(data);
    features.push_back(vector<double>());
    
    int counter = 0;
    while(fin >> data){
        if(counter < 6){
            features[row].push_back(data);
            ++counter;
        }
        else{
            counter = 0;
            ++row;
            classes.push_back(data);
            features.push_back(vector<double>());
        }
    }
    fin.close();
    return true;
}

int Leave_One_Out_Cross_Validation(/*vector<double> classes, vector<vector<double>> features, */vector<int> current_set){
    srand (time(NULL));
    return rand() % 100;
}

void Feature_Search(vector<double> classes, vector<vector<double>> features){
    vector<int> current_set;
    int best_accuracy = 0;
    int feature_to_add = 0;
    int accuracy = 0;
    
    for(int i = 0; i < features[0].size(); ++i){
        cout << "On level " << i+1 << " of the search tree." << endl;
        best_accuracy = 0;
        feature_to_add = 0;
        for(int j = 0; j < features[0].size(); ++j){
            
            cout << "-- Considering adding feature " << j+1 << endl;
            accuracy = Leave_One_Out_Cross_Validation(current_set);
            if(accuracy > best_accuracy){
                best_accuracy = accuracy;
                feature_to_add = j+1;
            }
        }
        current_set.push_back(feature_to_add);
        cout << "On level " << i+1 << " I added feature " << feature_to_add << " to current set" << endl;
    }
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
    
    Feature_Search(classes, features);
    
    return 0;
}
