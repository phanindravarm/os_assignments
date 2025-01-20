#include <fstream>
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
#include <arpa/inet.h>
#include <pthread.h>
using namespace std;
int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
map<string, string> mimeTypes = {
    {".js", "text/javascript"},
    {".mjs", "text/javascript"},
    {".json", "application/json"},
    {".webmanifest", "application/manifest+json"},
    {".doc", "application/msword"},
    {".dot", "application/msword"},
    {".wiz", "application/msword"},
    {".nq", "application/n-quads"},
    {".nt", "application/n-triples"},
    {".bin", "application/octet-stream"},
    {".a", "application/octet-stream"},
    {".dll", "application/octet-stream"},
    {".exe", "application/octet-stream"},
    {".o", "application/octet-stream"},
    {".obj", "application/octet-stream"},
    {".so", "application/octet-stream"},
    {".oda", "application/oda"},
    {".pdf", "application/pdf"},
    {".p7c", "application/pkcs7-mime"},
    {".ps", "application/postscript"},
    {".ai", "application/postscript"},
    {".eps", "application/postscript"},
    {".trig", "application/trig"},
    {".m3u", "application/vnd.apple.mpegurl"},
    {".m3u8", "application/vnd.apple.mpegurl"},
    {".xls", "application/vnd.ms-excel"},
    {".xlb", "application/vnd.ms-excel"},
    {".eot", "application/vnd.ms-fontobject"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pot", "application/vnd.ms-powerpoint"},
    {".ppa", "application/vnd.ms-powerpoint"},
    {".pps", "application/vnd.ms-powerpoint"},
    {".pwz", "application/vnd.ms-powerpoint"},
    {".wasm", "application/wasm"},
    {".bcpio", "application/x-bcpio"},
    {".cpio", "application/x-cpio"},
    {".csh", "application/x-csh"},
    {".dvi", "application/x-dvi"},
    {".gtar", "application/x-gtar"},
    {".hdf", "application/x-hdf"},
    {".h5", "application/x-hdf5"},
    {".latex", "application/x-latex"},
    {".mif", "application/x-mif"},
    {".cdf", "application/x-netcdf"},
    {".nc", "application/x-netcdf"},
    {".p12", "application/x-pkcs12"},
    {".pfx", "application/x-pkcs12"},
    {".ram", "application/x-pn-realaudio"},
    {".pyc", "application/x-python-code"},
    {".pyo", "application/x-python-code"},
    {".sh", "application/x-sh"},
    {".shar", "application/x-shar"},
    {".swf", "application/x-shockwave-flash"},
    {".sv4cpio", "application/x-sv4cpio"},
    {".sv4crc", "application/x-sv4crc"},
    {".tar", "application/x-tar"},
    {".tcl", "application/x-tcl"},
    {".tex", "application/x-tex"},
    {".texi", "application/x-texinfo"},
    {".texinfo", "application/x-texinfo"},
    {".roff", "application/x-troff"},
    {".t", "application/x-troff"},
    {".tr", "application/x-troff"},
    {".man", "application/x-troff-man"},
    {".me", "application/x-troff-me"},
    {".ms", "application/x-troff-ms"},
    {".ustar", "application/x-ustar"},
    {".src", "application/x-wais-source"},
    {".xsl", "application/xml"},
    {".rdf", "application/xml"},
    {".wsdl", "application/xml"},
    {".xpdl", "application/xml"},
    {".zip", "application/zip"},
    {".3gp", "audio/3gpp"},
    {".3gpp", "audio/3gpp"},
    {".3g2", "audio/3gpp2"},
    {".3gpp2", "audio/3gpp2"},
    {".aac", "audio/aac"},
    {".adts", "audio/aac"},
    {".loas", "audio/aac"},
    {".ass", "audio/aac"},
    {".au", "audio/basic"},
    {".snd", "audio/basic"},
    {".mka", "audio/matroska"},
    {".mp3", "audio/mpeg"},
    {".mp2", "audio/mpeg"},
    {".opus", "audio/opus"},
    {".aif", "audio/x-aiff"},
    {".aifc", "audio/x-aiff"},
    {".aiff", "audio/x-aiff"},
    {".ra", "audio/x-pn-realaudio"},
    {".wav", "audio/x-wav"},
    {".otf", "font/otf"},
    {".ttf", "font/ttf"},
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".avif", "image/avif"},
    {".bmp", "image/bmp"},
    {".emf", "image/emf"},
    {".fits", "image/fits"},
    {".g3", "image/g3fax"},
    {".gif", "image/gif"},
    {".ief", "image/ief"},
    {".jp2", "image/jp2"},
    {".jpg", "image/jpeg"},
    {".jpe", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".jpm", "image/jpm"},
    {".jpx", "image/jpx"},
    {".heic", "image/heic"},
    {".heif", "image/heif"},
    {".png", "image/png"},
    {".svg", "image/svg+xml"},
    {".t38", "image/t38"},
    {".tiff", "image/tiff"},
    {".tif", "image/tiff"},
    {".tfx", "image/tiff-fx"},
    {".ico", "image/vnd.microsoft.icon"},
    {".webp", "image/webp"},
    {".wmf", "image/wmf"},
    {".ras", "image/x-cmu-raster"},
    {".pnm", "image/x-portable-anymap"},
    {".pbm", "image/x-portable-bitmap"},
    {".pgm", "image/x-portable-graymap"},
    {".ppm", "image/x-portable-pixmap"},
    {".rgb", "image/x-rgb"},
    {".xbm", "image/x-xbitmap"},
    {".xpm", "image/x-xpixmap"},
    {".xwd", "image/x-xwindowdump"},
    {".eml", "message/rfc822"},
    {".mht", "message/rfc822"},
    {".mhtml", "message/rfc822"},
    {".nws", "message/rfc822"},
    {".css", "text/css"},
    {".csv", "text/csv"},
    {".html", "text/html"},
    {".htm", "text/html"},
    {".md", "text/markdown"},
    {".markdown", "text/markdown"},
    {".n3", "text/n3"},
    {".txt", "text/plain"},
    {".bat", "text/plain"},
    {".c", "text/plain"},
    {".h", "text/plain"},
    {".ksh", "text/plain"},
    {".pl", "text/plain"},
    {".srt", "text/plain"},
    {".rtx", "text/richtext"},
    {".rtf", "text/rtf"},
    {".tsv", "text/tab-separated-values"},
    {".vtt", "text/vtt"},
    {".py", "text/x-python"},
    {".rst", "text/x-rst"},
    {".etx", "text/x-setext"},
    {".sgm", "text/x-sgml"},
    {".sgml", "text/x-sgml"},
    {".vcf", "text/x-vcard"},
    {".xml", "text/xml"},
    {".mkv", "video/matroska"},
    {".mk3d", "video/matroska-3d"},
    {".mp4", "video/mp4"},
    {".mpeg", "video/mpeg"},
    {".m1v", "video/mpeg"},
    {".mpa", "video/mpeg"},
    {".mpe", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".mov", "video/quicktime"},
    {".qt", "video/quicktime"},
    {".webm", "video/webm"},
    {".avi", "video/x-msvideo"},
    {".movie", "video/x-sgi"}};

