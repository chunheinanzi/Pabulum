// MsgQueue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "threadpool.h"
#include <stdio.h>

std::mutex mutex_;
void fun(int index,std::string str) {
	std::lock_guard <std::mutex> lock(mutex_);
	std::cout << index << "job " <<str.c_str()<< std::endl;
}


typedef struct package {
	int port_;
	std::string ip_;


}PACKAGE;

void DealNetPackage(PACKAGE net_data) {
	std::lock_guard <std::mutex> lock(mutex_);
	std::cout<< "ip [" << net_data.ip_.c_str() << "] port [" << net_data.port_ <<"]"<< std::endl;

}


int main()
{
    std::cout << "Hello World!\n"; 

	ThreadPool<PACKAGE> pool(5, DealNetPackage);
	PACKAGE p;
	p.ip_ = "127.0.0.1";
	p.port_ = 80;

	pool.Submit(p);
	p.ip_ = "182.566.12.34";
	pool.Submit(p);

	for (size_t i = 0; i < 10000; i++)
	{
		pool.Submit(p);
	}
	system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
