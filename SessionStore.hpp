/*
 * =====================================================================================
 *
 *       Filename:  SessionStore.hpp
 *
 *    Description:  An in memory, thread safe Session manager
 *                  Because witty's 'internalPathChanged' thing is not being too happy
 *                  with html5 api .. I'm having to manage logged in users on my own.
 *
 *        Version:  1.0
 *        Created:  06/22/2011 08:32:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef SESSION_STORE_HPP
#define SESSION_STORE_HPP

#include <string>
#include <ctime>
#include <map>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

using std::string;
using std::time_t;
using std::time;
using std::map;
using boost::shared_mutex;
using boost::lock_guard;
using boost::shared_lock;
using boost::upgrade_lock;

namespace vidanueva {

/**
* Info about a single logged in session ..
*/
class Session {
private:
    string _username;
    time_t _loginTime;
public:
    Session(const string& username) : _username(username), _loginTime(time(NULL)) {}
    const string& username() const { return _username; };
    const time_t& loginTime() const { return _loginTime; };
    void updateLoginTime() { _loginTime = time(NULL); }
    unsigned int age() { return time(NULL) - _loginTime; } /// Returns the last touched age of this session in seconds
};

/**
* @brief Handles logged in users.
*
* All this class provides is a thread safe 'cookie to username' map.
* It handles timeouts on the server side .. so you prolly wanna make your browser cookies quite long lived.
*/
class SessionStore {
public:
    static const unsigned int defaultTimeout;
    typedef map<string, Session> SessionMap;
    typedef SessionMap::iterator PSession;
    typedef shared_mutex StoreMutex;
    typedef shared_lock<StoreMutex> ReadOnlyLock;
    typedef lock_guard<StoreMutex> FullLock;
    typedef upgrade_lock<StoreMutex> ReadWriteLock;
private:
    static SessionStore* globalInstance;
    shared_mutex _lock; // Many can read session info .. but only one can write it
    // TODO: Make this atomic<unsigned int>
    unsigned long _timeout; /// How long a single session lasts 
    SessionMap sessions; /// Holds data for logged in users. Key is the cookie.
    Session* findSession(ReadOnlyLock& lock, const string& cookie);
    // structors
    SessionStore() : _timeout(defaultTimeout) {}
    ~SessionStore() {
        // Delete all the sessions but while locked
        lock_guard<shared_mutex> lock(_lock);
        sessions.erase(sessions.begin(), sessions.end());
    }
public:
    static SessionStore& getInstance() {
        if (globalInstance == 0)
            globalInstance = new SessionStore(); // Create one for life of whole app. Will be deleted when app ends.
        return *globalInstance;
    }
    unsigned long getTimeout() { return _timeout; } /// timeout in secs .. default is 5 mins .. hopefully this is atomic
    void setTimeout(unsigned long newTimeout) { _timeout = newTimeout; }
    void login(const string& username, const string& cookie);
    bool isLoggedIn(const string& cookie);
    bool logout(const string& cookie);
    void touch(const string& cookie);
    void purgeOldSessions();
    string username(const string& cookie, bool touch=false);
};

} // namespace vidanueva

#endif // SESSION_STORE_HPP
