/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.cpp
 *
 *    Description:  Main Window code
 *
 *        Version:  1.0
 *        Created:  06/01/2011 02:49:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "MainWindow.hpp"
#include "App.hpp"
#include "ControlPanel.hpp"
#include "LoginWindow.hpp"
#include <Wt/WAnchor>
#include <Wt/WString>
#include <Wt/WLogger>

using Wt::WAnchor;

namespace vidanueva {

const std::string MainWindow::pageBaseURL = "/page/"; /// The base where pages are
const std::string MainWindow::loginURL = "/login"; /// The base where pages are

void MainWindow::refresh() {
    WTemplate::refresh();
    VidaApp* app = getApp();
    app->log("PATH") << "Main Window Refreshed with path: " << app->internalPath() << " url " << app->url() << " bookmarkURL " << app->bookmarkUrl();
    onPathChange(app->internalPath());
}

/**
* @brief Builds up the display and widgets
*
* @param parent Parent widget (probably WApplicaion::root())
*/
MainWindow::MainWindow(WContainerWidget* parent) : WTemplate(parent) {
    setTemplateText(tr("main-template"));
    VidaApp* app = getApp();
    app->log("DEBUG") << "Main Window Creating";
    app->userChanged()->connect(this, &MainWindow::onUserChanged);
    app->internalPathChanged().connect(this, &MainWindow::onPathChange);
    onUserChanged(app);
    setStatusText("");
    bindString("nav", "NAV HERE");
    onPathChange(app->internalPath());
}

/**
* @brief Called when a user logs in or out. Used to update the control panel
*/
void MainWindow::onUserChanged(VidaApp* app) {
    if (!app->userSession()->isLoggedIn()) {
        WAnchor* login = new WAnchor();
        login->setRefInternalPath(loginURL);
        login->setText(tr("Login"));
        bindWidget("controls", login);
    } else {
        setStatusText("Welcome " + app->userSession()->username());
        bindWidget("controls", new ControlPanel());
    }
}

void MainWindow::onPathChange(const std::string& path) {
    VidaApp* app = getApp();
    std::string nextPart = app->internalPathNextPart(path);
    Page* page = 0;
    if ((path == "/") && nextPart.empty()) {
        // Load the default/welcome page
        page = app->pages().load("");
    } else if (app->internalPathMatches(pageBaseURL)) {
        // Look up the page that belongs here
        std::string pageName = app->internalPathNextPart(pageBaseURL); // What comes after /page/ ..
        page = app->pages().load(pageName);
    }
    if (page != 0) {
        // Load a page and return
        setBody(page);
        return;
    }
    // If we didn't load a page
    if (app->internalPathMatches(loginURL)) {
        LoginWindow* loginWindow = new LoginWindow();
        setBody(loginWindow);
    }
}


} // namespace vidanueva {
