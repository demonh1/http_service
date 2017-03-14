#include "downloadhandler.h"
#include "../common/file_utils.h"

#include <iostream>

namespace http_service {

const char FILE_PATH[] = "../../../files"; // /home/demonh1/projects/C/cloud

DownloadHandler::DownloadHandler(const std::string& hash) : hash_(hash)
{

}

void DownloadHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    std::cout << "URI " << request.getURI() << std::endl;

    std::vector<char> data;
    std::string path(FILE_PATH);

    if( file_hlp::LoadFile( path.append( "/").append(hash_value()), data ) )
    {
        //std::cout << "path " << path << std::endl;
        std::cout << data.size() << std::endl;

        response.setContentType("application/file");
        response.sendBuffer(&data[0], data.size());

    }

}

} // namespace http_service
