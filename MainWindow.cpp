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
#include <Wt/WAnchor>

using Wt::WAnchor;

namespace vidanueva {

/**
* @brief Builds up the display and widgets
*
* @param parent Parent widget (probably WApplicaion::root())
*/
MainWindow::MainWindow(WContainerWidget* parent) : WTemplate(parent) {
    setTemplateText(tr("main-template"));
    VidaApp* app = dynamic_cast<VidaApp*>(WApplication::instance());
    app->userChanged()->connect(this, &MainWindow::onUserChanged);
    onUserChanged(app);
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
      bindString("controls", "Welcome " + app->username());
    }
}

} // namespace vidanueva {
