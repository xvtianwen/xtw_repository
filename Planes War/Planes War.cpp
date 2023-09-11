#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<time.h>

#define WIDTH 1200  //窗口宽度
#define HEIGHT 800  //窗口高度
#define BULLET_NUM 80  //玩家子弹数量
#define Enemy_BULLET_NUM 15  //敌人子弹数量
#define ENEMY_NUM 8  //  敌人数量
#define COUNT   550   //敌人移动次数
struct Image//图像
{
	IMAGE backGround; //背景
	IMAGE plane[2];  //玩家
	IMAGE bullet[2];  //玩家子弹
	IMAGE enemy_bullet[6];//敌机子弹
	IMAGE enemy[6];   //敌机
	IMAGE bomb[2];    //爆炸效果
}image;

enum TYPE//敌机类型
{
	SMALL, //0
	MIDDLE, //1
	BIG,   //2

};
struct Plane//玩家，敌机，子弹
{
	//坐标
	float x;
	float y;
	union
	{
		int type;//敌机类型
		int score;//玩家得分
	}un;
	bool flag;//是否存活
	int hp;//血量
	int count;//玩家杀敌数量
	int mark ;//敌机是否击落标志绘制爆炸效果
	int mark_1;//敌机子弹绘制类型
	int bekill;

	int move_type;//敌军移动类型（左右下）
	int move_count;//敌军移动次数
}player,bullet[BULLET_NUM],Sbullet[2][BULLET_NUM], SSbullet[2][BULLET_NUM],
 enemy[ENEMY_NUM],enemy_bullet[Enemy_BULLET_NUM];





void EnemyHp(int i);//敌军血量
void GameInit();//初始化
void GameDraw();//绘制
void CreateBullet();//创建玩家子弹
void BulletMove(float speed);//玩家子弹移动
void EnemyBulletMove(float speed);//敌机子弹移动
void EnemyBullet();//创建敌机子弹
void CreatEnemy();//创建敌机
void EnemyMove(float speed);//敌机移动
void KillEnemy();//击落敌机
void Behurt();//玩家受到攻击
void GameControl(float speed);//游戏控制//玩家移动和开火//敌机创建和移动
void Showscore();//显示玩家分数
void Showlife();//显示玩家生命值
void Showkillcount();//显示玩家杀敌数
void Begingame();//游戏开始界面
void Endgame();//游戏结束画面
 //DWORD WINAPI PlayMusic(LPVOID lp);//射击音效问题


int main()

{
	 
	Begingame();//游戏开始界面
	 
	initgraph(WIDTH, HEIGHT);//绘制窗口
	GameInit();     //游戏数据初始化
	
	
	//BeginBatchDraw();//双缓冲  解决闪屏问题
	
	while (1)
	{
		BeginBatchDraw();
		
		//控制我们的战机以及敌机活动，相互伤害
		GameControl(1.5);
		//游戏画面绘制
		GameDraw();
		Showscore();//玩家得分
		Showlife();//玩家生命
		Showkillcount();//玩家杀敌数
		
		//FlushBatchDraw();
		if (player.hp <= 0)//玩家阵亡，游戏结束标志
		{
			player.bekill++;
			EndBatchDraw();
			break;
		}
		 EndBatchDraw();
		
	}

	//EndBatchDraw();
	//closegraph;
	Endgame();//游戏结束函数
	return 0;
}






DWORD64 t1, t2, t3, t4, t5, t6;

//敌军血量和类型初始化
void EnemyHp(int i)
{
	if (rand() % 6 == 0)
	{
		enemy[i].un.type = BIG;
		enemy[i].hp = 3;
	}
	else if (rand() % 4 == 0)
	{
		enemy[i].un.type = MIDDLE;
		enemy[i].hp = 2;
	}
	else if (rand() % 1 == 0)
	{
		enemy[i].un.type = SMALL;
		enemy[i].hp = 1;
	}
}

