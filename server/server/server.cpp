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
#pragma comment(lib,"ws2_32.lib")
using namespace std;
//searchclienttotal修改文件路径
const char *IPaddress = "172.16.5.93";//客户端IP
const char *sendurl = "E:\\wenjian\\";
const char *revurl = "F:\\wenjian\\";
void solve(string s, string KeepFile)//处理传过来的字符串
{
	ofstream out;
	const char *dst;
	dst = KeepFile.c_str();
	out.open(dst, ios::out | ios::app);
	cout << "接收到的数据是:" << endl;
	int n = s.size();
	int len[10000] = { 0 };
	int cnt = 0;
	int t = 0;
	for (int i = 0; i<n; i++)
	{
		if (s[i] == '#')
		{
			t = i;
			break;
		}
	}
	int temp = 0;
	for (int i = t + 1; i<n; i++)
	{
		if (s[i] >= '0'&&s[i] <= '9')
		{
			temp = temp * 10 + s[i] - '0';
		}
		else
		{
			len[cnt++] = temp;
			temp = 0;
		}
	}
	//for(int i=0;i<cnt;i++)
	//cout<<len[i]<<' ';
	//cout<<endl;
	int k = 0;
	for (int i = 0; i<cnt; i++)
	{
		if (len[i] == 0) {
			cout << endl;
			out << endl;
		}
		else
		{
			for (int j = k; j<k + len[i]; j++)
			{
				cout << s[j];
				out << s[j];
			}
			cout << endl;
			out << endl;
			k += len[i];
		}
	}
	out.close();
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
					getAllFilesWay(p.assign(path).append("\\").append(fileinfo.name), files, size_f);
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
void getIPsfile()//查询某ip传过来的所有文件
{
	string str;
	cout << "请输入要查找的IP地址" << endl;
	cin >> str;
	string s = revurl;
	s += str;
	s += ".txt";
	//cout<<s<<endl;
	const char *temp;
	temp = s.c_str();
	/*vector<string> files;
	vector<int> f_size;
	getAllFiles(s,files,f_size);
	for(int i=0;i<files.size();i++)
	cout<<files[i]<<endl;*/
	fstream fout1;
	fout1.open(temp, ios::in);
	string red[1000];
	int cnt = 0;
	//cout<<"IIIIIIIIIIIIIIIIIIIII"<<endl;
	while (!fout1.eof())
	{
		getline(fout1, red[cnt]);
		cnt++;
	}
	cout << "从该IP读取的文件如下" << endl;
	for (int i = 0; i<cnt; i++)
		cout << red[i] << endl;
	//cout<<"OOOOOOOOOOOOOOO"<<endl;
}
int receivefileway()//接受客户端文件的内容 8890
{
	//getIPsfile();
	//SetConsoleTitle("服务器");
	//初始化WSA
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
	sin.sin_port = htons(8890);
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
	ofstream out;
	//out.open("d:\\wenjian\\recv.txt");
	while (true)
	{
		char revData[4095];
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
		string IPrev = inet_ntoa(remoteAddr.sin_addr);
		//cout<<"请输入保存的文件路径"<<endl;
		//string road;
		//cin>>road;
		string KeepFile = revurl;
		KeepFile += IPrev;
		KeepFile += ".txt";
		//cout<<KeepFile<<endl;
		//const char *temp;
		//temp=KeepFile.c_str();
		//cout<<IPrev<<endl;
		//接收数据
		int ret = recv(sClient, revData, 4095, 0);
		if (ret>0)
		{
			revData[ret] = 0x00;
			//cout<<revData<<endl;
			solve(revData, KeepFile);
			//cout<<"接收到的字符串是:"<<revData<< endl;
			out << revData << endl;
			out.flush();
		}
		break;
		//const char *sendData2="你好客户端\n";
		//send(sClient,sendData2,strlen(sendData2),0);
		//closesocket(sClient);
	}
	closesocket(sClient);
	out.close();
	closesocket(slisten);
	WSACleanup();
	//system("pause");
	return 1;
}
int receivefilewayextend()//接受客户端文件的内容 8890
{
	//getIPsfile();
	//SetConsoleTitle("服务器");
	//初始化WSA
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
	sin.sin_port = htons(8890);
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
	ofstream out;
	//out.open("d:\\wenjian\\recv.txt");
	while (true)
	{
		char revData[4095];
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
		string IPrev = inet_ntoa(remoteAddr.sin_addr);
		//cout<<"请输入保存的文件路径"<<endl;
		//string road;
		//cin>>road;
		//string KeepFile=revurl;
		//KeepFile+=IPrev;
		//KeepFile+=".txt";
		//cout<<KeepFile<<endl;
		//const char *temp;
		//temp=KeepFile.c_str();
		//cout<<IPrev<<endl;
		//接收数据
		int ret = recv(sClient, revData, 4095, 0);
		if (ret>0)
		{
			revData[ret] = 0x00;
			//cout<<revData<<endl;
			//solve(revData,KeepFile);
			//cout<<"接收到的字符串是:"<<revData<< endl;
			out << revData << endl;
			out.flush();
		}
		break;
		//const char *sendData2="你好客户端\n";
		//send(sClient,sendData2,strlen(sendData2),0);
		//closesocket(sClient);
	}
	closesocket(sClient);
	out.close();
	closesocket(slisten);
	WSACleanup();
	//system("pause");
	return 1;
}
int receiveclientdata()//接受客户端传送的路径 8888
{
	//getIPsfile();
	//SetConsoleTitle("服务器");
	//初始化WSA
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
	sin.sin_port = htons(8888);
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
	ofstream out;
	//out.open("d:\\wenjian\\recv.txt");
	while (true)
	{
		char revData[4095];
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
		//string IPrev=inet_ntoa(remoteAddr.sin_addr);
		//string KeepFile=revurl;//=revurl;
		string filename;
		cout << "请输入要保存接收数据的文件路径" << endl;
		cin >> filename;
		//KeepFile+=filename;
		//KeepFile+=".txt";
		//接收数据
		int ret = recv(sClient, revData, 4095, 0);
		if (ret>0)
		{
			revData[ret] = 0x00;
			//cout<<revData<<endl;
			solve(revData, filename);
			//cout<<"接收到的字符串是:"<<revData<< endl;
			out << revData << endl;
			out.flush();
		}
		break;
		//const char *sendData2="你好客户端\n";
		//send(sClient,sendData2,strlen(sendData2),0);
		//closesocket(sClient);
	}
	cout << endl;
	//closesocket(sClient);
	out.close();
	//closesocket(slisten);
	//WSACleanup();
	//system("pause");
	return 1;
}

int backsearch()// 让客户端收集数据 8889
{
	printf("请输入需要采集数据的客户端的IP\n");
	string IPback;
	cin >> IPback;
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
	serAddr.sin_port = htons(8889);
	const char *stra;
	stra = IPback.c_str();
	serAddr.sin_addr.s_addr = inet_addr(stra);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "AAAAAAAAAAAAAAAAAA" << endl;
		printf("connect error !\n");
		closesocket(sclient);
		return 0;
	}
	//cout<<sendData<<endl;
	//cout<<numb<<endl;
	const char *bck = "请收集数据";
	int n = strlen(bck);
	send(sclient, bck, n, 0);
	//string KeepFile=revurl;
	cout << "请输入保存的文件路径" << endl;
	string road;
	cin >> road;
	//KeepFile+=road;
	//KeepFile+=".txt";
	//SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	char revData[4095];
	int ret = recv(sclient, revData, 4095, 0);
	if (ret>0)
	{
		revData[ret] = 0x00;
		//cout<<"接受到客户端的数据为"<<endl;
		//cout<<revData<<endl;
		solve(revData, road);
		//cout<<"接收到的字符串是:"<<revData<< endl;
		//out<<revData<< endl;
		//out.flush();
	}
	//closesocket(sclient);
	//WSACleanup();
	cout << "PPPPPPPPPPPPPPPPPPPPPP" << endl;
	return 1;
}
void searchclienttotal()
{
	vector<string> files;
	vector<int> f_size;
	getAllFilesWay("F:\\wenjian\\", files, f_size);
	int n = files.size();
	cout << "从客户端收集过来的文件有:" << endl;
	for (int i = 0; i<n; i++)
		cout << files[i] << ' ' << f_size[i] << endl;
}
int acceptbytime()
{
	//SetConsoleTitle("服务器");
	//初始化WSA
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
	sin.sin_port = htons(8890);
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
	ofstream out;
	//out.open("d:\\wenjian\\recv.txt");
	while (true)
	{
		char revData[4095];
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s\r\n", inet_ntoa(remoteAddr.sin_addr));
		string IPrev = inet_ntoa(remoteAddr.sin_addr);
		string KeepFile = revurl;
		KeepFile += IPrev;
		KeepFile += ".txt";
		//cout<<KeepFile<<endl;
		//const char *temp;
		//temp=KeepFile.c_str();
		//cout<<IPrev<<endl;
		//接收数据
		int ret = recv(sClient, revData, 4095, 0);
		if (ret>0)
		{
			revData[ret] = 0x00;
			//cout<<revData<<endl;
			solve(revData, KeepFile);
			//cout<<"接收到的字符串是:"<<revData<< endl;
			out << revData << endl;
			out.flush();
		}
		/*cout<<"是否停止接收?   1 yes   2 no"<<endl;
		int t;
		cin>>t;
		if(t==1)
		break;
		else
		continue;*/
		//const char *sendData2="你好客户端\n";
		//send(sClient,sendData2,strlen(sendData2),0);
		//closesocket(sClient);
	}
	closesocket(sClient);
	out.close();
	closesocket(slisten);
	WSACleanup();
	//system("pause");
	return 1;
}
int sendcmd()//发送文件路径 8891
{
	string sendData;
	cout << "请选择需要运行的功能:" << endl;
	printf(" 1 接收客户端传送的文件内容 2 显示从指定IP传过来的文件 3 让客户端收集数据\n");
	printf(" 4 接受客户端传送的文件路径 5 查询客户端所有信息 6 分时接受文件路径\n");
	cin >> sendData;
	int t = sendData[0] - '0';
	if (t == 1 || t == 3 || t == 4 || t == 6) {
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
		serAddr.sin_port = htons(8891);
		serAddr.sin_addr.s_addr = inet_addr(IPaddress);
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			cout << "****************" << endl;
			printf("connect error !\n");
			closesocket(sclient);
			return 0;
		}
		int n = sendData.size();
		const char *stra;
		stra = sendData.c_str();
		send(sclient, stra, n, 0);
		closesocket(sclient);
		WSACleanup();
	}
	return t;
}

int main()
{
	while (1)
	{
		int t = sendcmd();
		if (t == 1)
			int w = receiveclientdata();
		else if (t == 2)
			getIPsfile();
		else if (t == 3)
		{
			int w1 = receivefilewayextend();
			int w2 = backsearch();
			cout << w2 << endl;
			if (w2 == 1) continue;
		}
		else if (t == 4)
		{
			int w = receivefileway();
		}
		else if (t == 5)
			searchclienttotal();
		else if (t == 6) {
			int w1 = receivefileway();
			int w = acceptbytime();
		}
	}
	return 0;
}
