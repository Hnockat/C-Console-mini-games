#include<iostream>
#include<Windows.h>
#include<cstdlib>
#include<ctime>
using namespace std;

//����Ԥ����
const int Length = 28; //ǽ�� y
const int Wide = 50;   //ǽ�� x
int tick = 0;
void gotoxy(int xpos, int ypos);//����ƶ�
void Menu();     //��Ϸ�˵�
void Game();     //��Ϸ����
void Wall();     //ǽˢ��
void SnakeMove();//���ƶ�
void Collide();  //��ײʵ��
void KeyDetection(); //�������
void UpdateSnakeLoc(); //������λ��
void HideTheCursor(); //�رչ��
void randfood();      //ʳ�����
struct Point { //����
	int x;
	int y;
};
int foit[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };//1���� 2���� 3���� 4���� λ��ӳ���
Point food = { 0,0 };
//�߽ṹ
Point snakearr[100] = { {8,10},{8,11},{8.12},{8,13},{8,14},{8,15} }; //���� λ�ü���
Point tail = { 0,0 }; //Ҫɾ������β��
int SnakearrLong = 6; //�߳���
int foint = 3; //1���� 2���� 3���� 4����

//main
int main() {
	Menu();  //�˵�
	Game();  //��Ϸ��ʼ
	return 0;
}
//��Ϸ����
inline void Game() {
	//��ʼ��
	Wall(); //ǽˢ��
	HideTheCursor();//�ر�ر�
	//�������ӡ
	for (int i = 0; i <= SnakearrLong - 1;i++) {
		gotoxy(snakearr[i].x,snakearr[i].y);
		cout << "��";
	}
	randfood(); //ʳ���ʼ��
	while (true) {
		KeyDetection();  //�������
		SnakeMove();//���ƶ�
		UpdateSnakeLoc();//����ͼ

		Sleep(100);
	}
}
//ʵ����ײ
inline void Collide() {
	if (snakearr[0].x <= 1 || snakearr->x >= Wide - 1) { //ǽ��ײ
		exit(1);
	}
	if (snakearr[0].y <= 0 || snakearr->y >= Length - 1) {//ǽ��ײ
		exit(2);
	}
	if (snakearr[0].x == food.x && snakearr[0].y == food.y) { //ʳ����ײ
		randfood(); //����ʳ��
		//β������
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
//����ƶ�
inline void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//���ع��
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
//�˵�
inline void Menu() {
	cout << "-------------------------------------------" << endl;
	cout << " W.��ʼ��Ϸ" << endl;
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
//��ӡǽ
inline void Wall() {
	for (int i = 1; i <= Length;i++) {
		for (int j = 1; j <= Wide;j++) {
			if (i == 1 || i == Length) {
				cout << "��";
				continue;
			}
			if (j == 1 || j == Wide) {
				cout << "��";
				continue;
			}
			cout << ' ';
		}
		cout << endl;
	}
}

//���ƶ�
inline void SnakeMove() {
	//�������¼ β��
	tail.x = snakearr[SnakearrLong - 1].x;
	tail.y = snakearr[SnakearrLong - 1].y;
	//ͷ����
	snakearr[0].x += foit[foint - 1][0];
	snakearr[0].y += foit[foint - 1][1];
	Collide(); //��ײ����
	//��������
	for (int i = SnakearrLong - 1; i >= 1;i--) {
		snakearr[i].x = snakearr[i - 1].x;
		snakearr[i].y = snakearr[i - 1].y;
	}
	
}
//������λ��
inline void UpdateSnakeLoc() {
	//��ӡͷ
	gotoxy(snakearr[0].x,snakearr[0].y);
    cout << "��";
	//ɾ��β��
	gotoxy(tail.x, tail.y);
	cout << ' ';
}
//������� int foint = 3; //1���� 2���� 3���� 4����
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

