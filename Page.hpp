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

namespace vidanueva {

class Page {
public:
    void save();
    static load(const string& url);
}

}

#endif // PAGE_HPP
