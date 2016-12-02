#ifndef _MY_SORT_H_
#define _MY_SORT_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <time.h>
#include <stdlib.h>

typedef bool (*Op_t)(int x, int y);

//void sort(std::vector<int>::iterator pbegin, std::vector<int>::iterator pend, Op_t op);

//template<typename CompFunc>
//std::vector<int> sortMutiThread(const std::vector<int> & data, const int threadNum, CompFunc op);


void my_sort(std::vector<int>::iterator pbegin, std::vector<int>::iterator pend, Op_t op)
{
		std::sort(pbegin, pend, op);//ʹ�ñ�׼�������㷨
}



template <typename CompFunc>
std::vector<int> sortMutiThread(const std::vector<int> & data, const int threadNum, CompFunc op)
{
	std::vector<int> databuf = data;
	if (data.size() <= 1 || threadNum <= 0 || op == NULL)//��������ȷ������
	{
		return databuf;
	}

	std::vector<std::thread> threads;
	//std::thread** threads = new std::thread*[threadNum];
	int size = databuf.size() / threadNum;	
	std::vector<int>::iterator pbegin = databuf.begin();
	std::vector<int>::iterator pend;
	for (int i = 0; i < threadNum; ++i)//[pbegin, pend)��������
	{
		if (i != (threadNum - 1))
			pend = databuf.begin() + (i + 1)*size;
		else if (i == (threadNum - 1))
			pend = databuf.end();
		
		threads.push_back(std::thread(my_sort, pbegin, pend, op));//�����߳�
		//std::thread bt(my_sort, pbegin, pend, op);
		//threads[i] = &bt;
		pbegin = pend;
	}
	for (int i = 0; i < threadNum; ++i)
		threads[i].join();
	
	std::vector<int>::iterator *p_arr = new std::vector<int>::iterator[threadNum];
	for (int i = 0; i < threadNum; ++i)
	{
		p_arr[i] = databuf.begin();
		p_arr[i] += i*size;
	}
	std::vector<int>::iterator *pe_arr = new std::vector<int>::iterator[threadNum];//����ÿ������β
	for (int i = 0; i < threadNum; ++i)
	{
		if ((i + 1) != threadNum)
		{
			pe_arr[i] = databuf.begin();
			pe_arr[i] += (i+1)*size;
		}
		else if ((i + 1) == threadNum)
			pe_arr[i] = databuf.end();
	}

	bool* p_bol = new bool[threadNum];//����Ƿ񵽴�����β��
	for (int i = 0; i < threadNum; ++i)
	{
		p_bol[i] = false;
	}

	std::vector<int> re_buf;


	while (1)//ֱ�����е������ﵽβ��
	{
		int end_num = 0;//�ѽ����ĸ���
		for (int i = 0; i < threadNum; ++i)//�ж�ÿ�������Ƿ����
		{
			if (p_bol[i] == false && p_arr[i] == pe_arr[i])
				p_bol[i] = true;
			if (p_bol[i] == true)
				++end_num;
		}

		if (end_num >= threadNum-1)//�������
		{
			for (int i = 0; i < threadNum; ++i)
			{
				if (p_bol[i] == false)
				{
					while (p_arr[i] != pe_arr[i])//���������һ������
					{
						re_buf.push_back(*p_arr[i]);
						++p_arr[i];	
					}
				}
			}
			break;//����ѭ����������
		}


		bool ret = false;//���val���Ƿ��Ѿ��г�ֵ
		int val, n;//��n��������ѡ������val;
		for (int i = 0; i < threadNum; ++i)
		{
			if (p_bol[i] == false)//��δ�������������ҳ�һ�����ʵ���
			{
				if (!ret)
				{
					val = *p_arr[i];
					n = i;
					ret = true;
				}
				if (!op(val, *p_arr[i]))
				{
					val = *p_arr[i];
					n = i;
				}
			}
		}
		++p_arr[n];
		re_buf.push_back(val);//ÿ��ѡ��һ����
	}

	delete[] p_arr;
	delete[] pe_arr;
	delete[] p_bol;

	return re_buf;
}

#endif
