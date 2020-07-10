#include "PlayerDetailDialog.h"
#include "ui_PlayerDetailDialog.h"

#include "Player.h"

PlayerDetailDialog::PlayerDetailDialog(const Player *player, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerDetailDialog)
{
    ui->setupUi(this);

    init(player);
}

PlayerDetailDialog::~PlayerDetailDialog()
{
    delete ui;
}

void PlayerDetailDialog::init(const Player *player)
{
    if( player == NULL ) { return; }

    ui->labelName->setText(player->name());

    uint appearances = player->matchCount();
    uint goals = 0;
    uint assists = 0;

    QList<QDate> dates = player->matchDates();
    QDate date;
    while( dates.count() > 0 ) {
        date = dates.takeFirst();
        goals += player->data(date, PlayDataItem::itemGoal).toUInt();
        assists += player->data(date, PlayDataItem::itemAssist).toUInt();
    }

    ui->labelAppearancesVal->setText(QString::number(appearances));
    ui->labelGoalsVal->setText(QString::number(goals));
    ui->labelAssistsVal->setText(QString::number(assists));

    double goalAverage = goals;
    double assistAverage = assists;

    if( goalAverage != 0 && appearances != 0 ) {
        goalAverage /= appearances;
    }
    if( assistAverage != 0 && appearances != 0 ) {
        assistAverage /= appearances;
    }

    ui->labelGoalAverageVal->setText(QString::number(goalAverage, 'f', 2));
    ui->labelAssistAverageVal->setText(QString::number(assistAverage, 'f', 2));
}