//初始化
void GameInit()
{
	//设置随机种子
	srand(GetTickCount64());
	t1 = t2 = t3 = t4 = t5 = t6 = GetTickCount64();//系统开机到现在经过的毫秒数

	//播放音乐
	mciSendString("open ./music/史诗.mp3 alias BGM ", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);

	//加载背景图片,玩家，敌机，子弹
	loadimage(&image.backGround, "./image/background.png");

	loadimage(&image.plane[0], "./image/plane_1.png");
	loadimage(&image.plane[1], "./image/plane_2.png");

	loadimage(&image.bullet[0], "./image/bullet_1.png");
	loadimage(&image.bullet[1], "./image/bullet_2.png");


	loadimage(&image.enemy[0], "./image/enemy plane_1_1.png");
	loadimage(&image.enemy[1], "./image/enemy plane_1_2.png");
	loadimage(&image.enemy[2], "./image/enemy plane_2_1.png");
	loadimage(&image.enemy[3], "./image/enemy plane_2_2.png");
	loadimage(&image.enemy[4], "./image/enemy plane_3_1.png");
	loadimage(&image.enemy[5], "./image/enemy plane_3_2.png");
	loadimage(&image.enemy_bullet[0], "./image/enemy_bullet_1_1.png");
	loadimage(&image.enemy_bullet[1], "./image/enemy_bullet_1_2.png");
	loadimage(&image.enemy_bullet[2], "./image/enemy_bullet_2_1.png");
	loadimage(&image.enemy_bullet[3], "./image/enemy_bullet_2_2.png");
	loadimage(&image.enemy_bullet[4], "./image/enemy_bullet_3_1.png");
	loadimage(&image.enemy_bullet[5], "./image/enemy_bullet_3_2.png");

	loadimage(&image.bomb[0], "./image/bool_1.jpg");
	loadimage(&image.bomb[1], "./image/bool_2.jpg");

	//初始化玩家数据
	player.x = WIDTH / 2 - 75;
	player.y = HEIGHT - 350;
	player.flag = true;
	player.hp = 100;
	player.un.score = 0;
	player.count = 0;
	player.bekill = 0;

	//初始化玩家子弹
	 
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;
		Sbullet[0][i].flag = false;
		Sbullet[1][i].flag = false;
		SSbullet[0][i].flag = false;
		SSbullet[1][i].flag = false;
	}
	 

	//初始化敌机子弹
	for (int i = 0; i < Enemy_BULLET_NUM; i++)
	{
		enemy_bullet[i].flag = false;
		enemy_bullet[i].mark_1 = 0;
		enemy_bullet[i].mark = 1;

	}
	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].flag = false;
		enemy[i].mark = 1;
		 
	}

}
//绘制
void GameDraw()
{
	//绘制背景
	 putimage(0, 0, &image.backGround);//背景图必须在这，因为设置了图片缓冲
	//绘制玩家
	putimage(player.x, player.y, &image.plane[0], NOTSRCERASE);
	putimage(player.x, player.y, &image.plane[1], SRCINVERT);
	//绘制玩家子弹

	for (int i = 0; i < BULLET_NUM; i++)
	{
		//一级火炮
		if (bullet[i].flag == true)
		{
			putimage(bullet[i].x, bullet[i].y, &image.bullet[0], NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &image.bullet[1], SRCINVERT);
			
		}
		//二级火炮
		if (Sbullet[0][i].flag == true)
		{
			putimage(Sbullet[0][i].x, Sbullet[0][i].y, &image.bullet[0], NOTSRCERASE);
			putimage(Sbullet[0][i].x, Sbullet[0][i].y, &image.bullet[1], SRCINVERT);

		}
		if (Sbullet[1][i].flag == true)
		{
			putimage(Sbullet[1][i].x, Sbullet[1][i].y, &image.bullet[0], NOTSRCERASE);
			putimage(Sbullet[1][i].x, Sbullet[1][i].y, &image.bullet[1], SRCINVERT);

		}
		//三级火炮
		if (SSbullet[0][i].flag == true)
		{
			putimage(SSbullet[0][i].x, SSbullet[0][i].y, &image.bullet[0], NOTSRCERASE);
			putimage(SSbullet[0][i].x, SSbullet[0][i].y, &image.bullet[1], SRCINVERT);

		}
		if (SSbullet[1][i].flag == true)
		{
			putimage(SSbullet[1][i].x, SSbullet[1][i].y, &image.bullet[0], NOTSRCERASE);
			putimage(SSbullet[1][i].x, SSbullet[1][i].y, &image.bullet[1], SRCINVERT);

		}
	}
	//绘制敌机

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag == true)
		{   
			//绘制一号敌机
			if (enemy[i].un.type == TYPE::SMALL)
			{
				putimage(enemy[i].x, enemy[i].y, &image.enemy[0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.enemy[1], SRCINVERT);
			}
			//绘制二号敌机
			else if (enemy[i].un.type == TYPE::MIDDLE)
			{
				putimage(enemy[i].x, enemy[i].y, &image.enemy[2], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.enemy[3], SRCINVERT);
			}
			//绘制三号敌机
			else if (enemy[i].un.type == TYPE::BIG)
			{
				putimage(enemy[i].x, enemy[i].y, &image.enemy[4], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &image.enemy[5], SRCINVERT);
			}

		}
		  //绘制敌机爆炸效果
		else if (enemy[i].flag == false && enemy[i].mark == -1)
		{
			//enemy[i].mark = 1;
			putimage(enemy[i].x, enemy[i].y, &image.bomb[0], NOTSRCERASE);
			putimage(enemy[i].x, enemy[i].y, &image.bomb[1], SRCINVERT);


		}
	}

	//绘制敌机子弹

	for (int i = 0; i < Enemy_BULLET_NUM; i++)
	{
		if (enemy_bullet[i].flag == true)
		{

			//敌机一号子弹
			if (enemy_bullet[i].mark_1 == 1)
			{
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[0], NOTSRCERASE);
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[1], SRCINVERT);
			}
			//敌机二号子弹
			else if (enemy_bullet[i].mark_1 == 2)
			{
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[2], NOTSRCERASE);
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[3], SRCINVERT);
			}
			//敌机三号子弹
			else if (enemy_bullet[i].mark_1 == 3)
			{
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[4], NOTSRCERASE);
				putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.enemy_bullet[5], SRCINVERT);
			}
 

		}
		else if (enemy_bullet[i].flag == false && enemy_bullet[i].mark == -1)
		{   //绘制子弹相撞时爆炸效果
			//enemy_bullet[i].mark = 1;
			putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.bomb[0], NOTSRCERASE);
			putimage(enemy_bullet[i].x, enemy_bullet[i].y, &image.bomb[1], SRCINVERT);


		}
	}




}


