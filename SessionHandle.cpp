/*
 * =====================================================================================
 *
 *       Filename:  SessionHandle.cpp
 *
 *    Description:  Handles logged in user sessions. Using Wt, Cookies and MongoDB
 *
 *        Version:  1.0
 *        Created:  06/25/2011 09:09:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "SessionHandle.hpp"
#include <Wt/WEnvironment>
#include "App.hpp"

using Wt::WEnvironment;

namespace vidanueva {

const string SessionHandle::cookieName("vida_app");

/**
* @brief Get's the cookie from the app
*
* @return The cookie that the client's browser is giving us
*/
inline string SessionHandle::getCookie() {
    VidaApp* app = getApp();
    const WEnvironment::CookieMap& cookies = app->environment().cookies();
    WEnvironment::CookieMap::const_iterator i = cookies.find(cookieName);
    if (i != cookies.end()) {
        return (*i).second;
    }
    // If the browser doesn't have a cookie, we might have just set it
    // Even if this server side cache is old (and the user has logged out, and the client side cookie deleted)
    // It shouldn't matter because the session store still won't return a username for it.
    // But still if there is a browser side cookie, that takes precedence
    return cookieCache;
}

/**
* @brief Tries to log the user in, creates the session, and sets the session cookie.
*
* @param username
* @param password
*
* @return true if login was successful
*/
bool SessionHandle::tryLogin(const string& username, const string& password) {
    if (userManager.checkLogin(username, password)) {
        cookieCache = WRandom::generateId();
        VidaApp* app = getApp();
        app->setCookie(cookieName, cookieCache, 60*60*24*365, "", "/", true); // TODO: set secure based on settings
        sessions.login(username, cookieCache);  // Record that they're logged in for other SessionHandles to find
        return true;
    }
    return false;
}

/**
* @brief Logs them out of all sessions
*/
void SessionHandle::logout() {
    userManager.logout();
    sessions.logout(getCookie());
}

/**
* @brief Returns the username of the logged in user, or empty string if no-one is logged in
*/
string SessionHandle::username() {
    // Check the session thing live .. this will possibly slow things down by locking if another thread is writing
    // but on the plus side it means if one thread los out .. all threads log out
    string username = "";
    string cookie = getCookie();
    if (!cookie.empty()) {
        username = sessions.username(cookie);
        if (username.empty()) {
            // If they have an old session cookie, but they're not logged in now .. delete the old cookie
            getApp()->setCookie(cookieName, cookie, 0, "", "/", true);
            cookieCache = "";
        }
    }
    return username;
}


/**
* @brief Call to let us know that the user is still logged in. Doesn't cost much.
*/
void SessionHandle::touchSession() {
    if (touchSessionsTimer == 0) {
        touchSessionsTimer = new WTimer(this);
        touchSessionsTimer->setSingleShot(true);
        touchSessionsTimer->setInterval(sessions.getTimeout());
    }
}

/**
* @brief Called when the timer runs out. Touches the session to let the store know it's still alive.
*/
void SessionHandle::onTouchSessionsActivate() {
    sessions.touch(getCookie());
    delete touchSessionsTimer;
    touchSessionsTimer = 0;
}

} // namespace vidanueva
