#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h> 
#include <time.h>
#include <Windows.h>
#include <graphics.h>
#pragma comment(lib, "Winmm.lib")

#define CANVASX 270                                                     /*游戏信息的宽度*/
#define CANVASY 768                                                     /*游戏信息的长度*/
#define CANVASWIDTH 512                                                 /*画布的宽度*/
#define CANVASHIGH 768                                                  /*画布的高度*/
#define MAXENEMY 5                                                      /*最大敌机数*/
#define MAXBULLET 150                                                   /*子弹总数*/
#define BATTLEPLANEWIDTH 110                                            /*战机宽度*/
#define BATTLEPLANEHIGH 85                                              /*战机长度*/
#define BULLETEWIDTH 26													/*子弹宽度*/
#define BULLETHIGH 71													/*子弹长度*/
#define ENEMYWIDTH 75													/*敌机宽度*/
#define ENEMYHIGHT 65													/*敌机长度*/
#define EBULLETWIDTH 13													/*敌机子弹宽度*/
#define EBULLETHIGH 30													/*敌机子弹长度*/
#define CMD_UP 1                                                        /*控制命令*/
#define CMD_DOWN 2
#define CMD_LEFT 4
#define CMD_RIGHT 8
#define CMD_SPACE 16

struct Enemy                                                            /*定义敌机结构体*/
{
	int speed_x, speed_y;                                               /*敌机的速度*/
	int position_x, position_y;                                         /*敌机的位置*/
	int sx;                                                             /*判断是否有X方向速度*/
	int tab;                                                            /*标记*/
}enemy[MAXENEMY];

struct BattlePlane                                                      /*定义战机结构体*/
{
	int x, y;                                                           /*战机的位置*/
}battleplane;

struct Bullet                                                           /*定义子弹结构体*/
{
	int bullet_x, bullet_y;                                             /*子弹的位置*/
	int speed = 6;                                                      /*子弹速度*/
}bullet[MAXBULLET];

struct eBullet                                                          /*敌机子弹*/
{
	int ebullet_x, ebullet_y;
	int espeed_x, espeed_y = 4;
}ebullet[3 * MAXBULLET];

//定义全局变量
int bulletsum = 0;                                                      /*子弹总数*/
int ebulletsum = 0;                                                     /*敌机发射子弹总数*/
int sumEnemy = 0;                                                       /*飞机总数*/
int score = 0;															/*当前得分*/
int flag = 0;                                                           /*标记*/
IMAGE backgrand, backgrand2, battleplane1_1, battleplane1_2, gameOver1, gameOver2, ebullet1, ebullet2;
IMAGE bullets1, bullets2, enemys1_1, enemys1_2, enemys2_1, enemys2_2, enemys3_1, enemys3_2, enemys4_1, enemys4_2;
IMAGE enemys5_1, enemys5_2, start, logo, gameover;

//函数原型
void loading();
void startup(int sumEnemy);
void updateWithoutInput(int sumEnemy, int bulletsum);
void updateWithInput();
int getCommand();
void dispatchCommand(int _cmd);
void onUp();
void onDown();
void onLeft();
void onRight();
void fire();
void gameOver();
void shootDownEnemy(int i, int j);
void gameInformation();

int main()
{
	srand((unsigned)time(NULL));
	initgraph(CANVASWIDTH + CANVASX, CANVASHIGH);
	BeginBatchDraw();
	loading();
	battleplane.x = CANVASWIDTH / 2 - BATTLEPLANEWIDTH / 2;
	battleplane.y = CANVASHIGH - BATTLEPLANEHIGH;
	putimage(0, 0, &backgrand);
	gameInformation();
	while (1)
	{
		if (flag == 0)                                                       //如果所有敌机越界，则重新生成
		{
			sumEnemy = rand() % MAXENEMY + 1;
			startup(sumEnemy);
		}

		//这里有时间可以用队列来搞一下
		//子弹跑出游戏区域
		if (bullet[0].bullet_y < 0 && bulletsum > 0)
		{
			for (int i = 1; i < bulletsum; i++)
			{
				bullet[i - 1].bullet_y = bullet[i].bullet_y;
				bullet[i - 1].bullet_x = bullet[i].bullet_x;
			}
			bulletsum--;
		}
		flag = 0;
		updateWithoutInput(sumEnemy, bulletsum);
		updateWithInput();
		FlushBatchDraw();
		Sleep(20);
	}
	EndBatchDraw();
	mciSendString(_T("close Nevada"), NULL, 0, NULL);
	mciSendString(_T("close enemy2_down"), NULL, 0, NULL);
	mciSendString(_T("close game_over"), NULL, 0, NULL);
	closegraph();
	return 0;
}

