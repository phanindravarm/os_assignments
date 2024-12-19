#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <unistd.h>
using namespace std;

int main()
{
    ifstream myfile("phani.txt");
    myfile.seekg(-1, ios::end);
    int len = myfile.tellg();
    ofstream otherfile("varma.txt");
    char ch;
    for (double i = 0; i <= len; i++)
    {
        double progress = (i / len) * 100;
        ch = myfile.get();
        otherfile << ch;
        cout << "\r";
        for (int j = 0; j < 100; j++)
        {
            if (j < ceil(progress))
            {
                cout << "=";
            }
            else
                cout << " ";
        }
        cout << progress << "%";

        myfile.seekg(-2, ios::cur);
    }
}