//创建玩家子弹
void CreateBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		//创建玩家一级火炮
		if (bullet[i].flag == false)
		{
			bullet[i].flag = true;

			bullet[i].x = player.x + 50;
			bullet[i].y = player.y - 10;
			break;
		}
		
	
	}
	for (int i = 0; i < BULLET_NUM; i++) {
		//创建玩家二级火炮
		if (Sbullet[0][i].flag == false && player.un.score >= 10)
		{
			Sbullet[0][i].flag = true;

			Sbullet[0][i].x = player.x - 20;
			Sbullet[0][i].y = player.y + 30;
			
		}
		if (Sbullet[1][i].flag == false && player.un.score >= 10)
		{
			Sbullet[1][i].flag = true;

			Sbullet[1][i].x = player.x + 130;
			Sbullet[1][i].y = player.y + 30;
			break;
		}
	}

	for (int i = 0; i < BULLET_NUM; i++) {

		//创建玩家三级火炮
		if (SSbullet[0][i].flag == false && player.un.score >= 20)
		{
			SSbullet[0][i].flag = true;

			SSbullet[0][i].x = player.x - 30;
			SSbullet[0][i].y = player.y + 40;
			
		}
		if (SSbullet[1][i].flag == false && player.un.score >= 20)
		{
			SSbullet[1][i].flag = true;

			SSbullet[1][i].x = player.x + 140;
			SSbullet[1][i].y = player.y + 40;
			break;
		}

	}
}

//玩家子弹移动
void BulletMove(float speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		//一级炮弹 一束直线子弹
		if (bullet[i].flag == true)
		{
			bullet[i].y -= speed;

			if (bullet[i].y < 0)

			{
				bullet[i].flag = false;
			}
		}
		//二级炮弹  添加 两束直线子弹
		if (Sbullet[0][i].flag == true)
		{
			Sbullet[0][i].y -= speed;

			if (Sbullet[0][i].y < 0)

			{
				Sbullet[0][i].flag = false;
			}
		}
		if (Sbullet[1][i].flag == true)
		{
			Sbullet[1][i].y -= speed;

			if (Sbullet[1][i].y < 0)

			{
				Sbullet[1][i].flag = false;
			}
		}
		//三级炮弹  再添加 两束斜线子弹
		if (SSbullet[0][i].flag == true)
		{
			SSbullet[0][i].x -= speed;
			SSbullet[0][i].y -= 1.7*speed;
			if (SSbullet[0][i].y < 0|| SSbullet[0][i].x<0)

			{
				SSbullet[0][i].flag = false;
			}
		}
		if (SSbullet[1][i].flag == true)
		{
			SSbullet[1][i].x += speed;
			SSbullet[1][i].y -= 1.7 * speed;
			if (SSbullet[1][i].y < 0 || SSbullet[1][i].x >WIDTH )

			{
				SSbullet[1][i].flag = false;
			}
		}
	}

}

