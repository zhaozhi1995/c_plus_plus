#include "Algorithm.h"
#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>

using std::copy;
using std::min;
using std::swap;
using std::vector;

/*
�ο����ϣ�
https://www.runoob.com/w3cnote/ten-sorting-algorithm.html
https://zhuanlan.zhihu.com/p/42586566
https://www.cnblogs.com/onepixel/articles/7674659.html
*/

/*******************************************************************************************************
*                                              ��������
*******************************************************************************************************/


/*******************************************************************************************************
*                                              ð������
*******************************************************************************************************/

/*
ԭ��https://www.runoob.com/w3cnote/bubble-sort.html
1. �Ƚ����ڵ�Ԫ�ء������һ���ȵڶ����󣬾ͽ�������������
2. ��ÿһ������Ԫ����ͬ���Ĺ������ӿ�ʼ��һ�Ե���β�����һ�ԡ��ⲽ���������Ԫ�ػ�����������
3. ������е�Ԫ���ظ����ϵĲ��裬�������һ����
4. ����ÿ�ζ�Խ��Խ�ٵ�Ԫ���ظ�����Ĳ��裬ֱ��û���κ�һ��������Ҫ�Ƚϡ�
*/

/*
*Function    : bubbleSort_Ascend
*Description : ð����������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array������
*Return      : void
*Caution     :
*/
template<class T>
void bubbleSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��

    for (int32_t i = 0; i < len - 1; ++i)    //��ѭ��Ϊ����������len��������len-1��
    {
        for (int32_t j = 0; j < len - 1 - i; ++j)    //��ѭ��Ϊÿ�˱ȽϵĴ�������i�˱Ƚ�len-i��
        {
            if (array[j] > array[j + 1])    //����Ԫ�رȽϲ�����
            {
                swap(array[j], array[j + 1]);
            }
        }
    }
}    //bubbleSort_Ascend()

/*******************************************************************************************************
*                                              ѡ������
*******************************************************************************************************/

/*
�㷨���裺https://www.runoob.com/w3cnote/selection-sort.html
1. ������δ�����������ҵ���СԪ�أ���ŵ��������е���ʼλ�á�
2. �ٴ�ʣ��δ����Ԫ���м���Ѱ����СԪ�أ�Ȼ��ŵ����������е�ĩβ��
3. �ظ��ڶ�����ֱ������Ԫ�ؾ�������ϡ�
*/

/*
*Function    : selectSort_Ascend
*Description : ѡ����������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array������
*Return      : void
*Caution     :
*/
template<class T>
void selectSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��

    for (int32_t i = 0; i < len - 1; ++i)    //len��������len-1��ѡ�񽻻�
    {
        int32_t minIndex = i;
        for (int32_t j = minIndex + 1; j < len; ++j)    //�߷�δ�����Ԫ��
        {
            if (array[j] < array[minIndex])
            {
                minIndex = j;    //��¼��Сֵ��λ��
            }
        }
        swap(array[i], array[minIndex]);    //δ����Ԫ�ص�ͷ��δ����Ԫ���е���Сֵ����
    }
}    //selectSort_Ascend()

/*******************************************************************************************************
*                                              ��������
*******************************************************************************************************/

/*
�㷨���裺https://www.runoob.com/w3cnote/insertion-sort.html
1. ����һ���������е�һ��Ԫ�ؿ���һ���������У��ѵڶ���Ԫ�ص����һ��Ԫ�ص�����δ�������С�
2. ��ͷ��β����ɨ��δ�������У���ɨ�赽��ÿ��Ԫ�ز����������е��ʵ�λ�á�������������Ԫ�������������е�ĳ��Ԫ����ȣ��򽫴�����Ԫ�ز��뵽���Ԫ�صĺ��档��
*/

