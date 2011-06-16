/*
 * =====================================================================================
 *
 *       Filename:  UserManager.hpp
 *
 *    Description:  Handles users and permssions
 *
 *        Version:  1.0
 *        Created:  06/01/2011 04:10:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include <string>

using std::string;

namespace vidanueva {

    class UserManager {
    private:
        string _hostname;
        string _namespace;
        void ensureIndex();
    public:
        void configure(const string& hostname, const string& dbName, const string& tableName="users");
        bool checkLogin(const string& username, const string& password);
        void savePassword(const string& username, const string& password);
    };

} // namespace vidanueva



#endif //  USER_MANAGER_HPP