//玩家击落敌机
void KillEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag == true)//敌机仍然存活
		{
			//玩家子弹击中敌机
			for (int j = 0; j < BULLET_NUM; j++)
			{
				//一级炮弹击中敌机
				if (bullet[j].flag == true)
				{
					if (bullet[j].x > enemy[i].x && bullet[j].x < enemy[i].x + 100
						&& bullet[j].y>enemy[i].y - 100 && bullet[j].y < enemy[i].y)
					{
						bullet[j].flag = false;
						enemy[i].hp--;

					}
				}
				//二级炮弹击中敌机
				if (Sbullet[0][j].flag == true)
				{
					if (Sbullet[0][j].x > enemy[i].x && Sbullet[0][j].x < enemy[i].x + 100
						&& Sbullet[0][j].y>enemy[i].y - 100 && Sbullet[0][j].y < enemy[i].y)
					{
						Sbullet[0][j].flag = false;
						enemy[i].hp--;

					}
				}
				if (Sbullet[1][j].flag == true)
				{
					if (Sbullet[1][j].x > enemy[i].x && Sbullet[1][j].x < enemy[i].x + 100
						&& Sbullet[1][j].y>enemy[i].y - 100 && Sbullet[1][j].y < enemy[i].y)
					{
						Sbullet[1][j].flag = false;
						enemy[i].hp--;

					}
				}
				//三级炮弹击中敌机
				if (SSbullet[0][j].flag == true)
				{
					if (SSbullet[0][j].x > enemy[i].x && SSbullet[0][j].x < enemy[i].x + 100
						&& SSbullet[0][j].y>enemy[i].y - 100 && SSbullet[0][j].y < enemy[i].y)
					{
						SSbullet[0][j].flag = false;
						enemy[i].hp--;

					}
				}
				if (SSbullet[1][j].flag == true)
				{
					if (SSbullet[1][j].x > enemy[i].x && SSbullet[1][j].x < enemy[i].x + 100
						&& SSbullet[1][j].y>enemy[i].y - 100 && SSbullet[1][j].y < enemy[i].y)
					{
						SSbullet[1][j].flag = false;
						enemy[i].hp--;

					}
				}

			}
			//击落敌机并加分
			if (enemy[i].hp <= 0)
			{
				enemy[i].flag = false;
				player.count++;
				enemy[i].mark = -1;

				if (enemy[i].un.type == TYPE::SMALL)
				{
					player.un.score++;
				}

				else if (enemy[i].un.type == TYPE::MIDDLE)
				{
					player.un.score += 2;
				}

				else if (enemy[i].un.type == TYPE::BIG)
				{
					player.un.score += 3;
				}



			}
		}


	}

}

//玩家受到攻击
void Behurt()
{
	//敌机子弹击中玩家
	for (int i = 0; i < Enemy_BULLET_NUM; i++)
	{
		if (enemy_bullet[i].flag == true)
		{

			if (enemy_bullet[i].x > player.x && enemy_bullet[i].x < player.x + 100
				&& enemy_bullet[i].y>player.y && enemy_bullet[i].y < player.y + 100)
			{
				enemy_bullet[i].flag = false;
				player.hp--;

			}


		}
	}
	//敌机本身击中玩家
	for (int j = 0; j < ENEMY_NUM; j++)
	{
		if (enemy[j].flag == true)
		{

			if (enemy[j].x > player.x - 40 && enemy[j].x < player.x + 80
				&& enemy[j].y>player.y - 40 && enemy[j].y < player.y + 60)
			{
				enemy[j].flag = false;

				enemy[j].mark = -1;
				player.count++;
				player.hp -= 10;
			}


		}
	}

}


