/***************************************************************************
 *   Copyright (C) 2006 by Matthew Williams    <matt@milliams.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <KAboutData>
#include <KCrash>
#include <KUser>
#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <kdelibs4configmigrator.h>
#include <KDBusService>

#include "ksquareswindow.h"
#include "ksquaresdemowindow.h"
#include "settings.h"

static const char description[] =
    I18N_NOOP("Take it in turns to draw lines.\nIf you complete a squares, you get another go.");

static const char version[] = "0.6";

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Kdelibs4ConfigMigrator migrate(QStringLiteral("ksquares"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("ksquaresrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("ksquaresui.rc"));
    migrate.migrate();
    KLocalizedString::setApplicationDomain("ksquares");
    KAboutData about(QStringLiteral("ksquares"), i18n("KSquares"), QLatin1String(version), i18n(description),
                     KAboutLicense::GPL, i18n("(C) 2006-2007 Matt Williams"));
    about.addAuthor(i18n("Matt Williams"), i18n("Original creator and maintainer"), QStringLiteral("matt@milliams.com"), QStringLiteral("http://milliams.com"));
    about.addCredit(i18n("Fela Winkelmolen"), i18n("Many patches and bugfixes"));
    about.addCredit(i18n("Tom Vincent Peters"), i18n("Hard AI"));
    about.setHomepage(QStringLiteral("http://games.kde.org/ksquares"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(about);
    KCrash::initialize();
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("demo"), i18n("Run game in demo (autoplay) mode")));

    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);
    KDBusService service;

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ksquares")));

    // default names for players
    KConfigGroup cg(KSharedConfig::openConfig(), "General");
    if (cg.readEntry<bool>("initializeNames", true)) {
        QStringList playerNames;
        playerNames << KUser().property(KUser::FullName).toString();
        playerNames << i18nc("default name of player", "Player %1", 2);
        playerNames << i18nc("default name of player", "Player %1", 3);
        playerNames << i18nc("default name of player", "Player %1", 4);
        Settings::setPlayerNames(playerNames);
        cg.writeEntry("initializeNames", false);
    }

    if (parser.isSet(QStringLiteral("demo"))) {
        KSquaresDemoWindow *demoWindow = new KSquaresDemoWindow;
        demoWindow->show();
        demoWindow->gameNew();
    } else {
        KSquaresWindow *mainWindow = new KSquaresWindow;
        mainWindow->show();
    }

    return app.exec();
}
