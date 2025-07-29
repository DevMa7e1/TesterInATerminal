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
#include <curl/curl.h>
#include "util.h"

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

// Pretty terminal colors
string RST = "\x1B[0m";
string RED = "\x1B[31m";
string GRN = "\x1B[32m";
string CYN = "\x1B[36m";
string YLW = "\x1B[93m";

void uni_sleep(int ms){
    #ifdef _WIN32
    Sleep(ms);
    #else
    sleep(ms/1000);
    #endif
}

char* makeARequest(char* url){
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);  
    chunk.size = 0;

    curl_handle = curl_easy_init();
    if(curl_handle) {
      curl_easy_setopt(curl_handle, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

      res = curl_easy_perform(curl_handle);

      if(res != CURLE_OK) {
        return "CURL_ERROR";
      } else {
        return chunk.memory;
      }
      curl_easy_cleanup(curl_handle);
      free(chunk.memory);
    }
    return "CURL_ERROR";
}

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
bool fileTest(string command, vector<string> test, bool reverse){
    FILE* pipe = POPEN(command.c_str(), "r");
    char buffer[128];
    std::string actual_output = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        actual_output += buffer;
    }
    int status = PCLOSE(pipe);
    if(status == 0)
        cout << GRN << "Program exited with code " << status << ".\n";
    else
        cout << RED << "Program exited with code " << status << ".\n";
    addToLog("Exited with code " + to_string(status));
    string trimmed_out = trim(actual_output);
    string trimmed_xout = trim(test[2]);
    if(split2(test[0], '\n')[2] == "Exact"){
        if(trimmed_out == trimmed_xout){
            cout << GRN << "Program test succeded!" << RST;
            addToLog("Test succeded.");
        }
        else{
            cout << RED << "Program test failed." << "\n";
            cout << CYN << "Output: " << trimmed_out;
            cout << "\nExpected Output: " << trimmed_xout << RST;
            addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
        }
        return (trimmed_out == trimmed_xout) ^ reverse;
    }
    if(split2(test[0], '\n')[2] == "Contains"){
        if(trimmed_out.find(trimmed_xout) != string::npos){
            cout << GRN << "Program test succeded!" << RST;
            addToLog("Test succeded.");
        }
        else{
            cout << RED << "Program test failed." << "\n";
            cout << CYN << "Output: " << trimmed_out;
            cout << "\nExpected Output to Contain: " << trimmed_xout << RST;
            addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
        }
        return (trimmed_out.find(trimmed_xout) != string::npos) ^ reverse;
    }
    if(split2(test[0], '\n')[2] == "Exact file"){
        if(exists(split2(test[2], '\n')[0])){
            string out = read2((split2(test[2], '\n')[0]).c_str());
            string trimmed_out = trim(out);
            string trimmed_xout = trim(includeAllButFirst(split2(test[2], '\n')));
            if(trimmed_out == trimmed_xout){
                cout << GRN <<"Program test succeded!" << RST;
                addToLog("Test succeded.");
            }
            else{
                cout << RED << "Program test failed." << "\n";
                cout << CYN << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout << RST;
                addToLog("Test failed.");
            addToLog("Output: ");
            addToLog(trimmed_out);
            }
        return (trimmed_out == trimmed_xout) ^ reverse;
        }
        else{
            cout << RED << "Program test failed." << "\n";
            cout << CYN << "Output: " << trimmed_out;
            cout << "\nExpected Output: " << trimmed_xout << RST;
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
                cout << GRN << "Program test succeded!" << RST;
                addToLog("Test succeded.");
            }
            else{
                cout << RED << "Program test failed." << "\n";
                cout << CYN << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout << RST;
                addToLog("Test failed.");
                addToLog("Output: ");
                addToLog(trimmed_out);
            }
            return (trimmed_out.find(trimmed_xout) != string::npos) ^ reverse;
        }
        else{
                cout << RED << "Program test failed." << "\n";
                cout << CYN << "Output: " << trimmed_out;
                cout << "\nExpected Output to Contain: " << trimmed_xout << RST;
                addToLog("Test failed.");
                addToLog("File not found.");
                addToLog(trimmed_out);
        }
    }
    cout << RST;
    return false;
}

bool webTest(string command, vector<string> test, bool reverse){
    #ifdef _WIN32
    system(("start "+command).c_str());
    #else
    system((command+" &").c_str());
    #endif
    uni_sleep(5000);
    vector<string> ioptions = split2(test[1], '\n');
    vector<string> ooptions = split2(test[2], '\n');
    char* url = ioptions[0].data();
    string data = makeARequest(url);
    if (data != "CURL_ERROR"){
        if(ooptions[0] == "Exact"){
            string exout = includeAllButFirst(ooptions);
            if(trim(exout) == trim(data)){
                addToLog("Web test succeded!");
                cout << GRN << "Web test succeded!" << RST;
                return true ^reverse;
            }
            else{
                cout << RED << "Web test failed.\n";
                cout << CYN << "Expected output: " << trim(exout);
                cout << "\nActual output: " << trim(data) << RST;
                addToLog("Web test failed.");
                addToLog("Output: ");
                addToLog(trim(data));
            }
        }
        else if(ooptions[0] == "Contains"){
            string exout = includeAllButFirst(ooptions);
            if(trim(data).find(trim(exout)) != string::npos){
                addToLog("Web test succeded!");
                cout << GRN << "Web test succeded" << RST;
                return true ^ reverse;
            }
            else{
                cout << RED << "Web test failed.\n";
                cout << CYN << "Expected output: " << trim(exout);
                cout << "\nActual output: " << trim(data) << RST;
                addToLog("Web test failed.");
                addToLog("Output: ");
                addToLog(trim(data));
            }
        }
    }
    else{
        cout << RED << "Web test failed\ncURL ERROR." << RST;
        addToLog("Test failed, cURL error.");
    }
    return false ^ reverse;
}

bool runTest(vector<string> test){
    write2("tempInput.txt", test[1]);
    string cmmd = "";
    cmmd = split2(test[0], '\n')[1];
    string command = cmmd + " < tempInput.txt";
    if(split2(test[0], '\n')[2] != "Web"){
        return fileTest(command, test, split2(test[0], '\n').size() > 3);
    }
    else{
        return webTest(command, test, split2(test[0], '\n').size() > 3);
    }
}

int main(){
    vector<string> configuration;
    configuration = split2(read2("config.txt"), '\n');
    chrono::time_point lwt = last_write_time(configuration[1]);
    //write2("log.log", ""); //Feature deemed unecessary, enable if you want (auto erases the log file)
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
        cout << "\n\n" << YLW <<"Successful tests: " << successful_tests << ".\n" << RST;
        if(successful_tests == stoi(trim(configuration[0])))
            cout << GRN << "All tests successful!" << "\n" << RST;
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
        cout << YLW << "\n\n-- Change Detected --\n\n" << RST;
        lwt = last_write_time(configuration[1]);
        uni_sleep(5000);
    }
    cout << RST;
    return 0;
}