#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

namespace http_client {

class ClientHandler {
public:
    ClientHandler() = default;
    virtual ~ClientHandler() = default;

    virtual void Start(unsigned int threads_count, const char* file_path, const char* out_dir) = 0;
};

} // namespace http_client

#endif // CLIENTHANDLER_H
