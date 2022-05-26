//Language: C++
//Compiler: Visual Studio
//Date: 11/18/2021
//Desc: implementation of 3 different backoff protocols: Linear, Binary Exponential, and LogLog
#include <iostream>
#include <string.h>
#include <random>
#include <fstream>
#include <math.h>

using namespace std;

unsigned long long backoff(int N) {
    //initialize variables-------
    default_random_engine generator;

    //int N = 100;//number of nodes
    int cN = N;
    unsigned int windowSize = 2;//2 for linear and binary; 4 for loglog
    //int numWindows = 1;
    int ran;
    int loop = 1;
    unsigned long long latency = 0;
    unsigned int* win;
    //srand(time(NULL));
    //call rand()%windowSize to select window to send to
    //insert protocol here------
    while (N != 0) {
        //linear = ++ //binary = times 2 //loglog = (1 + 1/log base 2 (size of current window)) * size of current window
        //if (cN != N) {
            //windowSize++;
            //cN = N;
        //}
        win = new unsigned int[windowSize];
        for (unsigned int i = 0; i < windowSize; i++) {
            win[i] = 0;
        }
        uniform_int_distribution<int> distribution(0, windowSize);
        for (int i = 0; i < N; i++) {//select slot to send to and increment slot
            ran = distribution(generator);
            //ran = rand()%windowSize;
            win[ran]++;
        }
        for (unsigned int i = 0; i < windowSize; i++) {//if number in slot != 1, then collision occurred
            latency++;
            if (win[i] == 1) {//if number in slot == 1, then the packet was accepted and there was no collision
                N--;//decrease current number of packets to send
            }
            if (N == 0) {//exit loop if there are no more packets to send
                i = windowSize;
            }
        }/*
        cout << "loop: " << loop << endl;
        for (int i = 0; i < windowSize; i++){//display current window
            cout << win[i] << endl;
        }
        cout << "N: " << N << endl;*/
        //loop++;
        //sleep(1);
      //Manually comment and uncomment to change to different backoff protocols.  Yes, I could have made this easier.
      /**************************************************************************************/
        if (N == 0) { delete[] win; return latency; }
        //linear = ++;    //linear
	      binary = times 2; //binary
	      //loglog = (1 + 1/log base 2 (size of current window)) * size of current window   //loglog
        //windowSize++;   //linear
        windowSize *= 2;  //binary
        //windowSize = ((1 + 1 / log2(log2(windowSize))) * windowSize);                   //loglog

    }
    //protocol done-------------
    //calculate latency: latency is total number of slots up to and including the last slot
    //cout << latency;
    //write number of nodes & time taken to file
    return latency;
}
//need to add a way to compute number of slots based on current window
int main()
{
    //manually change the file name to redirect output to the correct file.
    ofstream file("binaryLatency.txt");
    int j;
    for (j = 100; j <= 6000; j += 100) {
        file << backoff(j) << "\n";
    }
    file.close();
}
