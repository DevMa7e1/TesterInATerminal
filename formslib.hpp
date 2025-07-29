#include <iostream>
#include "keypress.hpp"
#include "helper_functions.hpp"
#include <string>
#include <vector>

using namespace std;

//Terminal ANSI escape codes
string RST = "\x1B[0m";
string RED = "\x1B[31m";
string GRN = "\x1B[32m";
string CYN = "\x1B[36m";
string YLW = "\x1B[93m";
string UDL = "\x1B[4m";

string getAllButLast(string a){
    string b = "";
    for(int i = 0; i < a.length()-1; i++){
        b += a[i];
    }
    return b;
}

class Form{
    public:
    vector<int> elements;
    vector<string> texts;
    vector<string> inputs;
    void init(){
        inputs.clear();
        for(int i = 0; i < elements.size(); i++){
            inputs.push_back("");
        }
    }
    void displayElements(){
        uni_clear();
        cout << "Tab to move, space to select.\n\n";
        if(cselkey == 9){
            selected = false;
            cselectedIndex = (cselectedIndex + 1) % elements.size();
        }
        if(selected){
            if(elements[cselectedIndex] == 0){
                if(cselkey != 8)
                    inputs[cselectedIndex] += cselkey;
                else
                    inputs[cselectedIndex] = getAllButLast(inputs[cselectedIndex]);
            }
            if(elements[cselectedIndex] == 1){
                vector<string> options = split2(split2(texts[cselectedIndex], '|')[1], ',');
                if(cselkey == 32){
                    int cval;
                    try{
                        cval = stoi(inputs[cselectedIndex]);
                    }
                    catch(invalid_argument){
                        cval = 0;
                    }
                    cval++;
                    cval = cval % options.size();
                    inputs[cselectedIndex] = to_string(cval);
                }
            }
        }
        if(cselkey == 32 && !selected){
            selected = true;
            if(elements[cselectedIndex] == 2){
                inputs[cselectedIndex] = "1";
            }
        }
        for(int i = 0; i < elements.size(); i++){
            if(i == cselectedIndex){
                cout << UDL;
            }
            if(elements[i] == 0){
                if(inputs.size() >= i+1){
                    displayTextInput(texts[i], inputs[i]);
                }
                else{
                    displayTextInput(texts[i], "");
                }
            }
            else if(elements[i] == 1){
                vector<string> properties = split2(texts[i], '|');
                vector<string> options = split2(properties[1], ',');
                if(inputs.size() >= i+1){
                    try{
                        displayMultiOpt(properties[0], options, stoi(inputs[i]));
                    }
                    catch(invalid_argument){
                        displayMultiOpt(properties[0], options, stoi("-1"));
                    }
                }
                else{
                    displayMultiOpt(properties[0], options, -1);
                }
            }
            else if(elements[i] == 2){
                displayButton(texts[i]);
            }
            cout << "\n" << RST;
        }
        cselkey = key_press();
    }
    private:
    int cselectedIndex = 0;
    int cselkey = 0;
    bool selected = false;
    void displayTextInput(string text, string input){
        cout << text << " [" << input << "]";
    }
    void displayButton(string text){
        cout << "\n[" << text << "]";
    }
    void displayMultiOpt(string text, vector<string> option, int index){
        cout << text << " ";
        if(index == -1){
            for(int i = 0; i < option.size()-1; i++){
                cout << "[" << option[i] << "] ";
            }
            cout << "[" << option[option.size()-1] << "]";
        }
        else{
            for(int i = 0; i < option.size(); i++){
                if(index == i){
                    cout << "[" << ">" << option[i] << "<] ";
                }
                else{
                    cout << "[" << option[i] + "] ";
                }
            }
        }
    }
};