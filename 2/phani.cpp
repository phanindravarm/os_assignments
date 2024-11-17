#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    char name[200];
    char hostName[200];
    char cwd[200];
    if (getlogin_r(name, sizeof(name)))
    {
        cout << "error in name" << endl;
    }
    if (gethostname(hostName, sizeof(hostName)))
    {
        cout << "error in hostName" << endl;
    }

    if (!getcwd(cwd, sizeof(cwd)))
    {
        cout << "error in cwd" << endl;
    }
    cout << name << "@" << hostName << ":" << cwd;
}
