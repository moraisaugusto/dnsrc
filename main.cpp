#include <iostream>
#include <fstream>
#include <giomm-2.4/giomm.h>

using namespace std;

static void printHelpFlag() {
    int rc = std::fputs("dnsRC 0.1 - (C) 2017 Augusto Morais\n"
          "Released under the GNU GPL.\n\n"
          "SERVER            dns server\n"
          "DOMAIN            domain to performs query\n"
          "-v                 Print version info\n"
          "\n" ,
          stdout);


    if (rc == EOF)
       std::perror("fputs()"); // POSIX requires that errno is set

   exit(0);
}

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char *argv[]) {

    string domain = "google.com";
    string dnsServer = "8.8.8.8";

    if (argc == 1) {
        printHelpFlag();
        exit(0);
    } else if ( (argc == 2) and (string(argv[1]) == "-v") ) {
        dnsServer = argv[1];
        printHelpFlag();
        exit(0);
    } else if (argc == 2) {
        dnsServer = argv[1];
    } else if (argc == 3) {
        dnsServer = argv[1];
        domain = argv[2];
    }

    // exec command
    string temp = "dig " + domain + " |grep SERVER | awk '{print $0}' | egrep -o '([0-9]+\.){3}[0-9]+' | head -1 > out.tmp";
    const char * c = temp.c_str();
    std::system(c);

    // read data from temp file
    std::ifstream t("out.tmp");
    string sLine;

    while (!t.eof()) {
        t >> sLine;
        break;
    }

    // create notification
    string message;
    if (dnsServer != sLine) {
        message = "Your DNS: " + dnsServer + "\nRouter DNS: " + sLine;

        auto Application = Gio::Application::create("routerDNS", Gio::APPLICATION_FLAGS_NONE);
        Application->register_application();
        auto Notification = Gio::Notification::create("DNS Does not match");
        Notification->set_body(message);
        auto Icon = Gio::ThemedIcon::create("dialog-information");
        Notification->set_icon (Icon);
        Application->send_notification(Notification);
    }

    // remove temp data
    if (is_file_exist("out.tmp") == 1)
        std::system("rm out.tmp");

    return 0;
}
