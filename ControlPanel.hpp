/*
 * =====================================================================================
 *
 *       Filename:  ControlPanel.hpp
 *
 *    Description:  Class that shows the control panel for logged in users
 *
 *        Version:  1.0
 *        Created:  06/02/2011 10:22:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef CONTROL_PANEL_HPP
#define CONTROL_PANEL_HPP

#include<Wt/WContainerWidget>
#include<Wt/WAnchor>
#include<string>
#include"App.hpp"

using Wt::WContainerWidget;
using Wt::WAnchor;
using std::string;

namespace vidanueva {

class ControlPanel : public WContainerWidget {
public:
    static const string addPageURL;
private:
    WAnchor* _addPageLink;
public:
    ControlPanel(WContainerWidget* parent=0) : WContainerWidget(parent) {
        VidaApp* app = getApp();
        setId("control-panel");
        _addPageLink = new WAnchor(this);
        _addPageLink->setText(tr("add-page"));
        _addPageLink->setRefInternalPath(addPageURL);
        app->internalPathChanged().connect(this, &ControlPanel::urlChanged);
    }
    void addNewPage();
    void urlChanged(const std::string& path);
};


} // namespace vidanueva

#endif // CONTROL_PANEL_HPP
