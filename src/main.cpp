/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include <KDebug>

#include "ksquareswindow.h"
#include "ksquaresdemowindow.h"

static const char description[] =
	I18N_NOOP("Take it in turns to draw lines.\nIf you complete a squares, you get another go.");

static const char version[] = "0.3";

int main(int argc, char **argv)
{
	KAboutData about("ksquares", 0, ki18n("KSquares"), version, ki18n(description),
		KAboutData::License_GPL, ki18n("(C) 2006-2007 Matt Williams"), KLocalizedString(), 
		"http://milliams.com/content/view/18/42/");
	about.addAuthor( ki18n("Matt Williams"), ki18n("Original creator and maintainer"), "matt@milliams.com", "http://milliams.com" );
	about.addCredit(ki18n("Fela Winkelmolen"), ki18n("Many patches and bugfixes"));
	
	KCmdLineArgs::init(argc, argv, &about);

	KCmdLineOptions options;
	options.add("demo", ki18n("Run game in demo (autoplay) mode"));
	KCmdLineArgs::addCmdLineOptions( options );
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	
	KApplication app;
	KGlobal::locale()->insertCatalog("libkdegames");
	
	if (args->isSet("demo"))
	{
		KSquaresDemoWindow *demoWindow = new KSquaresDemoWindow;
		demoWindow->show();
		demoWindow->gameNew();
	}
	else
	{
		KSquaresWindow *mainWindow = new KSquaresWindow;
		mainWindow->show();
	}
	args->clear();
	
	return app.exec();
}
