/*
 * =====================================================================================
 *
 *       Filename:  UserManager.cpp
 *
 *    Description:  Manages user logins using mong DB for storage
 *
 *        Version:  1.0
 *        Created:  06/01/2011 10:57:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "UserManager.hpp"
#include <mongo/client/connpool.h>
#include <openssl/sha.h>

using mongo::ScopedDbConnection;
using mongo::DBClientCursor;
using mongo::BSONObj;
using mongo::BSONArray;
using mongo::BSONElement;
using mongo::BSONArrayBuilder;

namespace vidanueva {

const int SHALength = 20;
typedef unsigned char SHAValue[SHALength];

/**
* @brief c++ version of SHA1. Pass input string, get a hash.
*
* @param input the string to have sha run on
* @param value auto pointer to the SHAValue
*/
void sha1(const std::string input, SHAValue& value) {
    SHA1(reinterpret_cast<const unsigned char*>(&input[0]), input.length(), &(value[0])); // Make a hash of the password supplied
}

void UserManager::ensureIndex() {
    ScopedDbConnection db(_hostname);
    db->ensureIndex(_namespace, BSON( "username" << 1 ));
}

void UserManager::configure(const string& hostname, const string& dbName, const string& tableName) {
    _hostname = hostname;
    _namespace = dbName + "." + tableName;
}

/**
* @brief Returns true if the username and password match are in the DB
*
* @param username the username being used to log in
* @param password the password being used to log in
*
* @return True if db has a match
*/
bool UserManager::checkLogin(const string& username, const string& password) {
    ScopedDbConnection db(_hostname);
    auto_ptr<DBClientCursor> cursor = db->query(_namespace, BSON( "username" << username ), 1);
    if (cursor->more()) {
        BSONObj data = cursor->next();
        db.done();
        // Compare the sha1 hash from the DB
        vector<BSONElement> dbPassHash = data["hash"].Array();
        SHAValue suppliedHash;
        sha1(password, suppliedHash);
        // Compare
        for(int i=0; i < SHALength; i++) {
            if (reinterpret_cast<const unsigned char*>(dbPassHash[i].value())[0] != suppliedHash[i])
                return false;
        }
        return true;
    }
    db.done();
    return false; // Not a valid login
}


/**
* @brief Adds or updates the password of a new user in the table
*
* @param username username to update/insert
* @param password new password
*/
void UserManager::savePassword(const string& username, const string& password) {
    // Make an SHA hash of the password we are saving to the DB
    SHAValue passHash;
    sha1(password, passHash);
    BSONArrayBuilder shaArrayBuilder;
    for(int i=0; i < SHALength; i++) {
        shaArrayBuilder.append(passHash[i]);
    }
    BSONArray passHashArray = shaArrayBuilder.arr();
    BSONObj newData = BSON( "username" << username << "hash" << passHashArray );
    ScopedDbConnection db(_hostname);
    db->update(_namespace, BSON( "username" << username ), newData, true);
    db.done();
}

}
