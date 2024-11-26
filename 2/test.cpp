#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
using namespace std;
void eraseBlanks(string &cmd)
{
    while (!cmd.empty() && cmd[0] == ' ')
    {
        cmd.erase(cmd.begin());
    }
}
int main()
{
    struct stat s;
    if (stat("phani.c", &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            cout << "d";
        }
        else
        {
            cout << "-";
        }
    }
    else
        cout << "none";
    cout << ((s.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((s.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((s.st_mode & S_IXUSR) ? "x" : "-");

    cout << ((s.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((s.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((s.st_mode & S_IXGRP) ? "x" : "-");

    cout << ((s.st_mode & S_IROTH) ? "r" : "-");
    cout << ((s.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((s.st_mode & S_IXOTH) ? "x" : "-");
    cout << getpwuid(s.st_uid)->pw_name << " ";
    cout << getgrgid(s.st_gid)->gr_name << " ";
    cout << s.st_mtime << " ";
}