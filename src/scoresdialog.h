/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SCORESDIALOG_H
#define SCORESDIALOG_H

#include "ui_scoresdialog.h"

class ScoresDialog : public QDialog, public Ui::ScoresDialog
{
	Q_OBJECT
	
	public:
		ScoresDialog(QWidget* parent = 0);
	public slots:
};

#endif // SCORESDIALOG_H
