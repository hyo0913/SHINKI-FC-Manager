#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>

#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QCalendar>
#include <QSpinBox>
#include <QMessageBox>

#include "BoardModel.h"
#include "BoardItemDelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_menuBoardTable(new QMenu(this)),
    m_actionImportExcel(NULL),
    m_actionAddMatch(NULL),
    m_actionRemoveMatch(NULL),
    m_actionAddPlayer(NULL),
    m_actionRemovePlayer(NULL),
    m_actionCreatePlayData(NULL),
    m_actionDeletePlayData(NULL),
    m_actionSortPlayer(NULL),
    m_boardModel(NULL)
{
    ui->setupUi(this);

    createActions();
    setupMenuBar();

    m_boardModel = new BoardModel(&m_matchs, &m_players);
    ui->tableViewBoard->setModel(m_boardModel);
    ui->tableViewBoard->setItemDelegate(new BoardItemDelegate());

    ui->tableViewBoard->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewBoard->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->radioButtonViewGoal, SIGNAL(clicked(bool)), this, SLOT(viewGoal()));
    connect(ui->radioButtonViewAssist, SIGNAL(clicked(bool)), this, SLOT(viewAssist()));
    connect(ui->radioButtonViewTotal, SIGNAL(clicked(bool)), this, SLOT(viewTotal()));

    ui->tableViewBoard->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardVerticalContextMenu(QPoint)));

    ui->tableViewBoard->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardHorizontalContextMenu(QPoint)));

    ui->tableViewBoard->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardTableContextMenu(QPoint)));

    m_boardModel->addMatch(QDate(2020, 06, 01));
    m_boardModel->addMatch(QDate(2020, 06, 02));
    m_boardModel->addMatch(QDate(2020, 06, 03));

    m_boardModel->addPlayer(QString::fromUtf8("숡퀴"));
    m_boardModel->addPlayer(QString::fromUtf8("읽퀴"));
    m_boardModel->addPlayer(QString::fromUtf8("횱퀴"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    m_actionImportExcel = new QAction(tr("Import"), this);

    m_actionAddMatch = new QAction(tr("Add Match"), this);
    connect(m_actionAddMatch, SIGNAL(triggered()), this, SLOT(addMatch()));

    m_actionRemoveMatch = new QAction(tr("Remove Match"), this);
    connect(m_actionRemoveMatch, SIGNAL(triggered()), this, SLOT(removeMatch()));

    m_actionAddPlayer = new QAction(tr("Add Player"), this);
    connect(m_actionAddPlayer, SIGNAL(triggered()), this, SLOT(addPlayer()));

    m_actionRemovePlayer = new QAction(tr("Remove Player"), this);
    connect(m_actionRemovePlayer, SIGNAL(triggered()), this, SLOT(removePlayer()));

    m_actionCreatePlayData = new QAction(tr("Create Play Data"), this);
    connect(m_actionCreatePlayData, SIGNAL(triggered()), this, SLOT(createPlayData()));

    m_actionDeletePlayData = new QAction(tr("Delete Play Data"), this);
    connect(m_actionDeletePlayData, SIGNAL(triggered()), this, SLOT(deletePlayData()));

    m_actionSortPlayer = new QAction(tr("Ascending"), this);
    connect(m_actionSortPlayer, SIGNAL(triggered()), this, SLOT(sortPlayer()));
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_actionImportExcel);

    QMenu* matchMenu = menuBar()->addMenu(tr("Match"));
    matchMenu->addAction(m_actionAddMatch);
    matchMenu->addAction(m_actionRemoveMatch);

    QMenu* playerMenu = menuBar()->addMenu(tr("Player"));
    playerMenu->addAction(m_actionAddPlayer);
    playerMenu->addAction(m_actionRemovePlayer);
}

void MainWindow::addMatch()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(new QVBoxLayout());
    //dialog.layout()->addWidget(new QLabel(tr("Select date")));

    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    dialog.layout()->addWidget(dateEdit);
    dialog.layout()->addWidget(&buttonBox);

    if( dialog.exec() == QDialog::Accepted ) {
        QDate date = dateEdit->date();
        if( date.isValid() ) {
            if( !m_boardModel->addMatch(date) ) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("SHINKI FC Manager");
                msgBox.addButton(QMessageBox::Close);
                msgBox.setText(tr("Already registered"));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
        }
    }
}

