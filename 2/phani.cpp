#include <iostream>
#include <unistd.h>
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
void echo(string cmd)
{
    int i = 0;
    while (i < 4)
    {
        cmd.erase(cmd.begin());
        i++;
    }

    eraseBlanks(cmd);
    for (int i = 0; i < sizeof(cmd); i++)
    {
        cout << cmd[i];
    }
    cout << endl;
}
void ls()
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(".");
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            if ((en->d_name[0] != '.'))
                cout << en->d_name << endl;
        }
        closedir(dr);
    }
}
void ls_a()
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(".");
    if (dr)
    {
        while ((en = readdir(dr)) != nullptr)
        {
            cout << en->d_name << endl;
        }
    }
}

void ls_l()
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(".");
    if (dr)
    {
        while ((en = readdir(dr)) != nullptr)
        {
            if ((en->d_name[0] != '.'))
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
                cout << s.st_mtime << " ";
                cout << en->d_name;
                cout << endl;
            }
        }
        closedir(dr);
    }
}

void ls_la()
{
    DIR *dr;
    struct dirent *en;
    dr = opendir(".");
    if (dr)
    {
        while ((en = readdir(dr)) != nullptr)
        {

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
                cout << s.st_mtime << " ";
                cout << en->d_name;
                cout << endl;
            }
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
                p = "";
            }
            else
            {
                p = path;
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
            int i = 0;
            while (i < 2)
            {
                cmd.erase(cmd.begin());
                i++;
            }
            eraseBlanks(cmd);
            if (cmd.substr(0, 2) == "-a")
            {
                int i = 0;
                while (i < 2)
                {
                    cmd.erase(cmd.begin());
                    i++;
                }
                eraseBlanks(cmd);
                if (cmd == "-l" || cmd == "l")
                {
                    ls_la();
                }
                else if (cmd == "")
                {
                    ls_a();
                }
                else
                    cout << "wrong command " << endl;
            }
            else if (cmd.substr(0, 2) == "-l")
            {
                int i = 0;
                while (i < 2)
                {
                    cmd.erase(cmd.begin());
                    i++;
                }
                eraseBlanks(cmd);
                if (cmd == "-a" || cmd == "a")
                {
                    ls_la();
                }
                else if (cmd == "")
                {
                    ls_l();
                }
                else
                    cout << "wrong command " << endl;
            }
            else if (cmd == "")
            {
                ls();
            }
            else
                cout << "wrong command " << endl;
        }
    }
}
