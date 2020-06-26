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
class BoardItemDelegate;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QAction* m_actionLoadExcel;

    QAction* m_actionAddMatch;
    QAction* m_actionRemoveMatch;
    QAction* m_actionAddPlayer;
    QAction* m_actionRemovePlayer;

    Matchs m_matchs;
    Players m_players;

    BoardModel* m_boardModel;
    BoardItemDelegate* m_boardItemDelegate;

    void createActions();
    void setupMenuBar();

private slots:
    void addMatch();
    void removeMatch();
    void addPlayer();
    void removePlayer();
};
#endif // MAINWINDOW_H
