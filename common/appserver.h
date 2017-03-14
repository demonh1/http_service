#ifndef APPSERVER_H
#define APPSERVER_H

#include <iostream>
#include <Poco/Util/ServerApplication.h>

#define APP_SERVER_MAIN_FUNC(AppServerClassName, Main) \
    int Main(int argc, char* argv[]) { \
        AppServerClassName app; \
        try { \
           return app.run (argc, argv); \
        }  \
        catch(Poco::Exception& e){ \
             std::cerr << "POCO ERROR: " << e.displayText() << std::endl; \
             app.logger().log (e); \
        }  \
        return Poco::Util::Application::EXIT_CONFIG; \
        }

#define APP_SERVER(AppServerClassName) APP_SERVER_MAIN_FUNC(AppServerClassName, main)




#endif // APPSERVER_H
