#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "Poco/Util/ServerApplication.h"

namespace http_service {

class ServerApp : public Poco::Util::ServerApplication
{
public:
    ServerApp();
    ~ServerApp() = default;

protected:
    int main(const std::vector<std::string>& args) override;
};

} // namespace http_service

#endif // SERVERAPP_H
