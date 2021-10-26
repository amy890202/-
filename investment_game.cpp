include "pch.h"
#include <time.h>
#include <iostream>
#include <fstream>//讀檔寫檔
#include <conio.h>//讀鍵盤輸入
#include<windows.h>
#include<Mmsystem.h>
#include<algorithm>
#pragma comment(lib,"winmm.lib")
using namespace std;

struct USER
{
	char name[13];
	char passwd[11];//10碼+\0
	double cash;
	int login_count;
	int sim_datei;
	int hold[5];
};
struct STOCK
{
	char name[10];
	int count;
	int *date;
	double *open, *high, *low, *close, *adj;
	int *vol;
};

STOCK read_stock(char name[])
{
	STOCK s;
	strcpy_s(s.name, name);
	char filename[20];
	strcpy_s(filename, name);
	strcat_s(filename, ".csv");
	ifstream fin;
	fin.open(filename);
	s.count = 0;//從開始讀
	char temp[1000];
	while (!fin.eof())
	{
		fin.getline(temp, 1000);
		s.count++;
	}
	fin.close();
	s.date = new int[s.count];
	s.open = new double[s.count];
	s.high = new double[s.count];
	s.low = new double[s.count];
	s.close = new double[s.count];
	s.adj = new double[s.count];
	s.vol = new int[s.count];
	int i;
	char tmp;
	fin.open(filename);
	for (i = 0; i < s.count; i++)
	{
		fin >> s.date[i];
		fin >> tmp;
		fin >> s.open[i];
		fin >> tmp;
		fin >> s.high[i];
		fin >> tmp;
		fin >> s.low[i];
		fin >> tmp;
		fin >> s.close[i];
		fin >> tmp;
		fin >> s.adj[i];
		fin >> tmp;
		fin >> s.vol[i];
	}
	fin.close();
	return s;
}

int fool_proofing_main(char num[], int tag)
{
	char c;
	int n = 0, i;
	do {
		system("cls");
		if (tag == 0)
		{
			cout << "User name:";
			for (i = 0; i < n; i++)//n第一位數是0 第二位數 1 第n位數是n-1
				cout << num[i];
		}
		else
		{
			if (tag == 1)
			{
				cout << "Password:";
			}
			else if (tag == 2)
				cout << "請輸入原密碼:";
			else if (tag == 3)
				cout << "請輸入新密碼:";
			else if (tag == 4)
				cout << "請確認新密碼:";
			else if (tag == 5)
				cout << "please confirm your password:";

			for (i = 0; i < n; i++)
				cout << "*";
		}

		c = _getch();
		if (c == 8 && n != 0)//後退鍵 n是0時後退鍵不work
		{
			n--;
		}

		if (c >= '0'&& c <= '9')//輸入0~9
		{
			num[n] = c;
			n++;
		}
		if (c >= 'a'&& c <= 'z')
		{
			num[n] = c;
			n++;
		}
		if (c >= 'A'&& c <= 'Z')
		{
			num[n] = c;
			n++;
		}
	} while (c != 13 || n == 0);//按的不是enter 或 第一位數時要繼續
	num[n] = '\0';
	return n;
}
void delete_stock(STOCK s)
{
	delete[] s.vol;
	delete[] s.adj;
	delete[] s.close;
	delete[] s.low;
	delete[] s.high;
	delete[] s.open;
	delete[] s.date;
}

