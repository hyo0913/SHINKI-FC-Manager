#ifndef MATCHDETAILDIALOG_H
#define MATCHDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class MatchDetailDialog;
}

class Match;
class Players;
class MatchDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatchDetailDialog(const Match *match, const Players *players, QWidget *parent = nullptr);
    ~MatchDetailDialog();

private:
    Ui::MatchDetailDialog *ui;

    void init(const Match *match, const Players *players);
};

#endif // MATCHDETAILDIALOG_H
