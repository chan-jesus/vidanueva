/*
 * =====================================================================================
 *
 *       Filename:  setupDB.cpp
 *
 *    Description:  Holds the function that sets up the mongo database for a new church website
 *
 *        Version:  1.0
 *        Created:  06/16/2011 08:45:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <mongo/client/connpool.h>
#include <mongo/bson/bsonmisc.h>
#include <cstring>
#include <string>
#include <iostream>
#include "UserManager.hpp"

using mongo::ScopedDbConnection;
using std::string;
using std::cout;
using std::endl;

namespace vidanueva {

void setupDB(string mongoHostname, string mongoDBName, string adminPassword) {
    // Create an admin user
    UserManager users;
    users.configure(mongoHostname, mongoDBName, "users");
    users.savePassword("admin", adminPassword);
    // Create the welcome page and the 404 
    ScopedDbConnection db(mongoHostname);
    // Welcome/home page
    db->insert(mongoDBName + ".page",
        BSON( mongo::GENOID <<
            "name" << "" <<
            "title" << "Welcome to my church website" <<
            "body" << "<h1>Sample Start Page</h1><p>This is a sample page. Just login and click edit.</p>"));
    // 404 page
    db->insert(mongoDBName + ".page",
        BSON( mongo::GENOID <<
            "name" << "404" <<
            "title" << "404 Page not found" <<
            "body" << "<h1>Sorry we couldn't find that page</h1><p>Maybe try just <a href=\"/\">navigating to the start</a>.</p>"));
    // Set up all the indexes necessary
    db->ensureIndex(mongoDBName + ".users", BSON("username" << 1));
    db->ensureIndex(mongoDBName + ".page", BSON("name" << 1));
    db.done();
}

} // namespace vidanueva

int main(int argc, char **argv) {
    // If they use: --setup mongoHostName mongoDBName adminPassword
    cout << "This program will do the initial setup on your mongodb" << endl;
    if ((argc == 5) && (std::strcmp(argv[1], "--setup") == 0)) {
        vidanueva::setupDB(argv[2], argv[3], argv[4]);
        cout << "Done" << endl;
    } else {
        cout << "Usage " << endl <<
            argv[0] << " --setup mongoHostName mongoDBName adminPassword" << endl <<
            "For example:" << endl <<
            argv[0] << " --setup localhost vidanueva bigSecret" << endl;
    }
}


