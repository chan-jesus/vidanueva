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

#include <string>
#include <Wt/WTemplate>

namespace Wt {
    class WContainerWidget;
    class WString;
}

using Wt::WTemplate;
using Wt::WContainerWidget;
using Wt::WString;

namespace vidanueva {

    // Forward declartations
    class VidaApp;

    // The MainWindow Class
    class MainWindow : public WTemplate {
    private:
        const static std::string pageBaseURL;
        const static std::string loginURL;
    public:
        MainWindow(WContainerWidget* parent=0);
        void refresh();
        void onUserChanged(VidaApp* app);
        void setStatusText(const WString& newMessage) { bindString("status-text", newMessage); }
        void setBody(WWidget* newChild) { bindWidget("content", newChild); }
        void onPathChange(const std::string& path);
    };

} // namespace vidavnueva

#endif // MAIN_WINDOW_HPP

