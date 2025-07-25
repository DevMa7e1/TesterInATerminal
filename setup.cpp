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

int main(){
    cout << "Let's set up tests.\nHow many test do you want to set up?>";
    int tests = 0;
    cin >> tests;
    cout << "Okay, what file should be tested?>";
    string path = "";
    cin >> path;
    string cancel = "";
    cout << "Now, let's configure the tests.\n";
    vector<string> files;
    vector<string> modes;
    vector<string> inputs;
    vector<string> argums;
    vector<string> outputs;
    for(int i = 0; i < tests; i++){
        cout << "Test no. " << i << "\nWhat file to execute?>";
        string file;
        cin >> file;
        files.push_back(file);
        cout << "Do you want to check for a file or stdout (F/S)?>";
        char fors;
        cin >> fors;
        string path2 = "";
        if(fors == 'F'){
            cout << "Name/path of file>";
            cin >> path2;
            cout << "Do you want to check if a file contains a value or if the file contents are some exact value (C/E)?>";
            char core;
            cin >> core;
            if(core == 'C'){
                modes.push_back("Contains file");
            }
            else if(core == 'E'){
                modes.push_back("Exact file");
            }
            else{
                cout << "Unsupported mode. Quitting...";
                exit(0);
            }
            cout << "Okay, please type expected file contents (type END to stop)\n";
            string output = "";
            string lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    output += lastinput + "\n";
            }
            output = "\n" + path2 + output;
            outputs.push_back(output);
        }
        else if(fors == 'S'){
            cout << "Do you want to check if stdout contains a value or if stdout is exactly an value (C/E)?>";
            char core;
            cin >> core;
            if(core == 'C'){
                modes.push_back("Contains");
            }
            else if(core == 'E'){
                modes.push_back("Exact");
            }
            else{
                cout << "Mode unsupported. Quitting...";
                exit(0);
            }
            cout << "Okay, please type expected output (type END to stop)\n";
            string output = "";
            string lastinput = "";
            while(lastinput != "END"){
                getline(cin, lastinput);
                if(lastinput != "END")
                    output += lastinput + "\n";
            }
            outputs.push_back(output);
        }
        cout << "Okay, what arguments should be supplied to the executable (type None for no arguments.)?\n";
        string args = "";
        cin >> args;
        argums.push_back(args);
        cout << "Now, what text to supply to exe through stdin (type END to stop)?\n";
        string input = "";
        string lastinput = "";
        while(lastinput != "END"){
            getline(cin, lastinput);
            if(lastinput != "END")
                input += lastinput + "\n";
        }
        inputs.push_back(input);
        cout << "That's it for this test.\n";
    }
    for(int i = 0; i < files.size(); i++){
        write2_c(("test"+to_string(i+1)+".txt").c_str(), files[i]+"\n"+argums[i]+"\nInput Start"+inputs[i]+"Input End\n"+modes[i]+"\nOutput Start"+outputs[i]+"Output End");
    }
    write2("config.txt", to_string(tests)+"\n"+path);
    return 0;
}