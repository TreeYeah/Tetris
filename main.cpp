#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

struct ColorData
{
    short color[2];
};

struct MatrixData
{
    bool data[4][4];
};

struct Block
{
    ColorData color;
    MatrixData matrix[4];
}block[7];

struct currentBlock
{
    int x,y;
    short type;
    int dir;
}curBlock,preBlock;

short  ground[10][21];
int score,line,burn,level = 6;
short nextBlocks[7],nextBlockI;
string wideNumbers[10] = {"０","１","２","３","４","５","６","７","８","９"};
short speed[20] = {800,716,633,550,466,383,300,216,133,100,84,83,83,67,67,66,50,50,50,33};
short linesForLevels[20] = {0,0,0,0,0,0,5,15,15,20,20,20,20,20,20,20,25,30,35};
//short linesForLevels[20] = {10,20,30,40,50,60,70,80,90,100,100,100,100,100,100,100,110,120,130};

//
short wallKickData[4/*四个初始方向*/][2/*两个旋转方向*/][4/*四个Test*/][2/* x，y坐标 */] =
        {
                {   //0
                        {{-1,0},{-1,-1},{0,2},{-1,2}},  //R
                        {{1,0},{1,-1},{0,2},{1,2}}   //L
                },
                {   //R
                        {{1,0},{1,1},{0,-2},{1,-2}},  //2
                        {{1,0},{1,1},{0,-2},{1,-2}}   //0
                },
                {   //2
                        {{1,0},{1,-1},{0,2},{1,2}},  //L
                        {{-1,0},{-1,-1},{0,2},{-1,2}}   //R
                },
                {   //L
                        {{-1,0},{-1,1},{0,-2},{-1,-2}},  //0
                        {{-1,0},{-1,1},{0,-2},{-1,-2}}   //2
                }
        };

short wallKickDataOfI[4/*四个初始方向*/][2/*两个旋转方向*/][4/*四个Test*/][2/* x，y坐标 */] =
        {
                {   //0
                        {{-2,0},{1,0},{-2,1},{1,-2}},  //R
                        {{-1,0},{2,0},{-1,-2},{2,1}}   //L
                },
                {   //R
                        {{-1,0},{2,0},{-1,-2},{2,1}},  //2
                        {{2,0},{-1,0},{2,-1},{-1,2}}   //0
                },
                {   //2
                        {{2,0},{-1,0},{2,-1},{-1,2}},  //L
                        {{1,0},{-2,0},{1,2},{-2,-1}}   //R
                },
                {   //L
                        {{1,0},{-2,0},{1,2},{-2,-1}},  //0
                        {{-2,0},{1,0},{-2,1},{1,-2}}   //2
                }
        };


int getRand(int mini,int maxi)
{
    return rand() % (maxi - mini + 1) + mini;
}

void go(float x,float y) //光标移动函数，x表示横坐标，y表示纵坐标。
{
    COORD coord;         //使用头文件自带的坐标结构
    coord.X = x * 2 + 2;            //这里将int类型值传给short,不过程序中涉及的坐标值均不会超过short范围
    coord.Y = y + 1;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);  //获得标准输出句柄
    SetConsoleCursorPosition(a,coord);         //以标准输出的句柄为参数设置控制台光标坐标
}
void color(int a)//设定颜色的函数
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}//黑 深蓝 深绿 浅蓝  红  深紫  土黄 浅灰 深灰 亮蓝 亮绿  最淡的蓝 桃红 浅紫  米黄  白