USER login()//使用正確帳號密碼登錄
{
	char name1[13];
	char passwd1[11];
	USER userlist[1000];
	int userno, i, j, tag;
	ifstream fin;
	fin.open("userlist.txt");
	fin >> userno;
	for (i = 0; i < userno; i++)
	{
		fin >> userlist[i].name;
		fin >> userlist[i].passwd;
		fin >> userlist[i].cash;
		fin >> userlist[i].login_count;
		fin >> userlist[i].sim_datei;
		for (j = 0; j < 5; j++)
			fin >> userlist[i].hold[j];
	}
	fin.close();
	do {
		//final增加防呆
		tag = 0;
		fool_proofing_main(name1, tag);
		tag++;
		fool_proofing_main(passwd1, tag);




		for (i = 0; i < userno; i++)
		{
			if (strcmp(userlist[i].name, name1) == 0)//比較使用者輸入的帳密是否正確
			{
				if (strcmp(userlist[i].passwd, passwd1) == 0)//0是一樣
				{
					userlist[i].login_count++;
					return userlist[i];//這個USER就是資料庫裡第i個人
				}
				else
				{
					cout << endl << "Incorrect password!!!!!!" << endl;
					cout << "Press any key" << endl;
					system("pause");
					//_getch();
					break;

				}
			}
		}

		if (i == userno)//他打出來的帳密整個資料庫都沒有
		{
			char passwd2[11];
			int tag = 5;
			cout << endl;// << "Confirm your password: ";
			fool_proofing_main(passwd2, tag);
			if (strcmp(passwd1, passwd2) == 0)//若兩次輸入的帳密都一樣，就把他新增到資料庫中
			{
				int n,x;
				srand(time(NULL));
				do
				{
					n = rand() % 10000;
				} while (n < 1000);
				cout << endl << "請輸入驗證碼[ " << n << " ]:";
				cin >> x;
				if (n == x)
				{
					cout << "歡迎" << name1 << "的加入!" << endl;
					system("pause");



					strcpy_s(userlist[userno].name, name1);
					strcpy_s(userlist[userno].passwd, passwd1);
					userlist[userno].login_count = 1;
					userlist[userno].cash = 10000;//一開始給新使用者10000塊
					userlist[userno].sim_datei = 2;
					userlist[userno].hold[0] = 0;
					userlist[userno].hold[1] = 0;
					userlist[userno].hold[2] = 0;
					userlist[userno].hold[3] = 0;
					userlist[userno].hold[4] = 0;
					ofstream fout;;//寫檔
					fout.open("userlist.txt");
					fout << userno + 1 << endl;
					for (j = 0; j <= userno; j++)//或是用append
					{
						fout << userlist[j].name << endl;
						fout << userlist[j].passwd << endl;
						fout << userlist[j].cash << endl;
						fout << userlist[j].login_count << endl;
						fout << userlist[j].sim_datei << endl;
						fout << userlist[j].hold[0] << " " << userlist[j].hold[1] << " " << userlist[j].hold[2] << " " << userlist[j].hold[3] << " " << userlist[j].hold[4] << endl;
					}
					fout.close();
					return userlist[userno];
				}
				else
				{
					cout << "輸入錯誤! ";
					system("pause");

				}
			}
			else
			{
				cout << endl << "很抱歉 新增帳戶失敗" << endl;
				system("pause");
			}

		}
	} while (true);//讓他一直繞圈圈
}
void mainUI(int select, USER one, STOCK s[])
{
	char left[12];
	char right[12];
	int i, r;
	double TotalAssests;
	for (i = 0; i < 11; i++)
	{
		left[i] = ' ';
		right[i] = ' ';

	}
	left[select] = '[';
	right[select] = ']';
	system("cls");
	TotalAssests = one.cash;
	for (r = 0; r < 5; r++)//◤◢
		TotalAssests += one.hold[r] * s[r].close[one.sim_datei];
	cout << endl << endl << endl;
	cout << "                   ╔ ═ ╗    ╔═╗ ╔═╗    ╔═══════╗" << endl;
	cout << "                   ║ █║  ╔ ◢◣◢◣╗  ║ ███║" << endl;
	cout << "                   ║ █║  ║ █◥◤█║  ║ █  ═ ╣" << endl;
	cout << "                   ║ █║  ║ █╔ ╗ █║  ║ ███║" << endl;
	cout << "                   ║ █║  ║ █║ ║ █║  ║ █ ╔═ ╝" << endl;
	cout << "                   ╚ ═ ╝  ╚ ═ ╝ ╚══ ╝  ╚ ══ ╝" << endl << endl << endl << endl;
	cout << "              " << one.name << " 您好，今日日期為 " << s[0].date[one.sim_datei] << endl;
	cout << "              你目前現金有 " << one.cash << " 元，總資產為 " << TotalAssests << " 元" << endl;
	cout << "=============================================================================" << endl;
	cout << "              " << left[1] << "1" << right[1] << " 投資決策" << endl;
	cout << "              " << left[2] << "2" << right[2] << " 數獨遊戲" << endl;
	cout << "              " << left[3] << "3" << right[3] << " 寂寞聊天室" << endl;
	cout << "              " << left[4] << "4" << right[4] << " 猜數字" << endl;
	cout << "              " << left[5] << "5" << right[5] << " 富爸爸" << endl;
	cout << "              " << left[6] << "6" << right[6] << " 變更密碼" << endl;
	cout << "              " << left[7] << "7" << right[7] << " 排行榜" << endl;//
	cout << "              " << left[8] << "8" << right[8] << " 登錄禮包" << endl;
	cout << "              " << left[9] << "9" << right[9] << " 賭博" << endl;
	cout << "              " << left[0] << "0" << right[0] << " 離開系統" << endl;
	cout << "=============================================================================" << endl << endl;
}
//把數獨的main用function2搬進來
void Sudoku_updateZ(int Z[9][9][10], int r, int c, int num)
{
	int x, y;
	for (x = 0; x < 9; x++)//把那個題目num的所有同列的那個num設0(ex題目有8就把同列的8都設為0 5.0,8~5,8,8都設0)
		Z[r][x][num] = 0;
	for (y = 0; y < 9; y++)//把同行的全部設0(ex 把0,7,8~8,7,8都設為0)
		Z[y][c][num] = 0;
	for (y = r / 3 * 3; y <= r / 3 * 3 + 2; y++)//九宮格從同列1、3、6開始跑
	{
		for (x = c / 3 * 3; x <= c / 3 * 3 + 2; x++)//從同行的1、3、6開始跑
		{
			Z[y][x][num] = 0;//把九宮格內的那個數(ex 8)都設為0
		}
	}
	Z[r][c][num] = 1;//那行那列那數設為1(ex把那格的8設為1(5,7,8設為1))
}

int Sudoku_findZ(int Z[9][9][10], int r, int c)
{
	int i, count, num;
	count = 0;
	for (i = 1; i <= 9; i++)
	{
		if (Z[r][c][i] == 1)
		{
			count++;
			num = i;
		}
	}
	if (count == 1)
		return num;
	else
		return 0;
}

