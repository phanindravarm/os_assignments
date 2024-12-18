#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <sstream>
#include <libproc.h>
#include <sys/sysctl.h>
using namespace std;
int main()
{
    string cmd = "phani";
    string hey = cmd;
    hey.erase(hey.begin());
    cout << "hey : " << hey << endl;
}