void pre()
{
    score = line = burn = 0;
    level = 6;
    memset(ground,0,sizeof(ground));

    system("mode con cols=48 lines=28");
    system("cls");
    color(15);
    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//隐藏光标

    block[0].color = {197,208};
    block[0].matrix[0] =
            {
                    0,0,0,0,
                    1,1,1,1,
                    0,0,0,0,
                    0,0,0,0
            };
    block[0].matrix[1] =
            {
                    0,0,1,0,
                    0,0,1,0,
                    0,0,1,0,
                    0,0,1,0
            };
    block[0].matrix[2] =
            {
                    0,0,0,0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,0,0
            };
    block[0].matrix[3] =
            {
                    0,1,0,0,
                    0,1,0,0,
                    0,1,0,0,
                    0,1,0,0
            };  //I


    block[1].color = {196,64};
    block[1].matrix[0] =
            {
                    0,1,1,0,
                    0,1,1,0,
                    0,0,0,0,
                    0,0,0,0
            };
    block[1].matrix[1] =
            {
                    0,1,1,0,
                    0,1,1,0,
                    0,0,0,0,
                    0,0,0,0
            };
    block[1].matrix[2] =
            {
                    0,1,1,0,
                    0,1,1,0,
                    0,0,0,0,
                    0,0,0,0
            };
    block[1].matrix[3] =
            {
                    0,1,1,0,
                    0,1,1,0,
                    0,0,0,0,
                    0,0,0,0
            };  //O

    block[2].color = {186,160};
    block[2].matrix[0] =
            {
                    0,0,1,1,
                    0,1,1,0,
                    0,0,0,0,
                    0,0,0,0
            };
    block[2].matrix[1] =
            {
                    0,0,1,0,
                    0,0,1,1,
                    0,0,0,1,
                    0,0,0,0
            };
    block[2].matrix[2] =
            {
                    0,0,0,0,
                    0,0,1,1,
                    0,1,1,0,
                    0,0,0,0
            };
    block[2].matrix[3] =
            {
                    0,1,0,0,
                    0,1,1,0,
                    0,0,1,0,
                    0,0,0,0
            };  //S

    block[3].color = {162,32};
    block[3].matrix[0] =
            {
                    0,1,1,0,
                    0,0,1,1,
                    0,0,0,0,
                    0,0,0,0
            };
    block[3].matrix[1] =
            {
                    0,0,0,1,
                    0,0,1,1,
                    0,0,1,0,
                    0,0,0,0
            };
    block[3].matrix[2] =
            {
                    0,0,0,0,
                    0,1,1,0,
                    0,0,1,1,
                    0,0,0,0
            };
    block[3].matrix[3] =
            {
                    0,0,1,0,
                    0,1,1,0,
                    0,1,0,0,
                    0,0,0,0
            };  //Z

    block[4].color = {57,144};
    block[4].matrix[0] =
            {
                    0,0,0,1,
                    0,1,1,1,
                    0,0,0,0,
                    0,0,0,0
            };
    block[4].matrix[1] =
            {
                    0,0,1,0,
                    0,0,1,0,
                    0,0,1,1,
                    0,0,0,0
            };
    block[4].matrix[2] =
            {
                    0,0,0,0,
                    0,1,1,1,
                    0,1,0,0,
                    0,0,0,0
            };
    block[4].matrix[3] =
            {
                    0,1,1,0,
                    0,0,1,0,
                    0,0,1,0,
                    0,0,0,0
            };  //L


    block[5].color = {145,16};

    block[5].matrix[0] =
            {
                    0,1,0,0,
                    0,1,1,1,
                    0,0,0,0,
                    0,0,0,0
            };
    block[5].matrix[1] =
            {
                    0,0,1,1,
                    0,0,1,0,
                    0,0,1,0,
                    0,0,0,0
            };
    block[5].matrix[2] =
            {
                    0,0,0,0,
                    0,1,1,1,
                    0,0,0,1,
                    0,0,0,0
            };
    block[5].matrix[3] =
            {
                    0,0,1,0,
                    0,0,1,0,
                    0,1,1,0,
                    0,0,0,0
            };  //J

    block[6].color = {230,96};
    block[6].matrix[0] =
            {
                    0,0,1,0,
                    0,1,1,1,
                    0,0,0,0,
                    0,0,0,0
            };
    block[6].matrix[1] =
            {
                    0,0,1,0,
                    0,0,1,1,
                    0,0,1,0,
                    0,0,0,0
            };
    block[6].matrix[2] =
            {
                    0,0,0,0,
                    0,1,1,1,
                    0,0,1,0,
                    0,0,0,0
            };
    block[6].matrix[3] =
            {
                    0,0,1,0,
                    0,1,1,0,
                    0,0,1,0,
                    0,0,0,0
            };  //T


    unsigned seed = time(0);
    srand(seed);//设置随机数种子

    /*以下为界面打印*/

    go(0.5,0);
    printf(" ━━━━━━━━━━━━━━━━━ \n");
    go(0.5,1);
    printf("┃    ＳＣＯＲＥ   ┃\n");
    go(0.5,2);
    printf("┃ 　　　　　      ┃\n");
    go(0.5,3);
    printf(" ━━━━━━━━━━━━━━━━━ \n");

    go(0,4);
    printf(  "□□□□□□□□□□□□     ==NEXT==\n"
             "  □                    □    |        |\n"
             "  □                    □    |        |\n"
             "  □                    □     ========\n"
             "  □                    □\n"
             "  □                    □\n"
             "  □                    □  LINE\n"
             "  □                    □\n"
             "  □                    □  BURN\n"
             "  □                    □\n"
             "  □                    □  LEVEL\n"
             "  □                    □\n"
             "  □                    □  ================\n"
             "  □                    □ |[↑][X]rotate cw|\n"
             "  □                    □ |[Ctrl][Z]rot ccw|\n"
             "  □                    □ |[←][→]move L/R|\n"
             "  □                    □ |[↓]NL soft drop|\n"
             "  □                    □ |[Sp]L hard drop |\n"
             "  □                    □ |                |\n"
             "  □                    □ |按任意键开始游戏|\n"
             "  □                    □  ================\n"
             "  □□□□□□□□□□□□");
}

