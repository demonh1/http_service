#ifndef DOWNLOADHANDLER_H
#define DOWNLOADHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>

namespace http_service {

class DownloadHandler : public Poco::Net::HTTPRequestHandler
{
public:
    explicit DownloadHandler( const std::string& hash );
    ~DownloadHandler() = default;

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
    const std::string& hash_value() const { return hash_; }
private:
    std::string hash_;
};

} // namespace http_service

#endif // DOWNLOADHANDLER_H
