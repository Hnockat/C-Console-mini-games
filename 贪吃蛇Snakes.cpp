#include<iostream>
#include<Windows.h>
#include<cstdlib>
#include<ctime>
using namespace std;

//函数预处理
const int Length = 28; //墙长 y
const int Wide = 50;   //墙宽 x
int tick = 0;
void gotoxy(int xpos, int ypos);//光标移动
void Menu();     //游戏菜单
void Game();     //游戏主体
void Wall();     //墙刷新
void SnakeMove();//蛇移动
void Collide();  //碰撞实体
void KeyDetection(); //按键检测
void UpdateSnakeLoc(); //更新蛇位置
void HideTheCursor(); //关闭光标
void randfood();      //食物更新
struct Point { //坐标
	int x;
	int y;
};
int foit[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };//1向左 2向右 3向上 4向下 位置映射表
Point food = { 0,0 };
//蛇结构
Point snakearr[100] = { {8,10},{8,11},{8.12},{8,13},{8,14},{8,15} }; //蛇身 位置集合
Point tail = { 0,0 }; //要删除的蛇尾巴
int SnakearrLong = 6; //蛇长度
int foint = 3; //1向左 2向右 3向上 4向下

//main
int main() {
	Menu();  //菜单
	Game();  //游戏开始
	return 0;
}
//游戏主体
inline void Game() {
	//初始化
	Wall(); //墙刷新
	HideTheCursor();//关标关闭
	//蛇身体打印
	for (int i = 0; i <= SnakearrLong - 1;i++) {
		gotoxy(snakearr[i].x,snakearr[i].y);
		cout << "■";
	}
	randfood(); //食物初始化
	while (true) {
		KeyDetection();  //按键检测
		SnakeMove();//蛇移动
		UpdateSnakeLoc();//更新图

		Sleep(100);
	}
}
//实体碰撞
inline void Collide() {
	if (snakearr[0].x <= 1 || snakearr->x >= Wide - 1) { //墙碰撞
		exit(1);
	}
	if (snakearr[0].y <= 0 || snakearr->y >= Length - 1) {//墙碰撞
		exit(2);
	}
	if (snakearr[0].x == food.x && snakearr[0].y == food.y) { //食物碰撞
		randfood(); //更新食物
		//尾巴生长
		SnakearrLong++;
	}
	for (int i = 1; i <= SnakearrLong - 1;i++) {
		if (snakearr[0].x == snakearr[i].x && snakearr[0].y == snakearr[i].y) {
			exit(0);
		}
	}
}
inline void randfood() {
	srand(snakearr[0].x * time(0));
	food.x = ((rand() % ((Wide - 2) - 2 + 1)) + 2);//[2,x-1]
	food.y = (rand() % ((Length - 2)))+ 1;// [0,y-1]
	gotoxy(food.x, food.y);
	cout << '*';
}
//光标移动
inline void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//隐藏光标
void HideTheCursor()
{
	CONSOLE_CURSOR_INFO cciCursor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (GetConsoleCursorInfo(hStdOut, &cciCursor))
	{
		cciCursor.bVisible = FALSE;
		SetConsoleCursorInfo(hStdOut, &cciCursor);
	}
}
//菜单
inline void Menu() {
	cout << "-------------------------------------------" << endl;
	cout << " W.开始游戏" << endl;
	cout << "-------------------------------------------" << endl;
	while (true) {
		if (GetKeyState(0x57) & 0x8000)
		{
			break;
		}
		Sleep(50);
	}
	system("cls");
}
//打印墙
inline void Wall() {
	for (int i = 1; i <= Length;i++) {
		for (int j = 1; j <= Wide;j++) {
			if (i == 1 || i == Length) {
				cout << "■";
				continue;
			}
			if (j == 1 || j == Wide) {
				cout << "■";
				continue;
			}
			cout << ' ';
		}
		cout << endl;
	}
}

//蛇移动
inline void SnakeMove() {
	//旧坐标记录 尾巴
	tail.x = snakearr[SnakearrLong - 1].x;
	tail.y = snakearr[SnakearrLong - 1].y;
	//头更新
	snakearr[0].x += foit[foint - 1][0];
	snakearr[0].y += foit[foint - 1][1];
	Collide(); //碰撞处理
	//更新坐标
	for (int i = SnakearrLong - 1; i >= 1;i--) {
		snakearr[i].x = snakearr[i - 1].x;
		snakearr[i].y = snakearr[i - 1].y;
	}
	
}
//更新蛇位置
inline void UpdateSnakeLoc() {
	//打印头
	gotoxy(snakearr[0].x,snakearr[0].y);
    cout << "■";
	//删除尾巴
	gotoxy(tail.x, tail.y);
	cout << ' ';
}
//按键检测 int foint = 3; //1向左 2向右 3向上 4向下
inline void KeyDetection() {
	if (GetKeyState(0x57) & 0x8000 && foint != 4)      //W
	{
		foint = 3;
	}
	else if (GetKeyState(0x53) & 0x8000 && foint != 3) {//S
		foint = 4;
	}
	else if (GetKeyState(0x41) & 0x8000 && foint != 2) {//A
		foint = 1;
	}
	else if (GetKeyState(0x44) & 0x8000 && foint != 1) {//D
		foint = 2;
	}
}

