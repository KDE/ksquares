/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ksquareswindow.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
/*#include <khighscore.h>
#include <kexthighscore.h>*/

#include <kdebug.h>

static const char description[] =
    I18N_NOOP("Take it in turns to draw lines.\nIf you complete a squares, you get another go.");

static const char version[] = "0.3";

int main(int argc, char **argv)
{
	KAboutData about("ksquares", I18N_NOOP("KSquares"), version, description,
			 KAboutData::License_GPL, "(C) 2006-2007 Matt Williams", 0, 
			 "http://milliams.com/content/view/18/42/", "matt@milliams.com");
	about.addAuthor( "Matt Williams", I18N_NOOP("Original creator and maintainer"), "matt@milliams.com", "http://milliams.com" );
	KCmdLineArgs::init(argc, argv, &about);
	
	KApplication app;
	
	/*KHighscore::init("ksquares");
	KExtHighscore::Manager highscoresManager;*/
	
	KSquaresWindow *mainWindow = new KSquaresWindow;
	mainWindow->show();
	mainWindow->gameNew();
	
	return app.exec();
}
