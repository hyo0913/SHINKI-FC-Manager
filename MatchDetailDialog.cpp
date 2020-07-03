#include "MatchDetailDialog.h"
#include "ui_MatchDetailDialog.h"

#include "Match.h"
#include "Player.h"

class MatchDetailTreeItem : public QTreeWidgetItem
{
public:
    MatchDetailTreeItem(){}
    ~MatchDetailTreeItem(){}

    QVariant data(int column, int role) const override;

    QString playerName;
    QVariant goal;
    QVariant assist;
};

QVariant MatchDetailTreeItem::data(int column, int role) const
{
    QVariant result;

    if( role == Qt::DisplayRole ) {
        switch( column )
        {
        case 0: result.setValue(playerName); break;
        case 1: result.setValue(goal); break;
        case 2: result.setValue(assist); break;
        case 3: result.setValue(goal.toULongLong()+assist.toULongLong()); break;
        }
    } else {
        result = QTreeWidgetItem::data(column, role);
    }

    return result;
}

/********************************************************************************************************************/
MatchDetailDialog::MatchDetailDialog(const Match *match, const Players *players, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatchDetailDialog)
{
    ui->setupUi(this);

    init(match, players);
}

MatchDetailDialog::~MatchDetailDialog()
{
    delete ui;
}

void MatchDetailDialog::init(const Match *match, const Players *players)
{
    ui->labelDate->setText(match->Date.toString("yyyy.M.d"));

    for( int i = 0; i < match->Players.count(); i++ ) {
        const Player* player = players->player(match->Players.at(i));
        if( player == NULL ) { continue; }

        const PlayData* playData = player->playData(match->Date);
        if( playData == NULL ) { continue; }

        MatchDetailTreeItem* item = new MatchDetailTreeItem();

        item->playerName = player->name();
        item->goal = playData->data(PlayDataItem::itemGoal);
        item->assist = playData->data(PlayDataItem::itemAssist);

        ui->treeWidgetBoard->addTopLevelItem(item);
    }
}
