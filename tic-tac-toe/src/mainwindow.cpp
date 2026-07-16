#include "mainwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QRadioButton>
#include <QMessageBox>
#include <random>
#include <QRandomGenerator>
#include <QButtonGroup>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
    for (int i = 0; i < LENGTH*LENGTH; i++)
    {
        board[i] = Player::None;
    }
    setWindowTitle("Tic-Tac-Toe");
    label = new QLabel(is_Xturn ? "X's turn" : "O's turn", this);
    label->setAlignment(Qt::AlignCenter);
    label->setMinimumHeight(50);

    Xwins = new QLabel("X win times: 0", this);
    Owins = new QLabel("O win times: 0", this);
    auto* hgrid = new QHBoxLayout;
    hgrid -> addWidget(Xwins);
    hgrid -> addWidget(Owins);

    QRadioButton *xFirstRadio = new QRadioButton("X first", this);
    QRadioButton *oFirstRadio = new QRadioButton("O first", this);
    QRadioButton *playwithPeople = new QRadioButton("play with peolple", this);
    QRadioButton *playwithComputer = new QRadioButton("play with computer", this);

    auto* m_firstPlayerGroup = new QButtonGroup(this);
    m_firstPlayerGroup->addButton(xFirstRadio);
    m_firstPlayerGroup->addButton(oFirstRadio);

    auto* m_playModeGroup = new QButtonGroup(this);
    m_playModeGroup->addButton(playwithPeople);
    m_playModeGroup->addButton(playwithComputer);
    
    xFirstRadio->setChecked(true);
    playwithPeople->setChecked(true);

    QHBoxLayout *firstPlayerLayout = new QHBoxLayout;
    firstPlayerLayout->addWidget(xFirstRadio);
    firstPlayerLayout->addWidget(oFirstRadio);
    firstPlayerLayout->addStretch();
    connect(xFirstRadio, &QRadioButton::toggled, 
            this, [this](bool checked){
                if (checked)
                {
                    Xfirst = true;
                    onReset();
                }
            });
    connect(oFirstRadio, &QRadioButton::toggled, 
            this, [this](bool checked){
                if (checked)
                {
                    Xfirst = false;
                    onReset();
                }
            });

    
    QHBoxLayout *PlayWith = new QHBoxLayout;
    PlayWith->addWidget(playwithPeople);
    PlayWith->addWidget(playwithComputer);
    PlayWith->addStretch();
    connect(playwithPeople, &QRadioButton::toggled, 
            this, [this](bool checked){
                if (checked)
                {
                    play_with_computer = false;
                    onReset();
                }
            });
    connect(playwithComputer, &QRadioButton::toggled, 
            this, [this](bool checked){
                if (checked)
                {
                    play_with_computer = true;
                    onReset();
                }
            });
    
    
    
    playwithPeople->setChecked(true);

    auto* grid = new QGridLayout;
    for (int i = 0; i < LENGTH*LENGTH; i++)
    {
        buttonboard[i] = new QPushButton(" ", this);
        QPushButton* b = buttonboard[i];
        b->setMinimumSize(60, 60);
        b->setFont(QFont("Arial", 24));
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(b, &QPushButton::clicked, 
                this, [this, i]() {onSquare(i);});
        grid->addWidget(b, i / LENGTH, i % LENGTH);
    }
    for (int row = 0; row < LENGTH; ++row) {
    grid->setRowStretch(row, 1);
    }
    for (int col = 0; col < LENGTH; ++col) {
    grid->setColumnStretch(col, 1);
    }

    QPushButton* reset = new QPushButton("Reset", this);
    connect(reset, &QPushButton::clicked, 
        this, &MainWindow::onReset);
    reset->setMinimumSize(80, 40);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addLayout(hgrid);
    mainLayout->addLayout(firstPlayerLayout);
    mainLayout->addLayout(PlayWith);
    mainLayout->addLayout(grid);
    mainLayout->setStretchFactor(grid, 1);
    mainLayout->addWidget(reset, 0, Qt::AlignHCenter);
}