//加载文件
void loading()
{
	mciSendString(_T("open music\\Nevada.mp3 alias Nevada"), NULL, 0, NULL);
	mciSendString(_T("open enemy2_down.mp3 alias enemy2_down"), NULL, 0, NULL);
	mciSendString(_T("open music\\game_over.mp3 alias game_over"), NULL, 0, NULL);
	//mciSendString(_T("play Nevada"), NULL, 0, NULL);
	loadimage(&backgrand, _T("GUI\\backgrand_1.jpg"));
	loadimage(&backgrand2, _T("GUI\\backgrand_2.jpg"));
	loadimage(&battleplane1_2, _T("GUI\\Xplane1_2.jpg"), BATTLEPLANEWIDTH, BATTLEPLANEHIGH);
	loadimage(&battleplane1_1, _T("GUI\\Xplane1_1.jpg"), BATTLEPLANEWIDTH, BATTLEPLANEHIGH);
	loadimage(&enemys1_2, _T("GUI\\enemy1_2.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys1_1, _T("GUI\\enemy1_1.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys2_2, _T("GUI\\enemy2_2.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys2_1, _T("GUI\\enemy2_1.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys3_2, _T("GUI\\enemy3_2.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys3_1, _T("GUI\\enemy3_1.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys4_2, _T("GUI\\enemy4_2.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys4_1, _T("GUI\\enemy4_1.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys5_2, _T("GUI\\enemy5_2.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&enemys5_1, _T("GUI\\enemy5_1.jpg"), ENEMYWIDTH, ENEMYHIGHT);
	loadimage(&bullets2, _T("GUI\\bullet1_2.jpg"), BULLETEWIDTH, BULLETHIGH);
	loadimage(&bullets1, _T("GUI\\bullet1_1.jpg"), BULLETEWIDTH, BULLETHIGH);
	loadimage(&gameOver1, _T("GUI\\gameOver1.gif"));
	loadimage(&gameOver2, _T("GUI\\gameOver2.gif"));
	loadimage(&ebullet1, _T("GUI\\enemybullet1_1.jpg"), EBULLETWIDTH, EBULLETHIGH);
	loadimage(&ebullet2, _T("GUI\\enemybullet1_2.jpg"), EBULLETWIDTH, EBULLETHIGH);
}

//初始化游戏
void startup(int sumEnemy)
{
	//初始化敌机
	for (int i = 0; i < sumEnemy; i++)
	{
		enemy[i].sx = rand() % 3;                                          /*2表示有负X方向速度，1表示正X方向有速度，0表示X方向没有速度*/
		if (enemy[i].sx == 1)                                              /*X正方向速度*/
			enemy[i].speed_x = rand() % (3 - 1 + 1) + 1;
		else if (enemy[i].sx == 2)                                         /*X负方向速度*/
			enemy[i].speed_x = rand() % 3 - 2;
		else                                                               /*没有速度*/
			enemy[i].speed_x = 0;
		enemy[i].speed_y = rand() % (3 - 1 + 1) + 1;                       /*Y方向的速度*/
		enemy[i].position_x = rand() % (CANVASWIDTH - ENEMYWIDTH);
		enemy[i].position_y = -ENEMYHIGHT / 2;
		for (int j = 0; j < i; j++)
			if (enemy[i].position_x == enemy[j].position_x && enemy[i].position_y == -ENEMYHIGHT / 2)  /*不能产生在同一个初始位置*/
			{
				enemy[i].position_x = rand() % (CANVASWIDTH - ENEMYWIDTH);
				break;
			}
		enemy[i].tab = 1;
	}
}

//与用户无关输入
void updateWithoutInput(int sumEnemy, int bulletsum)
{
	putimage(0, 0, &backgrand);
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
	/*子弹移动*/
	for (int i = 0; i < bulletsum; i++)
	{
		bullet[i].bullet_y -= bullet[i].speed;
		if (bullet[i].bullet_y > -1)
		{
			putimage(bullet[i].bullet_x, bullet[i].bullet_y, &bullets2, NOTSRCERASE);
			putimage(bullet[i].bullet_x, bullet[i].bullet_y, &bullets1, SRCINVERT);
		}
	}
	for (int i = 0; i < bulletsum; i++)									/*判断是否击中敌机*/
		for (int j = 0; j < sumEnemy; j++)
			shootDownEnemy(i, j);

	/*敌机移动*/
	for (int i = 0; i < sumEnemy; i++)
	{
		if (enemy[i].position_y < CANVASHIGH && enemy[i].position_x < CANVASWIDTH + 20 && enemy[i].position_x > -ENEMYWIDTH && enemy[i].tab != 0)    /*在游戏区域内*/
		{
			enemy[i].position_x += enemy[i].speed_x;
			enemy[i].position_y += enemy[i].speed_y;
			if (i == 0)
			{
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys1_2, NOTSRCERASE);
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys1_1, SRCINVERT);
			}
			else if (i == 1)
			{
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys2_2, NOTSRCERASE);
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys2_1, SRCINVERT);
			}
			else if (i == 2)
			{
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys3_2, NOTSRCERASE);
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys3_1, SRCINVERT);
			}
			else if (i == 3)
			{
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys4_2, NOTSRCERASE);
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys4_1, SRCINVERT);
			}
			else if (i == 4)
			{
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys5_2, NOTSRCERASE);
				putimage(enemy[i].position_x, enemy[i].position_y, &enemys5_1, SRCINVERT);
			}
			else
				continue;
		}

		if (enemy[i].position_y >= CANVASHIGH || enemy[i].position_x >= CANVASWIDTH + 20 || enemy[i].position_x <= -ENEMYWIDTH)    /*到达游戏边框*/
		{
			enemy[i].tab = 0;
		}

		if (enemy[i].position_y == (rand() % 6 + 15) || enemy[i].position_y == (rand() % 6 + 100) || enemy[i].position_y == (rand() % 6 + 200)
			|| enemy[i].position_y == (rand() % 6 + 300) || enemy[i].position_y == (rand() % 6 + 400))              /*敌机产生子弹*/
		{
			if (ebulletsum <= 13)
			{
				ebullet[ebulletsum].ebullet_x = enemy[i].position_x;
				ebullet[ebulletsum].ebullet_y = enemy[i].position_y + ENEMYHIGHT + 1;
				ebullet[ebulletsum].espeed_x = -2;
				ebulletsum++;
				ebullet[ebulletsum].ebullet_x = enemy[i].position_x + ENEMYWIDTH / 2;
				ebullet[ebulletsum].ebullet_y = enemy[i].position_y + ENEMYHIGHT + 1;
				ebullet[ebulletsum].espeed_x = 0;
				ebulletsum++;
				ebullet[ebulletsum].ebullet_x = enemy[i].position_x + ENEMYWIDTH;
				ebullet[ebulletsum].ebullet_y = enemy[i].position_y + ENEMYHIGHT + 1;
				ebullet[ebulletsum].espeed_x = 2;
				ebulletsum++;
			}
		}
	}
	for (int i = 0; i < ebulletsum; i++)                                          /*敌机子弹的移动*/
	{
		ebullet[i].ebullet_x += ebullet[i].espeed_x;
		ebullet[i].ebullet_y += ebullet[i].espeed_y;
		if (ebullet[i].ebullet_y < CANVASHIGH && ebullet[i].ebullet_x < CANVASWIDTH + 20 && ebullet[i].ebullet_x > -EBULLETWIDTH)
		{
			putimage(ebullet[i].ebullet_x, ebullet[i].ebullet_y, &ebullet1, NOTSRCERASE);
			putimage(ebullet[i].ebullet_x, ebullet[i].ebullet_y, &ebullet2, SRCINVERT);
		}
		if (ebullet[i].ebullet_y >= CANVASHIGH || ebullet[i].ebullet_x >= CANVASWIDTH + 20 || ebullet[i].ebullet_x <= -EBULLETWIDTH)
		{
			for (int k = i + 1; k < ebulletsum; k++)
			{
				ebullet[k - 1].ebullet_x = ebullet[k].ebullet_x;
				ebullet[k - 1].ebullet_y = ebullet[k].ebullet_y;
				ebullet[k - 1].espeed_x = ebullet[k].espeed_x;
				ebullet[k - 1].espeed_y = ebullet[k].espeed_y;
			}
			ebulletsum--;
		}
	}

	for (int i = 0; i < sumEnemy; i++)                                  /*判断是否所有敌机越界*/
	{
		if (enemy[i].tab == 1)
			flag = 1;
	}
	gameOver();														/*判断是否结束游戏*/
	 
	gameInformation();
	FlushBatchDraw();
}

