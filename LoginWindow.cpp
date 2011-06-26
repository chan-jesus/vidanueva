/*
 * =====================================================================================
 *
 *       Filename:  LoginWindow.cpp
 *
 *    Description:  Login Screen
 *
 *        Version:  1.0
 *        Created:  06/01/2011 02:51:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "LoginWindow.hpp"

#include <Wt/WRandom>
#include <Wt/WString>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WLogger>
#include <Wt/WPushButton>
#include <Wt/WContainerWidget>
#include "App.hpp"
#include "SessionStore.hpp"

using Wt::WString;

namespace vidanueva {

LoginWindow::LoginWindow(WContainerWidget* parent) : MoreAwesomeTemplate(parent) {
    setTemplateText(tr("login-template"));
    addStyleClass("yui-gd dialog form"); // 1/3 + 2/3 style
    bindAndCreateField(_usernameLabel, _usernameEdit, "username");

    // Password
    bindAndCreateField(_passwordLabel, _passwordEdit, "password");
    _passwordEdit->setEchoMode(WLineEdit::Password);

    // Buttons
    bindWidget("btn-bar", _btnBar = new ButtonBar(tr("Login"), tr("Cancel")));
    _btnBar->btn1()->clicked().connect(this, &LoginWindow::handleOKHit);
    _btnBar->btn2()->clicked().connect(this, &LoginWindow::handleCancelHit);

    // Hook up accept and reject signals
    // All these do an accept
    _passwordEdit->enterPressed().connect(this, &LoginWindow::handleOKHit);    

    // These do reject
    _usernameEdit->escapePressed().connect(this, &LoginWindow::handleCancelHit);
    _passwordEdit->escapePressed().connect(this, &LoginWindow::handleCancelHit);
}

/**
* @brief Called when the user hits OK to login
*/
void LoginWindow::handleOKHit() {
    VidaApp* app = getApp();
    // See if we can log them in
    string username = _usernameEdit->text().toUTF8();
    string password = _passwordEdit->text().toUTF8();
    if (app->userSession()->tryLogin(username, password)) {
        // Let the application know
        app->log("SECURITY") << username << " logged in";
        app->goHome(); // Back to the home page
        app->userChanged()->emit(app);
    } else {
        app->mainWindow()->setStatusText(WString::tr("Wrong username or password"));
        app->log("SECURITY") << username << " failed log in";
    }
}

void LoginWindow::handleCancelHit() {
    VidaApp* app = getApp();
    app->mainWindow()->setStatusText(tr("Login Cancelled"));
    app->goHome();
}


} // namespace vidanueva
