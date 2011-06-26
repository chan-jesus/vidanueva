/*
 * =====================================================================================
 *
 *       Filename:  SessionHandle.hpp
 *
 *    Description:  Hooks together witty-sessions- and the mongo UserManager
 *
 *        Version:  1.0
 *        Created:  06/25/2011 08:58:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef SESSION_HANDLE_HPP
#define SESSION_HANDLE_HPP

#include <string>
#include <Wt/WObject>
#include <Wt/WRandom>
#include <Wt/WTimer>
#include "SessionStore.hpp"
#include "UserManager.hpp"

using std::string;
using Wt::WObject;
using Wt::WRandom;
using Wt::WTimer;

namespace vidanueva {

/**
* @brief A handle to the session of the currently logged in user
*/
class SessionHandle : public WObject {
public:
    static const string cookieName;
private:
    UserManager userManager;
    SessionStore& sessions;
    string cookieCache;
    string getCookie();
    WTimer* touchSessionsTimer; // Used to stop sessions timing out without touching them too much
    void onTouchSessionsActivate();
public:
    SessionHandle(WObject* parent) : WObject(parent), sessions(SessionStore::getInstance()) {
        // Check if we're already logged in
        string username = sessions.username(getCookie(), true); // Touch the session as new app/view is openning for it
        if (!username.empty())
            userManager.forceLogin(username);
    }
    void configureMongo(const string& hostname, const string& dbName, const string& tableName="users") {
        userManager.configure(hostname, dbName, tableName);
    }
    bool tryLogin(const string& username, const string& password);
    void logout();
    void touchSession();
    string username();
    bool isLoggedIn() { return !username().empty(); }
};

} // namespace vidanueva

#endif // SESSION_HANDLE_HPP
