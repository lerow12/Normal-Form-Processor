#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Table {
    string columnNames;           //Column names for table
    string primaryKey;
    vector<string>* candidateKeys; //Candidate keys for table
    vector<string>* dependencies;
    int primes[26];
    public:
        //void GetPrimes();
        Table(string, string, vector<string>*, vector<string>*);
    private:
        void MakePrimes();
        bool Check2ndForm();
        bool Check3rdForm();
};

Table::Table (string columnNames, string primaryKey, vector<string>* candidateKeys, vector<string>* dependencies) {
    this->columnNames = columnNames;
    this->primaryKey = primaryKey;
    this->candidateKeys = candidateKeys;
    this-> dependencies = dependencies;

    MakePrimes();
    if (Check2ndForm()) {
        if (Check3rdForm()) {
            cout << "Table is in 3rd Normal Form" << endl;
        } else {
            cout << "Table is in 2nd Normal Form" << endl;
        }
    } else {
        cout << "Table is in 1st Normal Form" << endl;
    }
}

void Table::MakePrimes() {
    int alphabet[26] = {0};

    for(int i = 0; i < primaryKey.size(); i++) {
        alphabet[int(primaryKey[i]) - int('A')]++;
    }

    for(int i = 0; i < candidateKeys->size(); i++)
    {
        for(int j = 0; j < candidateKeys->at(i).size(); j++) {
            alphabet[int(candidateKeys->at(i)[j]) - int('A')]++;
        }
    }
    
    for(int i = 0; i < 26; i++) {
        if (alphabet[i] > 0) {
            primes[i] = 1;
        } else {
            primes[i] = 0;
        }
    }
}

bool Table::Check2ndForm() {
    bool entireKey = false;
    int keys[26];
    for (int i = 1; i < dependencies->size(); i+=2) {
        
        //cout << dependencies->at(i).at(0) << "\\" << primes[int(dependencies->at(i).at(0)) - int('A')] << "]";
        if (primes[int(dependencies->at(i).at(0)) - int('A')] == 0) {
            for(int j = 0; j < 26; j++) {
                keys[j] = 0;
            }
            entireKey = false;

            for (int j = 0; j < dependencies->at(i-1).size(); j++) {
                keys[int(dependencies->at(i-1)[j]) - int('A')]++;
            }

            for (int j = 0 ; j < primaryKey.size(); j++) {
                //cout << primaryKey.at(j) << " " << keys[int(primaryKey.at(j)) - int('A')] << " " << primaryKey.size() <<  endl;
                if (keys[int(primaryKey.at(j)) - int('A')] == 0) {
                    break;
                } else {
                    if (j == primaryKey.size() - 1) entireKey = true;
                }
            }
            if (entireKey) {
                for (int j = 0 ; j < primaryKey.size(); j++) {
                    keys[int(primaryKey.at(j)) - int('A')] = 0;
                }
            }

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

bool Table::Check3rdForm() {
    bool entireKey = false;
    int keys[26];
    for (int i = 1; i < dependencies->size(); i+=2) {
        
        //cout << dependencies->at(i).at(0) << "\\" << primes[int(dependencies->at(i).at(0)) - int('A')] << "]";
        if (primes[int(dependencies->at(i).at(0)) - int('A')] == 0) {
            for(int j = 0; j < 26; j++) {
                keys[j] = 0;
            }
            entireKey = false;

            for (int j = 0; j < dependencies->at(i-1).size(); j++) {
                keys[int(dependencies->at(i-1)[j]) - int('A')]++;
            }

            for (int j = 0 ; j < primaryKey.size(); j++) {
                //cout << primaryKey.at(j) << " " << keys[int(primaryKey.at(j)) - int('A')] << " " << primaryKey.size() <<  endl;
                if (keys[int(primaryKey.at(j)) - int('A')] == 0) {
                    break;
                } else {
                    if (j == primaryKey.size() - 1) entireKey = true;
                }
            }
            if (entireKey) {
                continue;
            }

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
                    continue;
                }
            }

            return false;
        }
    }
    return true;
}

