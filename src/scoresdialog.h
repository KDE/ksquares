/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include "ui_scoreswidget.h"

#include <QDialog>

class ScoresDialog : public QDialog, public Ui::ScoresWidget
{
public:
    explicit ScoresDialog(QWidget *parent = nullptr);
};

#endif // SCORESDIALOG_H