/*
*Function    : insertSort_Ascend
*Description : ������������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array
*Return      : void
*Caution     :
*/
template<class T>
void insertSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��

    for (int32_t i = 1; i < len; ++i)    //���±�Ϊ1��Ԫ�ؿ�ʼѡ����ʵ�λ�ò��룬��Ϊ�±�Ϊ0��ֻ��һ��Ԫ�أ�Ĭ���������
    {
        T insertElem = array[i];    //��¼Ҫ���������

        //���Ѿ�������������ұߵĿ�ʼ�Ƚϣ��ҵ�����С����ȵ���
        int32_t j;
        for (j = i - 1; j >= 0 && insertElem < array[j]; --j)
        {
            array[j + 1] = array[j];
        }

        array[j + 1] = insertElem;    //���뵽����С����ȵ����ĺ��һ��λ��
    }
}    //insertSort_Ascend()

/*******************************************************************************************************
*                                              ϣ������
*******************************************************************************************************/

/*
�㷨���裺https://blog.csdn.net/qq_39207948/article/details/80006224
1. ѡ��һ���������� t1��t2��������tk������ ti > tj, tk = 1��
2. ���������и��� k�������н��� k ������
3. ÿ�����򣬸��ݶ�Ӧ������ ti�����������зָ�����ɳ���Ϊ m �������У��ֱ�Ը��ӱ����ֱ�Ӳ������򡣽���������Ϊ 1 ʱ������������Ϊһ�������������ȼ�Ϊ�������еĳ��ȡ�
*/

/*
*Function    : shellSort_Ascend
*Description : ϣ����������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array������
*Return      : void
*Caution     :
*/
template<class T>
void shellSort_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��

    //����������[O(n^(3/2)) by Knuth, 1973]: 1, 4, 13, 40, 121, ...
    int32_t gap = 1;
    while (gap < len / 3)
    {
        gap = gap * 3 + 1;
    }

    //���ɴεĲ�������
    for (; gap >= 1; gap = gap / 3)
    {
        //��������
        //�Ը���Ĳ������򲢲����ȶ�һ�������������Ϻ��ٶ���һ����������򣬶���������ÿ������в�������
        for (int32_t i = gap; i < len; ++i)
        {
            T insertElem = array[i];
            int32_t j;
            for (j = i - gap; j >= 0 && insertElem < array[j]; j -= gap)
            {
                array[j + gap] = array[j];
            }
            array[j + gap] = insertElem;
        }
    }
}    //shellSort_Ascend()

/*******************************************************************************************************
*                                              �鲢����
*******************************************************************************************************/

/*********************************�ݹ鷨************************************/

/*
�㷨���裺https://www.runoob.com/w3cnote/merge-sort.html
1. ����ռ䣬ʹ���СΪ�����Ѿ���������֮�ͣ��ÿռ�������źϲ�������У�
2. �趨����ָ�룬���λ�÷ֱ�Ϊ�����Ѿ��������е���ʼλ�ã�
3. �Ƚ�����ָ����ָ���Ԫ�أ�ѡ�����С��Ԫ�ط��뵽�ϲ��ռ䣬���ƶ�ָ�뵽��һλ�ã�
4. �ظ����� 3 ֱ��ĳһָ��ﵽ����β��
5. ����һ����ʣ�µ�����Ԫ��ֱ�Ӹ��Ƶ��ϲ�����β��
*/

template<class T>
void mergeSortedArray_Recursion_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start1, int32_t end1, int32_t start2, int32_t end2)
{
    int32_t start1Old = start1;
    int32_t k = start1;

    //�Ƚ�����ָ����ָ���Ԫ�أ�ѡ�����С��Ԫ�ط��뵽�ϲ��ռ䣬���ƶ�ָ�뵽��һλ��
    while (start1 <= end1 && start2 <= end2)
    {
        tempArray[k++] = array[start1] < array[start2] ? array[start1++] : array[start2++];
    }

    //����һ����ʣ�µ�����Ԫ��ֱ�Ӹ��Ƶ��ϲ�����β������whileֻ��һ����ִ�У�
    while (start1 <= end1)
    {
        tempArray[k++] = array[start1++];
    }
    while (start2 <= end2)
    {
        tempArray[k++] = array[start2++];
    }

    //�ϲ�������ƻ�ԭ�����У�ע�⣺����ֻ���ƺϲ�����һ�Σ�������ȫ�����ƣ�
    memcpy(&array[start1Old], &tempArray[start1Old], (end2 - start1Old + 1) * sizeof(T));
}    //mergeSortedArray_Recursion_Ascend()

