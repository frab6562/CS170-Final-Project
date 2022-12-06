//Using Files 67 for Small dataset and 39 for Large dataset// 96 and 21
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

// This function reads in the desired file and then places all the appropiate data into their respective vectors.
bool readFile(string file, vector<double> &classes, vector<vector<double>> &features, char file_length){
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
    int count_to = 0;
    if(file_length == 's')
        count_to = 6;
    else
        count_to = 40;
        
        while(fin >> data){
            if(counter < count_to){
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

double Leave_One_Out_Cross_Validation(vector<double> classes, vector<vector<double>> features, vector<int> current_set){
    double k = classes.size();
    double distance = 0;
    double accuracy = 0;
    
    for(int i = 0; i < k; ++i){
        double nearest_neighbor_distance = 10000;
        double nearest_neighbor_location = 0;
        int nearest_neighbor_label = 0;
        
        for(int j = 0; j < k; ++j){
            if(j != i){
                for(int x = 0; x < current_set.size(); ++x){
                    distance = pow(features[i][current_set.at(x)]-features[j][current_set.at(x)],2) + distance;
                    distance = sqrt(distance);
                }
                if(distance < nearest_neighbor_distance){
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = classes.at(j);
                }
                distance = 0;
            }
        }
        if(nearest_neighbor_label == classes.at(i))
            ++accuracy;
    }
    return (accuracy/k)*100;
}

void Feature_Search(vector<double> classes, vector<vector<double>> features){
    vector<int> current_set;
    vector<int> best_set;
    double best_accuracy = 0;
    double bestest_accuracy = 0;
    double accuracy = 0;
    int feature_to_add = 0;
    bool is_in_set = false;

    for(int i = 0; i < features[0].size(); ++i){
        best_accuracy = 0;
        feature_to_add = 0;
        for(int j = 0; j < features[0].size(); ++j){
            for(int k = 0; k < current_set.size(); ++k){ //This extra for loop checks to see if the feature is already in the set.
                if( j == current_set.at(k))
                    is_in_set = true;
            }
            if(!is_in_set){
                current_set.push_back(j);
                accuracy = Leave_One_Out_Cross_Validation(classes,features,current_set);
                cout << "--- Using feature(s) " << j+1 << " is accuracy " << accuracy << "%" << endl;;
                if(accuracy > best_accuracy){
                    best_accuracy = accuracy;
                    feature_to_add = j;
                }
                current_set.pop_back();
            }
            is_in_set = false;
        }
        current_set.push_back(feature_to_add);
        cout << "Feature set { ";
        for(int x = 0; x < current_set.size(); ++x)
            cout << current_set.at(x)+1 << " ";
        cout << "} was best, accuracy was " << best_accuracy << "%\n" << endl;
        
        if(best_accuracy > bestest_accuracy){
            bestest_accuracy = best_accuracy;
                best_set.clear();
            for(int x = 0; x < current_set.size(); ++x){
                best_set.push_back(current_set.at(x));
            }
        }
    }
    cout << "Finished Search!" << endl;
    cout << "The best feature subset was { ";
    for(int x = 0; x < best_set.size(); ++x){
        cout << best_set.at(x)+1 << " ";
    }
    cout << "} which had an accuracy of " << bestest_accuracy << "%\n" << endl;
}

int main() {
    string file;
    char file_length;
    int algorithm;
    vector<int> intial{0,1,2,3,4,5};
    vector<vector<double>> features; //This Matrix will hold all the features
    vector<double> classes;  //This Vector will hold all the Classifications
    
    cout << "Welcone to the best Feature Selection Algoithm!" << endl;
    cout << "Is this a large or small file? (l for large or s for small)" << endl;
    cin >> file_length;
    cout << "Type the name of the file you want to run: " << endl;
    cin >> file;
    
    if(!readFile(file, classes, features,file_length))
        return 0;
    
    cout << "Type the number of the Algoithm you want to run: \n" << "1) Foward Selection\n" << "2) Backward Elimination" << endl;
    cin >> algorithm;
    
    cout << "The dataset has " << features[0].size() << " features (not including class attrubites), with " << features.size() << " instances" << endl;
    cout << "Running nearest neighbor with all " << features[0].size() << " features, using leave-one-out evaluation, I get an accuracy of " << Leave_One_Out_Cross_Validation(classes,features,intial) << "%" << endl;
    cout << "Beginning Search... \n" << endl;
    
    Feature_Search(classes, features);
    
    return 0;
}
