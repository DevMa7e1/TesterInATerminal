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

using namespace std;

// Pretty terminal colors
string RST = "\x1B[0m";
string RED = "\x1B[31m";
string GRN = "\x1B[32m";
string CYN = "\x1B[36m";
string YLW = "\x1B[93m";

int main(){
    cout << "Let's set up tests.\n"+CYN+"How many test do you want to set up?>";
    int tests = 0;
    cin >> tests;
    cout << YLW+"What file should be tested?>";
    string path = "";
    cin >> path;
    string cancel = "";
    cout << RST+"Now, let's configure the tests.\n";
    vector<string> files;
    vector<string> modes;
    vector<string> inputs;
    vector<string> argums;
    vector<string> outputs;
    for(int i = 0; i < tests; i++){
        cout << RST << "Test no. " << i << YLW+"\nFile/Command to execute>";
        string file;
        cin >> file;
        files.push_back(file);
        cout << CYN+"Output is in a file, web page or STDOUT (F/W/S)?>";
        char fors;
        cin >> fors;
        string path2 = "";
        if(fors == 'F'){
            cout << YLW+"Name/path of file>";
            cin >> path2;
            cout << CYN+"Check if file contains value or for exact file contents (C/E)?>";
            char core;
            cin >> core;
            if(core == 'C'){
                modes.push_back("Contains file");
            }
            else if(core == 'E'){
                modes.push_back("Exact file");
            }
            else{
                cout << RED << "Unsupported mode. Quitting..." << RST;
                exit(0);
            }
            cout << CYN << "Expected file contents (type END to stop)\n";
            string output = "";
            string lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    output += lastinput + "\n";
            }
            output = "\n" + path2 + output;
            outputs.push_back(output);
            cout << CYN << "Input to provide to STDIN (type END to stop)\n";
            string input = "";
            lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    input += lastinput + "\n";
            }
            inputs.push_back(input);
        }
        else if(fors == 'S'){
            cout << CYN <<"Contains value or Exact value (C/E)?>";
            char core;
            cin >> core;
            if(core == 'C'){
                modes.push_back("Contains");
            }
            else if(core == 'E'){
                modes.push_back("Exact");
            }
            else{
                cout << RED << "Mode unsupported. Quitting..." << RST;
                exit(0);
            }
            cout << CYN << "Expected output (type END to stop)\n";
            string output = "";
            string lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    output += lastinput + "\n";
            }
            outputs.push_back(output);
            cout << CYN << "Provided STDIN (type END to stop)\n";
            string input = "";
            lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    input += lastinput + "\n";
            }
            inputs.push_back(input);
        }
        else if(fors == 'W'){
            cout << CYN <<"Contains value or Exact value (C/E)?>";
            char core;
            cin >> core;
            modes.push_back("Web");
            string url = "";
            cout << CYN << "URL>";
            cin >> url;
            cout << CYN << "Expected response (type END to stop)\n";
            string output = "";
            if(core == 'C')
                output = "Contains\n";
            else if(core == 'E')
                output = "Exact\n";
            else{
                cout << RED << "Unsupported mode, quitting...";
                exit(0);
            }
            string lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    output += lastinput + "\n";
            }
            outputs.push_back(output);
            inputs.push_back(url);
        }
        cout << YLW << "Arguments (type None for no arguments.)\n";
        string args = "";
        getline(cin, args);
        argums.push_back(args);
        cout << GRN << "All done.\n" << RST;
    }
    for(int i = 0; i < files.size(); i++){
        write2_c(("test"+to_string(i+1)+".txt").c_str(), files[i]+"\n"+argums[i]+"\nInput Start\n"+inputs[i]+"Input End\n"+modes[i]+"\nOutput Start"+outputs[i]+"Output End");
    }
    write2("config.txt", to_string(tests)+"\n"+path);
    return 0;
}