//击中敌机
void shootDownEnemy(int i, int j)
{
	if ((enemy[j].position_x - bullet[i].bullet_x <= BULLETEWIDTH - 10 && enemy[j].position_x - bullet[i].bullet_x >= 0)
		|| (bullet[i].bullet_x - enemy[j].position_x <= ENEMYWIDTH - 10 && bullet[i].bullet_x - enemy[j].position_x >= 0))
	{
		if ((bullet[i].bullet_y - enemy[j].position_y <= ENEMYHIGHT - 20 && bullet[i].bullet_y - enemy[j].position_y >= 0 && bulletsum > 0)
			|| (enemy[j].position_y - bullet[i].bullet_y <= BULLETHIGH - 10 && enemy[j].position_y - bullet[i].bullet_y >= 0 && bulletsum > 0))
		{
			mciSendString(_T("play enemy2_down"), NULL, 0, NULL);
			enemy[j].tab = 0;
			enemy[j].position_x = CANVASWIDTH + CANVASX;
			enemy[j].position_y = CANVASHIGH;
			score += 3;
			putimage(0, 0, &backgrand);
			putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
			putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
			for (int k = i + 1; k < bulletsum; k++)
			{
				bullet[k - 1].bullet_y = bullet[k].bullet_y;
				bullet[k - 1].bullet_x = bullet[k].bullet_x;
			}
			bulletsum--;
			/*输出剩余子弹*/
			for (int k = 0; k < bulletsum; k++)
			{
				if (bullet[k].bullet_y > -1)
				{
					putimage(bullet[k].bullet_x, bullet[k].bullet_y, &bullets2, NOTSRCERASE);
					putimage(bullet[k].bullet_x, bullet[k].bullet_y, &bullets1, SRCINVERT);
				}
			}

			for (int t = 0; t < sumEnemy; t++)
			{
				if (enemy[t].position_y < CANVASHIGH && enemy[t].position_x < CANVASWIDTH + 20 && enemy[t].position_x > -ENEMYWIDTH && enemy[t].tab != 0)
				{
					putimage(enemy[t].position_x, enemy[t].position_y, &enemys1_2, NOTSRCERASE);
					putimage(enemy[t].position_x, enemy[t].position_y, &enemys1_1, SRCINVERT);
				}
			}
			gameInformation();
		}
	}
}

