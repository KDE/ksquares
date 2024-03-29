/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <KAboutData>
#include <KCrash>
#include <KUser>
#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <KDBusService>

#include "ksquareswindow.h"
#include "ksquaresdemowindow.h"
#include "settings.h"
#include "ksquares_version.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("ksquares"));

    KAboutData about(QStringLiteral("ksquares"), i18n("KSquares"),
                     QStringLiteral(KSQUARES_VERSION_STRING),
                     i18n("Take it in turns to draw lines.\nIf you complete a squares, you get another go."),
                     KAboutLicense::GPL,
                     i18n("(C) 2006-2007 Matt Williams"),
                     QString(),
                     QStringLiteral("https://apps.kde.org/ksquares"));
    about.addAuthor(i18n("Matt Williams"), i18n("Original creator and maintainer"), QStringLiteral("matt@milliams.com"), QStringLiteral("https://milliams.com"));
    about.addCredit(i18n("Fela Winkelmolen"), i18n("Many patches and bugfixes"));
    about.addCredit(i18n("Tom Vincent Peters"), i18n("Hard AI"));

    KAboutData::setApplicationData(about);

    KCrash::initialize();

    QCommandLineParser parser;
    parser.addOption(QCommandLineOption(QStringList() <<  QStringLiteral("demo"), i18n("Run game in demo (autoplay) mode")));
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    KDBusService service;

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ksquares")));

    // default names for players
    KConfigGroup cg(KSharedConfig::openConfig(), QStringLiteral("General"));
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
      auto demoWindow = new KSquaresDemoWindow;
      demoWindow->show();
      demoWindow->gameNew();
    } else {
      auto mainWindow = new KSquaresWindow;
      mainWindow->show();
    }

    return app.exec();
}
