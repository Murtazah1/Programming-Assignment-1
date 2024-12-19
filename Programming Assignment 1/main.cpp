#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include <unistd.h>
#include <cmath>


// struct for our input

// we make a struct because thats the only way the void function can take it
struct task
{
    char letter;
    double currEntropy;
    int currFreq;
    std::vector<double> entropiesTracker;
    std::vector<char> taskTracker;
    std::vector<int> timeTracker;
    int extraFreq;
    std::unordered_map<char, double> freqs;
    std::string input;
};



void *entropyCalc(void *arg_void_ptr)
{   
    // cast it to the task datatype like this
    task *arg = (task *)arg_void_ptr;

    std::stringstream ss(arg->input);
    arg->currFreq = 0;

    // putting the variables into the right places from the input
    while(ss >> arg->letter >> arg->extraFreq )
    {

        // this is the entropy formula
        double h;
        double currTerm;
        double newTerm;

        int newFreq = arg->currFreq + arg->extraFreq;
        if (newFreq == arg->extraFreq)
        {
            h = 0;
        }
        else
        {
            if (arg->freqs[arg->letter] == 0)
            {
                currTerm = 0;
            }
            else
            {
                currTerm = arg->freqs[arg->letter] * log2(arg->freqs[arg->letter]);
            }

            newTerm = (arg->freqs[arg->letter] + arg->extraFreq) * log2(arg->freqs[arg->letter] + arg->extraFreq);

            h = log2(newFreq) - ( (log2(arg->currFreq) - arg->currEntropy) * (arg->currFreq) - currTerm + newTerm)/newFreq; 
        }

        // for sure correct
        arg->currEntropy = h;
        arg->currFreq = newFreq;

        // is for formula calculations
        arg->freqs[arg->letter] = arg->extraFreq + arg->freqs[arg->letter];

        // is for printing
        arg->entropiesTracker.push_back(arg->currEntropy);
        arg->taskTracker.push_back(arg->letter);
        arg->timeTracker.push_back(arg->extraFreq);
    }
    
    return NULL;
}


int main()
{
    // vector to store input
    std::vector<std::string> lines;
    std::string input;

    // getting inputs from user
    
    while (std::getline(std::cin, input) && !input.empty()) {
        lines.push_back(input);    
    }

    int numOfString = lines.size();

    // dynamically allocate the array
    pthread_t *threads = new pthread_t[numOfString];
    task *args = new task[numOfString];


    for(int i = 0; i < numOfString; i++)
    {   
        // save the input into the input string in the struct
        args[i].input = lines[i];

        // create the thread
        if (pthread_create(&threads[i], NULL, entropyCalc, &args[i]))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    // join the threads
    for (int i = 0; i < numOfString; i++)
        pthread_join(threads[i], NULL);

    // print out the information
    for(int i = 0; i < numOfString; i++)
    {
        std::cout << "CPU" << i + 1 << std::endl;
        std::cout << "Task scheduling information: ";

        for (int j = 0; j < args[i].taskTracker.size(); j++)
        {
            std::cout << args[i].taskTracker[j] << "(" << args[i].timeTracker[j] << ")";
            // Check if it's not the last task, then print a comma
            if (j < args[i].taskTracker.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << std::endl;

        std::cout << "Entropy for CPU " << i + 1 << std::endl;
        for(int j = 0; j < args[i].entropiesTracker.size(); j++)
        {
            std::cout << std::fixed << std::setprecision(2) << args[i].entropiesTracker[j] << " ";
        }

        std::cout << std::endl;

        // Check if it's the last CPU to print and add an extra line space
        if (i < numOfString - 1) {
            std::cout << std::endl;
        }
    }

    // free the memory
    delete [] threads;
    delete [] args;

    return 0;
}