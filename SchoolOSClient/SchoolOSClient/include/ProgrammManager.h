#include "../../../SchoolOSProtocol/Server/CPP/User/Login.pb.h"
#include "string.h"

#ifndef PROGRAMMMANAGER
#define PROGRAMMMANAGER
using namespace std;

class ProgrammManager
{
    public:
        ProgrammManager();
        virtual ~ProgrammManager();
    protected:
    private:
    int shID;
    short *myPtr;
    Server::User::Programm *programms;
};

#endif // PROGRAMMMANAGER
