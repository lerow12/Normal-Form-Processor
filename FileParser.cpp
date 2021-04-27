#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Table.cpp"
using namespace std;

//Obtains all tables from input file and store them into instances of Table class
class FileParser {
    ifstream inputFile;           //Input file stream
    ofstream logFile;             //Log file stream
    bool logEnable;               //Enables or disables logging
    string columnNames;           //Temporary column names for current table
    string primaryKey;            //Temporary primary key for current table
    vector<string>* candidateKeys; //Temporary candidate keys for current table
    vector<string>* dependencies;  /*Temorary dependencies for current table
                                    Note: Pairs of indecies represent the dependency [2n]->[2n+1]*/
    vector<Table*>* tables;
    public:
        FileParser(string, bool);
        void run();
        void Tables();
    private:
        void GetColumnsAndKeys();
        void GetDependencies();
        void MakeTable();
};

//Constructor takes file name and opens corresponding file
FileParser::FileParser(string fileName, bool logEnable) {
    //Set logEnable variable to enable or disable the logfile
    this->logEnable = logEnable;
    if (logEnable) this->logFile.open("TableLog.txt", ios::out);

    //Open input file
    this->logFile << "Opening " + fileName + "..." << endl; 
    this->inputFile.open(fileName, ios::in);

    //Initialize vectors
    this->tables = new vector<Table*>;
}

//Run the file parser
void FileParser::run() {
    //Read and instantiate each table from the file until a blank like or end of file
    while (this->inputFile.peek() != '\n' && this->inputFile.peek() != EOF) {
        if (logEnable) logFile << endl << "Making new Table..." << endl;
        this->candidateKeys = new vector<string>;
        this->dependencies = new vector<string>;
        GetColumnsAndKeys();
        GetDependencies();
        MakeTable();
    }
}

//Obtain the column names and all keys for a table
void FileParser::GetColumnsAndKeys() {
    string firstLine;      //String to hold first line of table
    string candidateKey;   //String to hold separated cadidate keys

    //Get first line and convert to stream for parsing
    if (logEnable) logFile << "Reading columns and keys..." << endl;
    getline(inputFile, firstLine);
    stringstream str_strm(firstLine); 
    
    //Store first string as primary key
    str_strm >> columnNames;
    if (logEnable) logFile << "Coumun Names: " + columnNames << endl;

    //Store first string as primary key
    str_strm >> primaryKey;
    if (logEnable) logFile << "Primary Key: " + primaryKey << endl;           
    
    //Store all remaining strings as candidate keys
    if (logEnable) logFile << "Candidate Keys: ";
    while (str_strm >> candidateKey) {        
        candidateKeys->push_back(candidateKey);
        if (logEnable) logFile << candidateKey + " ";
    }
    if (logEnable) logFile << endl; 
}

//Obtain the dependencies of the columns
void FileParser::GetDependencies() {
    string nextLine;     //String to hold first line of table
    string depen;        //String to hold separated cadidate keys
    int tempLoc;

    //Get first line
    getline(inputFile, nextLine);

    //Check for end condition
    if (logEnable) logFile << "Reading dependencies..." << endl;
    while (nextLine[0] != 'x') { 
        //Pull out the two strings from inside ()()   
        for (int i = 0; i < 2; i++) {
            nextLine.erase(0, 1);                     //Erase first ( character
            tempLoc = nextLine.find(')');             //Find location of )
            depen = nextLine.substr(0, tempLoc);      //Create a substring of everythin in between ()
            nextLine.erase(0, tempLoc + 1);           //Erase everything up to the next (
            dependencies->push_back(depen);            //Add string to the list of dependencies
            if (logEnable) logFile << depen + " ";
        }
        if (logEnable) logFile << endl;

        //Get next line
        getline(inputFile, nextLine);
    }
}

void FileParser::MakeTable() {
    Table* newTable = new Table(columnNames, primaryKey, candidateKeys, dependencies);
    tables->push_back(newTable);
}

//Driver Code
int main(int argc,  char** argv) {
    bool logEnable = false;
    if (argc == 2) logEnable = argv[1];
    FileParser fileParser("input.txt", logEnable);
    fileParser.run();
    return 0;
}