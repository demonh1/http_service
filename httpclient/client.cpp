#include "client.h"
#include "downloadclienthandler.h"

#include <boost/lexical_cast.hpp>
#include <iostream>

namespace http_client {

Client::Client()
{

}

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
        if(argc == 0) { throw std::runtime_error("Not enought parameters. Run with -h for help."); }

        for(int i = 1; i < argc; ++i)
        {
            std::string param(argv[i]);
            std::cout << "param " << param << std::endl;

            if( param.find("-t") == 0 )
            {
                strtype.assign(param.substr(2));
            }
            else if( param.find("-n") == 0 )
            {
                strnum.assign(param.substr(2));
            }
            else if( param.find("-f") == 0 )
            {
                filename.assign(param.substr(2));
            }
            else if( param.find("-o") == 0 )
            {
                outdirname.assign(param.substr(2));

            }
            else if( param.find("-h") ){ throw std::runtime_error("Using comands: \n replcfg -f[ilename]<filename>\n -s[uffix]<suffix"); }
        }

    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

}

} // namespace http_client
