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

#include "App.hpp"
#include <Wt/WApplication>
#include <Wt/WString>
#include <Wt/WServer>
#include <Wt/WLogger>
#include <Wt/WPushButton>
#include <Wt/WFileResource>
#include <string>
#include "LoginWindow.hpp"
#include "SessionStore.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WString;
using Wt::WApplication;
using Wt::WEnvironment;
using Wt::WServer;
using mongo::ScopedDbConnection;

namespace vidanueva {

VidaApp::VidaApp(const WEnvironment &environment) : WApplication(environment), _pages(this) {
    log("NOTICE") << "Resources URL: " << resourcesUrl();
    _userSession = new SessionHandle(this);
    // Set up our signals
    _userChanged = new AppSignal(this);
    // Add some styles
    useStyleSheet(resourcesUrl() + "/themes/" + cssTheme() + "/forms.css");
    useStyleSheet(resourcesUrl() + "/themes/" + cssTheme() + "/fonts.css");
    useStyleSheet(resourcesUrl() + "/themes/" + cssTheme() + "/controlPanel.css");
    // Configure our user login look up tool
    std::string mongoUsersTable;
    readConfigurationProperty("mongo-host", _mongoHostName); 
    readConfigurationProperty("mongo-db", _mongoDB);
    readConfigurationProperty("mongo-users-table", mongoUsersTable);
    _userSession->configureMongo(_mongoHostName, _mongoDB, mongoUsersTable);
    // Load our message bundles
    // Bundles we'll use a lot
    messageResourceBundle().use(appRoot() + "messages/MainWindow");
    messageResourceBundle().use(appRoot() + "messages/DialogButtonBar");
    messageResourceBundle().use(appRoot() + "messages/misc");
    messageResourceBundle().use(appRoot() + "messages/Page");
    messageResourceBundle().use(appRoot() + "messages/ButtonBar");
    // Bundles we won't use quite so much
    messageResourceBundle().use(appRoot() + "messages/LoginWindow", false);
    messageResourceBundle().use(appRoot() + "messages/ControlPanel", false);
    messageResourceBundle().use(appRoot() + "messages/PageEdit", false);
    // Set up the UI
    setTitle(WString::tr("main-title"));
    _mainWindow = new MainWindow(root());
    setBodyClass("yui-skin-sam");
    // Show the stuff from the right thing for the right path the entered on
    setInternalPath(internalPath(), true);
}

/**
* @brief Insert/Update a record in the mongo DB.
* 
* Probably shouldn't be called much, better to use something like app.pages().save(...)
*
* @param tableName The name of the table to insert/update on
* @param index The index to look up the existing record
* @param data The data to replace/insert into the table
*/
void VidaApp::mongoSave(const string& tableName, mongo::BSONObj& index, mongo::BSONObj& data) {
    ScopedDbConnection db(_mongoHostName);
    db->update(mongoNSFor(tableName), index, data, true);
    db.done();
}

/**
* @brief The main vidanueva app
*
* @param env
*
* @return 
*/
WApplication *createApplication(const WEnvironment& env) { return new VidaApp(env); }

/**
* @brief If someone goes to / .. redirect them to /vida
*
* @param env
*
* @return 
*/
WApplication *createRedirectApp(const WEnvironment& env) {
    WApplication* app = new WApplication(env);
    app->redirect("/vida#" + app->internalPath());
    app->quit();
    return app;
}

} // namespace vidanueva

int main(int argc, char **argv) {
    try {
        WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(Wt::Application, vidanueva::createApplication, "", "/css/favicon.ico");
        //server.addEntryPoint(Wt::Application, vidanueva::createApplication, "/", "/css/favicon.ico");
        //server.addEntryPoint(Wt::Application, vidanueva::createRedirectApp, "/", "/css/favicon.ico");

        if (server.start()) {
            WServer::waitForShutdown();
            server.stop();
        }
    } catch (Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}