//游戏结束
void gameOver()
{
	for (int i = 0; i < sumEnemy; i++)
	{
		if ((enemy[i].position_x - battleplane.x <= (BATTLEPLANEWIDTH - 15) && enemy[i].position_x - battleplane.x >= 0)
			|| ((battleplane.x - enemy[i].position_x <= ENEMYWIDTH - 15) && battleplane.x - enemy[i].position_x >= 0))
		{
			if ((battleplane.y - enemy[i].position_y <= ENEMYHIGHT - 30 && battleplane.y - enemy[i].position_y >= 0)
				|| (enemy[i].position_y - battleplane.y <= BATTLEPLANEHIGH - 30 && enemy[i].position_y - battleplane.y >= 0))
			{
				putimage(CANVASWIDTH / 2 - 128 / 2, CANVASHIGH / 2 - 128 / 2, &gameOver1, NOTSRCERASE);
				putimage(CANVASWIDTH / 2 - 128 / 2, CANVASHIGH / 2 - 128 / 2, &gameOver2, SRCINVERT);
				mciSendString(_T("play game_over"), NULL, 0, NULL);
				gameInformation();
				FlushBatchDraw();
				mciSendString(_T("stop Nevada"), NULL, 0, NULL);
				Sleep(2000);
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				getch();
				exit(0);
			}
		}
	}
	for (int i = 0; i < ebulletsum; i++)
	{
		if ((ebullet[i].ebullet_x - battleplane.x <= BATTLEPLANEWIDTH - 20 && ebullet[i].ebullet_x - battleplane.x >= 0)
			|| ((battleplane.x - ebullet[i].ebullet_x <= EBULLETWIDTH - 20) && battleplane.x - ebullet[i].ebullet_x >= 0))
		{
			if ((battleplane.y - ebullet[i].ebullet_y <= EBULLETHIGH - 20 && battleplane.y - ebullet[i].ebullet_y >= 0)
				|| (ebullet[i].ebullet_y - battleplane.y <= BATTLEPLANEHIGH - 20 && ebullet[i].ebullet_y - battleplane.y >= 0))
			{
				putimage(CANVASWIDTH / 2 - 128 / 2, CANVASHIGH / 2 - 128 / 2, &gameOver1, NOTSRCERASE);
				putimage(CANVASWIDTH / 2 - 128 / 2, CANVASHIGH / 2 - 128 / 2, &gameOver2, SRCINVERT);
				mciSendString(_T("play game_over"), NULL, 0, NULL);
				gameInformation();
				FlushBatchDraw();
				mciSendString(_T("stop Nevada"), NULL, 0, NULL);
				Sleep(2000);
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				getch();
				exit(0);
			}
		}
	}
}

