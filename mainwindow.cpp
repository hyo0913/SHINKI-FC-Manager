#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>

#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QListWidget>

#include "BoardModel.h"
#include "BoardItemDelegate.h"

#include "MatchDetailDialog.h"

#include <xlnt/xlnt.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_menuBoardVerticalHeader(new QMenu(this)),
    m_menuBoardHorizontalHeader(new QMenu(this)),
    m_menuBoardTable(new QMenu(this)),
    m_actionImportExcel(NULL),
    m_actionExportExcel(NULL),
    m_actionAddMatch(NULL),
    m_actionRemoveMatch(NULL),
    m_actionViewMatchDetails(NULL),
    m_actionAddPlayer(NULL),
    m_actionRemovePlayer(NULL),
    m_actionRemoveMatchOnBoard(NULL),
    m_actionViewMatchDetailsOnBoard(NULL),
    m_actionRemovePlayerOnBoard(NULL),
    m_actionEditPlayerOnBoard(NULL),
    m_actionMoveLeftPlayerOnBoard(NULL),
    m_actionMoveRightPlayerOnBoard(NULL),
    m_actionCreatePlayDataOnBoard(NULL),
    m_actionDeletePlayDataOnBoard(NULL),
    m_matchs(new Matchs()),
    m_players(new Players()),
    m_boardModel(NULL)
{
    ui->setupUi(this);

    createActions();
    setupMenus();

    m_boardModel = new BoardModel(m_matchs, m_players);
    ui->tableViewBoard->setModel(m_boardModel);
    ui->tableViewBoard->setItemDelegate(new BoardItemDelegate());

    ui->tableViewBoard->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewBoard->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->radioButtonViewGoal, SIGNAL(clicked(bool)), this, SLOT(changeBoardViewTypeToGoal()));
    connect(ui->radioButtonViewAssist, SIGNAL(clicked(bool)), this, SLOT(changeBoardViewTypeToAssist()));
    connect(ui->radioButtonViewTotal, SIGNAL(clicked(bool)), this, SLOT(changeBoardViewTypeToTotal()));

    ui->tableViewBoard->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardVerticalContextMenu(QPoint)));
    connect(ui->tableViewBoard->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(viewMatchDetailsOnBoard()));

    ui->tableViewBoard->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardHorizontalContextMenu(QPoint)));

    ui->tableViewBoard->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableViewBoard, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(boardTableContextMenu(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_matchs;
    delete m_players;
    delete m_boardModel;
}

void MainWindow::createActions()
{
    // top menu actions
    // - file
    m_actionImportExcel = new QAction(tr("Import"), this);
    connect(m_actionImportExcel, SIGNAL(triggered()), this, SLOT(importExcel()));

    m_actionExportExcel = new QAction(tr("Export"), this);
    connect(m_actionExportExcel, SIGNAL(triggered()), this, SLOT(exportExcel()));

    // - match
    m_actionAddMatch = new QAction(tr("Add Match"), this);
    connect(m_actionAddMatch, SIGNAL(triggered()), this, SLOT(addMatch()));

    m_actionRemoveMatch = new QAction(tr("Remove Match"), this);
    connect(m_actionRemoveMatch, SIGNAL(triggered()), this, SLOT(removeMatch()));

    m_actionViewMatchDetails = new QAction(tr("View Match Details"), this);
    connect(m_actionViewMatchDetails, SIGNAL(triggered()), this, SLOT(viewMatchDetails()));

    // - player
    m_actionAddPlayer = new QAction(tr("Add Player"), this);
    connect(m_actionAddPlayer, SIGNAL(triggered()), this, SLOT(addPlayer()));

    m_actionRemovePlayer = new QAction(tr("Remove Player"), this);
    connect(m_actionRemovePlayer, SIGNAL(triggered()), this, SLOT(removePlayer()));

    m_actionEditPlayer = new QAction(tr("Edit Player"), this);
    connect(m_actionEditPlayer, SIGNAL(triggered()), this, SLOT(editPlayer()));

    // on board actions
    // - match
    m_actionRemoveMatchOnBoard = new QAction(tr("Remove Match"), this);
    connect(m_actionRemoveMatchOnBoard, SIGNAL(triggered()), this, SLOT(removeMatchOnBoard()));

    m_actionViewMatchDetailsOnBoard = new QAction(tr("View Match Details"), this);
    connect(m_actionViewMatchDetailsOnBoard, SIGNAL(triggered()), this, SLOT(viewMatchDetailsOnBoard()));

    // - player
    m_actionRemovePlayerOnBoard = new QAction(tr("Remove Player"), this);
    connect(m_actionRemovePlayerOnBoard, SIGNAL(triggered()), this, SLOT(removePlayerOnBoard()));

    m_actionEditPlayerOnBoard = new QAction(tr("Edit Player"), this);
    connect(m_actionEditPlayerOnBoard, SIGNAL(triggered()), this, SLOT(editPlayerOnBoard()));

    m_actionMoveLeftPlayerOnBoard = new QAction(tr("Move Left"), this);
    connect(m_actionMoveLeftPlayerOnBoard, SIGNAL(triggered()), this, SLOT(moveToLeftPlayerOnBoard()));

    m_actionMoveRightPlayerOnBoard = new QAction(tr("Move Right"), this);
    connect(m_actionMoveRightPlayerOnBoard, SIGNAL(triggered()), this, SLOT(moveToRightPlayerOnBoard()));

    // - playdata
    m_actionCreatePlayDataOnBoard = new QAction(tr("Create Play Data"), this);
    connect(m_actionCreatePlayDataOnBoard, SIGNAL(triggered()), this, SLOT(createPlayDataOnBoard()));

    m_actionDeletePlayDataOnBoard = new QAction(tr("Delete Play Data"), this);
    connect(m_actionDeletePlayDataOnBoard, SIGNAL(triggered()), this, SLOT(deletePlayDataOnBoard()));
}

void MainWindow::setupMenus()
{
    // menu bar
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_actionImportExcel);
    fileMenu->addAction(m_actionExportExcel);

    QMenu* matchMenu = menuBar()->addMenu(tr("Match"));
    matchMenu->addAction(m_actionAddMatch);
    matchMenu->addAction(m_actionRemoveMatch);
    matchMenu->addAction(m_actionViewMatchDetails);

    QMenu* playerMenu = menuBar()->addMenu(tr("Player"));
    playerMenu->addAction(m_actionAddPlayer);
    playerMenu->addAction(m_actionRemovePlayer);
    playerMenu->addAction(m_actionEditPlayer);

    // board vertical header
    m_menuBoardVerticalHeader->addAction(m_actionViewMatchDetailsOnBoard);
    m_menuBoardVerticalHeader->addAction(m_actionRemoveMatchOnBoard);

    // board horizontal header
    m_menuBoardHorizontalHeader->addAction(m_actionMoveLeftPlayerOnBoard);
    m_menuBoardHorizontalHeader->addAction(m_actionMoveRightPlayerOnBoard);
    m_menuBoardHorizontalHeader->addAction(m_actionEditPlayerOnBoard);
    m_menuBoardHorizontalHeader->addAction(m_actionRemovePlayerOnBoard);

    // board table
    //
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
                QMessageBox::warning(NULL, tr("SHINKI FC Manager"), tr("Already registered"), QMessageBox::Close);
            }
        }
    }
}

