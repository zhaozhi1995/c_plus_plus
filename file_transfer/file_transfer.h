#pragma once

#include <string>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <zmq.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <random>
#include <chrono>

#define IN
#define OUT
#define INOUT

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::cerr;
using std::cout;
using std::endl;
using std::to_string;

constexpr int FILE_TRANSFER_SOCKET_CONNECT_COUNT = 5;           //socker �������Ӵ�����ÿ�μ��Ϊ 1s)
constexpr int FILE_TRANSFER_ZMQ_RECV_TIMOUT = 5 * 1000;         //ZMQ ���ճ�ʱʱ�䣨���룩
constexpr int FILE_TRANSFER_ZMQ_PUB_SLEEP_TIME = 10;             //ZQM ������һ����Ϣǰ�� sleep ʱ�䣨ms��
constexpr int FILE_TRANSFER_ZMQ_LINGER = 0;                     //Set linger period for zmq socket shutdown
constexpr int FILE_TRANSFER_BUFFER_SIZE = 1 * 1024 * 1024;      //buffer ��С
constexpr int FILE_TRANSFER_FILE_END_FLAG_LEN = 64;             //�ļ�������־����
constexpr char FILE_TRANSFER_FILE_END_FLAG[FILE_TRANSFER_FILE_END_FLAG_LEN + 1] = "bLWTGv0o2NC9ll*!/6seXndW$/mik!OEJYiO*kqarWRu^OgQ%AUgAk3qR+ajhdlM";    //�ļ�������־

class file_transfer
{
public:

    static bool socket_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name);

    static bool socket_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name);

    static bool zmq_req_rep_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name);

    static bool zmq_req_rep_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name);

    static bool zmq_pub_sub_send_file(IN const string& ipv4_address, IN const string& pub_port, IN const string& sync_port, IN const int subscribers_num, IN const string& file_name);

    static bool zmq_pub_sub_recv_file(IN const string& ipv4_address, IN const string& sub_port, IN const string& sync_port, IN const string& file_name);

    static string generate_file_end_flag(IN const int file_end_flag_characters_num);
};    //file_transfer