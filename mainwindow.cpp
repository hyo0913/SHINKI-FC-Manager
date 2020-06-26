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

#include "BoardHeader.h"
#include "BoardModel.h"
#include "BoardItemDelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_actionLoadExcel(NULL),
    m_actionAddMatch(NULL),
    m_actionRemoveMatch(NULL),
    m_actionAddPlayer(NULL),
    m_actionRemovePlayer(NULL),
    m_boardModel(NULL)
{
    ui->setupUi(this);

    // header
    QFrame* headerFrame = new QFrame();
    headerFrame->setFrameShape(QFrame::StyledPanel);
    headerFrame->setFrameShadow(QFrame::Sunken);
    headerFrame->setLineWidth(1);
    headerFrame->setMidLineWidth(0);
    headerFrame->setLayout(new QHBoxLayout());
    headerFrame->layout()->setMargin(0);

    BoardHorizontalHeader* columnHeader = new BoardHorizontalHeader();
    headerFrame->layout()->addWidget(columnHeader);

    ui->verticalLayout->insertWidget(0, headerFrame);

    connect(ui->tableViewBoard->verticalHeader(), SIGNAL(sectionResized(int, int, int)), columnHeader, SLOT(sectionResize(int, int, int)));

    // board, model
    m_boardModel = new BoardModel(&m_matchs, &m_players);
    ui->tableViewBoard->setModel(m_boardModel);
    ui->tableViewBoard->setItemDelegate(new BoardItemDelegate());

    ui->tableViewBoard->verticalHeader()->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableViewBoard->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->tableViewBoard->horizontalHeader()->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableViewBoard->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    m_boardModel->setHorizontalHeader(columnHeader);

    createActions();
    setupMenuBar();

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
    m_actionLoadExcel = new QAction(tr("Load Excel"), this);

    m_actionAddMatch = new QAction(tr("Add Match"), this);
    connect(m_actionAddMatch, SIGNAL(triggered()), this, SLOT(addMatch()));

    m_actionRemoveMatch = new QAction(tr("Remove Match"), this);
    connect(m_actionRemoveMatch, SIGNAL(triggered()), this, SLOT(removeMatch()));

    m_actionAddPlayer = new QAction(tr("Add Player"), this);
    connect(m_actionAddPlayer, SIGNAL(triggered()), this, SLOT(addPlayer()));

    m_actionRemovePlayer = new QAction(tr("Remove Player"), this);
    connect(m_actionRemovePlayer, SIGNAL(triggered()), this, SLOT(removePlayer()));
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_actionLoadExcel);

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
}
