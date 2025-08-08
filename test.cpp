#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(){
    std::string a;
    std::string b;
    std::cin >> a;
    std::cin >> b;
    if(a.find("file") != string::npos){
        ofstream File("test.txt");
        File << b;
        File.close();
    }
    else{
        cout << a << " " << b << "\n";
    }
    return 0;
}