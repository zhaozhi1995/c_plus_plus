//Linux & C++11

//��������ʽ������ֱ�ߣ���������ֱ�ߵĽ���

#ifndef SINICH_EVEEN_LINE_
#define SINICH_EVEEN_LINE_

//��������
#define IN
#define OUT
#define INOUT

namespace sinicheveen
{
	struct tPoint
	{
		double x;
		double y;
	};    //tPoint

	class Line
	{
	public:
		//���㷨��ֱ��
		inline static Line getLine(IN tPoint sourcePoint, IN tPoint endPoint);

		//��������ֱ�ߵĽ���
		inline static tPoint calCrossPoint(IN Line line1, IN Line line2);

	public:
		double a;    //a*x + b*y + c = 0
		double b;
		double c;
	};    //Line

   /*
	*Name��getLine
	*Description�����㷨��ֱ��
	*Input��IN tPoint sourcePoint�����
	*              IN tPoint endPoint���ص�
	*Return��Line����õ�ֱ��
	*Caution��
	*/
	inline Line Line::getLine(IN tPoint sourcePoint, IN tPoint endPoint)
	{
		Line Line_;
		Line_.a = sourcePoint.y - endPoint.y;
		Line_.b = endPoint.x - sourcePoint.x;
		Line_.c = sourcePoint.x * endPoint.y - endPoint.x * sourcePoint.y;
		return Line_;
	}    //getLine

   /*
	*Name��calCrossPoint
	*Description����������ֱ�ߵĽ���
	*Input��IN Line line1��ֱ�� 1
	*              IN Line line2��ֱ�� 2
	*Return��tPoint������
	*Caution���������ֱ��ƽ�У��� x �� y Ϊ������Ҫ�� isnan() ���� isinf() �����ж�
	*/
	inline tPoint Line::calCrossPoint(IN Line line1, IN Line line2)
	{
		tPoint Point_;
		double D;
		D = line1.a * line2.b - line2.a * line1.b;
		Point_.x = (line1.b * line2.c - line2.b * line1.c) / D;
		Point_.y = (line1.c * line2.a - line2.c * line1.a) / D;
		return Point_;
	}    //calCrossPoint

}    //sinicheveen

#endif    //SINICH_EVEEN_LINE_

/*�÷�ʾ��
#include "Line.hpp"

#include <iostream>

using namespace std;
using namespace sinicheveen;

int main()
{
	tPoint a{ 0,0 };
	tPoint b{ 1,1 };
	tPoint c{ 1,0 };
	tPoint d{ 0,1 };
	Line line1 = Line::getLine(a, b);
	Line line2 = Line::getLine(c, d);
	tPoint e= Line::calCrossPoint(line1, line2);
	cout << e.x << endl << e.y << endl;

	return 0;
}
*/
