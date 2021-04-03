#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

struct ColorData
{
    short color[2]; //[0]储存方块本体颜色，[1]表示预览方块（Ghost piece）的颜色
};

struct MatrixData
{
    bool data[4][4];
};

struct Block        //7种方块的数据
{
    ColorData color;        //方块颜色
    MatrixData matrix[4];   //方块形状
}block[7];

struct currentBlock //当前方块
{
    int x,y;    //坐标
    short type; //种类
    int dir;    //方向
}curBlock,preBlock;

short  ground[10][21];  //地图
int score,line,burn,level = 6;  //分数；消行数；burn（Tetris 以外的消行数）；速度级别（从六级开始）
short nextBlocks[7],nextBlockI; //分别是 7-bag 随机系统中本包的七个方块 和对于当前下落中方块紧接着的一个方块
string wideNumbers[10] = {"０","１","２","３","４","５","６","７","８","９"}; //全角数字
short speed[20] = {800,716,633,550,466,383,300,216,133,100,84,83,83,67,67,66,50,50,50,33};  //每个级别方块下落一格的时间（ms）
short linesForLevels[20] = {0,0,0,0,0,0,5,15,15,20,20,20,20,20,20,20,25,30,35}; //（速度）每升一级的行数
//short linesForLevels[20] = {10,20,30,40,50,60,70,80,90,100,100,100,100,100,100,100,110,120,130};  //这是原游戏的数据，由于每级耗费时间太长，实际使用的数据将行数降低


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
        };  //SRS踢墙（wallkick）J, L, S, T, Z 数据

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
        };  //SRS踢墙（wallkick）I 数据


int getRand(int mini,int maxi)  //获取随机数的函数
{
    return rand() % (maxi - mini + 1) + mini;
}

void go(float x,float y)    //光标移动函数，x表示横坐标，y表示纵坐标
{
    COORD coord;            //使用头文件自带的坐标结构
    coord.X = x * 2 + 2;    //传入的x坐标是以一方格为单位的，而实际上一个单位是一个字符，故需乘2；另外加2在左侧适当留白
    coord.Y = y + 1;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);     //获得标准输出句柄
    SetConsoleCursorPosition(a,coord);              //以标准输出的句柄为参数设置控制台光标坐标
}
void color(int a)   //设定颜色的函数
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}//黑 深蓝 深绿 浅蓝  红  深紫  土黄 浅灰 深灰 亮蓝 亮绿  最淡的蓝 桃红 浅紫  米黄  白


