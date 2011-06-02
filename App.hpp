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
#include <Wt/WSignal>
#include "UserManager.hpp"
#include "MainWindow.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WEnvironment;
using Wt::WApplication;
using Wt::Signal;

namespace vidanueva {

class VidaApp : public WApplication {
public:
    typedef Signal<VidaApp*> AppSignal;
private:
    MainWindow* _mainWindow;
    std::string _username;     // Name of currently logged in user. "" if none
    UserManager _userManager;
    void onURLChange(const std::string& path);
    void showLoginDialog();
    AppSignal* _userChanged;
public:
    VidaApp(const WEnvironment &environment);
    bool loggedIn() { return !_username.empty(); }         /// Returns true if a user is logged in, otherwise false if current user is anonymous
    std::string username() { return _username; }           /// Returns the username if someone is logged in, "" otherwise
    AppSignal* userChanged() { return _userChanged; }      /// An event triggered when a user logs in or logs out
    MainWindow* mainWindow() { return _mainWindow; }       /// A pointer to the main window widget
};


/**
* Returns the single app instance for your thread.
*/
inline VidaApp* getApp() { return dynamic_cast<VidaApp*>(WApplication::instance()); }


} // namespace vidanueva

#endif // APP_HPP