void MainWindow::removeMatch()
{

}

void MainWindow::removeMatchOnBoard()
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

        const QDate &date = m_matchs->matchAt(index.row())->Date;
        if( !m_boardModel->removeMatch(date) ) {
            break;
        }
    }
}

void MainWindow::viewMatchDetails()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(new QVBoxLayout());
    QCalendarWidget* calender = new QCalendarWidget();
    dialog.layout()->addWidget(calender);
    dialog.layout()->addWidget(&buttonBox);

    int ret = 0;
    QDate date;
    while( true )
    {
        ret = dialog.exec();
        if( ret == QDialog::Accepted ) {
            date = calender->selectedDate();

            if( m_matchs->exist(date) ) {
                break;
            } else {
                QMessageBox::critical(NULL,
                                      tr("View Match Details"),
                                      tr("No match data"),
                                      QMessageBox::Close);
            }
        } else {
            break;
        }
    }

    if( ret == QDialog::Accepted ) {
        MatchDetailDialog dialog(m_matchs->match(date), m_players);
        dialog.exec();
    }
}

void MainWindow::viewMatchDetailsOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedRows();
    if( indexList.isEmpty() ) { return; }

    int row = indexList.first().row();
    if( row >= m_matchs->count() ) { return; }

    const Match* match = m_matchs->matchAt(row);
    if( match == NULL ) { return; }

    MatchDetailDialog dialog(match, m_players);
    dialog.exec();
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
                QMessageBox::warning(NULL, tr("SHINKI FC Manager"), tr("Already registered"), QMessageBox::Close);
            }
        }
    }
}

