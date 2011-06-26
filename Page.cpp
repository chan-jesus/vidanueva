/*
 * =====================================================================================
 *
 *       Filename:  Page.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/11/2011 03:59:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Page.hpp"
#include "App.hpp"
#include <string>
#include <Wt/WString>
#include <mongo/client/connpool.h>

using std::string;
using mongo::DBClientCursor;
using mongo::ScopedDbConnection;

namespace vidanueva {

/**
* @brief Returns a new BSON Obj for the page data
*
* @return A new BSON obj for putting into the DB
*/
BSONObj PageData::toBSON() const {
    return BSON(
        "name" << name() <<  // TODO: Put an index on name later
        "title" << title() <<
        "body" << body());
}

/**
* @brief Loads a page, uses the cache if possible
*
* @param url
*
* @return 
*/
Page* PageFactory::load(const string& name) {
    string realName = name;
    if (!name.empty() and name[0] == '/') {
        realName = name.substr(1); // Remove the '/' at the front
    }
    VidaApp* app = getApp();
    Page* result = 0;
    // Check the cache
    CacheType::iterator hit = cache.find(realName);
    if (hit == cache.end()) {
        // Not found, Load it from the DB
        ScopedDbConnection db(app->mongoHostname());
        auto_ptr<DBClientCursor> cursor = db->query(app->mongoNSFor("page"), BSON( "name" << realName ), 1);
        if (cursor->more()) {
            BSONObj data = cursor->next();
            // For now, just put it into the cache. TODO: Have a max cache size and do some clever cache managagement
            CacheType::value_type newVal(name, PageCacheEntry(data));
            result = cache.insert(newVal).first->second.createPage();
        } else {
            // Return a 404 page .. will create it if not in cache already. Magic :D
            if (realName != "404") {
                result = load("404");
            } else {
                // OK we're being asked to return a 404 because we don't have  404 page .. make up a default one
                CacheType::value_type newVal("404", PageCacheEntry("404", WString::tr("page-404-title").toUTF8(), WString::tr("page-404-body").toUTF8()));
                result = cache.insert(newVal).first->second.createPage();
            }
        }
        db.done();
    } else {
        result = hit->second.createPage();
    }
    return result;
}

/**
* @brief Save some page data into the DB
*
* @param name The name of the page. Used in the url.
* @param title The title of the page
* @param body The contents of the page body
*/
void PageFactory::save(const PageData& data) {
    BSONObj raw = data.toBSON();
    BSONObj query = BSON( "name" << data.name() );
    getApp()->mongoSave("page", query, raw);
}

} //namespace vidanueva
