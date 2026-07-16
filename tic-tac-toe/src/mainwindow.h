#pragma once
#include <QWidget>


class QLabel;
class QPushButton;
enum class Player{X, O, None};

constexpr int LENGTH = 7;

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:

    bool check_win(Player p, int index, int winsquares[], int& count);
    bool check_draw();
    void onSquare(int index);
    void onReset();
    void onChangefirst();
    void computerplay();
    
    QLabel* label;
    QLabel* Xwins;
    QLabel* Owins;
    QPushButton* buttonboard[LENGTH*LENGTH];
    Player board[LENGTH*LENGTH];
    int X_win_times = 0;
    int O_win_times = 0;
    bool is_Xturn = true;
    bool Xfirst = true;
    bool finished = false;
    bool play_with_computer = false;
};