//玩家敌机子弹相撞
void crash_bullet() {

	for (int i = 0; i < Enemy_BULLET_NUM; i++)//遍历敌军子弹
	{
		if (enemy_bullet[i].flag == true)
		{

			for (int j = 0; j < BULLET_NUM; j++)//遍历我方子弹
			{
				//一级炮弹相撞
				if (bullet[j].flag == true)
				{
					if (bullet[j].x > enemy_bullet[i].x -50&& bullet[j].x <enemy_bullet[i].x + 50
						&& bullet[j].y>enemy_bullet[i].y - 100 && bullet[j].y < enemy_bullet[i].y)
					{
						bullet[j].flag = false;
						enemy_bullet[i].flag = false;
						enemy_bullet[i].mark = -1;

					}
				}
				//二级炮弹相撞
				if (Sbullet[0][j].flag == true)
				{
					if (Sbullet[0][j].x > enemy_bullet[i].x && Sbullet[0][j].x < enemy_bullet[i].x + 100
						&& Sbullet[0][j].y>enemy_bullet[i].y - 100 && Sbullet[0][j].y < enemy_bullet[i].y)
					{
						Sbullet[0][j].flag = false;
						enemy_bullet[i].flag = false;
						enemy_bullet[i].mark = -1;
					}
				}
				if (Sbullet[1][j].flag == true)
				{
					if (Sbullet[1][j].x > enemy_bullet[i].x && Sbullet[1][j].x < enemy_bullet[i].x + 100
						&& Sbullet[1][j].y>enemy_bullet[i].y - 100 && Sbullet[1][j].y < enemy_bullet[i].y)
					{
						Sbullet[1][j].flag = false;
						enemy_bullet[i].flag = false;
						enemy_bullet[i].mark = -1;

					}
				}
				//三级炮弹相撞
				if (SSbullet[0][j].flag == true)
				{
					if (SSbullet[0][j].x > enemy_bullet[i].x && SSbullet[0][j].x < enemy_bullet[i].x + 100
						&& SSbullet[0][j].y>enemy_bullet[i].y - 100 && SSbullet[0][j].y < enemy_bullet[i].y)
					{
						SSbullet[0][j].flag = false;
						enemy_bullet[i].flag = false;
						enemy_bullet[i].mark = -1;

					}
				}
				if (SSbullet[1][j].flag == true)
				{
					if (SSbullet[1][j].x > enemy_bullet[i].x && SSbullet[1][j].x < enemy_bullet[i].x + 100
						&& SSbullet[1][j].y>enemy_bullet[i].y - 100 && SSbullet[1][j].y < enemy_bullet[i].y)
					{
						SSbullet[1][j].flag = false;
						enemy_bullet[i].flag = false;
						enemy_bullet[i].mark = -1;

					}
				}

			}
		}
	}

	

}

//创建敌机子弹
void EnemyBullet()
{
	for (int j = 0; j < ENEMY_NUM; j++)
	{
		if (enemy[j].flag == true)
		{


			for (int i = 0; i < Enemy_BULLET_NUM; i++)
			{


				if (enemy_bullet[i].flag == false)
				{
					enemy_bullet[i].flag = true;
					enemy_bullet[i].mark = 1;

					//一号子弹
					if (enemy[j].un.type == TYPE::SMALL)
					{
						enemy_bullet[i].x = enemy[j].x + 30;
						enemy_bullet[i].y = enemy[j].y + 70;
						enemy_bullet[i].mark_1 = 1;
						
					}
					//二号子弹
					else if (enemy[j].un.type == TYPE::MIDDLE)
					{
						enemy_bullet[i].x = enemy[j].x + 30;
						enemy_bullet[i].y = enemy[j].y + 70;
						enemy_bullet[i].mark_1 = 2;
						
					}
					//三号子弹
					else if (enemy[j].un.type == TYPE::BIG)
					{
						enemy_bullet[i].x = enemy[j].x + 55;
						enemy_bullet[i].y = enemy[j].y + 100;
						enemy_bullet[i].mark_1 = 3;
					
					}
					break;

				}

			}

		}


	}



}

//敌机子弹移动
void EnemyBulletMove(float speed)
{

	for (int i = 0; i < Enemy_BULLET_NUM; i++)
	{
		if (enemy_bullet[i].flag == true)
		{
			enemy_bullet[i].y += speed;

			if (enemy_bullet[i].y > HEIGHT)//判断子弹是否出界

			{
				enemy_bullet[i].flag = false;
			}
		}
	}

}

//创建敌机
void CreatEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		 
		if (enemy[i].flag == false)
		{
			enemy[i].flag = true;//敌机激活
			//随机产生坐标
			enemy[i].x = rand() % (WIDTH - 100) + 1;
			enemy[i].y = -20;
			enemy[i].mark = 1;
			 //初始化血量
			EnemyHp(i);

			enemy[i].move_type = rand()%3;
			enemy[i].move_count = COUNT;
			break;
		}
	}

};

