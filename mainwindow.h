#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
QT_END_NAMESPACE

#include "Match.h"
#include "Player.h"

class BoardModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;

    QMenu* m_menuFile;
    QMenu* m_menuMatch;
    QMenu* m_menuPlayer;
    QMenu* m_menuLanguage;
    QMenu* m_menuBoardVerticalHeader;
    QMenu* m_menuBoardHorizontalHeader;
    QMenu* m_menuBoardTable;

    // file
    QAction* m_actionImportExcel;
    QAction* m_actionExportExcel;

    // match
    QAction* m_actionAddMatch;
    QAction* m_actionRemoveMatch;
    QAction* m_actionViewMatchDetails;

    // player
    QAction* m_actionAddPlayer;
    QAction* m_actionRemovePlayer;
    QAction* m_actionEditPlayer;

    // match on board
    QAction* m_actionRemoveMatchOnBoard;
    QAction* m_actionViewMatchDetailsOnBoard;

    // player on board
    QAction* m_actionRemovePlayerOnBoard;
    QAction* m_actionEditPlayerOnBoard;
    QAction* m_actionMoveLeftPlayerOnBoard;
    QAction* m_actionMoveRightPlayerOnBoard;

    // playdata on board
    QAction* m_actionCreatePlayDataOnBoard;
    QAction* m_actionDeletePlayDataOnBoard;

    Matchs* m_matchs;
    Players* m_players;

    BoardModel* m_boardModel;

    void setupMenus();
    void createLanguageMenu();

    void loadLanguage(const QString &language);
    void changeEvent(QEvent *event);

    void retranslateUi();

private slots:
    void changeLanguage(QAction *action);

    void addMatch();
    void removeMatch();
    void removeMatchOnBoard();

    void viewMatchDetails();
    void viewMatchDetailsOnBoard();

    void viewPlayerDetailsOnBoard();

    void addPlayer();
    void removePlayer();
    void removePlayerOnBoard();
    void editPlayer();
    void editPlayerOnBoard();
    void moveToLeftPlayerOnBoard();
    void moveToRightPlayerOnBoard();

    void createPlayDataOnBoard();
    void deletePlayDataOnBoard();

    void changeBoardViewTypeToGoal();
    void changeBoardViewTypeToAssist();
    void changeBoardViewTypeToTotal();

    void boardVerticalContextMenu(const QPoint &pos);
    void boardHorizontalContextMenu(const QPoint &pos);
    void boardTableContextMenu(const QPoint &pos);

    void importExcel();
    void exportExcel();
};
#endif // MAINWINDOW_H
