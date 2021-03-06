#include"stdafx.h"
#include<vector>
#include<iostream>
#include<fstream>
#include<STDIO.H>
#include<string>
#include<io.h>
#include<stdlib.h>
#include<map>
#include<algorithm>
#include<WS2tcpip.h>
using namespace std;
const char *IPaddress = "172.16.5.92";//服务器的IP
const char *sendurl = "E:\\wenjian\\";//25 106 308 326 344 427
const char *revurl = "F:\\wenjian\\";
#pragma comment(lib,"ws2_32.lib")
int send_total_data(vector<string> files, int len)//所有文件里的内容一起发送 8888
{
	string red[1000];
	int cnt = 0;
	for (int i = 0; i<len; i++)
	{
		//red[cnt]=ite->first;
		// red[cnt++]+=" 的数据是 ";
		cout << "文件 " << files[i] << " 的数据是 :" << endl;
		ifstream fout1;
		string str = "e:\\wenjian\\";
		str += files[i];
		//cout<<str<<endl;
		const char *s = str.c_str();
		//cout<<s<<endl;fout1.open(s,ios::in);
		fout1.open(s, ios::in);
		while (!fout1.eof())
		{
			getline(fout1, red[cnt]);
			cout << red[cnt] << endl;
			cnt++;
		}
		fout1.close();
	}
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket!\n");
		return 0;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.s_addr = inet_addr(IPaddress);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !\n");
		closesocket(sclient);
		return 0;
	}
	string sendData;
	int n = 0;
	string numb = "#";
	for (int i = 0; i<cnt; i++)
	{
		sendData += red[i];
		int t = red[i].size();
		n += t;
		char temp[100] = { '0' };
		int g = 0;
		while (t)
		{
			int k = t % 10;
			temp[g++] = k + '0';
			t /= 10;
		}
		for (int j = g - 1; j >= 0; j--)
			numb += temp[j];
		numb += '#';
	}
	const char *str;
	const char *str1;
	str1 = numb.c_str();
	str = sendData.c_str();
	send(sclient, str, n, 0);
	send(sclient, str1, numb.size(), 0);
	/*char recData[1023];
	int ret=recv(sclient,recData,1023, 0);
	if(ret>0)
	{
	recData[ret] = 0x00;
	printf(recData);
	}*/
	closesocket(sclient);
	WSACleanup();
}
int send_single_data(vector<string> files, int n)//单独发送每一个文件的内容 8888
{
	string red[1000];
	int cnt = 0;
	for (int j = 0; j<n; j++)
	{
		cout << "文件 " << files[j] << " 的数据是 :" << endl;
		ifstream fout1;
		string str = "e:\\wenjian\\";
		str += files[j];
		//cout<<str<<endl;
		const char *s = str.c_str();
		//cout<<s<<endl;fout1.open(s,ios::in);
		fout1.open(s, ios::in);
		cnt = 1;
		red[0] = files[j];
		red[0] += " 的数据是 :";
		while (!fout1.eof())
		{
			getline(fout1, red[cnt]);
			cout << red[cnt] << endl;
			cnt++;
		}
		fout1.close();
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA data;
		if (WSAStartup(sockVersion, &data) != 0)
		{
			return 0;
		}
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
		{
			printf("invalid socket!\n");
			return 0;
		}
		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		serAddr.sin_addr.s_addr = inet_addr(IPaddress);
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("connect error !\n");
			closesocket(sclient);
			return 0;
		}
		string sendData;
		int n = 0;
		string numb = "#";
		for (int i = 0; i<cnt; i++)
		{
			sendData += red[i];
			int t = red[i].size();
			n += t;
			char temp[100] = { '0' };
			int g = 0;
			while (t)
			{
				int k = t % 10;
				temp[g++] = k + '0';
				t /= 10;
			}
			for (int j = g - 1; j >= 0; j--)
				numb += temp[j];
			numb += '#';
		}
		//cout<<numb<<endl;
		const char *stra;
		const char *stra1;
		stra1 = numb.c_str();
		stra = sendData.c_str();
		//cout<<sendData<<endl;
		//cout<<numb<<endl;
		send(sclient, stra, n, 0);
		send(sclient, stra1, numb.size(), 0);
		closesocket(sclient);
		//WSACleanup();
	}
	return 1;
}
int send_file_way(vector<string> files)//发送文件路径 8890
{
	//cout<<files[0]<<endl;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket!\n");
		return 0;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8890);
	serAddr.sin_addr.s_addr = inet_addr(IPaddress);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "WWWWWWWWWWWWWWWWWW" << endl;
		printf("connect error !\n");
		closesocket(sclient);
		return 0;
	}
	string sendData = "";
	string numb = "#";
	int cnt = files.size();
	for (int i = 0; i<cnt; i++)
	{
		sendData += files[i];
		int t = files[i].size();
		char temp[100] = { '0' };
		int g = 0;
		while (t)
		{
			int k = t % 10;
			temp[g++] = k + '0';
			t /= 10;
		}
		for (int j = g - 1; j >= 0; j--)
			numb += temp[j];
		numb += '#';
	}
	//cout<<sendData<<endl;
	//cout<<numb<<endl;
	sendData += numb;
	int n = sendData.size();
	const char *stra;
	stra = sendData.c_str();
	//cout<<sendData<<endl;
	//cout<<numb<<endl;
	send(sclient, stra, n, 0);
	closesocket(sclient);
	WSACleanup();
}
void getAllFiles(string path, vector<string>& files, vector<int>& size_f)//收集并筛选文件，返回文件名
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		while (_findnext(hFile, &fileinfo) == 0)
		{
			if ((fileinfo.attrib&_A_SUBDIR))
			{   //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, size_f);
				}
			}
			else
			{
				string str = fileinfo.name;
				//cout<<str<<endl;
				int n = str.size();
				string stra = str.substr(n - 4);
				if ((stra == ".txt") && (fileinfo.size<1024))
				{
					files.push_back(fileinfo.name);
					size_f.push_back(fileinfo.size);
					//if(files.size()>10) return;
				}
			}
		}
		_findclose(hFile);
	}
	return;
}
void getAllFilesWay(string path, vector<string>& files, vector<int>& size_f)//收集并筛选文件，返回路径
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		while (_findnext(hFile, &fileinfo) == 0)
		{
			if ((fileinfo.attrib&_A_SUBDIR))
			{  //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, size_f);
				}
			}
			else
			{
				string str = fileinfo.name;
				//cout<<str<<endl;
				int n = str.size();
				string stra = str.substr(n - 4);
				if ((stra == ".txt") && (fileinfo.size<1024))
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					size_f.push_back(fileinfo.size);
					//if(files.size()>10) return;
				}
			}
		}
		_findclose(hFile);
	}
	return;
}
void Run_searchsingle()//获得文件名,将每个文件单独发送到服务器
{
	vector<string> files;
	//cout<<files.max_size()<<endl;
	vector<int> f_size;
	getAllFiles("E:\\wenjian", files, f_size);
	cout << "符合要求的文件有：" << endl;
	int n = files.size();
	for (int i = 0; i<n; i++)
	{
		cout << files[i] << " " << f_size[i] << endl;
	}
	//***********************************************以上是读取文件操作
	send_single_data(files, n);
	//send_total_data(files,n);
	WSACleanup();
	//system("pause");
}
void Run_searchtotal()//获得文件名,将所有文件单独发送到服务器
{
	vector<string> files;
	//cout<<files.max_size()<<endl;
	vector<int> f_size;
	getAllFiles("E:\\wenjian", files, f_size);
	cout << "符合要求的文件有：" << endl;
	int n = files.size();
	for (int i = 0; i<n; i++)
	{
		cout << files[i] << " " << f_size[i] << endl;
	}
	//***********************************************以上是读取文件操作
	send_total_data(files, n);
	//send_total_data(files,n);
	//WSACleanup();
	//system("pause");
}
void Run_searchWay()//获得文件路径，并一起发送到服务器
{
	vector<string> files;
	//cout<<files.max_size()<<endl;
	vector<int> f_size;
	getAllFilesWay("E:\\wenjian", files, f_size);
	cout << "符合要求的文件有：" << endl;
	int n = files.size();
	for (int i = 0; i<n; i++)
	{
		cout << files[i] << " " << f_size[i] << endl;
	}
	send_file_way(files);
}
VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD)
{
	Run_searchWay();
	//此行可改为你每秒要调用的函数
	//Run_search();
}
void senddatabytime()//隔一段时间发送文件
{
	MSG msg;
	int cnt = 0;
	SetTimer(NULL, 001, 5000, TimerProc);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		cnt++;
		DispatchMessage(&msg);
		if (cnt>3) break;
	}
}
int dealserver()// 8889
{
	SOCKET sClient;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8889);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	//循环接收数据
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//out.open("d:\\wenjian\\recv.txt");
	char revData[4095];
	printf("等待连接...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		return 0;
	}
	printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
	int ret = recv(sClient, revData, 4095, 0);
	if (ret>0)
	{
		revData[ret] = 0x00;
	}
	//const char *sendData2="你好客户端\n";
	//send(sClient,sendData2,strlen(sendData2),0);
	if (!strcmp(revData, "请收集数据"))
	{
		vector<string> files;
		//cout<<files.max_size()<<endl;
		vector<int> f_size;
		getAllFilesWay("E:\\wenjian", files, f_size);
		cout << "符合要求的文件有：" << endl;
		int n = files.size();
		for (int i = 0; i<n; i++)
		{
			cout << files[i] << " " << f_size[i] << endl;
		}
		string red[1000];
		int cnt = 0;
		for (int i = 0; i<n; i++)
		{
			cout << "文件 " << files[i] << " 的数据是 :" << endl;
			ifstream fout1;
			//string str="e:\\wenjian\\";
			//str+=files[i];
			const char *s = files[i].c_str();
			fout1.open(s, ios::in);
			while (!fout1.eof())
			{
				//cout<<"*******************************"<<endl;
				getline(fout1, red[cnt]);
				cout << red[cnt] << endl;
				cnt++;
			}
			fout1.close();
		}
		string sendData = "";
		string numb = "#";
		//int cnt=files.size();
		for (int i = 0; i<cnt; i++)
		{
			sendData += red[i];
			int t = red[i].size();
			char temp[100] = { '0' };
			int g = 0;
			while (t)
			{
				int k = t % 10;
				temp[g++] = k + '0';
				t /= 10;
			}
			for (int j = g - 1; j >= 0; j--)
				numb += temp[j];
			numb += '#';
		}
		//cout<<sendData<<endl;
		//cout<<numb<<endl;
		sendData += numb;
		const char *stra;
		//cout<<"AAAAAAAAAAAAAA"<<endl;
		stra = sendData.c_str();
		send(sClient, stra, strlen(stra), 0);
	}
	closesocket(sClient);
	closesocket(slisten);
	return 1;
}
int start_client()// 8891
{
	SOCKET sClient;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8891);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	//循环接收数据
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[4095];
	printf("等待连接...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		return 0;
	}
	printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
	int ret = recv(sClient, revData, 4095, 0);
	if (ret>0)
	{
		revData[ret] = 0x00;
	}
	int t = 0;
	t = revData[0] - '0';
	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	//cout<<"FFFFFFFFFFFFF"<<endl;
	//cout<<t<<endl;
	return t;
}
int main()
{
	while (1)
	{
		int k = start_client();;
		if (k == 1)
			Run_searchtotal();
		//if (k == 2)
			//senddatabytime();
		else if (k == 3) {
			Run_searchWay();
			int t = dealserver();
		}
		else if (k == 4) {
			Run_searchWay();
		}
		else if (k == 6)
		{
			senddatabytime();
		}

	}
	return 0;
}
