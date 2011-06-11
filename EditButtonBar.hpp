/*
 * =====================================================================================
 *
 *       Filename:  EditButtonBar.hpp
 *
 *    Description:  Gives you save/add/cancel buttons in a nice row.
 *
 *        Version:  1.0
 *        Created:  06/04/2011 09:49:11 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef EDIT_BUTTON_BAR_HPP
#define EDIT_BUTTON_BAR_HPP

#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WSignal>
#include <Wt/WMessageBox>
#include "App.hpp"
#include "MoreAwesomeTemplate.hpp"

using Wt::WPushButton;
using Wt::WDialog;
using Wt::WMessageBox;

namespace vidanueva {

/**
* @brief A nice 'Save'/'Create' 'Cancel' button pair.
*
* Cancel will navigate the app back to "/".
* OK will trigger the 'onOk' event
*/
class EditButtonBar : public MoreAwesomeTemplate {
private:
    WPushButton* _okBtn;
    WPushButton* _cancelBtn;
    Wt::Signal<>* _okHit;
    /// Handles when cancel button is hit. Prompts the user, then navigates back to home page
    void handleCancel() {
        Wt::StandardButton result = WMessageBox::show(tr("Discard Edits"), tr("you-want-discard"), Wt::Ok | Wt::Cancel);
        if (result == Wt::Ok)
            getApp()->goHome();
    }
    /// Handles when ok button is hit. Prompts the user, then triggers the okHit() signal
    void handleOk() {
        Wt::StandardButton result = WMessageBox::show(tr("Save Changes"), tr("you-want-save"), Wt::Ok | Wt::Cancel);
        if (result == Wt::Ok) {
            _okHit->emit();
        }
    }
public:
    /**
    * @brief Gives you a nice button bar for saving a new or existing resource
    *
    * @param parent Parent Widget
    */
    EditButtonBar(WContainerWidget* parent=0) : MoreAwesomeTemplate(parent) {
        _okHit = new Wt::Signal<>(this);
        setTemplateText(tr("edit-button-bar"));
        bindAndCreateWidget(_okBtn, "ok-btn", tr("Save"));
        bindAndCreateWidget(_cancelBtn, "cancel-btn", tr("cancel-btn"));
        _okBtn->clicked().connect(this, &EditButtonBar::handleOk);
        _cancelBtn->clicked().connect(this, &EditButtonBar::handleCancel);
    }
    /**
    * @brief Connect to this event for when the OK button is hit and confirmed
    */
    Wt::Signal<>* okHit() { return _okHit; }
};

}

#endif // EDIT_BUTTON_BAR_HPP