void signalHandler(int signum)
{
    cout << "Closing.." << endl;
    close(serverSocket);
}
void MIME(string file, string &contentType)
{
    int pos = file.find_last_of(".");
    if (pos != string::npos)
    {
        string fileType = file.substr(pos);
        if (mimeTypes.find(fileType) != mimeTypes.end())
        {
            contentType = mimeTypes[fileType];
        }
        else
            contentType = "text/plain";
    }
    else
        contentType = "text/plain";
}
void ls(string path, string &body, string &contentType)
{
    signal(SIGINT, signalHandler);
    char *homeD = getenv("HOME");
    string stringHome = string(homeD);
    string absolutePath = stringHome + path;
    struct stat s;
    stat(absolutePath.c_str(), &s);
    if (S_ISDIR(s.st_mode))
    {
        contentType = "text/html";
        DIR *dr = opendir(absolutePath.c_str());
        struct dirent *en;
        body = body + "<ul>";
        while ((en = readdir(dr)) != NULL)
        {
            if (path == "/")
            {
                path = "";
            }
            string link = "<a href=\"" + path + "/" + string(en->d_name) + "\">" + string(en->d_name) + "</a>";
            body += "<li>" + link + "</li>";
        }
        body = body + "</ul>";
        closedir(dr);
    }
    else if (S_ISREG(s.st_mode))
    {

        MIME(path, contentType);
        ifstream file(absolutePath);
        string line;
        while (getline(file, line))
        {
            body = body + line + '\n';
        }
    }

    else
    {
        body = "Nothing";
    }
}
void *threading(void *clientArg)
{
    int clientSocket = *((int *)clientArg);
    char request[100000];
    int mes = recv(clientSocket, request, sizeof(request), 0);
    if (mes < 0)
    {
        perror("Error receiving request");
        return nullptr;
    }
    string method, path, version;
    string requestString(request);
    stringstream requestStream(requestString);
    requestStream >> method >> path >> version;
    string body;
    string contentType;
    ls(path, body, contentType);
    stringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n"
                   << "Content-Type: " << contentType + ";" << "charset=utf-8\r\n"
                   << "Content-Length: " << body.length() << "\r\n"
                   << "Connection: keep-alive\r\n"
                   << "Cache-Control: s-maxage=300, public, max-age=0\r\n"
                   << "Content-Language: en-US\r\n"
                   << "ETag: \"2e77ad1dc6ab0b53a2996dfd4653c1c3\"\r\n"
                   << "Server: meinheld/0.6.1\r\n"
                   << "Strict-Transport-Security: max-age=63072000\r\n"
                   << "X-Content-Type-Options: nosniff\r\n"
                   << "X-Frame-Options: DENY\r\n"
                   << "X-XSS-Protection: 1; mode=block\r\n"
                   << "Vary: Accept-Encoding,Cookie\r\n"
                   << "\r\n"
                   << body;
    string response = responseStream.str();
    int sen = send(clientSocket, response.c_str(), response.length(), 0);
    if (sen < 0)
    {
        perror("Error sending response");
        return nullptr;
    }
    close(clientSocket);
    pthread_exit(NULL);
}
int main()
{

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int bindInteger = ::bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindInteger < 0)
    {
        perror("Error binding");
        return 0;
    }
    int listenInteger = listen(serverSocket, 1);
    if (listenInteger < 0)
    {
        perror("Error listening");
        return 0;
    }
    string command;
    while (true)
    {

        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0)
        {
            perror("Error accepting");
            break;
        }
        pthread_t thread;
        pthread_create(&thread, NULL, threading, &clientSocket);
        pthread_join(thread, NULL);
    }
    close(serverSocket);
}
