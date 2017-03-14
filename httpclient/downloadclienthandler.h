#ifndef DOWNLOADCLIENTHANDLER_H
#define DOWNLOADCLIENTHANDLER_H

#include "clienthandler.h"
#include "../common/threadpool.h"

#include "Poco/Logger.h"

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <boost/noncopyable.hpp>

namespace http_client {

class DownloadClientHandler : public ClientHandler, private boost::noncopyable
{
public:
    explicit DownloadClientHandler(unsigned int threads_count);
    ~DownloadClientHandler();

    void Start(unsigned int threads_count, const char* file_path, const char* out_dir) override;

private:
    void LoadSettings(const std::string& file_path);
    void Download(const std::string& out_dir);

    void DownloadFile(const std::string& url, const std::string& filename);

    size_t download(const std::string& url, const std::string& filename);

private:
    uint32_t threads_count_;
    std::unordered_map<std::string, std::string> urls_;
    std::unique_ptr<utils::ThreadPool> task_pool_;
    Poco::Logger& logger_;
};

} // namespace http_client

#endif // DOWNLOADCLIENTHANDLER_H
