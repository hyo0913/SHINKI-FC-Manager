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

#include "BoardModel.h"
#include "BoardItemDelegate.h"

#include "MatchDetailDialog.h"

#include <xlnt/xlnt.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_menuBoardTable(new QMenu(this)),
    m_actionImportExcel(NULL),
    m_actionExportExcel(NULL),
    m_actionAddMatch(NULL),
    m_actionRemoveMatch(NULL),
    m_actionAddPlayer(NULL),
    m_actionRemovePlayer(NULL),
    m_actionCreatePlayData(NULL),
    m_actionDeletePlayData(NULL),
    m_actionViewMatchDetails(NULL),
    m_matchs(new Matchs()),
    m_players(new Players()),
    m_boardModel(NULL)
{
    ui->setupUi(this);

    createActions();
    setupMenuBar();

    m_boardModel = new BoardModel(m_matchs, m_players);
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
    delete m_matchs;
    delete m_players;
    delete m_boardModel;
}

void MainWindow::createActions()
{
    m_actionImportExcel = new QAction(tr("Import"), this);
    connect(m_actionImportExcel, SIGNAL(triggered()), this, SLOT(importExcel()));

    m_actionExportExcel = new QAction(tr("Export"), this);
    connect(m_actionExportExcel, SIGNAL(triggered()), this, SLOT(exportExcel()));

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

    m_actionViewMatchDetails = new QAction(tr("View Match Details"), this);
    connect(m_actionViewMatchDetails, SIGNAL(triggered()), this, SLOT(viewMatchDetails()));
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_actionImportExcel);
    fileMenu->addAction(m_actionExportExcel);

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

        const QDate &date = m_matchs->matchAt(index.row())->Date;
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

        const QString &name = m_players->playerAt(index.column())->name();
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

void MainWindow::deletePlayData()
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

void MainWindow::viewMatchDetails()
{
    QModelIndexList indexList = ui->tableViewBoard->selectionModel()->selectedRows();
    if( indexList.isEmpty() ) { return; }

    int row = indexList.first().row();
    const Match* match = m_matchs->matchAt(row);
    if( match == NULL ) { return; }

    MatchDetailDialog dialog(match, m_players);
    dialog.exec();
}

void MainWindow::boardVerticalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() ) { return; }

    if( ui->tableViewBoard->selectionModel()->isRowSelected(index.row(), QModelIndex()) ) {
        m_menuBoardTable->clear();

        if( index.row() < m_matchs->count() ) {
            m_menuBoardTable->addAction(m_actionViewMatchDetails);
            m_menuBoardTable->addAction(m_actionRemoveMatch);
        }

        m_menuBoardTable->popup(ui->tableViewBoard->mapToGlobal(pos));
    }
}

void MainWindow::boardHorizontalContextMenu(const QPoint &pos)
{
    if( pos.isNull() ) { return; }

    QModelIndex index = ui->tableViewBoard->indexAt(pos);
    if( !index.isValid() || index.row() == m_matchs->count() ) { return; }

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
    if( !index.isValid() || index.row() == m_matchs->count() ) { return; }

    const Match* match = m_matchs->matchAt(index.row());
    const Player* player = m_players->playerAt(index.column());

    if( !player->hasMatch(match->Date) ) {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionCreatePlayData);
    } else {
        m_menuBoardTable->clear();
        m_menuBoardTable->addAction(m_actionDeletePlayData);
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
    int columnCount = playerCount*2+1;
    for( int row = 3; row < rowCount+3; row++ )
    {
        sheet.row_properties(xlnt::row_t(row)).height = heightSize;

        match = m_matchs->matchAt(row-3);
        if( match == NULL ) { continue; }

        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).font(fontTemp);
        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).alignment(alignCenter);

        xlnt::date dateTemp(match->Date.year(), match->Date.month(), match->Date.day());
        sheet.cell(xlnt::column_t(1), xlnt::row_t(row)).value(dateTemp);


        for( int column = 2; column < columnCount; column += 2 )
        {
            sheet.cell(xlnt::column_t(column), xlnt::row_t(row)).font(fontTemp);
            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(row)).font(fontTemp);

            player = m_players->playerAt(column/2);
            if( player == NULL ) { continue; }

            playData = player->playData(match->Date);
            if( playData == NULL ) { continue; }

            sheet.cell(xlnt::column_t(column), xlnt::row_t(row)).value(playData->data(PlayDataItem::itemGoal).toInt());
            sheet.cell(xlnt::column_t(column+1), xlnt::row_t(row)).value(playData->data(PlayDataItem::itemAssist).toInt());
        }
    }

    excel.save(fileName.toStdString());
}