void Sudoku_solve(int Q[9][9], int A[9][9])
{
	int Z[9][9][10];//一格裡給9個數字1~9，開陣列[10]，[0]不用，有機會是答案的都設1，否則為0
	int r, c, i;
	for (r = 0; r < 9; r++)
	{
		for (c = 0; c < 9; c++)
		{
			if (Q[r][c] > 0)//若題目空格有數字
			{
				A[r][c] = Q[r][c];//則答案與空格裡的數字一樣
				for (i = 1; i <= 9; i++)//那格裡有機會是答案的只有題目那個數字，他設1,其他8個數字設0
					Z[r][c][i] = 0;
				Z[r][c][Q[r][c]] = 1;
			}
			else
			{
				A[r][c] = 0;
				for (i = 1; i <= 9; i++)
					Z[r][c][i] = 1;
			}
		}
	}
	for (r = 0; r < 9; r++)
	{
		for (c = 0; c < 9; c++)
		{
			if (Q[r][c] > 0)//如果題目裡有數字(ex5,7格有8)
			{
				Sudoku_updateZ(Z, r, c, Q[r][c]);//把同行同列都設0  r,c被填上Q了所以要更新Z
			}
		}
	}
	int flag = 1, num;
	while (flag == 1)//沒人更新就跳出迴圈
	{
		flag = 0;
		for (r = 0; r < 9; r++)//找9*9內
		{
			for (c = 0; c < 9; c++)
			{
				if (A[r][c] == 0)//答案還未填
				{
					num = Sudoku_findZ(Z, r, c);
					if (num > 0)
					{
						flag = 1;//有更新就把flag設為1
						A[r][c] = num;
						Sudoku_updateZ(Z, r, c, num);
					}
				}
			}
		}
	}
}
void Sudoku_printUI(int M[9][9], int r, int c)
{
	int R, C;
	cout << "------------------------------------------------" << endl;
	for (R = 0; R < 9; R++)
	{
		for (C = 0; C < 9; C++)
		{
			if (C == 0)
				cout << "|";
			if (R == r && C == c)
			{
				if (M[R][C] == 0)
					cout << "[   ]";
				else
					cout << "[ " << M[R][C] << " ]";
			}
			else if (M[R][C] > 0)
				cout << "  " << M[R][C] << "  ";
			else
				cout << "     ";
			if ((C + 1) % 3 == 0)
				cout << "|";
		}
		if ((C + 1) % 3 == 0)
			cout << endl << endl << "|" << endl;
		else
			if ((R + 1) % 3 == 0)
				cout << endl << "------------------------------------------------" << endl;
			else
				cout << endl << endl << endl;

	}
}

bool Sudoku_compare(int M[9][9], int M1[9][9])//bool布蘭 true=1 0 false=0
{
	int r, c;
	for (r = 0; r < 9; r++)
	{
		for (c = 0; c < 9; c++)
			if (M[r][c] != M1[r][c])
				return false;
	}
	return true;
}
int Sudoku_main()
{
	srand(time(NULL));//亂數開Q1 or Q2 or Q3
	int Q[9][9], i, j;//讀int進Q
	ifstream fin;//宣告(變數)fin
	char filename[] = "Q1.txt";//"C:\\Users\\User\\Desktop\\我是資料夾\\程式\\數獨讀檔\\數獨讀檔\\Q1.txt";(不要有中文路徑)
							   //把filename拆開 也就是filename[]={Q,1,.,t,x,t,\0}
	filename[1] = '1' + rand() % 3;//Q1.txt 的１在filename[1](第二個數)
	fin.open(filename);
	for (i = 0; i < 9; i++)//一個一個int 讀進去
	{
		for (j = 0; j < 9; j++)
			fin >> Q[i][j];
	}
	int A[9][9], A1[9][9];
	int r = 0, c = 0, R, C;//role column
	char x, y;
	for (R = 0; R < 9; R++)//
	{
		for (C = 0; C < 9; C++)
			A[R][C] = Q[R][C];
	}
	do {
		system("cls");
		Sudoku_printUI(A, r, c);//hw write printUI 將你填的答案顯示出來&美化
		x = _getch();//輸入數字或上下鍵
		if (x >= '0'&& x <= '9')
		{
			if (Q[r][c] == 0)//有題目的地方不能跳
				A[r][c] = x - '0';
		}
		if (x == -32)
		{
			y = _getch();
			switch (y)
			{
			case 72://向上
				r = (r + 8) % 9;
				break;
			case 75://向左
				c = (c + 8) % 9;
				break;
			case 77://向右
				c = (c + 1) % 9;
				break;
			case 80://向下
				r = (r + 1) % 9;
				break;
			}
		}
	} while (x != 13);
	Sudoku_solve(Q, A1);

	if (Sudoku_compare(A, A1))//A1是電腦的答案 A是你的答案
	{
		cout << "Correct" << endl;
		cout << "恭喜獲得現金5000!!" << endl;
		system("PAUSE");
		return 1;
	}
	else
	{
		cout << "Try again" << endl;
		cout << "很抱歉你失去了現金5000" << endl;
		system("PAUSE");

	}
	return 0;


	//return 1 win
	//return 0 lose
	return 0;
}

