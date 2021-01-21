/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include "ui_newgamewidget.h"

#include <QDialog>

class NewGameDialog : public QDialog, public Ui::NewGameWidget
{
    Q_OBJECT
public:
    explicit NewGameDialog(QWidget *parent = nullptr);

public Q_SLOTS:
    void adjustEnabledUsers(int);
};

#endif // NEWGAMEDIALOG_H
