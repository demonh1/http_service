#include "client.h"
#include "downloadclienthandler.h"

#include <boost/lexical_cast.hpp>
#include <getopt.h>
#include <iostream>

namespace http_client {


void Client::Setup(int argc, char* argv[])
{
    std::string strtype, strnum, filename, outdir;
    try
    {
        ParseCmdLine(argc,argv,strtype,strnum,filename,outdir);

        if(strtype == "download")
        {
            uint32_t num = 1;
            if(!strnum.empty())
            {
                num = boost::lexical_cast<uint32_t>(strnum);
            }

            handler_.reset(new DownloadClientHandler(num));
            handler_->Start(num, filename.c_str(), outdir.c_str());

        }

    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}


void Client::ParseCmdLine(int argc, char* argv[], std::string& strtype, std::string& strnum, std::string& filename, std::string& outdirname)
{
    try
    {
        if(argc == 1)
        {
            throw std::runtime_error("Not enought parameters. Run with -h for help.");
        }

        int opt;
        while ((opt = getopt(argc, argv, "t:n:f:o:h")) != -1)
        {
            switch (opt)
            {
            case 't':
                strtype.assign(optarg);
                break;
            case 'n':
                strnum.assign(optarg);
                break;
            case 'f':
                filename.assign(optarg);
                break;
            case 'o':
                outdirname.assign(optarg);
                break;
            case 'h':
            default:
                throw std::runtime_error("Using comands: \n replcfg -t[ype]<type> -n[umber]<thread count> -f[ilename]<filename> -o[utput]<directory>\n");

            }
        }

    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

}

} // namespace http_client
