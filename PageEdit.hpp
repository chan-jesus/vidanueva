/*
 * =====================================================================================
 *
 *       Filename:  PageEdit.hpp
 *
 *    Description:  Window for editing pages
 *
 *        Version:  1.0
 *        Created:  06/03/2011 09:40:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef PAGE_EDIT_HPP
#define PAGE_EDIT_HPP

#include "MoreAwesomeTemplate.hpp"

namespace Wt {
    class WLineEdit;
    class WLabel;
    class WTextEdit;
    class WDialog;
}

using Wt::WLineEdit;
using Wt::WLabel;
using Wt::WTextEdit;
using Wt::WDialog;

namespace vidanueva {

class EditButtonBar;

class PageEdit : public MoreAwesomeTemplate {
private:
    WLabel*    _titleLabel;
    WLineEdit* _titleEdit;
    WLabel*    _nameLabel;
    WLineEdit* _nameEdit;
    WLabel*    _bodyLabel;
    WTextEdit* _bodyEdit;
    EditButtonBar* _buttons;
    void setUpWidgets();
public:
    PageEdit(WContainerWidget* parent=0);
    PageEdit(WDialog& dialog);
    void saveChanges();
};

}

#endif // PAGE_EDIT_HPP
