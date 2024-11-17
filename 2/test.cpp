#include <iostream>
#include <unistd.h>
using namespace std;
int main()
{
    char name[200];
    getlogin_r(name, sizeof(name));
    cout << name << endl;
    char hostName[200];
    int res = gethostname(hostName, sizeof(hostName));
    if (res == 0)
    {
        cout << "hey : " << hostName << endl;
    }
}