void clearImfBar()
{
    color(15);
    for(int i = 17;i <= 23;i++)
    {
        go(13,i);
        printf("                ");
    }
}

void printScore()
{
    color(10);
    int a = score;
    for(int i = 0;i < 6;i++)
    {
        go(7.5 - i,2);
        cout << wideNumbers[a % 10];
        a /= 10;
    }
    color(15);
};

void printLine()
{
    color(11);
    go(18,10);
    cout << wideNumbers[line / 100] << wideNumbers[line / 10 % 10] << wideNumbers[line % 10];
    color(15);
}

void printBurn()
{
    color(11);
    go(18,12);
    cout << wideNumbers[burn / 100] << wideNumbers[burn / 10 % 10] << wideNumbers[burn % 10];
    color(15);
}

void printLevel()
{
    color(11);
    go(19,14);
    cout << wideNumbers[level / 10] << wideNumbers[level % 10];
    color(15);
}
void printBlock(int mode,currentBlock block1)
{
    if(mode == 1)
        color(block[block1.type].color.color[0]);
    else if(mode == 2)
        color(block[block1.type].color.color[1]);
    else
        color(15);

    for(int i = 0;i < 4;i++)
        for(int j = 0;j < 4;j++)
            if(block[block1.type].matrix[block1.dir].data[i][j] && block1.y + i >= 0 && !ground[block1.x + j][block1.y + i])
            {
                go(j + block1.x + 1,5 + i + block1.y);
                if(mode)
                {
                    printf("■");
                }
                else
                {
                    printf("  ");
                }
            }
    color(15);
}

bool wallHittingCheck(int x,int y,currentBlock block1)
{
    for(int i = 0;i < 4;i++)
        for(int j = 0;j < 4;j++)
            if(block[block1.type].matrix[block1.dir].data[i][j] && y + i >= 0)
                if(ground[x + j][y + i] || x + j < 0 || x + j > 9 || y + i > 19)
                    return true;
    return false;
}

