#include "downloadclienthandler.h"

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"

#include "Poco/AsyncChannel.h"
#include "Poco/ConsoleChannel.h"

namespace http_client {

DownloadClientHandler::DownloadClientHandler(unsigned int threads_count)
    : threads_count_(threads_count)
    , task_pool_(std::unique_ptr<utils::ThreadPool>(new utils::ThreadPool(4)))
    , logger_(Poco::Logger::get("DownloadClientLogger"))
{
    task_pool_->Start(threads_count_);

    Poco::AutoPtr<Poco::ConsoleChannel> console(new Poco::ConsoleChannel);
    Poco::AutoPtr<Poco::AsyncChannel> channel(new Poco::AsyncChannel(console));

    logger_.setChannel("DownloadClientLogger", channel);
}

DownloadClientHandler::~DownloadClientHandler()
{
    task_pool_->Stop();
}

void DownloadClientHandler::Start(unsigned int threads_count, const char* file_path, const char* out_dir)
{

    std::string str_path(file_path), str_outdir(out_dir);
    try
    {
        LoadSettings(str_path);
        Download(str_outdir);
    }
    catch(std::exception& e)
    {
        logger_.error(e.what());
    }
}

void DownloadClientHandler::LoadSettings(const std::string& file_path)
{
    std::ifstream file;
    file.open(file_path);

    if(!file.is_open()) throw std::runtime_error("File " + file_path + "not open");

    std::string str;
    std::vector<std::string> elems;
    while (!file.eof())
    {
        std::getline(file, str);
        boost::split(elems,str, boost::is_any_of("\t"));
        urls_.emplace(elems[1],elems[0]); // name-url
    }

    file.close();
}

void DownloadClientHandler::Download(const std::string& out_dir)
{
    if( !boost::filesystem::exists(out_dir) )
    {
        boost::filesystem::create_directories(out_dir);
    }

    for(auto& item : urls_)
    {
        task_pool_->AddTask(
                    new utils::Task( std::bind( &DownloadClientHandler::DownloadFile, this, item.second, out_dir + "/" + item.first) ),
                    utils::Priority::NORMAL
                    );
    }

}

void DownloadClientHandler::DownloadFile(const std::string& url, const std::string& filename)
{
    try
    {
        size_t size = download(url, filename);

        if(size)
        {
            poco_information_f1(logger_, "File '%s' is loaded", filename);
        }
    }
    catch(std::exception& e)
    {
       logger_.error(e.what());
    }
}

/// download file
size_t DownloadClientHandler::download(const std::string& url, const std::string& filename)
{
    Poco::URI uri(url);
    std::string path(uri.getPathAndQuery());

    if (path.empty())
        path = "/";

    Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
    session.sendRequest(request);

    Poco::Net::HTTPResponse response;
    Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
    if( status != Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK)
    {
        throw std::runtime_error(std::string("Status ").append(std::to_string(status)).append(" " + response.getReason()));
    }

    poco_information_f2( logger_, "Status %d, %s", (int)status, response.getReason() );

    std::ofstream outstream(filename, std::ios_base::binary | std::ios_base::out);
    std::size_t size = Poco::StreamCopier::copyStream(session.receiveResponse(response), outstream);

    outstream.close();
    return size;
}

} // namespace http_client
