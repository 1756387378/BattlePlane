#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h> 
#include <time.h>
#include <Windows.h>
#include <graphics.h>
#pragma comment(lib, "Winmm.lib")

#define CANVASX 270                                                     /*��Ϸ��Ϣ�Ŀ��*/
#define CANVASY 768                                                     /*��Ϸ��Ϣ�ĳ���*/
#define CANVASWIDTH 512                                                 /*�����Ŀ��*/
#define CANVASHIGH 768                                                  /*�����ĸ߶�*/
#define MAXENEMY 5                                                      /*���л���*/
#define MAXBULLET 150                                                   /*�ӵ�����*/
#define BATTLEPLANEWIDTH 110                                            /*ս�����*/
#define BATTLEPLANEHIGH 85                                              /*ս������*/
#define BULLETEWIDTH 26													/*�ӵ����*/
#define BULLETHIGH 71													/*�ӵ�����*/
#define ENEMYWIDTH 75													/*�л����*/
#define ENEMYHIGHT 65													/*�л�����*/
#define EBULLETWIDTH 13													/*�л��ӵ����*/
#define EBULLETHIGH 30													/*�л��ӵ�����*/
#define CMD_UP 1                                                        /*��������*/
#define CMD_DOWN 2
#define CMD_LEFT 4
#define CMD_RIGHT 8
#define CMD_SPACE 16

struct Enemy                                                            /*����л��ṹ��*/
{
	int speed_x, speed_y;                                               /*�л����ٶ�*/
	int position_x, position_y;                                         /*�л���λ��*/
	int sx;                                                             /*�ж��Ƿ���X�����ٶ�*/
	int tab;                                                            /*���*/
}enemy[MAXENEMY];

struct BattlePlane                                                      /*����ս���ṹ��*/
{
	int x, y;                                                           /*ս����λ��*/
}battleplane;

struct Bullet                                                           /*�����ӵ��ṹ��*/
{
	int bullet_x, bullet_y;                                             /*�ӵ���λ��*/
	int speed = 6;                                                      /*�ӵ��ٶ�*/
}bullet[MAXBULLET];

struct eBullet                                                          /*�л��ӵ�*/
{
	int ebullet_x, ebullet_y;
	int espeed_x, espeed_y = 4;
}ebullet[3 * MAXBULLET];

//����ȫ�ֱ���
int bulletsum = 0;                                                      /*�ӵ�����*/
int ebulletsum = 0;                                                     /*�л������ӵ�����*/
int sumEnemy = 0;                                                       /*�ɻ�����*/
int score = 0;															/*��ǰ�÷�*/
int flag = 0;                                                           /*���*/
IMAGE backgrand, backgrand2, battleplane1_1, battleplane1_2, gameOver1, gameOver2, ebullet1, ebullet2;
IMAGE bullets1, bullets2, enemys1_1, enemys1_2, enemys2_1, enemys2_2, enemys3_1, enemys3_2, enemys4_1, enemys4_2;
IMAGE enemys5_1, enemys5_2, start, logo, gameover;

//����ԭ��
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
		if (flag == 0)                                                       //������ел�Խ�磬����������
		{
			sumEnemy = rand() % MAXENEMY + 1;
			startup(sumEnemy);
		}

		//������ʱ������ö�������һ��
		//�ӵ��ܳ���Ϸ����
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

//�����ļ�
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

//��ʼ����Ϸ
void startup(int sumEnemy)
{
	//��ʼ���л�
	for (int i = 0; i < sumEnemy; i++)
	{
		enemy[i].sx = rand() % 3;                                          /*2��ʾ�и�X�����ٶȣ�1��ʾ��X�������ٶȣ�0��ʾX����û���ٶ�*/
		if (enemy[i].sx == 1)                                              /*X�������ٶ�*/
			enemy[i].speed_x = rand() % (3 - 1 + 1) + 1;
		else if (enemy[i].sx == 2)                                         /*X�������ٶ�*/
			enemy[i].speed_x = rand() % 3 - 2;
		else                                                               /*û���ٶ�*/
			enemy[i].speed_x = 0;
		enemy[i].speed_y = rand() % (3 - 1 + 1) + 1;                       /*Y������ٶ�*/
		enemy[i].position_x = rand() % (CANVASWIDTH - ENEMYWIDTH);
		enemy[i].position_y = -ENEMYHIGHT / 2;
		for (int j = 0; j < i; j++)
			if (enemy[i].position_x == enemy[j].position_x && enemy[i].position_y == -ENEMYHIGHT / 2)  /*���ܲ�����ͬһ����ʼλ��*/
			{
				enemy[i].position_x = rand() % (CANVASWIDTH - ENEMYWIDTH);
				break;
			}
		enemy[i].tab = 1;
	}
}

