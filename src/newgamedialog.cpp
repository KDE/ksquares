/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "newgamedialog.h"
#include <kdebug.h>

NewGameDialog::NewGameDialog(QWidget* parent) : KDialog(parent)
{
	setupUi(mainWidget());
	setButtons(Cancel|Ok);
	setCaption(i18n("New Game"));
	connect(spinNumOfPlayers, SIGNAL(valueChanged(int)), SLOT(adjustEnabledUsers(int)));
	
	adjustEnabledUsers(spinNumOfPlayers->value());
}

void NewGameDialog::adjustEnabledUsers(int numOfPlayers)
{
	switch(numOfPlayers)
	{
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
			kError() << "NewGameDialog::adjustEnabledUsers(): numOfPlayers out of range!!";
	}
	
	switch(numOfPlayers)
	{
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


