/*
 * =====================================================================================
 *
 *       Filename:  Main.hpp
 *
 *    Description:  The main window for the vidanueva class
 *
 *        Version:  1.0
 *        Created:  06/01/2011 12:15:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <Wt/WTemplate>

namespace Wt {
    class WContainerWidget;
}

using Wt::WTemplate;
using Wt::WContainerWidget;

namespace vidanueva {

    class MainWindow : public WTemplate {
    public:
        MainWindow(WContainerWidget* parent=0) : WTemplate(parent) {
            setTemplateText(tr("main-template"));
            bindString("content", "CONTENT HERE");
            bindString("nav", "NAV HERE");
        }

    };
} // namespace vidavnueva

#endif // MAIN_WINDOW_HPP