void Chat_main(USER one)
{
	ifstream fin;
	ofstream fout;
	char str[1000];//開一千個字元陣列
	do {
		system("cls");
		fin.open("msg.txt");
		while (!fin.eof())
		{
			fin.getline(str, 1000);
			cout << str << endl;

		}
		fin.close();
		cout << "================================" << endl;
		cin.getline(str, 1000);
		if (strcmp(str, "EXIT") != 0 && strlen(str) != 0)//比較 若輸入EXIT就離開 不接受只按enter -> 按enter會更新別人說的話
		{
			fout.open("msg.txt", std::ofstream::app);//寫檔 開檔方式是append(新寫入的會接在舊的後面不會覆蓋)
			fout << one.name << ":" << str << endl;
			fout.close();
		}

	} while (strcmp(str, "EXIT") != 0);
}

#include "pch.h"
#include<time.h>
#include<conio.h>
#include <iostream>
using namespace std;

void XAXB_split(int n, int A[]) //把輸入的四位數拆成4個數
{
	A[0] = n / 1000;
	n %= 1000;
	A[1] = n / 100;
	n %= 100;
	A[2] = n / 10;
	A[3] = n % 10;
}
int XAXB_legal(int A[])//檢查輸入是否合法(4個數字皆不重複) 合法return 1
{
	int out = 1;
	int i, j;
	if (A[0] >= 10)//排出輸入超過四位數的狀況
		return 0;
	for (i = 0; i < 3; i++)//比較所有i,j的排列組合(同一組四位數中)->c4取2 ，若有重複即非法 ex1246 1先跟2.4.6對 2再跟4.6對......)
	{
		for (j = i + 1; j < 4; j++)
		{
			if (A[i] == A[j])
				out = 0;
		}
	}
	return out;//1重頭到尾沒人重複 0是有人(不知道幾個)有重複 _>整個legal(int a[])=out
}

void XAXB_generate(int A[])//要讓電腦出題(產生4個不一樣的數)
{   /*
	//method 1
	A[0] = rand() % 10;
	do {
		A[1] = rand() % 10;
	} while (A[1] == A[0]);//一樣要重新產生
	do {
		A[2] = rand() % 10;
	} while (A[2] == A[0] || A[2] == A[1]);
	do {
		A[3] = rand() % 10;
	} while (A[3] == A[0] || A[3] == A[1] || A[3] == A[2]);
	//method 2 準備0~9號牌 洗亂後拿出前面4張牌 就是我的數字
	int card[10];
	int i, j, k, tmp;
	for (i = 0; i < 10; i++)//產生0~9的牌
		card[i] = i;
	for (i = 0; i < 100000; i++)
	{
		j = rand() % 10;
		k = rand() % 10;
		tmp = card[j];//把第J張跟第k張牌的數對換(j.k-> k.j)(洗牌)
		card[j] = card[k];
		card[k] = tmp;
	}
	for (i = 0; i < 4; i++)
		A[i] = card[i]; */
		//method 3
	do {
		XAXB_split(rand() % 10000, A);//亂數產生4位數 再用split函數拆成4個數字A[0]、A[1]、A[2]、A[3]
	} while (XAXB_legal(A) == 0);

}

void XAXB_Guess_input(int G[])
{
	int n;
	do {
		cout << endl << "Please Guess a number:";
		cin >> n;
		XAXB_split(n, G);//把四位數n拆開成4個數字
	} while (XAXB_legal(G) == 0);
}
void XAXB_input(char G[], int A[])
{
	char c;
	int n = 0, i, a = 0;
	do {
		do {
			
				system("cls");
				cout << "Please input your answer:";
			/*	else
				{
					if (a == 0)
						system("cls");
					cout << "Please Guess a number:";
				}*/
			for (i = 0; i < n; i++)//n第一位數是0 第二位數 1 第n位數是n-1
				cout << G[i];
			c = _getch();
			if (c == 8 && n != 0)//後退鍵 n是0時後退鍵不work
			{
				n--;
			}
			if (c >= '0' && c <= '9')//輸入0~9
			{
				G[n] = c;
				n++;
			}
			if (c == 13 && n != 4)
			{
				cout << endl << "輸入不合法請重新輸入" << endl;
				system("pause");
			}

		} while (c != 13 || n != 4);
		G[n] = '\0';
		for (i = 0; i < n; i++)
		{
			A[i] = G[i] - '0';

		}
		A[n] = '\0';
		/*	cout << "n;" << n <<"i"<<i<< endl;
			cout << endl<< G <<endl;
			cout << endl << A << endl;
			cout << A[0] << A[1] << A[2] << A[3] << A[4] << A[5];*/
		if (XAXB_legal(A) == 0 || n != 4)
		{
			cout << endl << "輸入不合法請重新輸入" << endl;
			system("pause");
		}

	} while (XAXB_legal(A) == 0);
	a++;
}
/*void XAXB_input(int G[], int tag)
{
	int n;
	do {
		if (tag == 0)//電腦猜時
			cout << "Please input your answer:";
		else//你猜時
			cout << "Please Guess a number:";
		cin >> n;
		XAXB_split(n, G);//把四位數n拆開成4個數字
	} while (XAXB_legal(G) == 0);
}
*/
void XAXB_compare(int G[], int A[], int AB[])//在函式裡改陣列不用return就能影響外面陣列數值
{
	int i, j;
	AB[0] = 0;
	for (i = 0; i < 4; i++)
	{
		if (G[i] == A[i])
			AB[0]++;
	}
	AB[1] = 0;
	for (i = 0; i < 4; i++)//c4取3排列組合的程式碼只能在同一組4位數中比 兩組要每個數字比
	{
		for (j = 0; j < 4; j++)
		{
			if (G[i] == A[j])
				AB[1]++;
		}
	}
	AB[1] -= AB[0];
}

