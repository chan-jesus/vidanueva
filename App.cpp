/*
 * =====================================================================================
 *
 *       Filename:  App.cpp
 *
 *    Description:  Main application
 *
 *        Version:  1.0
 *        Created:  06/01/2011 09:38:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <Wt/WApplication>
#include <Wt/WString>
#include <Wt/WServer>
#include <Wt/WLogger>
#include <string>
#include "MainWindow.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WString;
using Wt::WApplication;
using Wt::WEnvironment;

namespace vidanueva {

    class VidaApp : public WApplication {
    private:
        MainWindow* _mainWindow;
    public:
        VidaApp(const WEnvironment &environment) : WApplication(environment) {
            messageResourceBundle().use(appRoot() + "vidanueva");
            setTitle(WString::tr("main-title"));
            _mainWindow = new MainWindow(root());
            setBodyClass("yui-skin-sam");
        }

    };

    WApplication *createApplication(const WEnvironment& env) {
      return new VidaApp(env);
    }

} // namespace vidanueva

int main(int argc, char **argv) {
  return WRun(argc, argv, &vidanueva::createApplication);
}
