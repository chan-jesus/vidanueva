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

namespace vidanueva {

VidaApp::VidaApp(const WEnvironment &environment) : WApplication(environment) {
    // Set up our signals
    _userChanged = new AppSignal(this);
    // Add some styles
    useStyleSheet(resourcesUrl() + "/themes/" + cssTheme() + "/forms.css");
    useStyleSheet(resourcesUrl() + "/themes/" + cssTheme() + "/fonts.css");
    // Configure our user login look up tool
    std::string mongoHost, mongoDb, mongoUsersTable;
    readConfigurationProperty("mongo-host", mongoHost); 
    readConfigurationProperty("mongo-db", mongoDb);
    readConfigurationProperty("mongo-users-table", mongoUsersTable);
    _userManager.configure(mongoHost, mongoDb, mongoUsersTable);
    // Load our message bundles
    messageResourceBundle().use(appRoot() + "messages/MainWindow");
    messageResourceBundle().use(appRoot() + "messages/LoginWindow");
    // Set up the UI
    setTitle(WString::tr("main-title"));
    _mainWindow = new MainWindow(root());
    setBodyClass("yui-skin-sam");
    // Hook up the url event handlers
    internalPathChanged().connect(this, &VidaApp::onURLChange);
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
            setInternalPath("/"); // Back to the home page .. can't login twice
        }
    }
}

void VidaApp::showLoginDialog() {
    log("NOTICE") << "Showing login dialog";
    WDialog dialog(WString::tr("login"));
    LoginWindow* loginWindow = new LoginWindow(dialog);
    loginWindow->setFocus();
    if (dialog.exec() == WDialog::Accepted) {
        dialog.hide();
        // See if we can log them in
        if (_userManager.checkLogin(loginWindow->username(), loginWindow->password())) {
            _username = loginWindow->username();
            log("NOTICE") << loginWindow->username() << " logged in";
            log("NOTICE") << "GOING HOME 2";
            setInternalPath("/"); // Back to the home page
            _userChanged->emit(this);
            return;
        }
        log("NOTICE") << loginWindow->username() << " failed log in";
    }
    _username = ""; // If we make it here .. we're not logged in anymore
    log("NOTICE") << "GOING HOME 3";
    setInternalPath("/"); // Back to the home page
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
    app->redirect("/vida");
    app->quit();
    return app;
}

} // namespace vidanueva

int main(int argc, char **argv) {
    try {
        WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

        server.addEntryPoint(Wt::Application, vidanueva::createApplication, "/vida", "/css/favicon.ico");
        server.addEntryPoint(Wt::Application, vidanueva::createRedirectApp, "", "/css/favicon.ico");

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