//敌机移动
void EnemyMove(float speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].flag == true)
		{

			if (enemy[i].move_type == 0 && enemy[i].move_count >=0 ) { enemy[i].y += speed; enemy[i].move_count--; }


			else if (enemy[i].move_type == 1 && enemy[i].move_count >= 0 && enemy[i].x >= 30) { enemy[i].x -= speed; enemy[i].move_count--; }

			else if (enemy[i].move_type == 2 && enemy[i].move_count  >= 0 && enemy[i].x <= 1100) { enemy[i].x += speed; enemy[i].move_count--; }

			else { enemy[i].move_type = rand() % 3; enemy[i].move_count = COUNT; }
		}

			if (enemy[i].y >= HEIGHT)//越界判断
			{
				enemy[i].flag = false;
			}
	}
}






//射击音效问题
/*
DWORD WINAPI PlayMusic(LPVOID lp)
{
	mciSendString(TEXT("close enemy_1 "), 0, 0, 0);
	mciSendString(TEXT("open ./music/bullet_1.mp3 alias enemy_1 "), 0, 0, 0);
	mciSendString(TEXT("play enemy_1 "), 0, 0, 0);
	return 0;
}
*/

//游戏控制
//玩家移动和开火
//敌机创建和移动
void GameControl(float speed)
{

	//敌机创建
	if (t4 - t3 > 180)

	{
		CreatEnemy();

		t3 = t4;
	}
	t4 = GetTickCount64();

	EnemyMove(0.3);//敌机移动

	//敌机子弹创造
	if (t6 - t5 > 800)
	{
		EnemyBullet();
		t5 = t6;
	}
	t6 = GetTickCount64();

	EnemyBulletMove(0.3);//敌机子弹移动

	Behurt();    //玩家受到伤害

	//玩家移动
	if (GetAsyncKeyState(VK_UP) && player.y + 75 >= 0)
	{
		player.y -= speed;
	}

	if (GetAsyncKeyState(VK_DOWN) && player.y + 75 <= HEIGHT)
	{
		player.y += speed;
	}

	if (GetAsyncKeyState(VK_LEFT) && player.x + 65 >= 0)
	{
		player.x -= speed;
	}

	if (GetAsyncKeyState(VK_RIGHT) && player.x + 85 <= WIDTH)
	{
		player.x += speed;
	}
	//玩家发射子弹

	if (player.un.score > 50)//score>50 射击速度加快
	{
		if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 100)
		{

			//多线程	
			//HANDLE threadID = CreateThread(NULL, 0, PlayMusic, NULL, 0, 0);
			//CloseHandle(threadID);
			CreateBullet();//玩家产生子弹
			t1 = t2;
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 150)
		{

			//多线程	
			//HANDLE threadID = CreateThread(NULL, 0, PlayMusic, NULL, 0, 0);
			//CloseHandle(threadID);
			CreateBullet();//玩家产生子弹
			t1 = t2;
		}
	}
	
	t2 = GetTickCount64();

	BulletMove(1.2);//玩家子弹移动

	crash_bullet(); //子弹相撞

	KillEnemy(); //击杀敌机
	


}

//显示分数
void Showscore()

{
	 
	TCHAR score_text[50];
	if (player.hp >0||(player.hp <= 0 && !player.bekill))
	{
		_stprintf_s(score_text, "SCORE:%d", player.un.score);
		settextstyle(50, 0, "Consolas");
		outtextxy(0, 0, score_text);
	}
	else if(player.hp <=0&&player.bekill)
	{
		_stprintf_s(score_text, "得分:%d", player.un.score);
		settextstyle(50, 0, "Consolas");
		outtextxy(350, 50, score_text);
	}

}

//显示生命值
void Showlife()

{
	if (player.hp <= 0) player.hp = 0;
	TCHAR life_text[50];
	_stprintf_s(life_text, "LIFE:%d", player.hp);
	settextstyle(50, 0, "Consolas");
	outtextxy(1000, 0, life_text);

}

