#ifndef CLIENT_H
#define CLIENT_H

#include "clienthandler.h"

#include <string>
#include <memory>

namespace http_client {

class Client
{
public:
    Client();
    void Setup(int argc, char* argv[]);

private:
    void ParseCmdLine(int argc, char* argv[], std::string& strtype, std::string& strnum, std::string& filename, std::string& outdirname);

    std::unique_ptr<ClientHandler> handler_;
};

} // namespace http_client



#endif // CLIENT_H
