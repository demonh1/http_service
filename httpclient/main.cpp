#include "client.h"
#include "../common/singleton.h"

using ClientSingle = utils::Singleton<http_client::Client>;

int main(int argc, char *argv[])
{
    ClientSingle::instance().Setup(argc,argv);


    return 0;
}
