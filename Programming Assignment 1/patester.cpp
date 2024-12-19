#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>


// struct for our input

// we make a struct because thats the only way the void function can take it

/*
struct task
{
    char letter;
    double currEntropy;
    int currFreq;
    std::vector<double> entropiesTracker;
    int extraFreq;
    std::unordered_map<char, double> freqs;
    std::string input;

   
};
*/


// this is still the function to do the calculations

void *entropyCalc(std::string input, std::vector<double> &entropies)
{   
    // cast it to the task datatype like this
 

    std::stringstream ss(input);
    int currFreq = 0;
    std::unordered_map<char, double> freqs;
    char letter;
    int extraFreq;
   


    // putting the variables into the right places from the input
    while(ss >> letter >> extraFreq )
    {

        // this is the entropy formula
        double h;
        double currTerm;
        double newTerm;

        int newFreq = currFreq + extraFreq;
        if (newFreq == extraFreq)
        {
            h = 0;
        }
        else
        {
            if (freqs[letter] == 0)
            {
                currTerm = 0;
            }
            else
            {
                currTerm = freqs[letter] * log2(freqs[letter]);
            }

            newTerm = (freqs[letter] + extraFreq) * log2(freqs[letter] + extraFreq);

            h = log2(newFreq) - ( (log2(currFreq) - h) * (currFreq) - currTerm + newTerm)/newFreq; 
        }

        // for sure correct
        
        currFreq = newFreq;

        // is for formula calculations
        freqs[letter] = extraFreq + freqs[letter];

        // is for printing
        entropies.push_back(h);
       
    }
    
    return NULL;
}


int main()
{


    


    // vector to store input
    //std::vector<std::string> lines;
    std::string input = "A 2 B 4 C 3 A 7";

    // getting inputs from user
    


   

   
   // we no longer have a need for array of struct
   // it will just be 1 task variable
   
    std::vector<double> entropy;
    entropyCalc(input, entropy);

    for(int x = 0; x < entropy.size(); x++)
    {
        std::cout << entropy[x] << " " ;
    }


  

    return 0;
}