//Linux & C++11

#ifndef __CSV__
#define __CSV__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#define IN
#define OUT
#define INOUT

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::streamoff;
using std::ios_base;
using std::endl;
using std::move;
using std::getline;
using std::regex;
using std::regex_replace;

namespace sinicheveen
{
	template<class T>
	class CSV
	{
	public:
		//��һά������� CSV �ļ�
		inline static void saveArray(IN const T* pArray, IN int rowNum, IN const string strFileName);

		//�Ѷ�ά������� CSV �ļ�
		inline static void saveArray(IN const T* pArray, IN int lineNum, IN int rowNum, IN const string strFileName);

		//��һά vector ���飬���� CSV �ļ�
		inline static void saveVector(IN const vector<T>& vArray, IN const string strFileName);

		//�Ѷ�ά vector ���飬���� CSV �ļ�
		inline static void saveVector(IN const vector<vector<T>>& vArray, IN const string strFileName);

		//��ȡ CSV �ļ����ݣ����뵽һά vector ������
		inline static void csvToVector(IN const string strFileName, OUT vector<T>& vData);

		//��ȡ CSV �ļ����ݣ����뵽��ά vector ������
		inline static void csvToVector(IN const string strFileName, OUT vector<vector<T>>& vData);
	};

	/*
	*Name��saveArray
	*Description����һά������� CSV �ļ�
	*Input��IN cosnt T* pArray�������ַ
	*              IN int rowNum����������
	*              IN const string strFileName���ļ���
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::saveArray(IN const T* pArray, IN int rowNum, IN const string strFileName)
	{
		//�������鵽�ļ�������ļ������ڣ������ļ�����д�����ݣ�����ļ����ڣ��������д��
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		for (int i = 0; i < rowNum; i++)
		{
			if (i < rowNum - 1)
				fout << *(pArray + i) << ",";
			else
				fout << *(pArray + i) << endl;
		}
		fout.close();
	}    //saveArray()

	/*
	*Name��saveArray
	*Description���Ѷ�ά������� CSV �ļ�
	*Input��IN const T* pArray�������һ��Ԫ�ص�ַ��e.g.��&array[0][0]��
	*              IN int lineNum����������
	*              IN int rowNum����������
	*              IN const string strFileName���ļ���
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::saveArray(IN const T* pArray, IN int lineNum, IN int rowNum, IN const string strFileName)
	{
		//�������鵽�ļ�������ļ������ڣ������ļ�����д�����ݣ�����ļ����ڣ��������д��
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		for (int i = 0; i < lineNum; i++)
		{
			for (int j = 0; j < rowNum; j++)
			{
				if (j < rowNum - 1)
					fout << *(pArray + i * rowNum + j) << ",";    // pArray + i * rowNum + j���ҵ���ǰ����Ԫ�ص�˳������ֵ
				else
					fout << *(pArray + i * rowNum + j) << endl;
			}
		}
		fout.close();
	}    //saveArray()

	/*
	*Name��saveVector
	*Description����һά vector ���飬���� CSV �ļ�
	*Input��IN const vector<vector<T>>& vArray��vector һά����
	*              IN const string strFileName���ļ���
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::saveVector(IN const vector<T>& vArray, IN const string strFileName)
	{
		//�������鵽�ļ�������ļ������ڣ������ļ�����д�����ݣ�����ļ����ڣ��������д��
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		int rowNum = vArray.size();
		for (int i = 0; i < rowNum; i++)
		{
			if (i < rowNum - 1)
				fout << vArray[i] << ",";
			else
				fout << vArray[i] << endl;
		}
		fout.close();
	}    //saveVector()

	/*
	*Name��saveVector
	*Description���Ѷ�ά vector ���飬���� CSV �ļ�
	*Input��IN const vector<vector<T>>& vArray��vector ��ά����
	*              IN const string strFileName���ļ���
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::saveVector(IN const vector<vector<T>>& vArray, IN const string strFileName)
	{
		//�������鵽�ļ�������ļ������ڣ������ļ�����д�����ݣ�����ļ����ڣ��������д��
		ofstream fout;
		fout.open(strFileName.c_str(), ios_base::trunc);
		int lineNum = vArray.size(), rowNum = vArray[0].size();
		for (int i = 0; i < lineNum; i++)
		{
			for (int j = 0; j < rowNum; j++)
			{
				if (j < rowNum - 1)
					fout << vArray[i][j] << ",";
				else
					fout << vArray[i][j] << endl;
			}
		}
		fout.close();
	}    //saveVector()

	/*
	*Name��csvToVector
	*Description����ȡ CSV �ļ����ݣ����뵽һά vector ������
	*Input��IN const string strFileName���ļ���
	*             OUT vector<T>& vData����ȡ��������
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::csvToVector(IN const string strFileName, OUT vector<T>& vData)
	{
		ifstream fin;
		fin.open(strFileName.c_str(), ios_base::in);    //��ֻ���ķ�ʽ���ļ�

		//���� CSV �ļ���ͷ���ܳ��ֵĿ���
		char ch;
		while (fin.get(ch))
		{
			if (ch == '\n')
				continue;
			else
				break;
		}
		streamoff pos = fin.tellg();    //���浱ǰλ��
		fin.seekg(pos - 1, ios_base::beg);    //���ص��ļ��������ݿ�ʼ����һ�е���λ

		//��ȡ����
		string strBuf;
		getline(fin, strBuf);
		regex e(",");
		stringstream ss{ regex_replace(strBuf, e, " ") };
		T data;
		while (ss >> data)
		{
			vData.push_back(move(data));
		}
	}    //csvToVector()

	/*
	*Name��csvToVector
	*Description����ȡ CSV �ļ����ݣ����뵽��ά vector ������
	*Input��IN const string strFileName���ļ���
	*             OUT vector<vector<T>>& vData����ȡ��������
	*Return��void
	*Caution��
	*/
	template<class T>
	inline void CSV<T>::csvToVector(IN const string strFileName, OUT vector<vector<T>>& vData)
	{
		ifstream fin;
		fin.open(strFileName.c_str(), ios_base::in);    //��ֻ���ķ�ʽ���ļ�

		//���� CSV �ļ���ͷ���ܳ��ֵĿ���
		char ch;
		while (fin.get(ch))
		{
			if (ch == '\n')
				continue;
			else
				break;
		}
		streamoff pos = fin.tellg();    //���浱ǰλ��
		fin.seekg(pos - 1, ios_base::beg);    //���ص��ļ��������ݿ�ʼ����һ�е���λ

		//��ȡ CSV �ļ������ݵ�����
		int lineNum = 0, rowNum = 0;
		string strBuf;
		while (getline(fin, strBuf) && !strBuf.empty())
		{
			lineNum = lineNum + 1;
		}
		fin.clear();    //getline() ��ȡ���ļ�β������������������ϡ���Ҫ��������������������ã����������޷���ȡ�ļ�����

		//��ȡ CSV �ļ������ݵ�����
		fin.seekg(pos - 1, ios_base::beg);    //���ص��ļ��������ݿ�ʼ����һ�е���λ
		while (fin.get(ch))
		{
			if (ch == ',')
			{
				rowNum = rowNum + 1;
			}
			else
			{
				if ('\r' == ch || '\n' == ch)
					break;
			}
		}
		rowNum = rowNum + 1;

		//�� CSV �ļ��е����ݴ��� vector ��
		fin.seekg(pos - 1, ios_base::beg);    //���ص��ļ��������ݿ�ʼ����һ�е���λ
		strBuf.clear();
		stringstream ss;
		regex e(",");
		int j;
		vector<vector<T>> vData_(lineNum, vector<T>(rowNum));
		for (int i = 0; i < lineNum; i++)
		{
			//��ȡ����
			getline(fin, strBuf);
			ss.str(regex_replace(strBuf, e, " "));
			T data;
			j = 0;
			while (ss >> data)
			{
				vData_[i][j] = move(data);
				j++;
			}
			ss.clear();
			ss.str("");
			strBuf.clear();
		}
		fin.close();

		vData = move(vData_);    //ʹ���ƶ���ֵ��������С����
	}    //csvToVector()

}    //sinicheveen

#endif    //__CSV__
