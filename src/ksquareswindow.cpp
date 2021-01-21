/*
    SPDX-FileCopyrightText: 2006 Matthew Williams <matt@milliams.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

//header
#include "ksquareswindow.h"

//qt
#include <QAction>
#include <QDebug>
#include <QLabel>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTimer>

//kde
#include <KConfigDialog>
#include <KActionCollection>
#include <KLocalizedString>
#include <KScoreDialog>
#include <KStandardGameAction>

//generated
#include "settings.h"

//classes
#include "aicontroller.h"
#include "gameboardview.h"

//ui
#include "newgamedialog.h"
#include "scoresdialog.h"

KSquaresWindow::KSquaresWindow() : KXmlGuiWindow(), m_view(new GameBoardView(this)), m_scene(nullptr)
{
    setCentralWidget(m_view);
    QTimer::singleShot(0, this, &KSquaresWindow::initObject);
}

KSquaresWindow::~KSquaresWindow()
{
    delete m_view;
    delete m_scene;
    delete sGame;
}

void KSquaresWindow::initObject()
{
    sGame = new KSquaresGame();
    connect(sGame, &KSquaresGame::takeTurnSig, this, &KSquaresWindow::playerTakeTurn);
    connect(sGame, &KSquaresGame::gameOver, this, &KSquaresWindow::gameOver);
    m_view->setRenderHints(QPainter::Antialiasing);
    m_view->setFrameStyle(QFrame::NoFrame);
    setupActions();
    m_player = new QLabel(i18n("Current Player"));
    statusBar()->addPermanentWidget(m_player);
    statusBar()->show();
    setAutoSaveSettings();

    gameNew();
}

void KSquaresWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name, this);
    ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Easy"), i18n("Easy")));
    ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Medium"), i18n("Medium")));
    ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Hard"), i18n("Hard")));
    ksdialog.exec();
}

void KSquaresWindow::gameNew()
{
    //load settings
    NewGameDialog dialog(this);

    //create indexed arrays of the widgets
    QList<QLineEdit *> nameLineEditList;
    nameLineEditList.append(dialog.playerOneName);
    nameLineEditList.append(dialog.playerTwoName);
    nameLineEditList.append(dialog.playerThreeName);
    nameLineEditList.append(dialog.playerFourName);
    QList<QCheckBox *> humanCheckBoxList;
    humanCheckBoxList.append(dialog.playerOneHuman);
    humanCheckBoxList.append(dialog.playerTwoHuman);
    humanCheckBoxList.append(dialog.playerThreeHuman);
    humanCheckBoxList.append(dialog.playerFourHuman);

    //get settings from file
    for (int i = 0; i < Settings::playerNames().size(); i++) {
        nameLineEditList.at(i)->setText(Settings::playerNames().at(i));
    }
    for (int i = 0; i < Settings::playerNames().size(); i++) {
        if (Settings::humanList().at(i) == 2) {
            humanCheckBoxList.at(i)->setCheckState(Qt::Checked);
        } else {
            humanCheckBoxList.at(i)->setCheckState(Qt::Unchecked);
        }
    }
    dialog.spinNumOfPlayers->setValue(Settings::numOfPlayers());
    dialog.spinHeight->setValue(Settings::boardHeight());
    dialog.spinWidth->setValue(Settings::boardWidth());
    if (Settings::quickStart() == 2) {
        dialog.quickStartCheck->setCheckState(Qt::Checked);
    } else {
        dialog.quickStartCheck->setCheckState(Qt::Unchecked);
    }

    //run dialog
    dialog.exec();
    if (dialog.result() == QDialog::Rejected) {
        return;
    }

    //save settings
    Settings::setNumOfPlayers(dialog.spinNumOfPlayers->value());

    QStringList tempNames;
    for (int i = 0; i <= 3; i++) {
        tempNames.append(nameLineEditList.at(i)->text());
    }
    Settings::setPlayerNames(tempNames);

    QList<int> tempHuman;
    for (int i = 0; i <= 3; i++) {
        tempHuman.append(humanCheckBoxList.at(i)->checkState());
    }
    Settings::setHumanList(tempHuman);

    Settings::setBoardHeight(dialog.spinHeight->value());
    Settings::setBoardWidth(dialog.spinWidth->value());
    Settings::setQuickStart(dialog.quickStartCheck->checkState());
    Settings::self()->save();

    gameReset();
}

void KSquaresWindow::gameReset()
{
    //create players
    QVector<KSquaresPlayer> playerList;
    for (int i = 0; i < Settings::numOfPlayers(); i++) {
        QColor color;
        switch (i) {
        case 0: //Red
            color = QColor(191, 3, 3); //or darker: (156,15,15);
            break;
        case 1: //Blue
            color = QColor(0, 67, 138); //or darker: (0,49,110);
            break;
        case 2: //Green
            color = QColor(0, 137, 44); //or darker: (0,110,41);
            break;
        case 3: //Yellow
            color = QColor(243, 195, 0); //or darker: (227,173,0);
            break;
        default:
            qCritical() << "KSquaresGame::playerSquareComplete(); currentPlayerId() != 0|1|2|3";
        }
        playerList.append(KSquaresPlayer(Settings::playerNames().at(i), color, Settings::humanList().at(i)));
    }

    //create physical board
    GameBoardScene *temp = m_scene;
    m_scene = new GameBoardScene(Settings::boardWidth(), Settings::boardHeight());

    m_view->setScene(m_scene);
    delete temp;

    m_view->setBoardSize(); //refresh board zooming

    //start game etc.
    sGame->createGame(playerList, Settings::boardWidth(), Settings::boardHeight());
    connect(m_scene, &GameBoardScene::lineDrawn, sGame, &KSquaresGame::addLineToIndex);
    //QT5: VERIFY SIGNAL DOESN'T EXIST connect(m_scene,&GameBoardScene::signalMoveRequest,this,&KSquaresWindow::slotMoveRequest);
    connect(sGame, &KSquaresGame::drawLine, m_scene, &GameBoardScene::drawLine);
    connect(sGame, &KSquaresGame::highlightMove, m_scene, &GameBoardScene::highlightLine);
    connect(sGame, &KSquaresGame::drawSquare, m_scene, &GameBoardScene::drawSquare);

    if (Settings::quickStart() == 2) {
        //This is being done before sGame->start(); to avoid the players cycling
        aiController ai(-1, sGame->lines(), QList<int>(), sGame->boardWidth(), sGame->boardHeight());
        QList<int> lines = ai.autoFill(8);  //There will be 8 possible safe move for the players
        QListIterator<int> i(lines);
        while (i.hasNext()) {
            sGame->addLineToIndex(i.next());
        }
    }
    sGame->start();
}

void KSquaresWindow::gameOver(const QVector<KSquaresPlayer> &_playerList)
{
    QVector<KSquaresPlayer> playerList = _playerList;
    std::sort(playerList.begin(), playerList.end(), std::greater<KSquaresPlayer>());
    //m_scene->displayScoreTable(playerList);

    ScoresDialog scoresDialog(this);
    scoresDialog.setWindowTitle(i18n("Scores"));

    QStandardItemModel *scoreTableModel = new QStandardItemModel(&scoresDialog);
    scoreTableModel->setRowCount(playerList.size());
    scoreTableModel->setColumnCount(2);
    scoreTableModel->setHeaderData(0, Qt::Horizontal, i18n("Player Name"));
    scoreTableModel->setHeaderData(1, Qt::Horizontal, i18n("Completed Squares"));

    for (int i = 0; i <  playerList.size(); i++) {
        scoreTableModel->setItem(i, 0, new QStandardItem(playerList.at(i).name()));
        scoreTableModel->item(i, 0)->setEditable(false);

        QString temp;
        temp.setNum(playerList.at(i).score());
        scoreTableModel->setItem(i, 1, new QStandardItem(temp));
        scoreTableModel->item(i, 1)->setEditable(false);
    }

    scoresDialog.scoreTable->setModel(scoreTableModel);
    scoresDialog.scoreTable->resizeColumnsToContents();
    scoresDialog.exec();

    if (playerList.at(0).isHuman()) {
        int score = (int)(static_cast<double>(playerList.at(0).score()) - (static_cast<double>(Settings::boardWidth() * Settings::boardHeight()) / static_cast<double>(playerList.size())));

        KScoreDialog ksdialog(KScoreDialog::Name, this);
        switch (Settings::difficulty()) {
        case 0:
            ksdialog.setConfigGroup(qMakePair(QByteArray("Easy"), i18n("Easy")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Medium"), i18n("Medium")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Hard"), i18n("Hard")));
            break;
        case 1:
            ksdialog.setConfigGroup(qMakePair(QByteArray("Medium"), i18n("Medium")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Easy"), i18n("Easy")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Hard"), i18n("Hard")));
            break;
        case 2:
            ksdialog.setConfigGroup(qMakePair(QByteArray("Hard"), i18n("Hard")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Easy"), i18n("Easy")));
            ksdialog.addLocalizedConfigGroupName(qMakePair(QByteArray("Medium"), i18n("Medium")));
            break;
        }
        KScoreDialog::FieldInfo scoreInfo;
        scoreInfo[KScoreDialog::Name] = playerList.at(0).name();
        scoreInfo[KScoreDialog::Score].setNum(score);

        if (ksdialog.addScore(scoreInfo, KScoreDialog::AskName)) {
            ksdialog.exec();
        }
    }
}

void KSquaresWindow::playerTakeTurn(KSquaresPlayer *currentPlayer)
{
    ////qDebug() << "void KSquares::playerTakeTurn(KSquaresPlayer* currentPlayer)";
    m_player->setText(QStringLiteral("<font color=\"%1\">%2</font>")
                      .arg(currentPlayer->colour().name())
                      .arg(currentPlayer->name()));
    if (currentPlayer->isHuman()) {
        //Let the human player interact with the board through the GameBoardView

        setCursor(Qt::ArrowCursor);
        m_scene->enableEvents();
    } else { //AI
        //lock the view to let the AI do it's magic
        setCursor(Qt::WaitCursor);
        m_scene->disableEvents();

        QTimer::singleShot(200, this, &KSquaresWindow::aiChooseLine);
        setCursor(Qt::ArrowCursor);
    }
}

// testing only
void KSquaresWindow::aiChooseLine()
{
    // This can happen when we start a new game and the ai was playing, since we have a
    // 200 ms singleShot timer just above, the game may have changed since the timer was shot
    if (sGame->currentPlayer()->isHuman())
        return;

    aiController ai(sGame->currentPlayerId(), sGame->lines(), sGame->squares(), sGame->boardWidth(), sGame->boardHeight());
    sGame->addLineToIndex(ai.chooseLine());
}

void KSquaresWindow::setupActions()
{
    KStandardGameAction::gameNew(this, &KSquaresWindow::gameNew, actionCollection());
    QAction *resetGame = KStandardGameAction::restart(this, &KSquaresWindow::gameReset, actionCollection());
    resetGame->setStatusTip(i18n("Start a new game with the current settings"));

    KStandardGameAction::highscores(this, &KSquaresWindow::showHighscores, actionCollection());
    KStandardGameAction::quit(this, &QWidget::close, actionCollection());

    // Preferences
    KStandardAction::preferences(this, &KSquaresWindow::optionsPreferences, actionCollection());

    setupGUI();
}

void KSquaresWindow::optionsPreferences()
{
    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), Settings::self());

    QWidget *displaySettingsDialog = new QWidget;
    ui_prefs_display.setupUi(displaySettingsDialog);
    dialog->addPage(displaySettingsDialog, i18n("Display"), QStringLiteral("preferences-desktop-display"));

    QWidget *aiSettingsDialog = new QWidget;
    ui_prefs_ai.setupUi(aiSettingsDialog);
    dialog->addPage(aiSettingsDialog, i18n("Computer Player"), QStringLiteral("games-difficult"));

    connect(dialog, &KConfigDialog::settingsChanged, m_view, &GameBoardView::setBoardSize);
    dialog->show();
}