//���û��޹�����
void updateWithoutInput(int sumEnemy, int bulletsum)
{
	putimage(0, 0, &backgrand);
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
	/*�ӵ��ƶ�*/
	for (int i = 0; i < bulletsum; i++)
	{
		bullet[i].bullet_y -= bullet[i].speed;
		if (bullet[i].bullet_y > -1)
		{
			putimage(bullet[i].bullet_x, bullet[i].bullet_y, &bullets2, NOTSRCERASE);
			putimage(bullet[i].bullet_x, bullet[i].bullet_y, &bullets1, SRCINVERT);
		}
	}
	for (int i = 0; i < bulletsum; i++)									/*�ж��Ƿ���ел�*/
		for (int j = 0; j < sumEnemy; j++)
			shootDownEnemy(i, j);

	/*�л��ƶ�*/
	for (int i = 0; i < sumEnemy; i++)
	{
		if (enemy[i].position_y < CANVASHIGH && enemy[i].position_x < CANVASWIDTH + 20 && enemy[i].position_x > -ENEMYWIDTH && enemy[i].tab != 0)    /*����Ϸ������*/
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

		if (enemy[i].position_y >= CANVASHIGH || enemy[i].position_x >= CANVASWIDTH + 20 || enemy[i].position_x <= -ENEMYWIDTH)    /*������Ϸ�߿�*/
		{
			enemy[i].tab = 0;
		}

		if (enemy[i].position_y == (rand() % 6 + 15) || enemy[i].position_y == (rand() % 6 + 100) || enemy[i].position_y == (rand() % 6 + 200)
			|| enemy[i].position_y == (rand() % 6 + 300) || enemy[i].position_y == (rand() % 6 + 400))              /*�л������ӵ�*/
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
	for (int i = 0; i < ebulletsum; i++)                                          /*�л��ӵ����ƶ�*/
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

	for (int i = 0; i < sumEnemy; i++)                                  /*�ж��Ƿ����ел�Խ��*/
	{
		if (enemy[i].tab == 1)
			flag = 1;
	}
	gameOver();														/*�ж��Ƿ������Ϸ*/
	 
	gameInformation();
	FlushBatchDraw();
}

//���ел�
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
			/*���ʣ���ӵ�*/
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

//��Ϸ����
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

//��Ϸ��Ϣ
void gameInformation()
{
	TCHAR pt[11][60] = {
		_T("�л�����"),_T("�ӵ�����"),_T("��ǰ�÷�"),_T("˵��:ʹ�á����������������Ʒ���"),_T("�ո�������ӵ�"),
		_T("ע�⣺��Ϸ�ѶȽϴ�"),//_T("���������BUG��������߽���ѧϰ"),_T("����ϵ��m13168138249@163.com"),_T("�������κ�����"),
		_T("��֪BUG���ӵ���һ�����̫�࣬"),_T("Ŀǰ��δ�ҵ�����취")
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

//���û��й�����
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

// ��ȡ��������
int getCommand()
{
	int c = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	return c;
}

// �ַ���������ఴ������
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

// ����
void onUp()
{
	battleplane.y -= 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// ����
void onDown()
{
	battleplane.y += 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// ����
void onLeft()
{
	battleplane.x -= 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

// ����
void onRight()
{
	battleplane.x += 4;
	putimage(battleplane.x, battleplane.y, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y, &battleplane1_1, SRCINVERT);
}

//�����ӵ�
void fire()
{
	putimage(battleplane.x, battleplane.y - 1, &battleplane1_2, NOTSRCERASE);
	putimage(battleplane.x, battleplane.y - 1, &battleplane1_1, SRCINVERT);
	bullet[bulletsum].bullet_x = battleplane.x + BATTLEPLANEWIDTH / 2 - BULLETEWIDTH / 2;
	bullet[bulletsum].bullet_y = battleplane.y - BULLETHIGH / 2;
	bulletsum++;
}