/*
 * =====================================================================================
 *
 *       Filename:  SessionStore.cpp
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

#include "SessionStore.hpp"


namespace vidanueva {

SessionStore* SessionStore::globalInstance = 0;
const unsigned int SessionStore::defaultTimeout = 1800; // If a user doesn't touch a session for 30 minutes .. they log out

/**
* @brief Just finds a session. Doesn't care about expiry time.
*
* @param lock A reference to a scoped lock. You must have one to call this func
* @param cookie The cookie for the session we're looking for
*
* @return a pointer to the session if found, or 0
*/
inline Session* SessionStore::findSession(ReadOnlyLock& /* lock */, const string& cookie) {
  PSession pSession = sessions.find(cookie);
  if (pSession != sessions.end()) {
      return &((*pSession).second);
  }
  return 0;
}

/**
* @brief Records that a user has logged in and a new session is started
*
* @param username the login of the user
* @param cookie the cookie that we stored on their machine
*/
void SessionStore::login(const string& username, const string& cookie) {
  FullLock lock(_lock);
  sessions.insert(SessionMap::value_type(cookie, Session(username)));
}

/**
* @brief Returns true if the cookie passed is for a logged in user
*
* @param cookie The cookie that the user is giving us
*
* @return true if cookie is of a logged in user
*/
inline bool SessionStore::isLoggedIn(const string& cookie) {
    return !username(cookie).empty();
}


/**
* @brief Logs a user out
*
* @param cookie of the logged in user
*
* @return Returns true if logged the user out
*/
bool SessionStore::logout(const string& cookie) {
  FullLock lock(_lock);
  return sessions.erase(cookie) == 1;
}

/**
* @brief Returns the username of the currently logged in user or "" if none
*
* @param cookie the cookie that the client browser gave us
* @param touch If true, updates the session time out
*
* @return Username
*/
string SessionStore::username(const string& cookie, bool touch) {
  ReadOnlyLock lock(_lock);
  Session* session = findSession(lock, cookie);
  if (session != 0) {
      if (session->age() < _timeout) {
          if (touch) {
              ReadWriteLock touchLock(_lock);
              // Session is still good, update its timeout on our end
              session->updateLoginTime();
          }
          return session->username();
      } else {
          // Cookie is expired - delete its session and return empty
          ReadWriteLock eraseLock(_lock);
          sessions.erase(cookie);
      }
  }
  return ""; // No username for that cookie
}

/**
* @brief Resets the server side timeout for this session
*/
void SessionStore::touch(const string& cookie) {
  ReadOnlyLock lock(_lock);
  Session* session = findSession(lock, cookie);
  if (session != 0) {
    ReadWriteLock bigLock(_lock);
    session->updateLoginTime();
  }
}

/**
* @brief Gets the write lock and purges all the old sessions
*/
void SessionStore::purgeOldSessions() {
    FullLock lock(_lock);
    for (PSession it = sessions.begin(); it != sessions.end();) {
        if ((*it).second.age() > _timeout) {
            sessions.erase(it++);
        } else {
            ++it;
        }
    }
}

} // namespace vidanueva