void MainWindow::removePlayer()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(new QVBoxLayout());

    QListWidget* list = new QListWidget();
    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), &dialog, SLOT(accept()));
    dialog.layout()->addWidget(list);
    dialog.layout()->addWidget(&buttonBox);

    for( int i = 0; i < m_players->count(); i++ ) {
        list->addItem(new QListWidgetItem(m_players->playerAt(i)->name()));
    }

    if( dialog.exec() == QDialog::Accepted ) {
        QList<QListWidgetItem*> selectedItems = list->selectedItems();

        while( selectedItems.count() > 0 ) {
            QListWidgetItem* item = selectedItems.takeFirst();
            if( item == NULL ) { continue; }

            m_boardModel->removePlayer(item->text());
        }
    }
}

void MainWindow::removePlayerOnBoard()
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

        const QString &name = m_players->playerAt(index.column())->name();
        if( !m_boardModel->removePlayer(name) ) {
            break;
        }
    }
}

void MainWindow::editPlayer()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(new QVBoxLayout());

    QListWidget* list = new QListWidget();
    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), &dialog, SLOT(accept()));
    dialog.layout()->addWidget(list);
    dialog.layout()->addWidget(&buttonBox);

    for( int i = 0; i < m_players->count(); i++ ) {
        list->addItem(new QListWidgetItem(m_players->playerAt(i)->name()));
    }

    if( dialog.exec() == QDialog::Accepted ) {
        QList<QListWidgetItem*> selectedItems = list->selectedItems();

        if( selectedItems.count() > 0 ) {
            QListWidgetItem* item = selectedItems.takeFirst();
            if( item == NULL ) { return; }

            QString beforeName = item->text();

            QDialog nameDialog;
            nameDialog.setWindowTitle(tr("SHINKI FC Manager"));
            QDialogButtonBox buttonBox;
            buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
            connect(&buttonBox, SIGNAL(accepted()), &nameDialog, SLOT(accept()));
            connect(&buttonBox, SIGNAL(rejected()), &nameDialog, SLOT(reject()));

            nameDialog.setLayout(new QVBoxLayout());

            QLineEdit* lineEdit = new QLineEdit();
            nameDialog.layout()->addWidget(lineEdit);
            nameDialog.layout()->addWidget(&buttonBox);
            lineEdit->setText(beforeName);

            if( nameDialog.exec() == QDialog::Accepted ) {
                QString afterName = lineEdit->text();
                if( !afterName.isEmpty() ) {
                    if( m_players->exist(afterName) ) {
                        QMessageBox::warning(NULL, tr("SHINKI FC Manager"), tr("Already registered"), QMessageBox::Close);
                    } else {
                        m_boardModel->editPlayerName(beforeName, afterName);
                    }
                }
            }
        }
    }
}

void MainWindow::editPlayerOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedColumns();
    if( indexList.count() <= 0 ) { return; }

    QModelIndex index = indexList.takeFirst();
    if( !index.isValid() ) { return; }

    const QString beforeName = m_players->playerAt(index.column())->name();

    QDialog nameDialog;
    nameDialog.setWindowTitle(tr("SHINKI FC Manager"));
    QDialogButtonBox buttonBox;
    buttonBox.setStandardButtons(QDialogButtonBox::Yes|QDialogButtonBox::Cancel);
    connect(&buttonBox, SIGNAL(accepted()), &nameDialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &nameDialog, SLOT(reject()));

    nameDialog.setLayout(new QVBoxLayout());

    QLineEdit* lineEdit = new QLineEdit();
    nameDialog.layout()->addWidget(lineEdit);
    nameDialog.layout()->addWidget(&buttonBox);
    lineEdit->setText(beforeName);

    if( nameDialog.exec() == QDialog::Accepted ) {
        QString afterName = lineEdit->text();
        if( !afterName.isEmpty() ) {
            if( m_players->exist(afterName) ) {
                QMessageBox::warning(NULL, tr("SHINKI FC Manager"), tr("Already registered"), QMessageBox::Close);
            } else {
                m_boardModel->editPlayerName(beforeName, afterName);
            }
        }
    }
}

