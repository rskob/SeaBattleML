#include <string>
#include <algorithm>
#include <iostream>


void checkIfStringIsPositiveInt(const std::string& s, std::string& errorMessage)
{
    if(!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit)){
        try {
            unsigned long int num = std::stoul(s);
            if(num < 0) errorMessage = s + ": this argument must be a positive number";
        } 
        catch (const std::out_of_range& e) {
            errorMessage = s + ": Too large to be true";
        }
        if (s == "0") errorMessage = s + ": this argument must be a positive integer";
        
    } else {
        errorMessage = s + ": this argument must be a positive integer";
    }
}
