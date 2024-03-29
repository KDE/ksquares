/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "newgamedialog.h"
#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent)
{
  auto mainWidget = new QWidget(this);
  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);
  mainLayout->addWidget(mainWidget);
  setupUi(mainWidget);
  auto buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
  okButton->setDefault(true);
  okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &NewGameDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &NewGameDialog::reject);
  mainLayout->addWidget(buttonBox);
  setWindowTitle(i18nc("@title:window", "New Game"));
  connect(spinNumOfPlayers,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
          &NewGameDialog::adjustEnabledUsers);

  adjustEnabledUsers(spinNumOfPlayers->value());
}

void NewGameDialog::adjustEnabledUsers(int numOfPlayers)
{
    switch (numOfPlayers) {
    case 2:
        labelPlayer3Name->setEnabled(false);
        playerThreeName->setEnabled(false);
        playerThreeHuman->setEnabled(false);
        [[fallthrough]];
    case 3:
        labelPlayer4Name->setEnabled(false);
        playerFourName->setEnabled(false);
        playerFourHuman->setEnabled(false);
        [[fallthrough]];
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
        [[fallthrough]];
    case 3:
        labelPlayer3Name->setEnabled(true);
        playerThreeName->setEnabled(true);
        playerThreeHuman->setEnabled(true);
        [[fallthrough]];
    case 2:
    default:
        break;
    }
}

#include "moc_newgamedialog.cpp"