template<class T>
void internalMergeSort_Recursion_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start, int32_t end)
{
    if (start < end)    //�� start >= end ʱֹͣ�ָ�
    {
        int32_t middle = (start + end) / 2;
        internalMergeSort_Recursion_Ascend(array, tempArray, start, middle);    //�ݹ�ָ���������
        internalMergeSort_Recursion_Ascend(array, tempArray, middle + 1, end);    //�ݹ�ָ���������
        mergeSortedArray_Recursion_Ascend(array, tempArray, start, middle, middle + 1, end);    //�ϲ���������
    }
}    //internalMergeSort_Recursion_Ascend()

/*
*Function    : mergeSort_Recursion_Ascend
*Description : �鲢���򣨵ݹ鷨������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array������
*Return      : void
*Caution     :
*/
template<class T>
void mergeSort_Recursion_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��
    vector<T> tempArray(len);    //�ڲ�������ʱ�洢����
    internalMergeSort_Recursion_Ascend(array, tempArray, 0, len - 1);    //����
}    //mergeSort_Recursion_Ascend()

/********************************�ǵݹ鷨***********************************/

/*
�㷨���裺
1. ������ÿ�����������ֽ��й鲢�������γ�ceil(n/2)�����У������ÿ�����а�����/һ��Ԫ��
2. ����ʱ����������1�������������ٴι鲢���γ�ceil(n/4)�����У�ÿ�����а�����/����Ԫ��
3. �ظ�����2��ֱ������Ԫ��������ϣ���������Ϊ1
*/

template<class T>
void mergeSortedArray_Iteration_Ascend(INOUT vector<T>& array, IN vector<T>& tempArray, int32_t start1, int32_t end1, int32_t start2, int32_t end2)
{
    int32_t start1Old = start1;
    int32_t k = start1;

    //�Ƚ�����ָ����ָ���Ԫ�أ�ѡ�����С��Ԫ�ط��뵽�ϲ��ռ䣬���ƶ�ָ�뵽��һλ��
    while (start1 <= end1 && start2 <= end2)
    {
        tempArray[k++] = array[start1] < array[start2] ? array[start1++] : array[start2++];
    }

    //����һ����ʣ�µ�����Ԫ��ֱ�Ӹ��Ƶ��ϲ�����β������whileֻ��һ����ִ�У�
    while (start1 <= end1)
    {
        tempArray[k++] = array[start1++];
    }
    while (start2 <= end2)
    {
        tempArray[k++] = array[start2++];
    }
}    //mergeSortedArray_Iteration_Ascend()

/*
*Function    : mergeSort_Iteration_Ascend
*Description : �鲢���򣨵�����������
*Modify      : 2020.08.13
*Input       : INOUT vector<T>& array������
*Return      : void
*Caution     :
*/
template<class T>
void mergeSort_Iteration_Ascend(INOUT vector<T>& array)
{
    int32_t len = array.size();    //���鳤��
    vector<T> tempArray(len);    //�ڲ�������ʱ�洢����

    for (int32_t segment = 1; segment < len; segment *= 2)    //�������������ηֶΣ�1��2��4��8��...
    {
        for (int32_t i = 0; i < len; i += segment * 2)    //�鲢����
        {
            int32_t low = i, middle = min(i + segment - 1, len - 1), high = min(i + segment * 2 - 1, len - 1);
            int32_t start1 = low, end1 = middle, start2 = middle + 1, end2 = high;
            mergeSortedArray_Iteration_Ascend(array, tempArray, start1, end1, start2, end2);
        }

        //�������������ֵ��
        //�������ڲ�forѭ�����ι鲢��֮��tempArray���������array������Ԫ�أ���˿������ڲ�forִ��֮�󽻻����������ֵ��
        //�������ڲ�forѭ����ÿ�ι鲢��Ѻϲ�������ƻ�ԭ�����У��������Ч�ʣ�
        //�ݹ鷨ÿ�ι鲢tempArrayֻ�ǲ���λ�÷�����array��Ԫ�أ����ÿ�ι鲢��Ҫ�Ѻϲ�������ƻ�ԭ������
        array.swap(tempArray);
    }
}    //mergeSort_Iteration_Ascend()