#include<iostream>
#include<Windows.h>
#include<vector>
#include<fstream>
#include<string>
using namespace std;
//预处理
void Game();
void gotoxy(int xpos, int ypos);
void HideTheCursor();
//空间存储 模拟
const int X = 120;
const int Y = 120;
// 0代表空域 1代表玩家 2代表箱子 3代表箱子放置区 4代表阻挡方块 5代表胜利点有箱子
int world[Y][X];
struct Point{
	int x = 0;
	int y = 0;
};
class Rendering {
public:
	void replacement(int x) { //数字替换
		switch (x) {
		case 0: { cout << " ";  break; }
		case 1: { cout << "A"; break; }
		case 2: { cout << "B"; break; }
		case 3: { cout << "X";  break; }
		case 4: { cout << "■"; break; }
		}
	}
	void initialize() { //初始化地图
		
		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				if (world[i][j] != 0) {
					RUN(j, i, world[i][j]);
				}
			}
		}
	}
	void RUN(int tx, int ty, int id) { //消息动态渲染
		gotoxy(tx, ty);
		replacement(id);
	}
	void RUN(Point& p, int id) { //消息动态渲染
		gotoxy(p.x, p.y);
		replacement(id);
	}
};
class GAME {
public:
	//胜利设置
	vector<Point> points; //胜利点
	void InsertWin(vector<Point>& vec) { //插入胜利点
		for (Point i : vec) world[i.y][i.x] = 3;
		points = vec;
	}
	bool for_if_win() { //遍历是否游戏胜利条件
		for (Point a : points) {
			if (world[a.y][a.x] == 3) {
				return false;
			}
		}
		return true;
	}
	//箱子放置
	void InsertBox(vector<Point>& box) {
		for (Point a:box) {
			world[a.y][a.x] = 2;
		}
	}
	//墙放置
	void InsertWill(vector<Point>& will) {
		for (Point a: will) {
			world[a.y][a.x] = 4;
		}
	}
};
class Player {
public:
	int x;
	int y;
    Point KeyDetection() { //玩家移动 传递 向量
		Point p;
		if (GetKeyState(0x57) & 0x8000)      //W
		{
			p.y--;
		}
		else if (GetKeyState(0x53) & 0x8000) {//S
			p.y++;
		}
		else if (GetKeyState(0x41) & 0x8000) {//A
			p.x--;
		}
		else if (GetKeyState(0x44) & 0x8000) {//D
			p.x++;
		}
		return p;
	}
	Player(int x, int y) : x(x), y(y) {
		world[y][x] = 1;
	}
};

class Collide { //碰撞逻辑 箱子移动 墙停止 玩家位置改变
public:
	void Playermove(Player& player,Point nextMove,Rendering& rend,GAME& game) {
		if (!(nextMove.x != 0  || nextMove.y != 0)) {
			return;
		}
		Point nextmove = { player.x + nextMove.x ,player.y + nextMove.y};
		if (world[nextmove.y][nextmove.x] == 4) { //碰到墙
			return;
		}
		if (world[nextmove.y][nextmove.x] == 5) { //挪动占据点箱子
			Point NB = { nextmove.x + nextMove.x,nextmove.y + nextMove.y };
			if (world[NB.y][NB.x] == 0 || world[NB.y][NB.x] == 1) {//可以推的区域
				world[nextmove.y][nextmove.x] = 3;
				rend.RUN(NB.x, NB.y, 2);
				world[NB.y][NB.x] = 2; //移动箱子
			}
			else if (world[NB.y][NB.x] == 3) {
				world[nextmove.y][nextmove.x] = 3;
				rend.RUN(NB.x, NB.y, 2);
				world[NB.y][NB.x] = 5; //移动箱子
			}
			else return;
		}
		if (world[nextmove.y][nextmove.x] == 2) { //碰到箱子
			Point NB = {nextmove.x + nextMove.x,nextmove.y + nextMove.y};
			if (world[NB.y][NB.x] == 0 || world[NB.y][NB.x] == 1){//可以推的区域
				world[nextmove.y][nextmove.x] = 0;
				rend.RUN(NB.x, NB.y, 2);
				world[NB.y][NB.x] = 2; //移动箱子
			}
			else if (world[NB.y][NB.x] == 3) {
				world[nextmove.y][nextmove.x] = 0;
				rend.RUN(NB.x, NB.y, 2);
				world[NB.y][NB.x] = 5; //移动箱子
				if(game.for_if_win())exit(0);
			}
			else return;
		}

		if (world[player.y][player.x] == 3) rend.RUN(player.x, player.y, 3);
		else rend.RUN(player.x, player.y, 0);
		//正常移动
		player.x = nextmove.x;
		player.y = nextmove.y;
		rend.RUN(player.x, player.y, 1);
	}
};
vector<vector<Point>> MAPGet() { //地图获取器
	ifstream ifs("map.txt",ios::in);
	if (!ifs.is_open()) {
		cout << "读取地图失败!";
		exit(0);
	}
	vector<Point> player;
	vector<Point> Win;
	string buf;
	int i = 0;
	while (getline(ifs,buf)) {
		for (int j = 0; j < buf.size();j++) {
			if (buf[j] == '1') {
				Point a = {j,i};
				player.push_back(a);
			}
			else if (buf[j] == '3') {
				Point a = { j,i };
				Win.push_back(a);
			}
			

			world[i][j] = buf[j] - '0';
		}
		i++;
	}
	vector<vector<Point>> vecall = {player,Win};
	return vecall;
}
//游戏开始逻辑
void Game() {
	vector<vector<Point>> Loction = MAPGet();
	HideTheCursor(); //隐藏光标
	Player* player = new Player(Loction[0][0].x, Loction[0][0].y);
	GAME* gameloc = new GAME();   //游戏逻辑
	Collide* collide = new Collide();//碰撞检测
	Rendering* rend = new Rendering(); //渲染图像 (初始化渲染)
	gameloc->InsertWin(Loction[1]); //胜利点设置

	rend->initialize(); //初始化地图
	while (1) {
		collide->Playermove(*player, player->KeyDetection(),*rend,*gameloc);
		Sleep(70);
	}
	
}
//光标移动
void gotoxy(int xpos, int ypos)
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
int main() {
	Game();

	return 0;
}
