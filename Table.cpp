#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Table {
    string columnNames;            //Column names for table
    string primaryKey;             //Primary key for table
    vector<string>* candidateKeys; //Candidate keys for table
    vector<string>* dependencies;  //Dependencies for table
    int primes[26];                //List of prime attributes in alphabetical order
    public:
        Table(string, string, vector<string>*, vector<string>*);
    private:
        void MakePrimes();
        bool Check2ndForm();
        bool Check3rdForm();
};


//Initializes table and prints the standard normal form
Table::Table (string columnNames, string primaryKey, vector<string>* candidateKeys, vector<string>* dependencies) {
    this->columnNames = columnNames;
    this->primaryKey = primaryKey;
    this->candidateKeys = candidateKeys;
    this-> dependencies = dependencies;

    //Generates prime attribute list
    MakePrimes();

    //Outputs normal form of table
    cout << columnNames << " " << primaryKey << " ";
    for (int i = 0; i < candidateKeys->size(); i++) {
        cout << candidateKeys->at(i) << " ";
    }
    cout << endl;
    if (Check2ndForm()) {
        if (Check3rdForm()) {
            cout << "3 NF" << endl;
        } else {
            cout << "2 NF" << endl;
        }
    } else {
        cout << "1 NF" << endl;
    }
}


//Generates prime attribute list for table
void Table::MakePrimes() {
    int alphabet[26] = {0};

    //Increments index of alphabet for each key and candidate key
    for(int i = 0; i < primaryKey.size(); i++) {
        alphabet[int(primaryKey[i]) - int('A')]++;
    }

    for(int i = 0; i < candidateKeys->size(); i++)
    {
        for(int j = 0; j < candidateKeys->at(i).size(); j++) {
            alphabet[int(candidateKeys->at(i)[j]) - int('A')]++;
        }
    }
    
    //Stores prime list in Table object
    for(int i = 0; i < 26; i++) {
        if (alphabet[i] > 0) {
            primes[i] = 1;
        } else {
            primes[i] = 0;
        }
    }
}


//Verifies if Table is in 2nd normal form
bool Table::Check2ndForm() {
    bool entireKey = false;
    int keys[26];
    //Loop through the list of dependencies by 2s to get the 2nd element of each dependency
    for (int i = 1; i < dependencies->size(); i+=2) {
        
        //cout << dependencies->at(i).at(0) << "\\" << primes[int(dependencies->at(i).at(0)) - int('A')] << "]";
        //Checks to see if the 2nd element of the dependency is not prime
        //If it is prime then the dependency is valid
        if (primes[int(dependencies->at(i).at(0)) - int('A')] == 0) {
            //Initializes variables for dependency
            for(int j = 0; j < 26; j++) {
                keys[j] = 0;
            }
            entireKey = false;

            //Gets the column names from the first element of the dependency and calls them keys
            for (int j = 0; j < dependencies->at(i-1).size(); j++) {
                keys[int(dependencies->at(i-1)[j]) - int('A')]++;
            }

            //Check to see if the entire primary key is in the list of columns
            for (int j = 0 ; j < primaryKey.size(); j++) {
                //cout << primaryKey.at(j) << " " << keys[int(primaryKey.at(j)) - int('A')] << " " << primaryKey.size() <<  endl;
                if (keys[int(primaryKey.at(j)) - int('A')] == 0) {
                    break;
                } else {
                    if (j == primaryKey.size() - 1) entireKey = true;
                }
            }
            //Removes columns from keys list if the entire primary key is used
            if (entireKey) {
                for (int j = 0 ; j < primaryKey.size(); j++) {
                    keys[int(primaryKey.at(j)) - int('A')] = 0;
                }
            }

            //Duplicates above behavior for candidate keys
            for (int k = 0; k < candidateKeys->size(); k++) {
                entireKey = false;
                for (int j = 0 ; j < candidateKeys->at(k).size(); j++) {
                    //cout << candidateKeys->at(k)[j] << "\\" << keys[int(candidateKeys->at(k)[j]) - int('A')] << "]";
                    if (keys[int(candidateKeys->at(k)[j]) - int('A')] == 0) {
                        break;
                    } else {
                        if (j == candidateKeys->at(k).size() - 1) entireKey = true;
                    }
                }
                if (entireKey) {
                    for (int j = 0 ; j < candidateKeys->at(k).size(); j++) {
                        keys[int(candidateKeys->at(k)[j]) - int('A')] = 0;
                    }
                }
            }
            
            //Returns false if there are any columns that are prime left after removing entire primary and candidate keys
            for (int j = 0; j < 26; j++) {
                //cout << "F:" << keys[j] << " " <<primes[j] << endl;
                if (keys[j] > 0 && primes[j]) {
                    return false;
                }
            }
        }
    }
    return true;
}


//Verifies if table is in 3rd normal form
bool Table::Check3rdForm() {
    bool entireKey = false;
    int keys[26];
    //Loop through the list of dependencies by 2s to get the 2nd element of each dependency
    for (int i = 1; i < dependencies->size(); i+=2) {
        
        //cout << dependencies->at(i).at(0) << "\\" << primes[int(dependencies->at(i).at(0)) - int('A')] << "]";
        //Checks to see if the 2nd element of the dependency is not prime
        //If it is prime then the dependency is valid
        if (primes[int(dependencies->at(i).at(0)) - int('A')] == 0) {
            //Initializes variables for dependency
            for(int j = 0; j < 26; j++) {
                keys[j] = 0;
            }
            entireKey = false;

            //Gets the column names from the first element of the dependency and calls them keys
            for (int j = 0; j < dependencies->at(i-1).size(); j++) {
                keys[int(dependencies->at(i-1)[j]) - int('A')]++;
            }

            //Check to see if the entire primary key is in the list of columns
            for (int j = 0 ; j < primaryKey.size(); j++) {
                //cout << primaryKey.at(j) << " " << keys[int(primaryKey.at(j)) - int('A')] << " " << primaryKey.size() <<  endl;
                if (keys[int(primaryKey.at(j)) - int('A')] == 0) {
                    break;
                } else {
                    if (j == primaryKey.size() - 1) entireKey = true;
                }
            }
            //If the entire primary key is in the list of columns then it is a superkey and this dependency is valid
            if (entireKey) {
                continue;
            }

            //Same behavior for candidate keys
            for (int k = 0; k < candidateKeys->size(); k++) {
                entireKey = false;
                for (int j = 0 ; j < candidateKeys->at(k).size(); j++) {
                    //cout << candidateKeys->at(k)[j] << "\\" << keys[int(candidateKeys->at(k)[j]) - int('A')] << "]";
                    if (keys[int(candidateKeys->at(k)[j]) - int('A')] == 0) {
                        break;
                    } else {
                        if (j == candidateKeys->at(k).size() - 1) entireKey = true;
                    }
                }
                if (entireKey) {
                    break;
                }
            }
            if(entireKey) continue;

            //Returns false if there are no primary or candidate keys in the list of columns
            return false;
        }
    }
    return true;
}

