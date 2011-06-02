/*
 * =====================================================================================
 *
 *       Filename:  LoginWindow.hpp
 *
 *    Description:  Login screen to be placed at /login url
 *
 *        Version:  1.0
 *        Created:  06/01/2011 02:43:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LOGIN_WINDOW_HPP
#define LOGIN_WINDOW_HPP

#include <Wt/WTemplate>
#include <Wt/WLineEdit>
#include <string>

namespace Wt {
    class WString;
    class WLabel;
    class WContainerWidget;
    class WPushButton;
    class WDialog;
}

using Wt::WTemplate;
using Wt::WString;
using Wt::WLabel;
using Wt::WContainerWidget;
using Wt::WPushButton;
using Wt::WLineEdit;
using Wt::WDialog;

namespace vidanueva {

    class LoginWindow : public WTemplate {
    private:
        WLabel* _usernameLabel;
        WLineEdit* _usernameEdit;
        WLabel* _passwordLabel;
        WLineEdit* _passwordEdit;
        WPushButton* _okBtn;
        WPushButton* _cancelBtn;
        template <class T> void bindAndCreateWidget(T*& pointer, const std::string& name, const WString& labelName="");
    public:
        LoginWindow(WDialog& parentDialog);
        std::string username() { return _usernameEdit->text().toUTF8(); } /// Returns the username entered by the user
        std::string password() { return _passwordEdit->text().toUTF8(); } /// Returns the password entered by the user
        void setFocus() { _usernameEdit->setFocus(); }
    };

} // namespace vidanueva

#endif // LOGIN_WINDOW_HPP
