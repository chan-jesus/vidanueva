/*
 * =====================================================================================
 *
 *       Filename:  DialogButtonBar.hpp
 *
 *    Description:  Gives you an OK and cancel button to sit at the bottom of a dialog
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

#ifndef DIALOG_BUTTON_BAR_HPP
#define DIALOG_BUTTON_BAR_HPP

#include <Wt/WDialog>
#include <Wt/WPushButton>
#include "MoreAwesomeTemplate.hpp"

using Wt::WPushButton;
using Wt::WDialog;

namespace vidanueva {

class DialogButtonBar : public MoreAwesomeTemplate {
private:
    WPushButton* _okBtn;
    WPushButton* _cancelBtn;
public:
    DialogButtonBar(WDialog& dialog) : MoreAwesomeTemplate(dialog.contents()) {
        setTemplateText(tr("dialog-button-bar"));
        bindAndCreateWidget(_okBtn, "ok-btn", tr("ok-btn"));
        bindAndCreateWidget(_cancelBtn, "cancel-btn", tr("cancel-btn"));
        _okBtn->clicked().connect(&dialog, &WDialog::accept);
        _cancelBtn->clicked().connect(&dialog, &WDialog::reject);
    }
};

}

#endif // DIALOG_BUTTON_BAR_HPP
