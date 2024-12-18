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
void eraseBlanks(string &cmd)
{
    while (!cmd.empty() && cmd[0] == ' ')
    {
        cmd.erase(cmd.begin());
    }
}

int check(string cmd)
{
    stringstream c(cmd);
    string word;
    int i = 0;
    while (c >> word)
    {
        if (word == "&")
        {
            i = 1;
        }
        else if (word == " ")
            i = 0;
        else
            i = -1;
    }
    return i;
}

void ls(string cmd)
{
    DIR *dr;
    struct dirent *en;
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
    if (x & (check(cmd) != 1))
    {
        cout << "wrong" << endl;
    }

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

void pinfo()
{
    pid_t pid = getpid();
    char hey[200];
    getcwd(hey, sizeof(hey));
    cout << "pid : " << pid << endl;
    char path[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(pid, path, sizeof(path));
    string p(path);
    p.replace(0, strlen(hey), ".");
    cout << "Executable Path : " << p << endl;
    struct proc_taskinfo task_info;
    proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &task_info, sizeof(task_info));
    cout << "-" << task_info.pti_virtual_size << "{Virtual Memory}";
}
void background(string cmd)
{
    int i = check(cmd);

    if (i == 1)
    {
        int pos = cmd.find("&");
        cmd.erase(pos + 1);
        pid_t pid = fork();
        if (pid == 0)
        {
            sleep(2);
            cout << "after waiting : " << endl;
            ls(cmd);
        }
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
        }
        else if (cmd.substr(0, 5) == "echo ")
        {
            int i = check(cmd);
            cout << "i : " << i << endl;
            if (i == 1)
            {
                background(cmd);
            }
            else
                echo(cmd);
        }
        else if (cmd.substr(0, 2) == "ls")
        {
            
            int i = check(cmd);
            cout << " i : " << i << endl;
            if (i == 1)
            {
                background(cmd);
            }
            else if (i == -1)
            {
                ls(cmd);
            }
        }
        else if (cmd == "pinfo")
        {
            pinfo();
        }
        else
        {
            // const char *c = cmd.c_str();
            // system(c);
            cout << "wrong cmd" << endl;
        }
    }
}