/* 失敗的數獨防呆QQ
int defense_InputNum(char num[])//只可接受0~9 . - enter backspace
{
	system("cls");
	char c;
	int n = 0, i;
	do {
		for (i = 0; i < n; i++)//n第一位數是0 第二位數 1 第n位數是n-1
			cout << num[i];
		c = _getch();
		if (c == 8 && n != 0)//後退鍵 n是0時後退鍵不work
		{
			n--;
		}
		if (c >= '0'&& c <= '9')//輸入0~9
		{
			if (n > 3)//讓超出的輸入改變最後一位數(輸入滿了就推一個回來)
				n--;
			num[n] = c;
			n++;
		}
	} while (c != 13 || n != 4);//按的不是enter 或 第一位數時要繼續
	return n;
}
void defense_main(int G[])
{
	char A[4];
	int n,i;
	for (i = 0; i < 4; i++)
		A[i] = G[i] + '0';
	n = defense_InputNum(A);
	for (i = 0; i < n; i++)
		G[i] = A[i] - '0';
}
*/
int XAXB_main()
{
	system("cls");
	int cA[4], hA[5], cG[4], hG[4], cAB[2], hAB[2], tA[4], tAB[2];//A是電腦出的題目 G是你輸入的答案 AB是幾A幾B
	char hA1[5];
	int AP[10000], i;//0~9999
	int count = 0;//輸入的次數
	srand(time(NULL));
	XAXB_generate(cA);//前面generate函數回傳A[0]、A[1]、A[2]、A[3]的值
	XAXB_input(hA1, hA);//human input hA
	//開始先設陣列表0～9999全是１ 不可能的答案再設為0　(1->有可能是答案　０->不可能是答案)幾A幾B去對陣列表中還是１的就好了
	//設tA為陣列表中為1(有可能是答案)的數 tA跟cG比產生tAB(幾A幾B) tAB跟cAB不一樣的則被設為0
	//cout << cA[0] << cA[1] << cA[2] << cA[3] << endl;//告訴你電腦的答案 怕你猜不出來
	for (i = 0; i < 10000; i++)//準備0~9999陣列表 有重複數字設為0 無重複(legal=1)設為1
	{
		XAXB_split(i, tA);
		if (XAXB_legal(tA) == 1)
			AP[i] = 1;
		else
			AP[i] = 0;
	}
	do
	{
		XAXB_Guess_input(hG);//前面input函數讓使用者輸入4位數答案 並回傳G[0]、G[1]、G[2]、G[3]的值
		XAXB_compare(hG, cA, hAB);//AB[0]->A AB[1] ->B
		count++;
		cout << "(H)" << count << ":" << hAB[0] << "A" << hAB[1] << "B" << endl;
		//computer guess ->cG
		for (i = 0; i < 10000; i++)
		{
			if (AP[i] == 1)
			{
				XAXB_split(i, cG);
				cout << "computer guess:" << cG[0] << cG[1] << cG[2] << cG[3] << endl;
				break;//跳出最近的迴圈(for迴圈)
			}
		}
		XAXB_compare(cG, hA, cAB);
		cout << "(c)" << count << ":" << cAB[0] << "A" << cAB[1] << "B" << endl;
		//update AP
		for (i = 0; i < 10000; i++)
		{
			if (AP[i] == 1)
			{
				XAXB_split(i, tA);
				XAXB_compare(cG, tA, tAB);
				if (tAB[0] != cAB[0] || tAB[1] != cAB[1])//陣列表中A或B的值的值不一樣的就設為0
					AP[i] = 0;
			}
		}
	} while (hAB[0] != 4 && cAB[0] != 4);//兩者輸入有至少一人等於４時就跳出
	if (hAB[0] == 4 && cAB[0] == 4)
	{
		cout << "平手 獲得現金1000" << endl;
		system("PAUSE");
		return 2;
	}
	else if (hAB[0] == 4)
	{
		cout << "你贏了!!恭喜獲得現金5000" << endl;
		system("PAUSE");
		return 1;

	}
	else
		cout << "電腦贏了 你失去現金100" << endl;
	system("PAUSE");
	return 0;

	//return 1 win
	//return 0 lose
}

