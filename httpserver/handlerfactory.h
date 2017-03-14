#ifndef HANDLERFACTORY_H
#define HANDLERFACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace http_service {

class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler (const Poco::Net::HTTPServerRequest& request) override;
};

} // namespace http_service

#endif // HANDLERFACTORY_H
