/*
 * =====================================================================================
 *
 *       Filename:  App.hpp
 *
 *    Description:  Main Application include file
 *
 *        Version:  1.0
 *        Created:  06/01/2011 04:24:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef APP_HPP
#define APP_HPP

#include <Wt/WApplication>
#include "UserManager.hpp"
#include "MainWindow.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WEnvironment;
using Wt::WApplication;

namespace vidanueva {

    class VidaApp : public WApplication {
    private:
        MainWindow* _mainWindow;
        std::string _username;     // Name of currently logged in user. "" if none
        UserManager _userManager;
        void onURLChange(const std::string& path);
        void showLoginDialog();
    public:
        VidaApp(const WEnvironment &environment);
        bool loggedIn() { return !_username.empty(); } /// Returns true if a user is logged in, otherwise false if current user is anonymous
    };

WApplication *createApplication(const WEnvironment& env) { return new VidaApp(env); }
WApplication *createRedirectApp(const WEnvironment& env) {
    WApplication* app = new VidaApp(env);
    app->redirect("/vida");
    app->quit();
    return app;
}


} // namespace vidanueva

#endif // APP_HPP
