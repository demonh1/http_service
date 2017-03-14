#include "serverapp.h"
#include "handlerfactory.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/AutoPtr.h"

namespace http_service {

ServerApp::ServerApp()
{

}

int ServerApp::main(const std::vector<std::string>& args)
{

    Poco::AutoPtr<Poco::Net::HTTPServerParams> params(new Poco::Net::HTTPServerParams());
    params->setMaxQueued(64);
    params->setMaxThreads(4);

    Poco::Net::ServerSocket socket(3675);
    Poco::ThreadPool pool;

    Poco::Net::HTTPServer server(new HandlerFactory(), pool, socket, params.get());

    server.start();
    waitForTerminationRequest();
    server.stop();

    return 0;

}

} // namespace http_service
