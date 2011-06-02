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
#include <Wt/WAnchor>
#include <Wt/WString>

using Wt::WAnchor;

namespace vidanueva {

/**
* @brief Builds up the display and widgets
*
* @param parent Parent widget (probably WApplicaion::root())
*/
MainWindow::MainWindow(WContainerWidget* parent) : WTemplate(parent) {
    setTemplateText(tr("main-template"));
    VidaApp* app = getApp();
    app->userChanged()->connect(this, &MainWindow::onUserChanged);
    onUserChanged(app);
    setStatusText("");
    bindString("nav", "NAV HERE");
}

/**
* @brief Called when a user logs in or out. Used to update the control panel
*/
void MainWindow::onUserChanged(VidaApp* app) {
    if (!app->loggedIn()) {
        WAnchor* login = new WAnchor();
        login->setRefInternalPath("/login");
        login->setText(tr("login"));
        bindWidget("controls", login);
    } else {
        setStatusText("Welcome " + app->username());
        bindWidget("controls", new ControlPanel());
    }
}


void MainWindow::setStatusText(const WString& newMessage) {
    bindString("status-text", newMessage);
}

} // namespace vidanueva {
