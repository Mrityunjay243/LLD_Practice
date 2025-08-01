#include <iostream>
#include "String.hpp"
using namespace std; 

int main(){

    String s1("Hello"); 
    String s2(" World"); 

    cout << "Initially s1: " << s1 << ", s2: " << s2 << endl; 

    s1 += s2; 
    cout << "After Concat s1: " << s1 << endl;
    s1.push_back('!'); 
    cout << "After push back s1 " << s1 << endl; 

    String s3 = s1; 
    cout << "Copied string: " << s3 << endl; 

    String s4 = std::move(s1); 
    cout << "s4 moved from s1: " << s4 << endl; 

    if (s3 == s4){
        cout<< "control will enter here if the data of the 2 strings is same" << endl; 
    }

    return 0; 
}