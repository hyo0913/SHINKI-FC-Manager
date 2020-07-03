#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    QMenu* m_menuBoardTable;

    QAction* m_actionImportExcel;
    QAction* m_actionAddMatch;
    QAction* m_actionRemoveMatch;
    QAction* m_actionAddPlayer;
    QAction* m_actionRemovePlayer;
    QAction* m_actionCreatePlayData;
    QAction* m_actionDeletePlayData;

    QAction* m_actionViewMatchDetails;

    Matchs m_matchs;
    Players m_players;

    BoardModel* m_boardModel;

    void createActions();
    void setupMenuBar();

private slots:
    void addMatch();
    void removeMatch();
    void addPlayer();
    void removePlayer();

    void createPlayData();
    void deletePlayData();

    void viewGoal();
    void viewAssist();
    void viewTotal();

    void viewMatchDetails();

    void boardVerticalContextMenu(const QPoint &pos);
    void boardHorizontalContextMenu(const QPoint &pos);
    void boardTableContextMenu(const QPoint &pos);
};
#endif // MAINWINDOW_H
