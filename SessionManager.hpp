/*
 * =====================================================================================
 *
 *       Filename:  SessionManager.hpp
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

#include <string>
#include <c_time>
#include <map>
#include <boost/thread/mutex.hpp>

using std::string;
using std::time_t;
using std::map;
using boost::UpgradeLockable;

namespace vidanueva {

class Session;

/**
* @brief Handles logged in users
*/
class SessionManager {
private:
   typedef map<Session>::iterator PSession;
   UpgradeLockable _lock; // Many can read session info .. but only one can write it
   unsigned long _timeout; /// How long a single session lasts
   map<Session> _loggedInUsers; /// Holds data for logged in users. Key is the cookie.
public:
    SessionManager(unsigned long timeout) : _timeout(timeout); /// timeout for logged in session in msecs
    ~SessionManager();
    void login(const string& username, const string& ip, const string& cookie);
    bool isLoggedIn(const string& cookie);
    bool logout(const string& cookie);
};

} // namespace vidanueva
