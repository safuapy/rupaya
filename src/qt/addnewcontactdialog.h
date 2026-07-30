// Copyright (c) 2019-2020 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RUPX_QT_ADDNEWCONTACTDIALOG_H
#define RUPX_QT_ADDNEWCONTACTDIALOG_H

#include "focuseddialog.h"

namespace Ui {
class AddNewContactDialog;
}

class AddNewContactDialog : public FocusedDialog
{
    Q_OBJECT

public:
    explicit AddNewContactDialog(QWidget *parent = nullptr);
    ~AddNewContactDialog();

    void setTexts(QString title, const char* message = nullptr);
    void setData(QString address, QString label);
    void showEvent(QShowEvent *event) override;
    QString getLabel();

    bool res = false;

public Q_SLOTS:
    void accept() override;
private:
    Ui::AddNewContactDialog *ui;
    const char* message = nullptr;
};

#endif // RUPX_QT_ADDNEWCONTACTDIALOG_H
