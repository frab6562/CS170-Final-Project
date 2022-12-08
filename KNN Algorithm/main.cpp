//Using Files 67 for Small dataset and 39 for Large dataset
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

//This function uses KNN algorithm to classify objects. It returns an accuracy using cross fold validation.
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

//This function searches for the best subset of features starting with one feature and finishing with all features.
void Feature_Search(vector<double> classes, vector<vector<double>> features){
    vector<int> current_set;
    vector<int> best_set;
    double best_accuracy = 0;
    double bestest_accuracy = 0;
    double accuracy = 0;
    int feature_to_add = 0;
    bool is_in_set = false;
    bool only_once = true;
    
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
                cout << "--- Using feature(s) " << j+1 << " is accuracy " << accuracy << "%" << endl;
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
        else if(only_once){
            cout << "(Warning Accuracy has decreased! Coninuing search in case of local maxima)\n" << endl;
            only_once = false;
        }
    }
    cout << "Finished Search!" << endl;
    cout << "The best feature subset was { ";
    for(int x = 0; x < best_set.size(); ++x){
        cout << best_set.at(x)+1 << " ";
    }
    cout << "} which had an accuracy of " << bestest_accuracy << "%\n" << endl;
}

//This function searches for the best subset of features starting with all the features and removing them one by one until there is only one left.
void Backward_Feature_Search(vector<double> classes, vector<vector<double>> features, vector<int> &current_set){
    vector<int> best_set;
    vector<int> bestest_set;
    vector<int> temp;
    bool only_once = true;
    double best_accuracy = 0;
    double bestest_accuracy = 0;
    double accuracy = 0;
    double counter = current_set.size();
    double counter2 = current_set.size()-1;
    
    for(int k = 0; k < counter2; ++k){
        best_accuracy = 0;
        for(int i = 0; i < counter; ++i){
            temp.clear();
            for(int j = 0; j < counter; ++j){ //This for loop creates a temporary array that holds the subset of the current_set -1 of each feature.
                if(i != j){
                    int x = current_set.at(j);
                    temp.push_back(x);
                }
            }
            accuracy = Leave_One_Out_Cross_Validation(classes,features,temp); //Here we test the accuracy of each subset made previously.
            cout << "--- Using feature(s) { ";
            for(int j = 0; j < counter-1; ++j){
                cout << temp[j]+1 << " ";
            }
            cout << "} is accuracy " << accuracy << "%" << endl;
            if(accuracy > best_accuracy){
                best_accuracy = accuracy;
                best_set.clear();
                for(int j = 0; j < counter-1; ++j)
                    best_set.push_back(temp.at(j));
            }
        }
        current_set.clear();
        cout << "Feature set { ";
        for(int i = 0; i < counter-1; ++i){
            current_set.push_back(best_set.at(i));
            cout << best_set.at(i)+1 << " ";
        }
        cout << "} was best, accuracy was " << best_accuracy << "%\n" << endl;
        
        if(best_accuracy > bestest_accuracy){
            bestest_accuracy = best_accuracy;
            bestest_set.clear();
            for(int x = 0; x < counter-1; ++x){
                bestest_set.push_back(best_set.at(x));
            }
        }
        else if(only_once){
            cout << "(Warning Accuracy has decreased! Coninuing search in case of local maxima)\n" << endl;
            only_once = false;
        }
        --counter;
    }
    
    cout << "Finished Search!" << endl;
    cout << "The best feature subset was { ";
    for(int x = 0; x < bestest_set.size(); ++x){
        cout << bestest_set.at(x)+1 << " ";
    }
    cout << "} which had an accuracy of " << bestest_accuracy << "%\n" << endl;
}

int main() {
    string file;
    char file_length;
    int algorithm;
    vector<int> small{0,1,2,3,4,5};
    vector<int> large;
    vector<vector<double>> features; //This Matrix will hold all the features
    vector<double> classes;  //This Vector will hold all the Classifications
    
    cout << "Welcone to the best Feature Selection Algoithm!" << endl;
    cout << "Is this a large or small file? (l for large or s for small)" << endl;
    cin >> file_length;
    cout << "Type the name of the file you want to run: " << endl;
    cin >> file;
    
    //Checks if the file exists and places all the appropriate variables into arrays.
    if(!readFile(file, classes, features,file_length))
        return 0;
    
    //Ask user for type of algoithm.
    cout << "Type the number of the Algorithm you want to run: \n" << "1) Foward Selection\n" << "2) Backward Elimination" << endl;
    cin >> algorithm;
    
    cout << "The dataset has " << features[0].size() << " features (not including class attrubites), with " << features.size() << " instances" << endl;
    if(file_length == 's'){
        cout << "Running nearest neighbor with all " << features[0].size() << " features, using leave-one-out evaluation, I get an accuracy of " << Leave_One_Out_Cross_Validation(classes,features,small) << "%" << endl;
    }
    else{
        for(int i = 0; i < 40; ++i)
            large.push_back(i);
        cout << "Running nearest neighbor with all " << features[0].size() << " features, using leave-one-out evaluation, I get an accuracy of " << Leave_One_Out_Cross_Validation(classes,features,large) << "%" << endl;
    }
    cout << "Beginning Search... \n" << endl;
    
    if(algorithm == 1)
        Feature_Search(classes, features);
    else if(file_length == 's')
        Backward_Feature_Search(classes, features, small);
    else
        Backward_Feature_Search(classes, features, large);
    return 0;
}
