#ifndef SINICH_EVEEN_INTEGRAL_
#define SINICH_EVEEN_INTEGRAL_

/*
*Environment��
*Linux(Ubuntu), C++11��gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
*Description��
*���㶨���֡��������֡�ģ�ؿ��޻��ַ�
*�÷�ʾ������ǰ�ģ�����ʵ���������
*/

#include <iostream>
#include <cmath>
#include <ctime>

//��������
#define IN
#define OUT
#define INOUT

using std::cout;
using std::endl;

namespace sinicheveen
{
	struct tIntegralVal
	{
		double value;    //����ֵ
		double error;    //���
	};    //tIntegralVal

	class Integral
	{
	public:
		inline static double definiteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double step = 0.01);
		inline static double infiniteIntegral(IN double(*pf)(double), IN double a, IN double step = 0.01, IN int N = 100, IN double precision = 0.01, IN bool type = 1);
		inline static tIntegralVal monteCarloDefiniteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double precision = 0.01);
	};    //Integral

   /*
	*Name��definiteIntegral
	*Description�����㺯����[a,b]�Ķ�����
	*Input��IN double(*pf)(double)������ָ��
	*              IN double a����������
	*              IN double b����������
	*              IN double step��������Ĭ��0.01��
	*Return��double������ֵ
	*Caution��
	*/
	inline double Integral::definiteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double step)
	{
		double S = 0;
		double value1 = (*pf)(a);
		double value2;
		double trapezoidalArea;
		for (double i = a; i < b; i = i + step)
		{
			value2 = (*pf)(i + step);
			trapezoidalArea = (value1 + value2) * step / 2;
			value1 = value2;
			S = S + trapezoidalArea;
		}
		return S;
	}

	////ʾ����Visual Stuido Community 2017
	//#include <iostream>
	//#include"Integral.h"
	//
	//double fexp(double x);
	//
	//using namespace std;
	//
	//int main()
	//{
	// Integral caculute;
	// double(*pf)(double x);
	// p = fexp;
	// double sum = caculute.definiteIntegral(p, 0, 1, 0.001);
	// cout << sum << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	////exp(x)����
	//double fexp(double x)
	//{
	// return exp(x);
	//}

	/*
	*Name��infiniteIntegral
	*Description�����㺯����������֡�ԭ���趨һ����ʼ�������ޣ����ϵ���������(ÿ������N)��ֱ������һ�εĻ���֮�������趨�ľ���
	*Input��IN double(*pf)(double)������ָ��
	*              IN double a����������
	*              IN double step��������Ĭ��0.01��
	*              IN int N����������ÿ�����ӵ�ֵ��Ĭ��100��
	*              IN double precision�����ȣ�Ĭ��0.001��
	*              IN int type��true ���㺯����[a,infinite)��������֣�false ���㺯����(infinite,a]��������֡���Ĭ��Ϊ true��
	*Return��double������ֵ
	*Caution��
	*/
	inline double Integral::infiniteIntegral(IN double(*pf)(double), IN double a, IN double step, IN int N, IN double precision, IN bool type)
	{
		if (type == true)
		{
			//���㺯����[a,infinity)���������
			//�����һ������
			double S1 = 0;
			double value1 = (*pf)(a);    //���ε��ϵ�
			double value11;    //���ε��µ�
			double trapezoidalArea1;
			for (double i = a; i < a + N; i = i + step)
			{
				value11 = (*pf)(i + step);
				trapezoidalArea1 = (value1 + value11) * step / 2;    //���ε����
				value1 = value11;
				S1 = S1 + trapezoidalArea1;
			}

			//����һ�飬�����������
			double S2 = S1;
			a = a + N;
			double value2 = (*pf)(a);
			double value22;
			double trapezoidalArea2;
			for (double i = a; i < a + N; i = i + step)
			{
				value22 = (*pf)(i + step);
				trapezoidalArea2 = (value2 + value22) * step / 2;
				value2 = value22;
				S2 = S2 + trapezoidalArea2;
			}

			while (S2 - S1 > precision)    //�ж�����һ��������Ͳ�����֮ǰ��������Ƿ�С����Ҫ��ľ���ֵ
			{
				S1 = S2;
				a = a + N;
				double value3 = (*pf)(a);
				double value33;
				double trapezoidalArea3;
				for (double i = a; i < a + N; i = i + step)
				{
					value33 = (*pf)(i + step);
					trapezoidalArea3 = (value3 + value33) * step / 2;
					value3 = value33;
					S2 = S2 + trapezoidalArea3;
				}
			}
			return S1;
		}
		else
		{
			//���㺯����(infinity,a]���������
			//�����һ������
			double S1 = 0;
			double value1 = (*pf)(a);    //���ε��ϵ�
			double value11;    //���ε��µ�
			double trapezoidalArea1;
			for (double i = a; i > a - N; i = i - step)
			{
				value11 = (*pf)(i - step);
				trapezoidalArea1 = (value1 + value11) * step / 2;    //���ε����
				value1 = value11;
				S1 = S1 + trapezoidalArea1;
			}
			return S1;

			//����һ�飬�����������
			double S2 = S1;
			a = a - N;
			double value2 = (*pf)(a);
			double value22;
			double trapezoidalArea2;
			for (double i = a; i > a - N; i = i - step)
			{
				value22 = (*pf)(i - step);
				trapezoidalArea2 = (value2 + value22) * step / 2;
				value2 = value22;
				S2 = S2 + trapezoidalArea2;
			}


			while (S2 - S1 > precision)    //�ж�����һ��������Ͳ�����֮ǰ��������Ƿ�С����Ҫ��ľ���ֵ
			{
				S1 = S2;
				a = a - N;
				double value3 = (*pf)(a);
				double value33;
				double trapezoidalArea3;
				for (double i = a; i > a - N; i = i - step)
				{
					value33 = (*pf)(i - step);
					trapezoidalArea3 = (value3 + value33) * step / 2;
					value3 = value33;
					S2 = S2 + trapezoidalArea3;
				}
			}
			return S1;
		}
	}    //infiniteIntegral()

	////ʾ����Visual Stuido Community 2017
	//#include <iostream>
	//#include"Integral.h"
	//
	//double fexp(double x);
	//double ffexp(double x);
	//
	//using namespace std;
	//
	//int main()
	//{
	// Integral caculute;
	// double(*pf)(double x);
	// p = fexp;
	// double sum1 = caculute.infiniteIntegral(p, 0, 0.001, 1000, 0.01, 2);
	// p = ffexp;
	// double sum2 = caculute.infiniteIntegral(p, 0, 0.001, 1000, 0.01, 1);
	// cout << sum1 << endl;
	// cout << sum2 << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	////exp(x)����
	//double fexp(double x)
	//{
	// return exp(x);
	//}
	//
	////exp(-x)����
	//double ffexp(double x)
	//{
	// return exp(-x);
	//}

	/*
	*Name��monteCarloDefiniteIntegral
	*Description�����ؿ�����ַ������㺯���Ķ����֣���Գ������λ��ַ����������
	*Input��IN double(*pf)(double)������ָ��
	*              IN double a����������
	*              IN double b����������
	*              IN double precision�����ȣ�Ĭ��0.01��
	*Return��tIntegralVal�����ֽ����������ֵ����
	*Caution��
	*/
	inline tIntegralVal Integral::monteCarloDefiniteIntegral(IN double(*pf)(double), IN double a, IN double b, IN double precision)
	{
		double S, S1, S2, u, x, error;    //S�ǻ���ֵ��S1��S2����������
		int N = int(floor(abs(b - a) / 0.1));
		while (1)
		{
			S1 = 0.0;
			S2 = 0.0;
			srand((unsigned)time(NULL));    //��ʼ������ʱ��Ϊϵͳʱ��
			for (int i = 0; i < N; i++)
			{
				u = 1.0 * rand() / (RAND_MAX + 1);    //����0.0~1.1֮��������
				x = a + (b - a) * u;
				S1 = S1 + (*pf)(x);
				S2 = S2 + (*pf)(x) * (*pf)(x);
			}
			S = S1 * (b - a) / N;    //��������ֵ
			S1 = S1 / N;
			S2 = S2 / N;
			error = (b - a) * sqrt((S2 - S1 * S1) / N);    //������
			if (error > precision)
			{
				N = N + N;
				continue;
			}
			else
			{
				break;
			}
		}

		//���ػ���ֵ�����
		tIntegralVal tIntegralVal_;
		tIntegralVal_.value = S;
		tIntegralVal_.error = error;
		return tIntegralVal_;
	}    //monteCarloDefiniteIntegral()

	////ʾ����Visual Studio Community 2017
	////���ؿ�����ַ��ͳ������λ��ַ��Ա�
	//#include <iostream>
	//#include"Integral.h"
	//
	//using namespace std;
	//
	//double fun(double);
	//
	//int main()
	//{
	// Integral caculate;
	// double(*pf)(double);
	// double a = 0; //��������
	// double b = 10; //��������
	// double c = 0.01; //����
	// p = fun;
	// auto A = caculate.monteCarloDefiniteIntegral(p, a, b, c);
	// double B = caculate.definiteIntegral(p, a, b, c);
	// cout << A.value << endl;
	// cout << A.error << endl;
	// cout << B << endl;
	//
	// system("pause");
	// return 0;
	//}
	//
	//double fun(double x)
	//{
	// const double pi = 3.1415926;
	// double y;
	// y = sin(x);
	// //y = cos(x);
	// //y = x * x;
	// //y = 2.0*sqrt(x) / ((x + 1.0)*(x + 1.0));
	// //y = 0.2 / (pow((x - 6.0), 2.0) + 0.02);
	// //y = x * cos(10.0*pow(x, 2.0)) / (pow(x, 2.0) + 1.0);
	// //y = sqrt(x) / (x*x + 1.0);
	// //y = sin(pi*x*x) / ((x - pi)*(x - pi) + 1);
	// return y;
	//}

}    //sinicheveen

#endif    //SINICH_EVEEN_INTEGRAL_
