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

/**
* @brief Replace a template placeholder 'name' with a widget (pointed to by pointer)
*
* @tparam T The type of widget to create
* @param pointer The pointer to the newly created Widget
* @param name of the place in the template we're replacing
* @param labelName The name to give a label or button, etc. optional
*/
template <class T> inline void LoginWindow::bindAndCreateWidget(T*& pointer, const std::string& name, const WString& labelName) {
    bindWidget(name,  pointer = new T(labelName));
}

LoginWindow::LoginWindow(WDialog& parentDialog) : WTemplate(parentDialog.contents()) {
    setTemplateText(tr("login-template"));
    addStyleClass("yui-b dialog"); // 1/3 + 2/3 style
    // Username
    bindAndCreateWidget(_usernameLabel, "username-label", tr("username-label"));
    bindAndCreateWidget(_usernameEdit, "username-edit");
    _usernameLabel->setBuddy(_usernameEdit);

    // Password
    bindAndCreateWidget(_passwordLabel, "password-label", tr("password-label"));
    bindAndCreateWidget(_passwordEdit, "password-edit");
    _passwordLabel->setBuddy(_passwordEdit);
    _passwordEdit->setEchoMode(WLineEdit::Password);

    // Buttons
    bindAndCreateWidget(_okBtn, "ok-btn", tr("ok-btn"));
    bindAndCreateWidget(_cancelBtn, "cancel-btn", tr("cancel-btn"));

    // Hook up accept and reject signals
    // All these do an accept
    _passwordEdit->enterPressed().connect(&parentDialog, &WDialog::accept);
    _okBtn->clicked().connect(&parentDialog, &WDialog::accept);
    
    // These do reject
    _cancelBtn->clicked().connect(&parentDialog, &WDialog::reject);
    _usernameEdit->escapePressed().connect(&parentDialog, &WDialog::accept);
    _passwordEdit->escapePressed().connect(&parentDialog, &WDialog::accept);
}

} // namespace vidanueva
