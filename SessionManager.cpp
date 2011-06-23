/*
 * =====================================================================================
 *
 *       Filename:  SessionManager.cpp
 *
 *    Description:  G
 *
 *        Version:  1.0
 *        Created:  06/22/2011 09:00:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "SessionManager.hpp"

using boost::thread::lock_guard;
using boost::thread::shared_lock;

namespace vidanueva {

/**
* Info about a single logged in session ..
* Only a single copy of this object exists at one time, and thread locking is handled by
* its container .. SessionManager
*/
class Session {
private:
    string _username;
    time_t _loginTime;
    string _ip;
    string _cookieContents;
public:
    Session() {};
    Session(const string& username, const string& ip, const string& cookie) :
      _username(username), _loginTime(), _ip(ip), _cookieContents(cookie) {}
    const string& username() const { return _username; };
    const time_t& loginTime() const { return _loginTime; };
    const string& ip() const { return _ip; };
    const string& cookieContents() const { return _cookieContents; };
};

void SessionManager::login(const string& username, const string& ip, const string& cookie) {
  lock_guard lock(_lock);
  sessions.insert(sessions::value_type(cookie, Session(username, ip, cookie)));
}

/**
* @brief Returns true if the cookie passed is for a logged in user
*
* @param cookie The cookie that the user is giving us
*
* @return true if cookie is of a logged in user
*/
bool SessionManager::isLoggedIn(const string& cookie) {
  shared_lock lock(_lock);
  PSession pSession = sessions.find(cookie);
  if (pSession != sessions::end()) {
      // Check that it hasn't timed out
      time_t now = time(NULL);
      Session& session = (*pSession).second;
      if ((now - session.loginTime()) < _timeout) {
          // Cookie is expired - delete it and return result
          sessions.erase(cookie) == 1;

      }
  }
  return false; // Cookie not found
}

/**
* @brief Logs a user out
*
* @param cookie of the logged in user
*
* @return Returns true if logged the user out
*/
bool SessionManager::logout(const string& cookie) {
  guard_lock lock(_lock);
  return sessions.erase(cookie) == 1;
}

SessionManager::~SessionManager() {
  // Delete all the sessions but while locked
  guard_lock lock(_lock);
  sessions.erase(sessions.begin(), sessions.end());
}


} // namespace vidanueva
