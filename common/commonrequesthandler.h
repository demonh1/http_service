#ifndef COMMONREQUESTHANDLER_H
#define COMMONREQUESTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace http {

class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler (const Poco::Net::HTTPServerRequest& request) override;
};


class CommonRequestHandler: public Poco::Net::HTTPRequestHandler
{
private:
    std::string hash_str_;
public:
    CommonRequestHandler(const std::string& hash);
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
    const std::string& hash_value() const { return hash_str_; }
private:
    void SetHash(std::string& uri);
};

} //namespace http

#endif // COMMONREQUESTHANDLER_H
