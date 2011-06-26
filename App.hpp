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

#include "MainWindow.hpp"
#include <Wt/WApplication>
#include <Wt/WSignal>
#include <string>
#include <mongo/client/connpool.h>
#include "Page.hpp"
#include "SessionHandle.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WEnvironment;
using Wt::WApplication;
using Wt::Signal;
using std::string;
using mongo::BSONObj;

namespace vidanueva {

class VidaApp : public WApplication {
public:
    typedef Signal<VidaApp*> AppSignal;
private:
    MainWindow* _mainWindow;
    AppSignal* _userChanged;
    string _mongoHostName;
    string _mongoDB;
    PageFactory _pages;
    SessionHandle* _userSession;
public:
    VidaApp(const WEnvironment &environment);
    AppSignal* userChanged() { return _userChanged; }      /// An event triggered when a user logs in or logs out
    MainWindow* mainWindow() { return _mainWindow; }       /// A pointer to the main window widget
    void goHome() { setInternalPath("/", true); }
    SessionHandle* userSession() { return _userSession; }          /// Returns a pointer to our session handle
    const string& mongoHostname() { return _mongoHostName; }     /// Returns the hostname of our mongo db
    const string& mongoDB() { return _mongoDB; }           /// Returns the name of the actual database inside of mongo
    const string mongoNSFor(const string& tableName) { return _mongoDB + "." + tableName; } /// Returns the mongo namespace for any given tablename eg: "pages" => "vidanueva.pages"
    void mongoSave(const string& tableName, mongo::BSONObj& index, mongo::BSONObj& data);
    PageFactory& pages() {return _pages;}
};

/**
* Returns the single app instance for your thread.
*/
inline VidaApp* getApp() { return dynamic_cast<VidaApp*>(Wt::WApplication::instance()); }


} // namespace vidanueva

#endif // APP_HPP
