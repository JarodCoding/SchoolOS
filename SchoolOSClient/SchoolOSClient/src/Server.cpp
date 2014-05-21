#include "Server.h"

SchoolOSServer::SchoolOSServer(string Domain )
{

    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(Domain, "SchoolOS");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    (*this).Socket = socket(io_service);
    Socket.connect(endpoint_iterator);
    ProgrammsManager = new ProgrammManager();
}

SchoolOSServer::~SchoolOSServer()
{
        (*this).Socket.close();
}