void MainWindow::moveToLeftPlayerOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedColumns();
    if( indexList.count() <= 0 ) { return; }

    QModelIndex index = indexList.takeFirst();
    if( !index.isValid() ) { return; }

    const QString name = m_players->playerAt(index.column())->name();

    m_boardModel->movePlayerToLeft(name);
}

void MainWindow::moveToRightPlayerOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedColumns();
    if( indexList.count() <= 0 ) { return; }

    QModelIndex index = indexList.takeFirst();
    if( !index.isValid() ) { return; }

    const QString name = m_players->playerAt(index.column())->name();

    m_boardModel->movePlayerToRight(name);
}

void MainWindow::createPlayDataOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedIndexes();
    while( indexList.count() > 0 ) {
        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) { continue; }

        Match* match = m_matchs->matchAt(index.row());
        Player* player = m_players->playerAt(index.column());

        if( !player->hasMatch(match->Date) ) {
            if( !match->Players.contains(player->name()) ) {
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

void MainWindow::deletePlayDataOnBoard()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedIndexes();
    while( indexList.count() > 0 ) {
        QModelIndex index = indexList.takeFirst();
        if( !index.isValid() ) { continue; }

        Match* match = m_matchs->matchAt(index.row());
        Player* player = m_players->playerAt(index.column());

        if( player->hasMatch(match->Date) ) {
            if( match->Players.contains(player->name()) ) {
                match->Players.removeOne(player->name());
            }
            player->removeMatch(match->Date);
        }
    }
}

void MainWindow::changeBoardViewTypeToGoal()
{
    m_boardModel->setViewItem(BoardModel::BoardGoal);
}

void MainWindow::changeBoardViewTypeToAssist()
{
    m_boardModel->setViewItem(BoardModel::BoardAssist);
}

void MainWindow::changeBoardViewTypeToTotal()
{
    m_boardModel->setViewItem(BoardModel::BoardTotal);
}

void MainWindow::boardVerticalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() ) { return; }

    int row = index.row();

    if( index.row() < m_matchs->count() &&
        ui->tableViewBoard->verticalHeader()->selectionModel()->isRowSelected(row, QModelIndex()) )
    {
        m_menuBoardVerticalHeader->popup(ui->tableViewBoard->mapToGlobal(pos));
    }
}

void MainWindow::boardHorizontalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() || index.row() == m_matchs->count() ) { return; }

    m_menuBoardHorizontalHeader->popup(ui->tableViewBoard->mapToGlobal(pos));
}

void MainWindow::boardTableContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() || index.row() == m_matchs->count() ) { return; }

    const Match* match = m_matchs->matchAt(index.row());
    const Player* player = m_players->playerAt(index.column());

    if( !player->hasMatch(match->Date) ) {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionCreatePlayDataOnBoard);
    } else {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionDeletePlayDataOnBoard);
    }

    m_menuBoardTable->popup(ui->tableViewBoard->mapToGlobal(pos));
}

