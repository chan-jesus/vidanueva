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
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WFileResource>
#include <string>
#include "LoginWindow.hpp"

namespace Wt {
    class WEnvironment;
}

using Wt::WString;
using Wt::WApplication;
using Wt::WEnvironment;
using Wt::WDialog;
using Wt::WServer;
using mongo::ScopedDbConnection;

namespace vidanueva {

VidaApp::VidaApp(const WEnvironment &environment) : WApplication(environment), _pages(this) {
    log("NOTICE") << "Resources URL: " << resourcesUrl();
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
    _userManager.configure(_mongoHostName, _mongoDB, mongoUsersTable);
    // Load our message bundles
    // Bundles we'll use a lot
    messageResourceBundle().use(appRoot() + "messages/MainWindow");
    messageResourceBundle().use(appRoot() + "messages/DialogButtonBar");
    messageResourceBundle().use(appRoot() + "messages/misc");
    messageResourceBundle().use(appRoot() + "messages/Page");
    messageResourceBundle().use(appRoot() + "messages/EditButtonBar");
    // Bundles we won't use quite so much
    messageResourceBundle().use(appRoot() + "messages/LoginWindow", false);
    messageResourceBundle().use(appRoot() + "messages/ControlPanel", false);
    messageResourceBundle().use(appRoot() + "messages/PageEdit", false);
    // Set up the UI
    setTitle(WString::tr("main-title"));
    _mainWindow = new MainWindow(root());
    setBodyClass("yui-skin-sam");
    // Hook up the url event handlers
    internalPathChanged().connect(this, &VidaApp::onURLChange);
    setInternalPath(internalPath(), true);
}

/**
* @brief Called when the url is changed
* 
* At the moment we're only interested in showing the login dialog if the user is logged in
*/
void VidaApp::onURLChange(const std::string& path) {
    log("NOTICE") << "Changing path: " << path;
    if (internalPathMatches("/login")) {
        log("NOTICE") << "Matches login path";
        if (_username == "") {
            // If no-one's logged in. Show the login dialog
            showLoginDialog();
        } else {
            log("NOTICE") << "GOING HOME 1";
            goHome(); // Back to the home page .. can't login twice
        }
    } else if (internalPathMatches("/page/")) {
        // Look up the page that belongs here
        std::string pageName = internalPathNextPart("/page/");
        Page* page = pages().load(pageName);
        mainWindow()->setBody(page);
    }
}

/**
* @brief Shows a modal login dialog
*/
void VidaApp::showLoginDialog() {
    WDialog dialog(WString::tr("login"));
    LoginWindow* loginWindow = new LoginWindow(dialog);
    loginWindow->setFocus();
    if (dialog.exec() == WDialog::Accepted) {
        dialog.hide();
        // See if we can log them in
        _userManager.savePassword(loginWindow->username(), loginWindow->password());
        if (_userManager.checkLogin(loginWindow->username(), loginWindow->password())) {
            _username = loginWindow->username();
            log("SECURITY") << loginWindow->username() << " logged in";
            goHome(); // Back to the home page
            _userChanged->emit(this);
            return;
        } else {
            mainWindow()->setStatusText(WString::tr("Wrong username or password"));
            log("SECURITY") << loginWindow->username() << " failed log in";
        }
    }
    _username = ""; // If we make it here .. we're not logged in anymore
    goHome(); // Back to the home page
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

        server.addEntryPoint(Wt::Application, vidanueva::createApplication, "/vida", "/css/favicon.ico");
        server.addEntryPoint(Wt::Application, vidanueva::createRedirectApp, "/", "/css/favicon.ico");

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