void MainWindow::removeMatch()
{
    while( true )
    {
        QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedRows();
        if( indexList.count() <= 0 ) {
            break;
        }

        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) {
            break;
        }

        const QDate &date = m_matchs.matchAt(index.row())->Date;
        if( !m_boardModel->removeMatch(date) ) {
            break;
        }
    }
}

void MainWindow::addPlayer()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(new QVBoxLayout());
    //dialog.layout()->addWidget(new QLabel(tr("Input player name")));

    QLineEdit* lineEdit = new QLineEdit();
    dialog.layout()->addWidget(lineEdit);
    dialog.layout()->addWidget(&buttonBox);

    if( dialog.exec() == QDialog::Accepted ) {
        QString name = lineEdit->text();
        if( !name.isEmpty() ) {
            if( !m_boardModel->addPlayer(name) ) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("SHINKI FC Manager");
                msgBox.addButton(QMessageBox::Close);
                msgBox.setText(tr("Already registered"));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
        }
    }
}

void MainWindow::removePlayer()
{
    while( true )
    {
        QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedColumns();
        if( indexList.count() <= 0 ) {
            break;
        }

        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) {
            break;
        }

        const QString &name = m_players.playerAt(index.column())->name();
        if( !m_boardModel->removePlayer(name) ) {
            break;
        }
    }
}

void MainWindow::createPlayData()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedIndexes();
    while( indexList.count() > 0 ) {
        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) { continue; }

        Match* match = m_matchs.matchAt(index.row());
        Player* player = m_players.playerAt(index.column());

        if( !player->hasMatch(match->Date) ) {
            if( match->Players.contains(player->name()) ) {
                match->Players << player->name();
            }
            PlayData* playData = player->addMatch(match->Date);
            if( playData != nullptr ) {
                QVariant temp;
                temp.setValue(0);
                playData->setData(PlayDataItem::itemGoal, temp);
                playData->setData(PlayDataItem::itemAssist, temp);
            }
        }
    }
}

void MainWindow::deletePlayData()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedIndexes();
    while( indexList.count() > 0 ) {
        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) { continue; }

        Match* match = m_matchs.matchAt(index.row());
        Player* player = m_players.playerAt(index.column());

        if( player->hasMatch(match->Date) ) {
            if( match->Players.contains(player->name()) ) {
                match->Players.removeOne(player->name());
            }
            player->removeMatch(match->Date);
        }
    }
}

void MainWindow::sortPlayer()
{

}

void MainWindow::viewGoal()
{
    m_boardModel->setViewItem(BoardModel::BoardGoal);
}

void MainWindow::viewAssist()
{
    m_boardModel->setViewItem(BoardModel::BoardAssist);
}

void MainWindow::viewTotal()
{
    m_boardModel->setViewItem(BoardModel::BoardTotal);
}

void MainWindow::boardVerticalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() ) { return; }

    if( ui->tableViewBoard->selectionModel()->isRowSelected(index.row(), QModelIndex()) ) {
        m_menuBoardTable->clear();

        m_menuBoardTable->addAction(m_actionSortPlayer);

        if( index.row() < m_matchs.count() ) {
            m_menuBoardTable->addAction(m_actionRemoveMatch);
        }

        m_menuBoardTable->popup(ui->tableViewBoard->mapToGlobal(pos));
    }
}

void MainWindow::boardHorizontalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() || index.row() == m_matchs.count() ) { return; }

    if( ui->tableViewBoard->selectionModel()->isColumnSelected(index.column(), QModelIndex()) ) {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionRemovePlayer);

        m_menuBoardTable->popup(ui->tableViewBoard->mapToGlobal(pos));
    }
}

void MainWindow::boardTableContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() || index.row() == m_matchs.count() ) { return; }

    const Match* match = m_matchs.matchAt(index.row());
    const Player* player = m_players.playerAt(index.column());

    if( !player->hasMatch(match->Date) ) {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionCreatePlayData);
    } else {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionDeletePlayData);
    }

    m_menuBoardTable->popup(ui->tableViewBoard->mapToGlobal(pos));
}
