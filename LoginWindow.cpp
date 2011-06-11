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

#include <Wt/WString>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WDialog>

using Wt::WString;

namespace vidanueva {

LoginWindow::LoginWindow(WDialog& parentDialog) : MoreAwesomeTemplate(parentDialog.contents()) {
    setTemplateText(tr("login-template"));
    addStyleClass("yui-gd dialog form"); // 1/3 + 2/3 style
    bindAndCreateField(_usernameLabel, _usernameEdit, "username");

    // Password
    bindAndCreateField(_passwordLabel, _passwordEdit, "password");
    _passwordEdit->setEchoMode(WLineEdit::Password);

    // Buttons
    bindWidget("btn-bar", _btnBar = new DialogButtonBar(parentDialog));

    // Hook up accept and reject signals
    // All these do an accept
    _passwordEdit->enterPressed().connect(&parentDialog, &WDialog::accept);
    
    // These do reject
    _usernameEdit->escapePressed().connect(&parentDialog, &WDialog::accept);
    _passwordEdit->escapePressed().connect(&parentDialog, &WDialog::accept);
}

} // namespace vidanueva
