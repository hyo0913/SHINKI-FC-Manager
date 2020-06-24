#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QCalendar>

#include "BoardItem.h"
#include "BoardItemDelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_actionLoadExcel(NULL),
    m_actionAddMatch(NULL),
    m_actionDeleteMatch(NULL),
    m_actionAddPlayer(NULL),
    m_actionDeletePlayer(NULL)
{
    ui->setupUi(this);

    connect(ui->pushButtonAddMatch, SIGNAL(pressed()), this, SLOT(addMatch()));
    connect(ui->pushButtonDeleteMatch, SIGNAL(pressed()), this, SLOT(deleteMatch()));
    connect(ui->pushButtonAddPlayer, SIGNAL(pressed()), this, SLOT(addPlayer()));
    connect(ui->pushButtonDeletePlayer, SIGNAL(pressed()), this, SLOT(deletePlayer()));

    ui->tableWidgetBoard->setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    ui->tableWidgetBoard->setColumnCount(1);
    ui->tableWidgetBoard->setHorizontalHeaderItem(0, new BoardItem("Date"));

    ui->tableWidgetBoard->setItemDelegate(new BoardItemDelegate());

    createActions();
    setupMenuBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    m_actionLoadExcel = new QAction(tr("Open Excel"), this);

    m_actionAddMatch = new QAction(tr("Add Match"), this);
    connect(m_actionAddMatch, SIGNAL(triggered()), this, SLOT(addMatch()));

    m_actionDeleteMatch = new QAction(tr("Delete Match"), this);
    connect(m_actionDeleteMatch, SIGNAL(triggered()), this, SLOT(deleteMatch()));

    m_actionAddPlayer = new QAction(tr("Add Player"), this);
    connect(m_actionAddPlayer, SIGNAL(triggered()), this, SLOT(addPlayer()));

    m_actionDeletePlayer = new QAction(tr("Delete Player"), this);
    connect(m_actionDeletePlayer, SIGNAL(triggered()), this, SLOT(deletePlayer()));
}

void MainWindow::setupMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_actionLoadExcel);

    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(m_actionAddMatch);
    editMenu->addAction(m_actionDeleteMatch);
    editMenu->addAction(m_actionAddPlayer);
    editMenu->addAction(m_actionDeletePlayer);
}

void MainWindow::addMatch()
{
    int row = ui->tableWidgetBoard->rowCount();

    ui->tableWidgetBoard->setRowCount(row+1);

    ui->tableWidgetBoard->setItem(row, 0, new BoardItem(QDate::currentDate().toString("yyyy-MM-dd")));
}

void MainWindow::deleteMatch()
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
    dialog.layout()->addWidget(new QLabel(tr("Input player name")));
    QLineEdit* lineEdit = new QLineEdit();
    dialog.layout()->addWidget(lineEdit);
    dialog.layout()->addWidget(&buttonBox);

    dialog.adjustSize();

    if( dialog.exec() == QDialog::Accepted ) {
        QString name = lineEdit->text();
        if( !name.isEmpty() ) {
            int column = ui->tableWidgetBoard->columnCount();
            ui->tableWidgetBoard->setColumnCount(column+1);
            ui->tableWidgetBoard->setHorizontalHeaderItem(column, new BoardItem(name));
        }
    }
}

void MainWindow::deletePlayer()
{
}

