#ifndef PLAYERDETAILDIALOG_H
#define PLAYERDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class PlayerDetailDialog;
}

class Player;
class PlayerDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerDetailDialog(const Player *player, QWidget *parent = nullptr);
    ~PlayerDetailDialog();

private:
    Ui::PlayerDetailDialog *ui;

    void init(const Player *player);
};

#endif // PLAYERDETAILDIALOG_H
