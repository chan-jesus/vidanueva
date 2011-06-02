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

#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include<Wt/WContainerWidget>
#include<Wt/WPushButton>
#include"App.hpp"

using Wt::WContainerWidget;
using Wt::WPushButton;

namespace vidanueva {

class ControlPanel : public WContainerWidget {
private:
    WPushButton* _addPageBtn;
public:
    ControlPanel(WContainerWidget* parent=0) : WContainerWidget(parent) {
        VidaApp* app = getApp();
        setId("control-panel");
        _addPageBtn = new WPushButton(tr("add-page"), this);
    }
};


} // namespace vidanueva

#endif // CONTROLS_HPP