//显示玩家杀敌数
void Showkillcount()
{

	TCHAR skillcount_text[50];
	if (player.hp > 0|| (player.hp <= 0 && !player.bekill))
	{
		_stprintf_s(skillcount_text, "KILL:%d", player.count);
		settextstyle(50, 0, "Consolas");
		outtextxy(500, 0, skillcount_text);
	}
	else if (player.hp <= 0 && player.bekill)
	{
		_stprintf_s(skillcount_text, "击杀:%d", player.count);
		settextstyle(50, 0, "Consolas");
		outtextxy(650,50, skillcount_text);
	}

}

//游戏开始界面
void Begingame()
{

	IMAGE a, b;
	initgraph(1200, 800);
	loadimage(&a, "./image/begine.png");
	putimage(0, 0, &a);
	system("pause");
	closegraph;
	initgraph(1200, 800);
	loadimage(&b, "./image/introduce.png");
	putimage(0, 0, &b);
	system("pause");
	closegraph;
}

//游戏结束画面
void Endgame()
{
    //玩家爆炸效果
	putimage( player.x+45, player.y+30, &image.bomb[0], NOTSRCERASE);
	putimage(player.x+45, player.y+30, &image.bomb[1], SRCINVERT);
	 
	//阵亡字体
	settextcolor(RGB(50, 50, 50));
	settextstyle(150, 0, "Consolas"); 
	setbkmode(TRANSPARENT);//TRANSPARENT 字体的背景颜色透明
	outtextxy(200, 250, "已  阵  亡");

   //关闭游戏主窗口音乐
	closegraph;
	mciSendString("close BGM ", 0, 0, 0);
	//阵亡音效
	mciSendString("open ./music/死亡音效.mp3 alias s ", 0, 0, 0);
	mciSendString("play s ", 0, 0, 0);
	Sleep(4500);
	


	//创建窗口游戏结束背景
	IMAGE c;
	initgraph(1200, 800);
	loadimage(&c, "./image/background_1.png");
	putimage(0, 0, &c);

	
	Showkillcount();
	Showscore();

	//结束窗口文字
	TCHAR evaluate_text[50];
    _stprintf_s(evaluate_text, "段位评价");
	settextstyle(50, 0, "Consolas");
	outtextxy(470, 100, evaluate_text);
	TCHAR end_text[50];
	_stprintf_s(end_text, "请  按  任  意  键  结  束  游  戏");
	settextstyle(30, 0, "Consolas");
	outtextxy(320, 750, end_text);



	//玩家段位评判
	IMAGE Class[9];
	settextstyle(80, 0, "Consolas");
	if (player.un.score >=160)
	{
		
		outtextxy(410, 180, "荣耀王者");
		loadimage(&Class[8], "./image/8.png");
		putimage(240, 270, &Class[8]);
	}
	else if (player.un.score >= 140 && player.un.score < 160)
	{
		loadimage(&Class[7], "./image/7.png");
		putimage(240, 250, &Class[7]);
		outtextxy(410, 180, "最强王者");
		 
	}
	else if (player.un.score >= 120 && player.un.score < 140)
	{
		loadimage(&Class[6], "./image/6.png");
		putimage(240, 250, &Class[6]);
		outtextxy(410, 180, "至尊星耀");
	}
	else if (player.un.score >= 100 && player.un.score < 120)
	{
		loadimage(&Class[5], "./image/5.png");
		putimage(240, 250, &Class[5]);
		outtextxy(410, 180, "永恒钻石");
		 
	}
	else if (player.un.score >= 80 && player.un.score < 100)
	{
		loadimage(&Class[4], "./image/4.png");
		putimage(240, 250, &Class[4]);
		outtextxy(410, 180, "尊贵铂金");
	}
	else if (player.un.score >= 60 && player.un.score < 80)
	{
		loadimage(&Class[3], "./image/3.png");
		putimage(240, 250, &Class[3]);
		outtextxy(410, 180, "荣耀黄金");
		 
	}
	else if (player.un.score >= 40 && player.un.score < 60)
	{
		loadimage(&Class[2], "./image/2.png");
		putimage(240, 250, &Class[2]);
		outtextxy(410, 180, "秩序白银");
		 
	}
	else if (player.un.score >= 20 && player.un.score < 40)
	{
		loadimage(&Class[1], "./image/1.png");
		putimage(240, 250, &Class[1]);
		outtextxy(410, 180, "倔强青铜");
		 
	}
	else
	{
		loadimage(&Class[0], "./image/0.png");
		putimage(400, 250, &Class[0]);
		outtextxy(410, 180, "不服黑铁");
		 
	}


	Sleep(3000);
	
	system("pause");
	closegraph;
	
}