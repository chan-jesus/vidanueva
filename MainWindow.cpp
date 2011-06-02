/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.cpp
 *
 *    Description:  Main Window code
 *
 *        Version:  1.0
 *        Created:  06/01/2011 02:49:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "MainWindow.hpp"
#include <Wt/WAnchor>

using Wt::WAnchor;

namespace vidanueva {

/**
* @brief Builds up the display and widgets
*
* @param parent Parent widget (probably WApplicaion::root())
*/
MainWindow::MainWindow(WContainerWidget* parent) : WTemplate(parent) {
    setTemplateText(tr("main-template"));
    WAnchor* login = new WAnchor();
    login->setRefInternalPath("/login");
    login->setText("LOGIN!!!!");
    bindWidget("content", login);
    bindString("nav", "NAV HERE");
}

} // namespace vidanueva {
