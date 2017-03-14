#include "handlerfactory.h"
#include "downloadhandler.h"

#include <iostream>
#include <boost/regex.hpp>


namespace http_service {

Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler (const Poco::Net::HTTPServerRequest& request)
{
    boost::smatch res;

    try
    {
        if( boost::regex_match( request.getURI(), res, boost::regex( "\\/download\\?sha1=([0-9a-fA-F]{40})$" ) ) )
        {
            return new DownloadHandler(res[1]);
        }

    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return nullptr;
}


} // namespace http_service
