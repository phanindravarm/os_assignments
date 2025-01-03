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
#include <vector>
#include <string>
#include <map>
#include <sys/fcntl.h>

using namespace std;
map<string, function<void(string)>> commands;
vector<pid_t> backgroundPIDs;
void eraseBlanks(string &cmd)
{
    while (!cmd.empty() && cmd[0] == ' ')
    {
        cmd.erase(cmd.begin());
    }
}
int check(string cmd)
{
    for (int i = 0; i < cmd.size(); i++)
    {
        if (cmd[i] == '&')
        {
            return 1;
        }
    }
    return 0;
}
void ls(string cmd)
{
    DIR *dr;
    struct dirent *en;
    int l = 0;
    int a = 0;
    int x = 0;
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
        cout << "wrong ls" << endl;
    }
    else
    {
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
}
void echo(string cmd)
{
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
void spacing(string &cmd)
{
    for (int i = 0; i < cmd.size(); i++)
    {
        if (cmd[i] == '&')
        {
            cmd.insert(cmd.begin() + i, ' ');
            cmd.insert(cmd.begin() + i + 2, ' ');
            i = i + 2;
        }
    }
}
void sortProcess(string cmd, vector<string> &bProcess, string &fProcess)
{
    stringstream cmdStream(cmd);
    string word;
    vector<string> commands;
    while (cmdStream >> word)
    {
        commands.push_back(word);
    }
    bool endwithAnd = false;
    for (int i = 0; i < commands.size(); i++)
    {
        if (commands[i] == " ")
            continue;
        if (commands[i] == "&")
        {
            endwithAnd = true;
            continue;
        }
        string command = "";
        for (; i < commands.size() && commands[i] != "&"; i++)
        {
            command = command + " " + commands[i];
        }
        endwithAnd = false;
        bProcess.push_back(command);
    }
    if (!endwithAnd)
    {
        fProcess = bProcess.back();
        bProcess.pop_back();
    }
}
void background(vector<string> bProcess)
{
    for (int i = 0; i < bProcess.size(); i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            sleep(5);
            cout << endl;
            cout << "after waiting : " << endl;
            const char *c = bProcess[i].c_str();
            system(c);
            backgroundPIDs.erase(backgroundPIDs.begin());
            exit(0);
        }
        else if (pid > 0)
        {
            backgroundPIDs.push_back(pid);
        }
    }
}
void printBackgroundPIDs()
{
    cout << "BProcess PIDs: " << endl;
    for (pid_t pid : backgroundPIDs)
    {
        cout << pid << endl;
    }
}
int checkRedirect(string cmd)
{
    int pos = cmd.find('>');
    int pos1 = cmd.find('<');
    if (pos == string::npos && pos1 == string::npos)
    {
        return 0;
    }
    if (pos != string::npos && pos1 != string::npos)
    {
        if (pos1 < pos)
        {
            return 2;
        }
        else
            return 1;
    }
    if (pos != string::npos)
    {
        return 1;
    }
    if (pos1 != string::npos)
    {
        return 2;
    }
    return 0;
}
void redirect(string cmd)
{
    size_t pos = cmd.find('>');
    if (pos == string::npos)
    {
        return;
    }

    string command = cmd.substr(0, pos);
    string file = cmd.substr(pos + (cmd[pos] == '>' && cmd[pos + 1] != '>' ? 1 : 2));
    eraseBlanks(command);
    eraseBlanks(file);
    int i = 0;
    while (i != string::npos && file[i] != ' ')
    {
        i++;
    }
    file = file.substr(0, i);
    int stdout_copy = dup(STDOUT_FILENO);
    int flags;
    if (cmd[pos] == '>' && cmd[pos + 1] == '>')
    {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else
    {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    }
    int fd = open(file.c_str(), flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("fd");
        close(stdout_copy);
        return;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        close(stdout_copy);
        return;
    }

    close(fd);
    if (commands.find(command) != commands.end())
    {
        commands[command]("");
    }
    else
    {
        const char *c = command.c_str();
        system(c);
    }
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdout_copy);
}
void inputRedirect(string cmd)
{

    int pos = cmd.find('<');
    int pos1 = cmd.find('>');
    if (pos == string::npos)
    {
        return;
    }
    string command = cmd.substr(0, pos);
    string file = cmd.substr(pos + 1);
    eraseBlanks(command);
    eraseBlanks(file);
    int i = 0;
    while (i != string::npos && file[i] != ' ')
    {
        i++;
    }
    file = file.substr(0, i);
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    if (stdin_copy == -1)
    {
        perror("dup");
        return;
    }
    int fd = open(file.c_str(), O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        close(stdin_copy);
        close(stdout_copy);
        return;
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        close(stdin_copy);
        close(stdout_copy);
        return;
    }
    if (pos1 != string::npos)
    {
        string file1 = cmd.substr(pos1 + (cmd[pos1] == '>' && cmd[pos1 + 1] != '>' ? 1 : 2));
        eraseBlanks(file1);
        int flags = (cmd[pos1 + 1] == '>') ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC;
        int fd1 = open(file1.c_str(), flags, S_IRUSR | S_IWUSR);
        if (fd1 == -1)
        {
            perror("open");
            close(fd);
            close(stdin_copy);
            close(stdout_copy);
            return;
        }
        if (dup2(fd1, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(fd1);
            close(fd);
            close(stdin_copy);
            close(stdout_copy);
            return;
        }
        close(fd1);
    }
    close(fd);
    if (commands.find(command) != commands.end())
    {
        commands[command]("");
    }
    else
    {
        const char *c = command.c_str();
        system(c);
    }

    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdin_copy);
    close(stdout_copy);
}
int checkRemaindme(string cmd)
{
    string c = cmd.substr(0, 9);
    if (c == "remaindme")
    {
        return 1;
    }
    return 0;
}
bool isNumber(const std::string &str)
{
    try
    {
        std::stod(str);
        return true;
    }
    catch (const std::invalid_argument &)
    {
        return false;
    }
    catch (const std::out_of_range &)
    {
        return false;
    }
}
void remaindme(string cmd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        cmd.erase(0, 9);
        eraseBlanks(cmd);

        int pos = cmd.find(' ');
        string timeStr = (pos == string::npos) ? cmd : cmd.substr(0, pos);
        if (!isNumber(timeStr))
        {
            cout << "Invalid time format." << endl;
            return;
        }
        int time = stoi(timeStr);
        sleep(time);
        string task = (pos == string::npos) ? "" : cmd.substr(pos + 1);
        if (!task.empty() && task.front() == '"' && task.back() == '"')
        {
            task = task.substr(1, task.size() - 2);
        }
        cout << endl;
        cout << task << endl;
    }
}
void mapCommands()
{
    commands["ls"] = ls;
    commands["echo"] = echo;
    commands["remaindme"] = remaindme;
}
void exeCommands(string cmd)
{
    int pos = cmd.find(' ');
    string command = cmd.substr(0, pos);
    string next;
    if (pos != string::npos)
    {
        next = cmd.substr(pos + 1);
    }
    else
        next = "";
    if (commands.find(command) != commands.end())
    {
        commands[command](next);
    }
    else
    {
        const char *c = cmd.c_str();
        system(c);
    }
}
int main()
{
    mapCommands();
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
        spacing(cmd);
        int i = check(cmd);
        if (i)
        {
            vector<string> bProcess;
            string fProcess;
            sortProcess(cmd, bProcess, fProcess);
            cmd = fProcess;
            cout << endl;
            background(bProcess);
        }
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
        else if (cmd == "jobs")
        {
            printBackgroundPIDs();
        }

        else if (checkRedirect(cmd))
        {
            checkRedirect(cmd) == 1 ? redirect(cmd) : inputRedirect(cmd);
        }
        else if (checkRemaindme(cmd))
        {
            remaindme(cmd);
        }
        else
            exeCommands(cmd);
    }
}