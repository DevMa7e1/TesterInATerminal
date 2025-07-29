#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>
#include <ctime>
#include "formslib.hpp"

using namespace std;

vector<int> file_elements = {0, 1, 0, 0, 1, 2};
vector<string> file_texts = {"Name/path of output file", "Check for file content|full,portion", "Expected file contents", "Input provided to STDIN", "Reverse condition?|Yes,No", "Done"};
vector<int> stdout_elements = {1, 0, 0, 1, 2};
vector<string> stdout_texts = {"Check for STDOUT content|full,portion", "Expected STDOUT output", "Input provided to STDIN", "Reverse condition?|Yes,No", "Done"};
vector<int> web_elements = { 0, 1, 0, 1, 2};
vector<string> web_texts = {"URL", "Check for response content|full,portion", "Expected response", "Reverse condition?|Yes,No", "Done"};

vector<string> fss = {"Output is read from|A file,An HTTP response,STDOUT", "Command to run", "Ok"};
vector<int> fse = {1, 0, 2};

int main(){
    cout << "Let's set up tests.\n"+CYN+"How many test do you want to set up?>";
    int tests = 0;
    cin >> tests;
    string c = "";
    getline(cin, c);
    cout << YLW+"What file will be tested?>";
    string path = "";
    getline(cin, path);
    cout << RST+"Now, let's configure the tests.\n";
    vector<string> modes;
    vector<string> inputs;
    vector<string> outputs;
    vector<string> cmmds;
    for(int i = 0; i < tests; i++){
        Form form;
        Form selmode;
        selmode.elements = fse;
        selmode.texts = fss;
        selmode.init();
        while(true){
            selmode.displayElements();
            if(selmode.inputs[2] == "1")
                break;
        }
        cmmds.push_back(selmode.inputs[1]);
        if(selmode.inputs[0] == "0" || selmode.inputs[0] == ""){
            form.elements = file_elements;
            form.texts = file_texts;
        }
        else if(selmode.inputs[0] == "1"){
            form.elements = web_elements;
            form.texts = web_texts;
        }
        else if(selmode.inputs[0] == "2"){
            form.elements = stdout_elements;
            form.texts = stdout_texts;
        }
        form.init();
        while(true){
            form.displayElements();
            if(form.elements == file_elements){
                if(form.inputs[5] == "1")
                    break;
            }
            else{
                if(form.inputs[4] == "1")
                    break;
            }
        }
        if(selmode.inputs[0] == "0" || selmode.inputs[0] == ""){
            if(form.inputs[1] == "0"){
                if(form.inputs[4] == "0")
                    modes.push_back("Exact file\nReverse");
                else
                    modes.push_back("Exact file");
            }
            else{
                if(form.inputs[4] == "0")
                    modes.push_back("Contains file\nReverse");
                else
                    modes.push_back("Contains file");
            }
            outputs.push_back(form.inputs[0]+"\n"+form.inputs[2]);
            inputs.push_back(form.inputs[3]);
        }
        else if(selmode.inputs[0] == "2"){
            if(form.inputs[1] == "0"){
                if(form.inputs[3] == "0")
                    modes.push_back("Exact\nReverse");
                else
                    modes.push_back("Exact");
            }
            else{
                if(form.inputs[3] == "0")
                    modes.push_back("Contains\nReverse");
                else
                    modes.push_back("Contains");
            }
            outputs.push_back(form.inputs[1]);
            inputs.push_back(form.inputs[2]);
        }
        else if(selmode.inputs[0] == "1"){
            if(form.inputs[3] == "0")
                modes.push_back("Web\nReverse");
            else
                modes.push_back("Web");
            string output;
            if(form.inputs[1] == "0")
                output += "Exact\n";
            else
                output += "Contains\n";
            output += form.inputs[2];
            outputs.push_back(output);
            inputs.push_back(form.inputs[0]);
        }
    }
    for(int i = 0; i < modes.size(); i++){
        cout << CYN << "Writing test " << i+1 << "\n" << RST;
        write2_c(("test"+to_string(i+1)+".txt").c_str(), path+"\n"+cmmds[i]+"\nInput Start\n"+inputs[i]+"\nInput End\n"+modes[i]+"\nOutput Start\n"+outputs[i]+"\nOutput End");
    }
    write2("config.txt", to_string(tests)+"\n"+path);
    return 0;
}