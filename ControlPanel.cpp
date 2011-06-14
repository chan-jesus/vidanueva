/*
 * =====================================================================================
 *
 *       Filename:  ControlPanel.cpp
 *
 *    Description:  Control Panel code goes here
 *
 *        Version:  1.0
 *        Created:  06/03/2011 09:39:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "ControlPanel.hpp"
#include "PageEdit.hpp"
#include <Wt/WDialog>
#include "App.hpp"

using Wt::WDialog;

namespace vidanueva {

const string ControlPanel::addPageURL = "/newPage";

/**
* Called when the user hits the add page button. Shows the add a new page dialog.
*/
void ControlPanel::addNewPage() {
    PageEdit* pageEdit = new PageEdit();
    getApp()->mainWindow()->setBody(pageEdit);
}

/**
* @brief If someone navigates to /page/new .. show a new page dialog.
*
* @param path
*/
void ControlPanel::urlChanged(const std::string& path) {
    if (path == addPageURL) {
        addNewPage();
    }
}

} // namespace vidanueva
