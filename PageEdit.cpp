/*
 * =====================================================================================
 *
 *       Filename:  PageEdit.cpp
 *
 *    Description:  Code for editing the web pages
 *
 *        Version:  1.0
 *        Created:  06/03/2011 09:44:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "PageEdit.hpp"
#include "App.hpp"
#include <string>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include <Wt/WTextEdit>
#include <Wt/WDialog>
#include "EditButtonBar.hpp"

using std::string;
using Wt::WDialog;
using mongo::BSONObj;

namespace vidanueva {

PageEdit::PageEdit(WContainerWidget* parent) : MoreAwesomeTemplate(parent) {
    setUpWidgets();
}

PageEdit::PageEdit(WDialog& dialog) : MoreAwesomeTemplate(dialog.contents()) {
    setUpWidgets();
    _nameEdit->escapePressed().connect(&dialog, &WDialog::reject);
    _titleEdit->escapePressed().connect(&dialog, &WDialog::reject);
}

void PageEdit::setUpWidgets() {
    setTemplateText(tr("page-edit-template"));
    setStyleClass("form");
    bindAndCreateField(_titleLabel, _titleEdit, "title");
    bindAndCreateField(_nameLabel, _nameEdit, "name");
    bindAndCreateField(_bodyLabel, _bodyEdit, "body");
    bindWidget("buttons", _buttons = new EditButtonBar());
    _buttons->okHit()->connect(this, &PageEdit::saveChanges);
    _nameEdit->setFocus();
}

/**
* @brief Saves any changes made to the DB then navigates to the page
*/
void PageEdit::saveChanges() {
    VidaApp* app = getApp();
    string name =_nameEdit->text().toUTF8();
    PageData data(name, _titleEdit->text().toUTF8(), _bodyEdit->text().toUTF8());
    app->pages().save(data);
    app->setInternalPath("/page/" + name, true); // Navigate to the new page
    app->mainWindow()->setStatusText(tr("Page Saved")); // Update the status text
}


}