void MainWindow::importExcel()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "Import", ".", "*.xlsx", NULL, QFileDialog::DontUseNativeDialog);
    if( fileName.isEmpty() ) { return; }

    Matchs* newMatchs = new Matchs();
    Match* match = NULL;
    Players* newPlayers = new Players();
    Player* player = NULL;
    PlayData* playData = NULL;
    QString strTemp;

    QVariant valGoal;
    QVariant valAssist;

    int row = 0;
    int column = 0;
    int rowCount = 0;
    int columnCount = 0;

    QDate date;
    QString name;

    xlnt::workbook excel;
    try {
        excel.load(fileName.toStdString());
    } catch (xlnt::exception exception) {
        QString msg = QString::fromUtf8(exception.what());
        QMessageBox::critical(NULL, tr("Import"), msg, QMessageBox::Close);

        delete newMatchs;
        delete newPlayers;
        return;
    }

    xlnt::worksheet sheet = excel.sheet_by_index(0);
    rowCount = sheet.rows().length();
    columnCount = sheet.columns().length();

    // check document format
    // check date column
    strTemp = QString::fromStdString(sheet.cell(xlnt::column_t(1), xlnt::row_t(2)).to_string());
    if( strTemp != "Date" ) {
        QMessageBox::critical(NULL,
                              tr("Import"),
                              tr("Document format error") + "\n" + tr("Could not find Date header"),
                              QMessageBox::Close);

        delete newMatchs;
        delete newPlayers;
        return;
    }

    // check goal assist
    bool valid = true;
    for( column = 2; column < columnCount; column += 2)
    {
        strTemp = QString::fromStdString(sheet.cell(xlnt::column_t(column), xlnt::row_t(2)).to_string());
        if( strTemp != "Goal" ) {
            valid = false;
            break;
        }

        strTemp = QString::fromStdString(sheet.cell(xlnt::column_t(column+1), xlnt::row_t(2)).to_string());
        if( strTemp != "Assist" ) {
            valid = false;
            break;
        }
    }
    if( !valid ) {
        QMessageBox::critical(NULL,
                              tr("Import"),
                              tr("Document format error") + "\n" + tr("Could not find Goal&Assist header"),
                              QMessageBox::Close);

        delete newMatchs;
        delete newPlayers;
        return;
    }

    // create play data
    for( row = 3; row < rowCount; row++ )
    {
        xlnt::cell cellDate = sheet.cell(xlnt::column_t(1), xlnt::row_t(row));
        if( !cellDate.has_value() || !cellDate.is_date() ) { break; }

        xlnt::date dateTemp = cellDate.value<xlnt::date>();
        date.setDate(dateTemp.year, dateTemp.month, dateTemp.day);

        if( newMatchs->exist(date) ) {
            row++;
            continue;
        }

        match = newMatchs->makeMatch(date);
        if( match == NULL ) {
            row++;
            continue;
        }

        for( column = 2; column < columnCount; column += 2 )
        {
            xlnt::cell cellName = sheet.cell(xlnt::column_t(column), xlnt::row_t(1));
            if( !cellName.has_value() ) { break; }

            name = QString::fromStdString(cellName.to_string());
            if( name.isEmpty() ) { break; }

            if( !newPlayers->exist(name) ) {
                player = newPlayers->makePalyer(name);
            } else {
                player = newPlayers->player(name);
            }
            if( player == NULL ) { break; }

            // goal
            xlnt::cell cellGoal = sheet.cell(xlnt::column_t(column), xlnt::row_t(row));
            xlnt::cell cellAssist = sheet.cell(xlnt::column_t(column+1), xlnt::row_t(row));

            if( (cellGoal.has_value() && (cellGoal.data_type() == xlnt::cell_type::number)) ||
                (cellAssist.has_value() && (cellAssist.data_type() == xlnt::cell_type::number)) )
            {
                if( !match->Players.contains(name) ) {
                    match->Players << name;
                }

                if( player->hasMatch(date) ) {
                    playData = player->playData(date);
                } else {
                    playData = player->addMatch(date);
                }

                if( cellGoal.data_type() == xlnt::cell_type::number ) {
                    valGoal.setValue(cellGoal.value<int>());
                } else {
                    valGoal.setValue(0);
                }

                if( cellAssist.data_type() == xlnt::cell_type::number ) {
                    valAssist.setValue(cellAssist.value<int>());
                } else {
                    valAssist.setValue(0);
                }

                playData->setData(PlayDataItem::itemGoal, valGoal);
                playData->setData(PlayDataItem::itemAssist, valAssist);
            }
        }
    }

    // import
    m_boardModel->changeModelData(newMatchs, newPlayers);
    delete m_matchs;
    m_matchs = newMatchs;
    delete m_players;
    m_players = newPlayers;
    return;
}

