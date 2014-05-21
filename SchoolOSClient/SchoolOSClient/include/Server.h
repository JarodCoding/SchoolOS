#include "string.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "ProgrammManager.h"
#ifndef SERVER
#define SERVER

using namespace std;
class SchoolOSServer
{
    public:
        SchoolOSServer(string domain);
        virtual ~SchoolOSServer();
    protected:
    private:
    boost::asio::ip::tcp::socket Socket;
    ProgrammManager *ProgrammsManager;
};

#endif // SERVER