//游戏信息
void gameInformation()
{
	TCHAR pt[11][60] = {
		_T("敌机总数"),_T("子弹总数"),_T("当前得分"),_T("说明:使用↑、↓、←、→控制方向，"),_T("空格键发射子弹"),
		_T("注意：游戏难度较大！"),//_T("如果发现有BUG、建议或者交流学习"),_T("请联系：m13168138249@163.com"),_T("不接受任何批评"),
		_T("已知BUG：子弹按一次输出太多，"),_T("目前尚未找到解决办法")
	};
	TCHAR st[3][20];
	putimage(CANVASWIDTH, 0, &backgrand2);
	outtextxy(CANVASWIDTH + 10, 0 * 20 + 100, pt[0]);
	_stprintf(st[0], _T("%d"), sumEnemy);
	outtextxy(CANVASWIDTH + 100, 0 * 20 + 100, st[0]);
	outtextxy(CANVASWIDTH + 10, 1 * 20 + 100, pt[1]);
	_stprintf(st[1], _T("%d"), bulletsum);
	outtextxy(CANVASWIDTH + 100, 1 * 20 + 100, st[1]);
	outtextxy(CANVASWIDTH + 10, 2 * 20 + 100, pt[2]);
	_stprintf(st[2], _T("%d"), score);
	outtextxy(CANVASWIDTH + 100, 2 * 20 + 100, st[2]);
	outtextxy(CANVASWIDTH + 10, 3 * 20 + 100, pt[3]);
	outtextxy(CANVASWIDTH + 10, 4 * 20 + 100, pt[4]);
	outtextxy(CANVASWIDTH + 10, 5 * 20 + 100, pt[5]);
	outtextxy(CANVASWIDTH + 10, 6 * 20 + 100, pt[6]);
	outtextxy(CANVASWIDTH + 10, 7 * 20 + 100, pt[7]);
	//outtextxy(CANVASWIDTH + 10, 8 * 20 + 100, pt[8]);
	//outtextxy(CANVASWIDTH + 10, 30 * 20 + 100, pt[9]);
	//outtextxy(CANVASWIDTH + 10, 31 * 20 + 100, pt[10]);
	FlushBatchDraw();
}

//与用户有关输入
void updateWithInput()
{
	int c = 0;
	c = getCommand();
	dispatchCommand(c);
	char key1 = 1;
	if (kbhit())
		key1 = getch();
	if (key1 == 32)
		fire();
	gameInformation();
}

// 获取控制命令
int getCommand()
{
	int c = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	return c;
}

// 分发控制命令，多按键处理
void dispatchCommand(int _cmd)
{
	char key1 = 1;
	if ((_cmd & CMD_UP) && battleplane.y >= 0)
		onUp();
	if ((_cmd & CMD_DOWN) && battleplane.y <= CANVASHIGH - BATTLEPLANEHIGH)
		onDown();
	if ((_cmd & CMD_LEFT) && battleplane.x >= -BATTLEPLANEWIDTH / 2)
		onLeft();
	if ((_cmd & CMD_RIGHT) && battleplane.x <= CANVASWIDTH - BATTLEPLANEWIDTH / 2)
		onRight();
}

// 上移
void onUp()
{
	battleplane.y -= 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// 下移
void onDown()
{
	battleplane.y += 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// 左移
void onLeft()
{
	battleplane.x -= 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// 右移
void onRight()
{
	battleplane.x += 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

//发射子弹
void fire()
{
	putimage(battleplane.x, battleplane.y - 1, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y - 1, &battleplane1_1, SRCINVERT);
	bullet[bulletsum].bullet_x = battleplane.x + BATTLEPLANEWIDTH / 2 - BULLETEWIDTH / 2;
	bullet[bulletsum].bullet_y = battleplane.y - BULLETHIGH / 2;
	bulletsum++;
}