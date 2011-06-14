/*
 * =====================================================================================
 *
 *       Filename:  Page.hpp
 *
 *    Description:  Helper class for loading and saving pages. Bridges mongodb and witty.
 *
 *        Version:  1.0
 *        Created:  06/03/2011 09:41:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef PAGE_HPP
#define PAGE_HPP

#include "MoreAwesomeTemplate.hpp"
#include <mongo/client/connpool.h>

using mongo::BSONObj;

namespace vidanueva {

// Forward declarations
class VidaApp;

/**
* @brief Holds the data from the DB for a page
*/
class PageData {
private:
    string _name;
    string _title;
    string _body;
public:
    PageData (const string& name, const string& title, const string& body) :
        _name(name), _title(title), _body(body) {}
    PageData (const BSONObj& raw) :
        _name(raw["name"].String()),
        _title(raw["title"].String()),
        _body(raw["body"].String()) {}
    BSONObj toBSON() const;
    const string& name() const {return _name;}
    const string& title() const {return _title;}
    const string& body() const {return _body;}
};

/**
* @brief This holds an actual page widget and its data.
*
*
*/
class Page : public MoreAwesomeTemplate {
public:
    Page(PageData& data) : MoreAwesomeTemplate() {
        setTemplateText("page-template");
        bindString("title", data.title());
        bindString("body", data.body());
    }
};

/**
* @brief Represents a cache entry for a Page
*/
class PageCacheEntry {
private:
    PageData _data;
    unsigned long _hits;
    time_t _lastHit;
public:
    PageCacheEntry(const string& name, const string& title, const string& body) : _data(name, title, body), _hits(1), _lastHit(time(NULL)) {}
    PageCacheEntry(const BSONObj& raw) : _data(raw), _hits(0), _lastHit(time(NULL)) {}
    void hit() {
        _lastHit = time(NULL);
        _hits += 1;
    }
    unsigned long hits() {return _hits;}
    time_t lastHit() {return _lastHit;}
    Page* createPage() {return new Page(_data);}
    PageData& data() {return _data;}
};

/**
* @brief Creates, loads and caches pages
*/
class PageFactory {
private:
    typedef map<std::string, PageCacheEntry> CacheType;
    VidaApp* app;
    CacheType cache;
protected:
public:
    PageFactory(VidaApp* app) : app(app) {}
    static Page* LoadFromDB(const string& name);
    Page* load(const string& name);
    void save(const PageData& data);
};

} // namespace vidanueva

#endif // PAGE_HPP