void getRandomBlocks(bool itIsTheFirstBlock)
{
    for(int i = 0;i < 7;i++)
        nextBlocks[i] = i;

    int times = getRand(0,9);
    for(int i = 0;i < times;i++)
        swap(nextBlocks[getRand(0,6)],nextBlocks[getRand(0,6)]);

    if(itIsTheFirstBlock && nextBlocks[0] >= 1 && nextBlocks[0] <= 3)
        getRandomBlocks(true);
}

bool land()
{
    for(int i = 3;i >= 0;i--)
        for(int j = 0;j < 4;j++)
            if(block[curBlock.type].matrix[curBlock.dir].data[i][j])
            {
                if(curBlock.y + i < 0)
                    return true;
                ground[curBlock.x + j][curBlock.y + i] = curBlock.type + 1;
            }
    return false;
}

void printTheNextBlock()
{
    int ki = 0,kj = 0;
    if(block[nextBlocks[nextBlockI]].matrix[0].data[0][0] + block[nextBlocks[nextBlockI]].matrix[0].data[1][0] + block[nextBlocks[nextBlockI]].matrix[0].data[2][0] + block[nextBlocks[nextBlockI]].matrix[0].data[3][0] == 0)
        kj++;
    if(block[nextBlocks[nextBlockI]].matrix[0].data[0][0] + block[nextBlocks[nextBlockI]].matrix[0].data[0][1] + block[nextBlocks[nextBlockI]].matrix[0].data[0][2] + block[nextBlocks[nextBlockI]].matrix[0].data[0][3] == 0)
    {
        ki++;
        if(block[nextBlocks[nextBlockI]].matrix[0].data[1][0] + block[nextBlocks[nextBlockI]].matrix[0].data[1][1] + block[nextBlocks[nextBlockI]].matrix[0].data[1][2] + block[nextBlocks[nextBlockI]].matrix[0].data[1][3] == 0)
            ki++;
    }

    color(15);
    for(int i = 0;i < 2;i++)
        for(int j = 0;j < 4;j++)
        {
            go(14.5 + j,5 + i);
            printf("  ");
        }

    color(block[nextBlocks[nextBlockI]].color.color[0]);
    for(int i = ki;i < ki + 2;i++)
        for(int j = kj;j < 4;j++)
            if(block[nextBlocks[nextBlockI]].matrix[0].data[i][j])
            {
                go((nextBlocks[nextBlockI] == 1 ? 15 : 14.5) + j - (kj / 2.0),5 + i - ki);
                printf("■");
            }
    color(15);
}

void preview()
{
    if(preBlock.dir != -1)
        printBlock(0,preBlock);
    preBlock = curBlock;
    while(!wallHittingCheck(preBlock.x,preBlock.y + 1,preBlock))
        preBlock.y++;
    printBlock(2,preBlock);
}

void effectiveSpin(long long &lockDelay,int x,int y,bool clockWise)
{
    curBlock.dir = (curBlock.dir + (clockWise ? 1 : 3)) % 4;
    curBlock.x += x;
    curBlock.y += y;
    preview();

    if(lockDelay != -1)
    {
        lockDelay = GetTickCount();
        while(!wallHittingCheck(curBlock.x,curBlock.y + 1,curBlock))
            curBlock.y++;
    }
}