bool MainWindow::check_win(Player p, int index, int winsquares[], int& count) 
{
    if (board[index] != p) return false;
    int row = index / LENGTH;
    int col = index % LENGTH;
    int dirs[4][2] = {
        {0, 1},  // 水平（左右）
        {1, 0},  // 垂直（上下）
        {1, 1},  // 主对角线（右下左上）
        {1, -1}  // 副对角线（左下右上）
    };

    for (int d = 0; d < 4; ++d) {
        count = 1;          // 当前这颗棋子
        int pos = 0;            // 用来记录胜利格子的索引
        winsquares[pos++] = index; // 把当前棋子存进胜利列表

        // ---- 方向 1：正方向走（行+，列+） ----
        for (int step = 1; step < LENGTH; ++step) {
            int nr = row + dirs[d][0] * step;
            int nc = col + dirs[d][1] * step;
            if (nr < 0 || nr >= LENGTH || nc < 0 || nc >= LENGTH) break;
            int idx = nr * LENGTH + nc;
            if (board[idx] != p) break;
            winsquares[pos++] = idx;
            count++;
        }

        // ---- 方向 2：反方向走（行-，列-） ----
        for (int step = 1; step < LENGTH; ++step) {
            int nr = row - dirs[d][0] * step;
            int nc = col - dirs[d][1] * step;
            if (nr < 0 || nr >= LENGTH || nc < 0 || nc >= LENGTH) break;
            int idx = nr * LENGTH + nc;
            if (board[idx] != p) break;
            winsquares[pos++] = idx;
            count++;
        }

        // 如果这条线上连续的棋子数量 >= 棋盘胜利长度，就赢了！
        if (count >= 5) {
            return true;
        }
    }
    return false;
}


bool MainWindow::check_draw()
{
    for(int i = 0; i < LENGTH*LENGTH; i++)
    {
        if (board[i] == Player::None)
            return false;
    }
    return true; 
    //对于X或O获胜的情况，将在调用该函数前提前调用check_win函数检查，此处不写
}

void MainWindow::computerplay()
{
    int index;
    while(true)
    {
        index = QRandomGenerator::global()->bounded(LENGTH*LENGTH); 
        if (board[index] == Player::None)//小心死循环！
        {
            break;
        }
    }
    QPushButton* button = buttonboard[index];
    if (is_Xturn)
    {
        button -> setText("X");
        board[index] = Player::X;
        is_Xturn = false;
    }
    else 
    {
        button -> setText("O");
        board[index] = Player::O;
        is_Xturn = true;
    }
    int winsquares[LENGTH];
    int count = 0;
    if (check_win(Player::X, index, winsquares, count))
    {
        finished = true;
        label -> setText("Computer wins!");
        for (int i = 0; i < count; i++) 
        {
            buttonboard[winsquares[i]]->setStyleSheet("background-color: #90EE90;"); // 淡绿色
        }
    }
    else if (check_win(Player::O, index, winsquares, count))
    {
        finished = true;
        label -> setText("Computer wins!");
        for (int i = 0; i < count; i++) 
        {
            buttonboard[winsquares[i]]->setStyleSheet("background-color: #90EE90;"); // 淡绿色
        }
    }
    else if (check_draw())
    {
        finished  = true;
        label -> setText("It's a draw!");
    }
    else
    {
        label -> setText(is_Xturn ? "X's turn" : "O's turn");
    }
}

void MainWindow::onSquare(int index)
{
    QPushButton* button = buttonboard[index];
    if (finished || board[index] != Player::None)
        return; 
    if (is_Xturn)
    {
        button -> setText("X");
        board[index] = Player::X;
        is_Xturn = false;
    }
    else 
    {
        button -> setText("O");
        board[index] = Player::O;
        is_Xturn = true;
    }
    int winsquares[LENGTH];
    int count = 0;
    if (check_win(Player::X, index, winsquares, count))
    {
        finished = true;
        label -> setText("X wins!");
        X_win_times++;
        Xwins->setText("X win times: " + QString::number(X_win_times));
        for (int i = 0; i < count; i++) 
        {
            buttonboard[winsquares[i]]->setStyleSheet("background-color: #90EE90;"); // 淡绿色
        }
    }
    else if (check_win(Player::O, index, winsquares, count))
    {
        finished = true;
        label -> setText("O wins!");
        O_win_times++;
        Owins->setText("O win times: " + QString::number(O_win_times));
        for (int i = 0; i < count; i++) 
        {
            buttonboard[winsquares[i]]->setStyleSheet("background-color: #90EE90;"); // 淡绿色
        }
    }
    else if (check_draw())
    {
        finished  = true;
        label -> setText("It's a draw!");
    }
    else
    {
        label -> setText(is_Xturn ? "X's turn" : "O's turn");
    }
    if (!finished && play_with_computer)
        computerplay();
}

void MainWindow::onReset()
{
    auto ret = QMessageBox::question(this, "Confirm", "Start a new game?");
    if (ret != QMessageBox::Yes) {
    return;
    }
    is_Xturn = Xfirst;
    finished = false;
    label -> setText(Xfirst ? "X's turn" : "O's turn");
    for (int i = 0; i <LENGTH*LENGTH; i++)
    {
        board[i] = Player::None;
        buttonboard[i] -> setText(" ");
        buttonboard[i] -> setStyleSheet("");
    }
}

