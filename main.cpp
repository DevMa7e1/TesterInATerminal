#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>
#include "helper_functions.hpp"
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#define POPEN _popen
#define PCLOSE _pclose
#else
#include <unistd.h>
#define POPEN popen
#define PCLOSE pclose
#endif

using namespace std;
using namespace std::filesystem;
vector<string> processTest(string config){
    vector<string> lines = split2(config, '\n');
    string glines;
    string input;
    string output;
    bool in = false;
    bool out = false;
    for(int i = 0; i < lines.size(); i++){
        string line = lines[i];
        if(line != "Input Start" && line != "Input End" && line != "Output Start" && line != "Output End"){
            if(in)
                input += line + "\n";
            else if(out)
                output += line + "\n";
            else
                glines += line + "\n";
        }
        else if(line == "Input Start")
            in = true;
        else if(line == "Input End")
            in = false;
        else if(line == "Output Start")
            out = true;
        else if(line == "Output End")
            out = false;
    }
    vector<string> r;
    r.push_back(glines);
    r.push_back(input);
    r.push_back(output);
    return r;
}

string includeAllButFirst(vector<string> element){
    string result = "";
    for(int i = 1; i < element.size(); i++){
        result += element[i]+"\n";
    }
    return result;
}

void addToLog(string data){
    if(!exists("log.log")){
        write2("log.log", "");
    }
    string r = read2("log.log");
    write2("log.log", r + data);
}

bool runTest(vector<string> test){
    write2("tempInput.txt", test[1]);
    string args = "";
    if(split2(test[0], '\n')[1] != "None"){
        args = split2(test[0], '\n')[1];
    }
    string command = "./" + split2(test[0], '\n')[0] + args + " < tempInput.txt";
    FILE* pipe = POPEN(command.c_str(), "r");
    char buffer[128];
    std::string actual_output = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        actual_output += buffer;
    }
    int status = PCLOSE(pipe);
    cout << "Program exited with code " << status << ".\n";
    addToLog("Exited with code " + to_string(status));
    string trimmed_out = trim(actual_output);
    string trimmed_xout = trim(test[2]);
    if(split2(test[0], '\n')[2] == "Exact"){
        if(trimmed_out == trimmed_xout){
            cout << "Program test succeded!";
            addToLog("Test succeded.");
        }
        else{
            cout << "Program test failed." << "\n";
            cout << "Output: " << trimmed_out;
            cout << "\nExpected Output: " << trimmed_xout;
            addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
        }
        return trimmed_out == trimmed_xout;
    }
    if(split2(test[0], '\n')[2] == "Contains"){
        if(trimmed_out.find(trimmed_xout) != string::npos){
            cout << "Program test succeded!";
            addToLog("Test succeded.");
        }
        else{
            cout << "Program test failed." << "\n";
            cout << "Output: " << trimmed_out;
            cout << "\nExpected Output to Contain: " << trimmed_xout;
            addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
        }
        return trimmed_out.find(trimmed_xout) != string::npos;
    }
    if(split2(test[0], '\n')[2] == "Exact file"){
        if(exists(split2(test[2], '\n')[0])){
            string out = read2((split2(test[2], '\n')[0]).c_str());
            string trimmed_out = trim(out);
            string trimmed_xout = trim(includeAllButFirst(split2(test[2], '\n')));
            if(trimmed_out == trimmed_xout){
                cout << "Program test succeded!";
                addToLog("Test succeded.");
            }
            else{
                cout << "Program test failed." << "\n";
                cout << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout;
                addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
            }
        return trimmed_out == trimmed_xout;
        }
        else{
            cout << "Program test failed." << "\n";
            cout << "Output: " << trimmed_out;
            cout << "\nExpected Output: " << trimmed_xout;
            addToLog("Test failed.");
            addToLog("File not found.");
            addToLog(trimmed_out);
        }
    }
    if(split2(test[0], '\n')[2] == "Contains file"){
        if(exists(split2(test[2], '\n')[0])){
            string trimmed_xout = trim(includeAllButFirst(split2(test[2], '\n')));
            string out = read2(split2(test[2], '\n')[0].c_str());
            string trimmed_out = trim(out);
            if(trimmed_out.find(trimmed_xout) != string::npos){
                cout << "Program test succeded!";
                addToLog("Test succeded.");
            }
            else{
                cout << "Program test failed." << "\n";
                cout << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout;
                addToLog("Test failed.");
                addToLog("Output: ");
                addToLog(trimmed_out);
            }
            return trimmed_out.find(trimmed_xout) != string::npos;
        }
        else{
                cout << "Program test failed." << "\n";
                cout << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout;
                addToLog("Test failed.");
                addToLog("File not found.");
                addToLog(trimmed_out);
        }
    }
    return false;
}

void uni_sleep(int ms){
    #ifdef _WIN32
    Sleep(ms);
    #else
    sleep(ms/1000);
    #endif
}

int main(){
    vector<string> configuration;
    configuration = split2(read2("config.txt"), '\n');
    chrono::time_point lwt = last_write_time(configuration[1]);
    write2("log.log", "");
    while(true){
        int successful_tests = 0;
        time_t timestamp = time(NULL);
        addToLog(ctime(&timestamp));
        for(int i = 1; i < stoi(trim(configuration[0]))+1; i++){
            vector<string> test = processTest((read2(("test"+to_string(i)+".txt").c_str())));
            cout << i << ": ";
            if(runTest(test)){
                successful_tests++;
            }
            cout << "\n";
        }
        cout << "\n\n" << "Successful tests: " << successful_tests << ".\n";
        if(successful_tests == stoi(trim(configuration[0])))
            cout << "All tests successful!" << "\n";
        chrono::time_point wt = last_write_time(configuration[1]);
        while(lwt == wt){
            if(exists(configuration[1])){
                uni_sleep(1000);
                try{
                    wt = last_write_time(configuration[1]);
                }
                catch(const filesystem_error& e){
                    wt == lwt;                    
                }
            }
        }
        cout << "\n\n-- Change Detected --\n\n";
        lwt = last_write_time(configuration[1]);
        uni_sleep(5000);
    }
    return 0;
}