USER Change_password(USER one)
{
	system("cls");
	char passwd1[11], NewPasswd[11], NewPasswd1[11];
	int tag = 2, i, n;
	do
	{
		//cout << "請輸入原密碼:";
		fool_proofing_main(passwd1, tag);
		if (strcmp(one.passwd, passwd1) == 0)
		{
			tag++;
			//cout << "請輸入新密碼:";
			n = fool_proofing_main(NewPasswd, tag);
			tag++;
			//cout << "請確認新密碼:";
			fool_proofing_main(NewPasswd1, tag);
			if (strcmp(NewPasswd, NewPasswd1) == 0)
			{
				for (i = 0; i < n; i++)
					one.passwd[i] = NewPasswd[i];
				cout << endl << "新密碼變更成功!" << endl;

				system("pause");
				//_getch();
				break;
			}
			else
				cout << endl << "新密碼輸入錯誤" << endl;
			cout << "請按任意鍵退出~" << endl;
			system("pause");
			break;
		}
		else
		{
			cout << endl << "密碼輸入錯誤!!";
			system("pause");
			break;
		}
	} while (true);
	return one;
}
void SaveUser(USER one)
{
	USER userlist[1000];
	int userno, i, j;
	ifstream fin;
	//讀取userlist.txt放入userlist[]中
	fin.open("userlist.txt");
	fin >> userno;
	for (i = 0; i < userno; i++)
	{
		fin >> userlist[i].name;
		fin >> userlist[i].passwd;
		fin >> userlist[i].cash;
		fin >> userlist[i].login_count;
		fin >> userlist[i].sim_datei;
		for (j = 0; j < 5; j++)
			fin >> userlist[i].hold[j];
		if (strcmp(userlist[i].name, one.name) == 0) //讀到目前login的使用者，覆蓋他的資料
		{
			strcpy_s(userlist[i].passwd, one.passwd);
			userlist[i].cash = one.cash;
			userlist[i].login_count = one.login_count;
			userlist[i].sim_datei = one.sim_datei;
			for (j = 0; j < 5; j++)
				userlist[i].hold[j] = one.hold[j];
		}
	}
	fin.close();
	//更新完後寫回userlist.txt
	ofstream fout;
	fout.open("userlist.txt");
	fout << userno << endl;
	for (i = 0; i < userno; i++)
	{
		fout << userlist[i].name << endl;
		fout << userlist[i].passwd << endl;
		fout << userlist[i].cash << endl;
		fout << userlist[i].login_count << endl;
		fout << userlist[i].sim_datei << endl;
		fout << userlist[i].hold[0] << " " << userlist[i].hold[1] << " " << userlist[i].hold[2] << " " << userlist[i].hold[3] << " " << userlist[i].hold[4] << endl;
	}
	fout.close();
}
void music()
{
	PlaySound(L"C:\spring.wav", NULL, SND_FILENAME | SND_ASYNC);
	cin.get();
}
USER investment_UI(USER one, STOCK s[], int select)
{
	//one.cash
	//one.hold[0]-one.hold[4]
	int i, j;
	system("cls");
	for (i = 0; i < 5; i++)
	{
		cout << s[i].name << endl;
		for (j = -2; j <= 0; j++)
		{
			cout << s[i].date[one.sim_datei + j] << " ";
			cout << s[i].close[one.sim_datei + j];
			if (j != 0)
				cout << endl;
		}
		cout << "        Hold: " << one.hold[i] << endl;
	}
	cout << "=============================================================================" << endl;

	cout << "你要購買或售出哪一支股票呢?" << endl;

	char left[11];
	char right[11];
	int r;
	for (r = 0; r < 11; r++)
	{
		left[r] = ' ';
		right[r] = ' ';

	}
	left[select] = '[';
	right[select] = ']';
	//system("cls");
	cout << endl;

	cout << "              " << s[0].name << " " << left[1] << "購買" << right[1] << " " << left[6] << "售出" << right[6] << endl;
	cout << "              " << s[1].name << " " << left[2] << "購買" << right[2] << " " << left[7] << "售出" << right[7] << endl;
	cout << "               " << s[2].name << "  " << left[3] << "購買" << right[3] << " " << left[8] << "售出" << right[8] << endl;
	cout << "             " << s[3].name << " " << left[4] << "購買" << right[4] << " " << left[9] << "售出" << right[9] << endl;
	cout << "              " << s[4].name << " " << left[5] << "購買" << right[5] << " " << left[10] << "售出" << right[10] << endl;
	cout << "                     " << left[0] << "離開股市" << right[0] << endl;




	// one.cash -= s[2].close[one.sim_datei];
	// one.hold[2]++;
	/////system("pause");
	//_getch();
	return one;
}

