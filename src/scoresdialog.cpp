/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "scoresdialog.h"
#include <QDialogButtonBox>
#include <QPushButton>


ScoresDialog::ScoresDialog(QWidget* parent)
 : QDialog(parent)
{
      QVBoxLayout *topLayout = new QVBoxLayout;
      setLayout(topLayout);

      QWidget *w = new QWidget;
      topLayout->addWidget(w);

      QVBoxLayout *mainLayout = new QVBoxLayout;
      w->setLayout(mainLayout);
      QWidget *mainWidget = new QWidget;
      setupUi(mainWidget);
      mainLayout->addWidget(mainWidget); 

      QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
      connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
      connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
      mainLayout->addWidget(buttonBox);

}