void tryToSpin(long long &lockDelay,bool clockWise)
{
    currentBlock block1 = curBlock;
    block1.dir = (block1.dir + ( clockWise ? 1 : 3)) % 4;
    if(!wallHittingCheck(block1.x,block1.y,block1))
    {
        effectiveSpin(lockDelay,0,0,clockWise);
    }
    else
    {
        if(block1.type != 0)
        {
            for(int i = 0;i < 4;i++)
            {
                if(!wallHittingCheck(block1.x + wallKickData[curBlock.dir][!clockWise][i][0],block1.y + wallKickData[curBlock.dir][!clockWise][i][1],block1))
                {
                    effectiveSpin(lockDelay,wallKickData[curBlock.dir][!clockWise][i][0],wallKickData[curBlock.dir][!clockWise][i][1],clockWise);
                    break;
                }
            }
        }
        else
        {
            for(int i = 0;i < 4;i++)
            {
                if(!wallHittingCheck(block1.x + wallKickDataOfI[curBlock.dir][!clockWise][i][0],block1.y + wallKickDataOfI[curBlock.dir][!clockWise][i][1],block1))
                {
                    effectiveSpin(lockDelay,wallKickDataOfI[curBlock.dir][!clockWise][i][0],wallKickDataOfI[curBlock.dir][!clockWise][i][1],clockWise);
                    break;
                }
            }
        }
    }
}

void lineCheck(int y)
{
    short linesN = 0;
    vector <int>lines;
    for(int i = y;i <= min(19,y + 3);i++)
    {
        bool flag = true;
        for(int j = 0;j < 10;j++)
            if(ground[j][i] == 0)
                flag = false;
        if(flag)
        {
            linesN++;
            lines.push_back(i);
        }
    }
    if(!linesN)
        return;

    color(15);

    string nameOfLines[4] = {"  Single! "," Double!! "," Triple!!!","Tetris!!!!"};

    go(14.5,20);
    cout << nameOfLines[linesN - 1];

    for(int j = 0;j < 5;j++)
    {
        for(auto i = lines.begin();i < lines.end();i++)
        {
            go(1 + 5 + j,5 + *i);
            printf("  ");
            go(1 + 4 - j,5 + *i);
            printf("  ");
        }
        Sleep(100);
    }

    for(auto k = lines.begin();k < lines.end();k++)
    {
        for(int i = *k;i >= 0;i--)
            for(int j = 0;j < 10;j++)
                if(i)
                    ground[j][i] = ground[j][i - 1];
                else
                    ground[j][i] = 0;
    }

    if(linesN < 4)
    {
        burn += linesN;
        printBurn();
    }
    line += linesN;
    printLine();
    switch (linesN)
    {
        case 1:
            score += 40;
            break;
        case 2:
            score += 100;
            break;
        case 3:
            score += 300;
            break;
        case 4:
            score += 1200;
            break;
    }

    for(int i = 0;i < min(y + 4,20);i++)
    {
        for(int j = 0;j < 10;j++)
        {
            go(j + 1,i + 5);
            if(ground[j][i])
            {
                color(block[ground[j][i] - 1].color.color[0]);
                printf("■");
            }
            else
            {
                color(15);
                printf("  ");
            }
        }
    }
    color(15);

    printScore();

    int l = line,i = 0;
    while(l > linesForLevels[i])
        l -= linesForLevels[i++];
    level = i;
    printLevel();

    go(14.5,20);
    printf("游戏进行中");
}

int main();

