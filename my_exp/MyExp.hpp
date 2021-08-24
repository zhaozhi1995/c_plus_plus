#ifndef _MY_EXP_
#define _MY_EXP_

/*
*Environment��
*Linux(Ubuntu), C++11��gcc 7.5.0��g++ 7.5.0
*Description��
*�Զ����쳣��
*/

#include <exception>
#include <stdexcept>
#include <string>

using std::exception;
using std::runtime_error;
using std::string;
using std::move;

class MyExp1 :public exception
{
public:
    explicit MyExp1(const string& rMessage) :rMessage_(rMessage) {}
    explicit MyExp1(string&& rMessage) :rMessage_(move(rMessage)) {}
    virtual char const* what() const noexcept override { return rMessage_.c_str(); }    //����� noexcept��������벻ͨ��
private:
    string rMessage_;
};

class MyExp2 :public exception
{
public:
    explicit MyExp2(const string& rMessage) :runtimeError_(rMessage) {}
    explicit MyExp2(string&& rMessage) :runtimeError_(move(rMessage)) {}
    virtual char const* what() const noexcept override { return runtimeError_.what(); }    //����� noexcept��������벻ͨ��
private:
    runtime_error runtimeError_;
};

#endif    //_MY_EXP_