void MainWindow::exportExcel()
{
    QString fileName = QFileDialog::getSaveFileName(NULL, "Export", ".", "*.xlsx", NULL, QFileDialog::DontUseNativeDialog);
    if( fileName.isEmpty() ) { return; }
    if( !fileName.contains(".xlsx") ) { fileName.append(".xlsx"); }

    const double widthSize = 5.50;
    const double heightSize = 16.50;
    xlnt::font fontTemp;
    fontTemp.size(11);

    xlnt::alignment alignCenter;
    alignCenter.horizontal(xlnt::horizontal_alignment::center);

    xlnt::workbook excel;
    xlnt::worksheet sheet = excel.active_sheet();

    int playerCount = m_players->count();
    for( int i = 0; i < playerCount; i++ ) {
        QString name = m_players->playerAt(i)->name();

        sheet.column_properties(xlnt::column_t(i*2)+2).width = widthSize;
        sheet.column_properties(xlnt::column_t(i*2)+3).width = widthSize;

        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(1)).font(fontTemp);
        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(1)).alignment(alignCenter);
        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(1)).value(name.toStdString());

        sheet.merge_cells(xlnt::range_reference((i*2)+2, 1, (i*2)+3, 1));

        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(2)).font(fontTemp);
        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(2)).alignment(alignCenter);
        sheet.cell(xlnt::column_t((i*2)+2), xlnt::row_t(2)).value("Goal");

        sheet.cell(xlnt::column_t((i*2)+3), xlnt::row_t(2)).font(fontTemp);
        sheet.cell(xlnt::column_t((i*2)+3), xlnt::row_t(2)).alignment(alignCenter);
        sheet.cell(xlnt::column_t((i*2)+3), xlnt::row_t(2)).value("Assist");
    }

    sheet.row_properties(xlnt::row_t(1)).height = heightSize;
    sheet.row_properties(xlnt::row_t(2)).height = heightSize;

    sheet.cell(xlnt::column_t(1), xlnt::row_t(2)).font(fontTemp);
    sheet.cell(xlnt::column_t(1), xlnt::row_t(2)).alignment(alignCenter);
    sheet.cell(xlnt::column_t(1), xlnt::row_t(2)).value("Date");

    const Match* match = NULL;
    const Player* player = NULL;
    const PlayData* playData = NULL;
    int rowCount = m_matchs->count();
    int columnCount = playerCount*2;
    for( int row = 3; row < rowCount+3; row++ )
    {
        sheet.row_properties(xlnt::row_t(row)).height = heightSize;

        match = m_matchs->matchAt(row-3);
        if( match == NULL ) { continue; }

        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).font(fontTemp);
        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).alignment(alignCenter);

        xlnt::date dateTemp(match->Date.year(), match->Date.month(), match->Date.day());
        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).value(dateTemp);

        for( int column = 2; column < columnCount+2; column += 2 )
        {
            sheet.cell(xlnt::column_t(column), xlnt::row_t(row)).font(fontTemp);
            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(row)).font(fontTemp);

            player = m_players->playerAt((column/2)-1);
            if( player == NULL ) { continue; }

            playData = player->playData(match->Date);
            if( playData == NULL ) { continue; }

            sheet.cell(xlnt::column_t(column), xlnt::row_t(row)).value(playData->data(PlayDataItem::itemGoal).toInt());
            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(row)).value(playData->data(PlayDataItem::itemAssist).toInt());
        }
    }

    // Total
    sheet.cell(xlnt::column_t(1), xlnt::row_t(rowCount+3)).font(fontTemp);
    sheet.cell(xlnt::column_t(1), xlnt::row_t(rowCount+3)).alignment(alignCenter);
    sheet.cell(xlnt::column_t(1), xlnt::row_t(rowCount+3)).value("Total");

    for( int column = 2; column < columnCount+2; column += 2 )
    {
        player = m_players->playerAt((column/2)-1);
        if( player == NULL ) { continue; }

        int valGoal = 0;
        int valAssist = 0;
        QList<QDate> dates = player->matchDates();
        if( dates.count() > 0 )
        {
            while( dates.count() > 0 )
            {
                playData = player->playData(dates.takeFirst());
                if( playData == NULL ) { continue; }

                valGoal += playData->data(PlayDataItem::itemGoal).toInt();
                valAssist += playData->data(PlayDataItem::itemAssist).toInt();
            }

            sheet.cell(xlnt::column_t(column), xlnt::row_t(rowCount+3)).font(fontTemp);
            sheet.cell(xlnt::column_t(column), xlnt::row_t(rowCount+3)).value(valGoal);

            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(rowCount+3)).font(fontTemp);
            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(rowCount+3)).value(valAssist);
        }
    }

    excel.save(fileName.toStdString());
}
