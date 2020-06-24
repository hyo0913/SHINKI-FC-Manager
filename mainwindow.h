#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
QT_END_NAMESPACE

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
    QAction* m_actionDeleteMatch;
    QAction* m_actionAddPlayer;
    QAction* m_actionDeletePlayer;

    void createActions();
    void setupMenuBar();

private slots:
    void addMatch();
    void deleteMatch();
    void addPlayer();
    void deletePlayer();
};
#endif // MAINWINDOW_H
