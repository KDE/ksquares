/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include "ui_newgamedialog.h"

class NewGameDialog : public QDialog, public Ui::NewGameDialog
{
	public:
		explicit NewGameDialog(QWidget* parent = 0) : QDialog(parent) {setupUi(this);}
};

#endif // NEWGAMEDIALOG_H
