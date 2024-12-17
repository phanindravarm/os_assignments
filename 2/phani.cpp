#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <sstream>
using namespace std;
void eraseBlanks(string &cmd)
{
    while (!cmd.empty() && cmd[0] == ' ')
    {
        cmd.erase(cmd.begin());
    }
}
void echo(string cmd)
{
    int i = 0;
    while (i < 4)
    {
        cmd.erase(cmd.begin());
        i++;
    }

    eraseBlanks(cmd);
    cout << cmd;
    cout << endl;
}
void ls(int a, int l)
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(".");
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            if (!a && en->d_name[0] == '.')
            {
                continue;
            }
            if (l)
            {
                struct stat s;
                if (stat(en->d_name, &s) == 0)
                {
                    if (s.st_mode & S_IFDIR)
                    {
                        cout << "d";
                    }
                    else
                        cout << "-";
                }
                cout << ((s.st_mode & S_IRUSR) ? "r" : "-");
                cout << ((s.st_mode & S_IWUSR) ? "w" : "-");
                cout << ((s.st_mode & S_IXUSR) ? "x" : "-");
                cout << ((s.st_mode & S_IRGRP) ? "r" : "-");
                cout << ((s.st_mode & S_IWGRP) ? "w" : "-");
                cout << ((s.st_mode & S_IXGRP) ? "x" : "-");
                cout << ((s.st_mode & S_IROTH) ? "r" : "-");
                cout << ((s.st_mode & S_IWOTH) ? "w" : "-");
                cout << ((s.st_mode & S_IXOTH) ? "x" : "-");
                cout << " ";
                cout << getpwuid(s.st_uid)->pw_name << " ";
                cout << getgrgid(s.st_gid)->gr_name << " ";
                struct tm *tm;
                char time[200];
                tm = localtime(&s.st_mtime);
                strftime(time, sizeof(time), "%b %d %H:%M", tm);
                cout << time << " ";
            }
            cout << en->d_name << endl;
        }
        closedir(dr);
    }
}

int main()
{
    char *name = getenv("USER");
    char hostName[200];
    char cwd[200];
    char *homeD = getenv("HOME");

    if (name == nullptr)
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
    string cmd;
    string p;
    while (true)
    {
        string c(cwd);
        string h(homeD);
        if (c.substr(0, 12) == h)
        {
            c.replace(0, 12, "~");
        }
        cout << name << "@" << hostName << ":" << c << ">";
        getline(cin, cmd);
        eraseBlanks(cmd);
        if (cmd == "exit")
        {
            cout << "Exiting shell......";
            break;
        }
        if (cmd.substr(0, 3) == "cd ")
        {

            string path = cmd.substr(3);
            eraseBlanks(path);

            if (chdir(path.c_str()))
            {
                cout << "cd: no such file or directory: " << path << endl;
            }
            else
            {
                getcwd(cwd, sizeof(cwd));
            }
        }
        else if (cmd.substr(0, 3) == "pwd")
        {
            cout << cwd << endl;
        }
        else if (cmd.substr(0, 5) == "echo ")
        {
            echo(cmd);
        }
        else if (cmd.substr(0, 2) == "ls")
        {
            int l = 0;
            int a = 0;
            int x = 0;
            for (int i = 0; i < 2; i++)
            {
                cmd.erase(cmd.begin());
            }
            stringstream c(cmd);
            string word;
            while (c >> word)
            {
                if (word == "-a")
                {
                    a = 1;
                }
                else if (word == "-l")
                {
                    l = 1;
                }
                else if (word == "-la" || word == "-al")
                {
                    a = 1;
                    l = 1;
                }
                else
                {
                    x = 1;
                    break;
                }
            }
            if (x)
            {
                cout << "wrong" << endl;
            }
            else
            {
                ls(a, l);
            }
        }
        else
        {
            const char *c = cmd.c_str();
            system(c);
        }
    }
}