USER investment_main(USER one, STOCK s[])
{
	int inv_select = 1;
	char c3, c4;
	do
	{
		one = investment_UI(one, s, inv_select);
		c3 = _getch();
		if (c3 == -32)
		{
			c4 = _getch();
			if (c4 == 72)//上
				inv_select = (inv_select + 10) % 11;
			if (c4 == 80)//下
				inv_select = (inv_select + 1) % 11;
			if (c4 == 77)//右
			{
				if (inv_select != 0 && inv_select <= 5)
					inv_select += 5;
				else if (inv_select > 5)
					inv_select -= 5;
				else if (inv_select == 0)
					inv_select = 6;

			}
			if (c4 == 75)//左
			{
				if (inv_select > 5 && inv_select <= 10)
					inv_select -= 5;
				else if (inv_select == 0)
					inv_select = 1;
				else if (inv_select <= 5)
					inv_select += 5;
			}
		}
		if (c3 == 13)
		{
			if (inv_select == 1) //
			{
				if (one.cash >= s[0].close[one.sim_datei])
				{
					one.cash -= s[0].close[one.sim_datei];
					one.hold[0]++;
				}
				else
				{
					cout << "很抱歉，你手上無足夠現金購買" << s[0].name << "股票" << endl;
					system("pause");
				}
			}
			if (inv_select == 2) //
			{
				if (one.cash >= s[1].close[one.sim_datei])
				{
					one.cash -= s[1].close[one.sim_datei];
					one.hold[1]++;
				}
				else
				{
					cout << "很抱歉，你手上無足夠現金購買" << s[1].name << "股票" << endl;
					system("pause");
				}
			}
			if (inv_select == 3) //
			{
				if (one.cash >= s[2].close[one.sim_datei])
				{
					one.cash -= s[2].close[one.sim_datei];
					one.hold[2]++;
				}
				else
				{
					cout << "很抱歉，你手上無足夠現金購買" << s[2].name << "股票" << endl;
					system("pause");
				}
			}
			if (inv_select == 4) //
			{
				if (one.cash >= s[3].close[one.sim_datei])
				{
					one.cash -= s[3].close[one.sim_datei];
					one.hold[3]++;
				}
				else
				{
					cout << "很抱歉，你手上無足夠現金購買" << s[3].name << "股票" << endl;
					system("pause");
				}
			}
			if (inv_select == 5) //
			{
				if (one.cash >= s[4].close[one.sim_datei])
				{
					one.cash -= s[4].close[one.sim_datei];
					one.hold[4]++;
				}
				else
				{
					cout << "很抱歉，你手上無足夠現金購買" << s[4].name << "股票" << endl;
					system("pause");
				}
			}

			if (inv_select == 6) //
			{
				one.cash += s[0].close[one.sim_datei];
				if (one.hold[0] > 0)
				{
					one.hold[0]--;
				}
				else
				{
					cout << "很抱歉~你手上無" << s[0].name << "股票可售出" << endl;
					system("pause");
				}
			}
			if (inv_select == 7) //
			{
				one.cash += s[1].close[one.sim_datei];
				if (one.hold[1] > 0)
				{
					one.hold[1]--;
				}
				else
				{
					cout << "很抱歉~你手上無" << s[1].name << "股票可售出" << endl;
					system("pause");
				}
			}
			if (inv_select == 8) //
			{
				if (one.hold[2] > 0)
				{
					one.hold[2]--;
				}
				else
				{
					cout << "很抱歉~你手上無" << s[2].name << "股票可售出" << endl;
					system("pause");
				}
			}
			if (inv_select == 9) //
			{
				if (one.hold[3] > 0)
				{
					one.hold[3]--;
				}
				else
				{
					cout << "很抱歉~你手上無" << s[3].name << "股票可售出" << endl;
					system("pause");
				}
			}
			if (inv_select == 10) //
			{
				if (one.hold[4] > 0)
				{
					one.hold[4]--;
				}
				else
				{
					cout << "很抱歉~你手上無" << s[4].name << "股票可售出" << endl;
					system("pause");
				}
			}
			if (inv_select == 0) //
			{
				break;
			}

		}
	} while (true);
	////system("pause");
	return one;
}
void dice(USER one)
{
	int x,guess;
	srand(time(NULL));
	x = rand() % 6 + 1;
	system("cls");
	cout << "猜猜骰子骰出數字幾呢 請輸入1~6其中一個數字:";
	cin >> guess;
	if (guess == x)
	{
		cout << "哇猜對了呢!幸運之神是你準沒錯，恭喜獲得現金1000元!!" << endl;
		one.cash += 1000;
	}
	else
	{
		cout << "骰子骰出的是" << x << "喔" << endl;
		if (x == 6)
		{
			cout << "恭喜獲得現金600元，今天運勢滿點呢~" << endl;
			one.cash += 600;

		}
		if (x == 5)
		{
			cout << "恭喜獲得現金500元，今天運勢不錯喔~" << endl;
			one.cash += 500;
		}
		if (x == 4)
		{
			cout << "恭喜獲得現金400元，今天運勢不錯喔~" << endl;
			one.cash += 400;
		}
		if (x == 3)
		{
			cout << "恭喜獲得現金300元，今天運勢還可以~" << endl;
			one.cash += 300;
		}
		if (x == 2)
		{
			cout << "恭喜獲得現金200元，今天運勢還可以~" << endl;
			one.cash += 200;
		}
		if (x == 1)
		{
			cout << "看你可憐給你現金100元，下次再接再勵吧" << endl;
			one.cash += 200;
		}
	}
	//system("pause");
}
int rate(USER one, STOCK s[])
{
	system("cls");
	cout << "資產排行榜" << endl;
	char name1[13], ratename[1000];;
	char passwd1[11];
	double cash[1000], TotalAssests[1000], totalassests;
	int rate = 0, rate1 = 0, UserRate1=0, UserRate=0;
	USER userlist[1000];
	int userno, i, r, j, tag = 1;
	ifstream fin;
	fin.open("userlist.txt");
	fin >> userno;
	for (i = 0; i < userno; i++)
	{
		fin >> userlist[i].name;
		fin >> userlist[i].passwd;
		fin >> userlist[i].cash;
		fin >> userlist[i].login_count;
		fin >> userlist[i].sim_datei;
		for (j = 0; j < 5; j++)
			fin >> userlist[i].hold[j];
	}
	fin.close();
	for (i = 0; i < userno; i++)
	{
		TotalAssests[i] = userlist[i].cash;
		for (r = 0; r < 5; r++)//◤◢
			TotalAssests[i] += userlist[i].hold[r] * s[r].close[userlist[i].sim_datei];
	}
	totalassests = one.cash;
	for (r = 0; r < 5; r++)//◤◢
		totalassests += one.hold[r] * s[r].close[one.sim_datei];
	sort(TotalAssests, TotalAssests + userno);
	reverse(TotalAssests, TotalAssests + userno);
	for (r = 0; r < userno; r++)
	{
		tag = 1;
		if (r > 1)
		{
			if (TotalAssests[r] != TotalAssests[r - 1])
				rate1++;
			else
				tag = 0;
		}
		else
		{
			rate1++;
		}
		if (totalassests == TotalAssests[r])
			UserRate1 = rate1;
		if (tag == 1)
		{

			cout << "第" << rate1 << "名:";
			for (i = 0; i < userno; i++)
			{
				if (userlist[i].cash + userlist[i].hold[0] * s[0].close[userlist[i].sim_datei] + userlist[i].hold[1] * s[1].close[userlist[i].sim_datei] + userlist[i].hold[2] * s[2].close[userlist[i].sim_datei] + userlist[i].hold[3] * s[3].close[userlist[i].sim_datei] + userlist[i].hold[4] * s[4].close[userlist[i].sim_datei] == TotalAssests[r])
				{
					cout << userlist[i].name << " ";
				}
			}
			cout << "持有資產:" << TotalAssests[r] << endl;
		}

	}
	if(UserRate1!=0)
		cout << endl << one.name << "您現在持有資產排名第" << UserRate1 << "名" << endl << endl;





	cout << endl << endl << "現金排行榜" << endl;
	//mid = 0;
	for (r = 0; r < userno; r++)
		cash[r] = userlist[r].cash;




	sort(cash, cash + userno);
	reverse(cash, cash + userno);
	for (r = 0; r < userno; r++)
	{
		tag = 1;
		if (r > 1)
		{
			if (cash[r] != cash[r - 1])
				rate++;
			else
				tag = 0;
		}
		else
		{
			rate++;
		}
		if (one.cash == cash[r])
			UserRate = rate;
		if (tag == 1)
		{

			cout << "第" << rate << "名:";
			for (i = 0; i < userno; i++)
			{
				if (userlist[i].cash == cash[r])
				{
					cout << userlist[i].name << " ";
				}
			}
			cout << "持有現金:" << cash[r] << endl;
		}

	}
	if (UserRate != 0)
		cout << endl << one.name << "您現在持有現金排名第" << UserRate << "名" << endl << endl;
	system("pause");
	return UserRate1;
}
int main()
{
	USER one;
	STOCK s[5];
	s[0] = read_stock("AAPL");
	s[1] = read_stock("AMZN");
	s[2] = read_stock("FB");
	s[3] = read_stock("GOOGL");
	s[4] = read_stock("MSFT");
	one = login();//放進第i個使用者資料

	int select = 1,Rate;
	int taglogin = 0, gift,taglogin1 = 0;
	char c1, c2;
	music();
	do
	{
		mainUI(select, one, s);


		c1 = _getch();
		if (c1 == -32)
		{
			c2 = _getch();
			if (c2 == 72)//上
				select = (select + 9) % 10;//+1
			if (c2 == 80)//下
				select = (select + 1) % 10;//+1
			if (c2 == 77)//右
				c1 = 13;
			if (c2 == 81)//pagedown
			{
				one.sim_datei++;
			}
		}
		if (c1 >= '0'&& c1 <= '9')
			select = c1 - '0';

		if (c1 == 13)
		{
			if (select == 1) //投資決策
			{
				
				one = investment_main(one, s);

				SaveUser(one);


			}
			else if (select == 2) //數獨  
			{
				int win = Sudoku_main();
				if (win == 1)
					one.cash += 5000;
				else
					one.cash -= 5000;
				SaveUser(one);
			}
			else if (select == 3) //聊天室
			{
				Chat_main(one);
			}
			else if (select == 4) //猜數字
			{
				int win = XAXB_main();
				if (win == 1)
					one.cash += 5000;
				else if (win == 0)
					one.cash += 1000;
				else
					one.cash -= 100;
				SaveUser(one);
			}
			else if (select == 5) //富爸爸
			{
				system("cls");
				cout << "獲得現金1000" << endl;
				one.cash += 1000;
				cout << "目前現金總共:" << one.cash << endl;
				system("pause");
				SaveUser(one);
			}
			else if (select == 6) //變更密碼
			{
				one = Change_password(one);
				SaveUser(one);
			}
			else if (select == 7) //排行榜
			{
				SaveUser(one);
				Rate=rate(one,s);
			}
			else if (select == 8)
			{

				
				srand(time(NULL));
				if (taglogin == 0)
				{
					system("cls");
					gift = rand() % 100000;
					cout << "恭喜獲得登錄禮現金" << gift << "!"<<endl;
					one.cash += gift;
					taglogin++;
					system("pause");
				}
				else
				{
					system("cls");
					cout << "你已經領過這次登錄的禮包囉，下次登錄時再來吧~"<<endl;
					system("pause");
				}
				SaveUser(one);
			}
			else if (select == 9) //dice
			{
				system("cls");
				cout << "你要花費500元賭一局嗎 要請按enter 否則請按0退出" << endl;
				char a;
				a = _getch();
				do{
					a = _getch();


				} while (a != 13 && a!= 0);
				if (a == 13)
				{
					one.cash -= 500;
					dice(one);

				}
				system("pause");
				SaveUser(one);

			}
			else  // select == 0
			{
				break;
			}
		}
	} while (true);
	SaveUser(one); //把user狀態存回userlist.txt
	delete_stock(s[0]);
	delete_stock(s[1]);
	delete_stock(s[2]);
	delete_stock(s[3]);
	delete_stock(s[4]);

	/*
	cout << one.cash << endl;
		cout << one.login_count << endl;
	*/
	return 0;
}
