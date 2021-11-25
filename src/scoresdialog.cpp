/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "scoresdialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

ScoresDialog::ScoresDialog(QWidget *parent)
    : QDialog(parent)
{
  auto topLayout = new QVBoxLayout;
  setLayout(topLayout);

  auto w = new QWidget;
  topLayout->addWidget(w);

  auto mainLayout = new QVBoxLayout;
  w->setLayout(mainLayout);
  auto mainWidget = new QWidget;
  setupUi(mainWidget);
  mainLayout->addWidget(mainWidget);

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  mainLayout->addWidget(buttonBox);

}