void gaming()
{
    SetConsoleTitle("俄罗斯方块（ Esc/F1/Enter暂停游戏 ）");
    while(!kbhit())
        Sleep(100);
    int i = getch();

    clearImfBar();

    go(14.5,20);
    printf("游戏进行中");

    nextBlockI = 0;
    getRandomBlocks(true);
    while(score != -1)
    {
        bool speedUp = false;

        curBlock.type = nextBlocks[nextBlockI];
        nextBlockI++;
        if(nextBlockI == 7)
        {
            getRandomBlocks(false);
            nextBlockI = 0;
        }
        printTheNextBlock();

        curBlock.x = 3;
        curBlock.y = -1;
        curBlock.dir = 0;
        if(curBlock.type == 0)
            curBlock.y--;

        if(wallHittingCheck(curBlock.x,curBlock.y + 1,curBlock))
        {
            printBlock(1,curBlock);
            break;
        }

        preview();

        long long lockDelay = -1;
        while(lockDelay == -1 || GetTickCount() - lockDelay < 500)
        {
            if(lockDelay == -1)
                curBlock.y++;
            printBlock(1,curBlock);
            if(speedUp)
            {
                speedUp = false;
                Sleep(speed[level] / 4);
            }
            else
                Sleep(speed[level]);

            printBlock(0,curBlock);
            if(kbhit())
            {
                int key = getch();
                if(key == 224)
                {
                    key = getch();
                    switch (key)
                    {
                        case 72:
                            tryToSpin(lockDelay,true);
                            break;
                        case 80:
                            speedUp = true;
                            break;
                        case 75:
                            if(!wallHittingCheck(curBlock.x - 1,curBlock.y,curBlock))
                            {
                                curBlock.x--;
                                preview();
                                if(lockDelay != -1)
                                    lockDelay = GetTickCount();
                            }
                            break;
                        case 77:
                            if(!wallHittingCheck(curBlock.x + 1,curBlock.y,curBlock))
                            {
                                curBlock.x++;
                                preview();
                                if(lockDelay != -1)
                                    lockDelay = GetTickCount();
                            }
                            break;
                    }
                }
                else if(key == 'X' || key == 'x')
                    tryToSpin(lockDelay,true);
                else if(key == 'Z' || key == 'z')
                    tryToSpin(lockDelay,false);
                else if(key == 27 || key == 0 || key == 13 || key == 59)
                {
                    SetConsoleTitle("暂停中……");

                    go(14.5,20);
                    printf("暂停中……");

                    printBlock(1,curBlock);
                    while(kbhit())
                        Sleep(100);
                    while(!getch())
                        Sleep(100);

                    SetConsoleTitle("二 ……");
                    go(14.5,20);
                    printf("    二    ");
                    Sleep(1000);

                    SetConsoleTitle("一 ……………………");
                    go(14.5,20);
                    printf("    一    ");
                    Sleep(1000);

                    SetConsoleTitle("〓〓〓〓〓〓〓〓〓〓 游戏继续 〓〓〓〓〓〓〓〓〓〓");
                    go(14.5,20);
                    printf("游戏继续！");
                    Sleep(1000);

                    go(14.5,20);
                    printf("游戏进行中");
                    SetConsoleTitle("俄罗斯方块（ Esc/F1/Enter暂停游戏 ）");

                    printBlock(0,curBlock);
                }
                else if(key == 32)
                {
                    curBlock = preBlock;
                    break;
                }
            }
            if(GetKeyState(VK_CONTROL) < 0)
            {
                tryToSpin(lockDelay,false);
            }
            if(wallHittingCheck(curBlock.x,curBlock.y + 1,curBlock))
            {
                if(lockDelay == -1)
                    lockDelay = GetTickCount();
            }
            else
                lockDelay = -1;
        }
        printBlock(1,curBlock);
        preBlock.dir = -1;
        if(land())
            break;
        lineCheck(curBlock.y);
    }
    SetConsoleTitle("游戏结束");
    go(14.5,20);
    printf("          ");
    go(15,17);
    printf("游戏结束");
    go(13.5,19);
    printf("得分：%d",score);

    ifstream infile("Highest.dat",ios::in);
    if(!infile)
    {
        go(13,19);
        printf("读取最高记录失败");
    }
    int highest = -1;
    infile >> highest;
    infile.close();

    if(highest <= score)
    {
        ofstream outfile("Highest.dat",ios::out);
        if(!outfile)
        {
            go(13,20);
            cerr << "保存最高记录失败" << endl;
        }
        outfile << score;
        highest = score;
    }

    go(13.5,21);
    printf("最高：%d",highest);
    if(highest == score)
        printf("(当前)");

    go(13,23);
    printf("Enter进入新游戏");
    int key = getch();
    if(key == 13)
    {
        main();
    }
}

int main()
{
    pre();
    printScore();
    printLine();
    printBurn();
    printLevel();

    gaming();

    go(0,26);
    system("pause");

    return 0;
}
