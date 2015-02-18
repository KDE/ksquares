/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "newgamedialog.h"
#include <QDebug>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent)
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    setupUi(mainWidget);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewGameDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NewGameDialog::reject);
    //PORTING SCRIPT: WARNING mainLayout->addWidget(buttonBox) must be last item in layout. Please move it.
    mainLayout->addWidget(buttonBox);
    setWindowTitle(i18n("New Game"));
    connect(spinNumOfPlayers, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &NewGameDialog::adjustEnabledUsers);

    adjustEnabledUsers(spinNumOfPlayers->value());
}

void NewGameDialog::adjustEnabledUsers(int numOfPlayers)
{
    switch (numOfPlayers) {
    case 2:
        labelPlayer3Name->setEnabled(false);
        playerThreeName->setEnabled(false);
        playerThreeHuman->setEnabled(false);
    // no break!
    case 3:
        labelPlayer4Name->setEnabled(false);
        playerFourName->setEnabled(false);
        playerFourHuman->setEnabled(false);
    case 4:
        break;
    default:
        qCritical() << "NewGameDialog::adjustEnabledUsers(): numOfPlayers out of range!!";
    }

    switch (numOfPlayers) {
    case 4:
        labelPlayer4Name->setEnabled(true);
        playerFourName->setEnabled(true);
        playerFourHuman->setEnabled(true);
    case 3:
        labelPlayer3Name->setEnabled(true);
        playerThreeName->setEnabled(true);
        playerThreeHuman->setEnabled(true);
    case 2:
    default:
        break;
    }
}