void pre()  //初始化
{
    score = line = burn = 0;
    level = 6;
    memset(ground,0,sizeof(ground));

    system("mode con cols=48 lines=28");    //设置窗口大小
    system("cls");
    color(15);

    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //隐藏光标

    block[0].color = {179,48};  //7种方块的初始化
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


    block[1].color = {230,96};
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

    block[2].color = {162,32};
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

    block[3].color = {196,64};
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

    block[4].color = {120,128};
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


    block[5].color = {49,16};

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

    block[6].color = {213,80};
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
    srand(seed);    //设置随机数种子

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

void clearImfBar()  //清空信息栏（右下的框框）
{
    color(15);
    for(int i = 17;i <= 23;i++)
    {
        go(13,i);
        printf("                ");
    }
}

void printScore()   //打印分数
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

void printLine()    //打印行数
{
    color(11);
    go(18,10);
    cout << wideNumbers[line / 100] << wideNumbers[line / 10 % 10] << wideNumbers[line % 10];
    color(15);
}

void printBurn()    //打印burn
{
    color(11);
    go(18,12);
    cout << wideNumbers[burn / 100] << wideNumbers[burn / 10 % 10] << wideNumbers[burn % 10];
    color(15);
}

void printLevel()   //打印关卡数
{
    color(11);
    go(19,14);
    cout << wideNumbers[level / 10] << wideNumbers[level % 10];
    color(15);
}
void printBlock(int mode,currentBlock block1)   //打印方块block1；mode = 1 时打印方块本体，mode = 2 时打印 ghost piece
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

bool wallHittingCheck(int x,int y,currentBlock block1)  //  碰撞检测
{
    for(int i = 0;i < 4;i++)
        for(int j = 0;j < 4;j++)
            if(block[block1.type].matrix[block1.dir].data[i][j] && y + i >= 0)  //当前方块的这个地方不为空且此处没有超过第20行
                if(ground[x + j][y + i] || x + j < 0 || x + j > 9 || y + i > 19)    //这里本来也有方块或当前方块越界了
                    return true;
    return false;
}

void getRandomBlocks(bool itIsTheFirstBlock)    //7-bag 随机生成方块
{
    for(int i = 0;i < 7;i++)
        nextBlocks[i] = i;

    int times = getRand(0,9);
    for(int i = 0;i < times;i++)
        swap(nextBlocks[getRand(0,6)],nextBlocks[getRand(0,6)]);

    if(itIsTheFirstBlock && nextBlocks[0] > 1 && nextBlocks[0] <= 3)   //如果这是第一组方块且第一个方块为S或Z
        getRandomBlocks(true);
}

bool land()     //方块着陆后处理
{
    for(int i = 3;i >= 0;i--)
        for(int j = 0;j < 4;j++)
            if(block[curBlock.type].matrix[curBlock.dir].data[i][j])
            {
                if(curBlock.y + i < 0)
                    return true;
                ground[curBlock.x + j][curBlock.y + i] = curBlock.type + 1; //将类型+1（为了区分开I和空位）储存到地图中
            }
    return false;
}

void printTheNextBlock()    //打印下一个方块
{
    int ki = 0,kj = 0;
    if(block[nextBlocks[nextBlockI]].matrix[0].data[0][0] + block[nextBlocks[nextBlockI]].matrix[0].data[1][0] + block[nextBlocks[nextBlockI]].matrix[0].data[2][0] + block[nextBlocks[nextBlockI]].matrix[0].data[3][0] == 0)
        kj++;
    if(block[nextBlocks[nextBlockI]].matrix[0].data[0][0] + block[nextBlocks[nextBlockI]].matrix[0].data[0][1] + block[nextBlocks[nextBlockI]].matrix[0].data[0][2] + block[nextBlocks[nextBlockI]].matrix[0].data[0][3] == 0)
    {
        ki++;
        if(block[nextBlocks[nextBlockI]].matrix[0].data[1][0] + block[nextBlocks[nextBlockI]].matrix[0].data[1][1] + block[nextBlocks[nextBlockI]].matrix[0].data[1][2] + block[nextBlocks[nextBlockI]].matrix[0].data[1][3] == 0)
            ki++;
    }       //打印位置的处理

    color(15);
    for(int i = 0;i < 2;i++)
        for(int j = 0;j < 4;j++)
        {
            go(14.5 + j,5 + i);
            printf("  ");
        }   //清空预览区域

    color(block[nextBlocks[nextBlockI]].color.color[0]);
    for(int i = ki;i < ki + 2;i++)
        for(int j = kj;j < 4;j++)
            if(block[nextBlocks[nextBlockI]].matrix[0].data[i][j])
            {
                go((nextBlocks[nextBlockI] == 1 ? 15 : 14.5) + j - (kj / 2.0),5 + i - ki);
                printf("■");
            }   //打印
    color(15);
}

void preview()  //预览方块下落的位置（ghost piece）
{
    if(preBlock.dir != -1)
        printBlock(0,preBlock);
    preBlock = curBlock;
    while(!wallHittingCheck(preBlock.x,preBlock.y + 1,preBlock))    //直到不能再下落
        preBlock.y++;
    printBlock(2,preBlock);
}

void effectivelySpin(long long &lockDelay,int x,int y,bool clockWise) //有效地旋转
{
    curBlock.dir = (curBlock.dir + (clockWise ? 1 : 3)) % 4;
    curBlock.x += x;
    curBlock.y += y;
    preview();

    if(lockDelay != -1)
    {
        lockDelay = GetTickCount(); //重置锁定延迟
        while(!wallHittingCheck(curBlock.x,curBlock.y + 1,curBlock))
            curBlock.y++;
    }
}

void tryToSpin(long long &lockDelay,bool clockWise) //尝试旋转，clockWise表示是否为顺时针
{
    currentBlock block1 = curBlock; //临时定义一个旋转后的方块
    block1.dir = (block1.dir + ( clockWise ? 1 : 3)) % 4;
    if(!wallHittingCheck(block1.x,block1.y,block1)) //若旋转后没有碰撞，则旋转
    {
        effectivelySpin(lockDelay,0,0,clockWise);
    }
    else    //否则使用SRS逐个尝试其它备选位置
    {
        if(block1.type != 0)    //若当前方块不是I，使用一般的wallkick数据
        {
            for(int i = 0;i < 4;i++)
            {
                if(!wallHittingCheck(block1.x + wallKickData[curBlock.dir][!clockWise][i][0],block1.y + wallKickData[curBlock.dir][!clockWise][i][1],block1))
                {
                    effectivelySpin(lockDelay,wallKickData[curBlock.dir][!clockWise][i][0],wallKickData[curBlock.dir][!clockWise][i][1],clockWise);
                    break;
                }
            }
        }
        else                    //否则使用I块的wallkick数据
        {
            for(int i = 0;i < 4;i++)
            {
                if(!wallHittingCheck(block1.x + wallKickDataOfI[curBlock.dir][!clockWise][i][0],block1.y + wallKickDataOfI[curBlock.dir][!clockWise][i][1],block1))
                {
                    effectivelySpin(lockDelay,wallKickDataOfI[curBlock.dir][!clockWise][i][0],wallKickDataOfI[curBlock.dir][!clockWise][i][1],clockWise);
                    break;
                }
            }
        }
    }
}

void lineCheck(int y)   //消行检测
{
    short linesN = 0;   //被消的行数
    vector <int>lines;  //被消的行们
    for(int i = y;i <= min(19,y + 3);i++)   //在有变动的行中检测，超出屏幕的行不检测
    {
        bool flag = true;   //每格是否都不为空
        for(int j = 0;j < 10;j++)
            if(ground[j][i] == 0)
                flag = false;
        if(flag)
        {
            linesN++;
            lines.push_back(i);
        }
    }
    if(!linesN) //没有消行则结束函数
        return;

    color(15);

    string nameOfLines[4] = {"  Single! "," Double!! "," Triple!!!","Tetris!!!!"};

    go(14.5,20);
    cout << nameOfLines[linesN - 1];

    for(int j = 0;j < 5;j++)    //消行动画
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

    for(auto k = lines.begin();k < lines.end();k++) //将被消行上方的行移下来
    {
        for(int i = *k;i >= 0;i--)
            for(int j = 0;j < 10;j++)
                if(i)
                    ground[j][i] = ground[j][i - 1];
                else
                    ground[j][i] = 0;
    }

    for(int i = 0;i < min(y + 4,20);i++)    //更新屏幕
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

    if(linesN < 4)
    {
        burn += linesN;
        printBurn();
    }
    line += linesN;
    printLine();
    switch (linesN) //得分
    {
        case 1:
            score += 40 * (level + 1);
            break;
        case 2:
            score += 100 * (level + 1);
            break;
        case 3:
            score += 300 * (level + 1);
            break;
        case 4:
            score += 1200 * (level + 1);
            break;
    }

    printScore();

    int l = line,i = 0;     //计算等级
    while(l > linesForLevels[i])
        l -= linesForLevels[i++];
    level = i;
    printLevel();

    go(14.5,20);
    printf("游戏进行中");
}

int main();

void gaming()   //游戏函数
{
    SetConsoleTitle("俄罗斯方块（ Esc/F1/Enter暂停游戏 ）");
    while(!kbhit())
        Sleep(100);
    int i = getch();    //按下任意键开始

    clearImfBar();

    go(14.5,20);
    printf("游戏进行中");

    nextBlockI = 0;
    getRandomBlocks(true);
    while(score != -1)  //实际上是死循环，score不为1只是为了让我的编译器没有警告而已
    {
        bool speedUp = false;   //是否加速

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

        long long lockDelay = -1;   //用于计算锁定延迟，-1表示未落地
        while(lockDelay == -1 || GetTickCount() - lockDelay < 500)  //未落地或锁定延迟未过完（500ms）时
        {
            if(lockDelay == -1)
                curBlock.y++;
            printBlock(1,curBlock);
            if(speedUp) //若加速
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
                if(key == 224)  //方向键读取（会读入两个数字，第一个是224）
                {
                    key = getch();
                    switch (key)
                    {
                        case 72:    //上
                            tryToSpin(lockDelay,true);
                            break;
                        case 80:    //下
                            speedUp = true;
                            break;
                        case 75:    //左
                            if(!wallHittingCheck(curBlock.x - 1,curBlock.y,curBlock))
                            {
                                curBlock.x--;
                                preview();
                                if(lockDelay != -1)
                                    lockDelay = GetTickCount();
                            }
                            break;
                        case 77:    //右
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
                    tryToSpin(lockDelay,true);  //顺时针旋转
                else if(key == 'Z' || key == 'z')
                    tryToSpin(lockDelay,false); //逆时针旋转
                else if(key == 27 || key == 0 || key == 13 || key == 59)    //暂停
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

                    lockDelay = GetTickCount();

                    printBlock(0,curBlock);
                }
                else if(key == 32)  //硬降
                {
                    curBlock = preBlock;
                    break;
                }
            }
            if(GetKeyState(VK_CONTROL) < 0) //Ctrl逆时针旋转
            {
                tryToSpin(lockDelay,false);
            }
            if(wallHittingCheck(curBlock.x,curBlock.y + 1,curBlock))    //若落地，开始计算锁定延迟
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
    SetConsoleTitle("游戏结束,Esc退出");  //游戏结束
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
    while(key != 27 && key != 0 && key != 59)
    {
        if(key == 13)
        {
            main();
            break;
        }
        key = getch();
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

    return 0;
}
