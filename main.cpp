#include<iostream>
#include<string.h>
#include<stdio.h>
#include<string>
#include<time.h>
#include<iomanip>
#include<stdlib.h>
#include<fstream>
#define LENGTH 20
#define CDNO 6
using namespace std;
void maininterface()
{
	cout << "****************************************" << endl;
	cout << "**            银行卡管理软件          **" << endl;
	cout << "**            创建新卡，输入'1'       **" << endl;
	cout << "**            登陆查询，输入'2'       **" << endl;
	cout << "**            推出系统，输入'0'       **" << endl;
	cout << "****************************************" << endl;
}
struct Card
{
	int cardno;//卡号
	char username[LENGTH];//用户名
	double sum;//余额
};
//登陆信息日志
struct Log
{
	int cardno;
	char date[64];
	char type[6];
	double money;
	double sum;
};
void CreatNewCard()
{
	char *filename;	
	Card t_cd;
	int i_cd;
	char s_name[LENGTH];
	cout << "请输入卡号：" << endl;
	cin >> t_cd.cardno;
	cout << "请输入用户名：" << endl;
	cin >> t_cd.username;
	t_cd.sum = 0;
	sprintf(filename, "%d", t_cd.cardno);
	fstream writefile(strcat(filename, ".txt"), ios::out);
	writefile.write((char*)&t_cd, sizeof(t_cd));
	writefile.close();
}
class Logoperate
{
public:
	Logoperate(int n)	
	{
		cardno = n;
		top = -1;
	}
	void readlog()
	{
		Log l;
		sprintf(cardlog, "%d", cardno);
		fstream file(strcat(cardlog, "log"), ios::in);
		while (1)
		{
			file.read((char*)&l, sizeof(l));
			if (!file)
			break;
			top++;
			lf[top] = l;
		}
		file.close();
	}
	void writelog(Log lg)
	{
		readlog();
		top++;
		lf[top] = lg;
		fstream file(cardlog, ios::out);
		for (int i=0; i<=top; i++)
		{
			file.write((char *)&lf[i], sizeof(lf[i]));
		}
		file.close();
	}
	void listlog()
	{
		readlog();
		for (int i=0; i<=top; i++)
		{
			cout << "卡号：" << lf[i].cardno << "日期：" << lf[i].date << "方式：" << lf[i].type << "输入金额" << lf[i].money << "总金额" << lf[i].sum << endl;
		}
	}
private:
	char cardlog[CDNO];
	Log lf[100];
	int top;
	int cardno;
};
class Cardoperate
{
public:
	Cardoperate(int cardindex)
	{
		t = time(0);
		sprintf(filename, "%d", cardindex);
		fstream file(strcat(filename, ".txt"), ios::in);
		while (1)
		{
			file.read((char *)&card, sizeof(card));
			if (!file)
				break;
		}
		file.close();
	}
	~Cardoperate()
	{
		fstream file(filename, ios::out);
		file.write((char *)&card, sizeof(card));
		file.close();
	}
	void Cardin(double m);
	void Cardout(double m);
	void listlog();
private:
	Card card;
	Log lg;
	char filename[CDNO];
	time_t t;
	char datetime[64];
};
void Cardoperate::Cardin(double m)
{
	t = time(0);
	card.sum += m;
	lg.cardno = card.cardno;
	strftime(datetime, sizeof(datetime), "%Y年%m月%d日%H时%M分%S秒", localtime(&t));
	strcpy(lg.date, datetime);
	strcpy(lg.type, "存钱");
	lg.money = m;
	lg.sum = card.sum;
	cout << setiosflags(ios::fixed);
	cout << endl << "卡号" << card.cardno << "当前总金额:" << setprecision(2) << card.sum << endl; 
	Logoperate l_file(card.cardno);
	l_file.writelog(lg);
}
void Cardoperate::Cardout(double m)
{
	if (card.sum > m)
	{
		t = time(0);
		card.sum -= m;
		lg.cardno = card.cardno;
		lg.sum = card.sum;
		cout << setiosflags(ios::fixed);
		cout << endl << "卡号：" << card.cardno << "总金额" << setprecision(2) << card.sum << endl;
		Logoperate l_file(card.cardno);
		l_file.writelog(lg);
	}
	else
	{
		cout << "警告：余额不足，请续存！" << endl;
	}
}
void Cardoperate::listlog()
{
	Logoperate l_file(card.cardno);
	l_file.listlog();
}
void loginterface() //登陆查询接界面
{	
	cout << "****************************************" << endl;
	cout << "**            登陆银行卡查询          **" << endl;
	cout << "**            查询存款，输入1       **" << endl;
	cout << "**            查询取款，输入2       **" << endl;
	cout << "**            读取日志，输入3       **" << endl;
	cout << "**            推出，输入0           **" << endl;
	cout << "****************************************" << endl;
}
void LogCard(int cd) //登陆查询函数
{
	double money;
	Cardoperate cdop(cd);
	char in;
 	do
	{
		loginterface();
		cin >> in;
		switch (in)
		{
			case '1' :	
				cout << "请输入存款金额：" << endl;
				cin >> money;
				cdop.Cardin(money);		
				break;
			case '2' :
				cout << "请输入取款金额：" << endl;
				cin >> money;
				cdop.Cardout(money);
			case '3':
				cdop.listlog();
				break;
			case '0':
				break;
			default:
				cout << "输入有误，请重输。" << endl;
		}	
	} while (in != 0);

}


int main()
{
	char sel;
	int cardID;
	do
	{
		//登陆界面
		maininterface();
		cin >> sel;
		switch (sel)
		{
			case '1':
			{
				CreatNewCard();
			}
				break;
			case '2':
			{
				cout << "请输入卡号：";
				cin >> cardID;
				LogCard(cardID);
			}
				break;
			case '0':
				break;
			default:
				cout << "输入有误，请重输。" << endl;
		}
	
	} while (sel != '0');
	return 0;
}


