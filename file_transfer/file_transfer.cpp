#include "file_transfer.h"

bool file_transfer::socket_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //����ļ��Ƿ����
    FILE* fp = fopen(file_name.c_str(), "rb");    //�Զ����Ʒ�ʽ���ļ�
    if (NULL == fp)
    {
        cout << file_name << " open failed" << endl;
        fclose(fp);
        return false;
    }

    //�����׽���
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //���׽��ֺ�IP���˿ڰ�
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));    //ÿ���ֽڶ��� 0 ���
    server_address.sin_family = AF_INET;    //ʹ��IPv4��ַ
    server_address.sin_addr.s_addr = inet_addr(ipv4_address.c_str());    //IP��ַ
    server_address.sin_port = htons(stoi(port));    //�˿�
    if (-1 == bind(server_socket, (struct sockaddr*) & server_address, sizeof(server_address))) 
    { 
        cout << "socket bind failed" << endl;
        fclose(fp);
        close(server_socket);
        return false;
    }

    //�������״̬���ȴ��û���������
    listen(server_socket, 20);

    //���տͻ�������
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*) & client_address, &client_address_size);

    //ѭ���������ݣ�ֱ���ļ���β
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //�ļ�������
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        send(client_socket, buffer, count, 0);
    }
    shutdown(client_socket, SHUT_WR);    //�ļ���ȡ��ϣ��Ͽ����������ͻ��˷��� FIN ��
    recv(client_socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);    //�������ȴ��ͻ��˽������

    //�ر��ļ����׽���
    fclose(fp);
    close(client_socket);
    close(server_socket);
    return true;
}    //socket_send_file()

bool file_transfer::socket_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //�򿪣��������ļ�
    FILE* fp = fopen(file_name.c_str(), "wb");    //�Զ����Ʒ�ʽ�򿪣��������ļ�
    if (NULL == fp)
    {
        cout << file_name << " create failed" << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //�����׽���
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    //����������ض���IP�Ͷ˿ڣ���������
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));    //ÿ���ֽڶ��� 0 ���
    server_address.sin_family = AF_INET;    //ʹ��IPv4��ַ
    server_address.sin_addr.s_addr = inet_addr(ipv4_address.c_str());    //�����IP��ַ
    server_address.sin_port = htons(stoi(port));    //�˿�
    int connect_count = 0;
    while (-1 == connect(client_socket, (struct sockaddr*) & server_address, sizeof(server_address)))
    {
        cout << "socket connecting ..." << endl;
        connect_count++;
        if (FILE_TRANSFER_SOCKET_CONNECT_COUNT + 1 == connect_count)
        {
            cout << "socket connect failed" << endl;
            fclose(fp);
            remove(file_name.c_str());
            return false;
        }
        sleep(1);
    }

    //ѭ���������ݣ�ֱ���ļ��������
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //�ļ�������
    int count;
    while ((count = recv(client_socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, count, 1, fp);
    }
    
    //�ر��ļ����׽���
    close(client_socket);
    fclose(fp);
    return true;
}    //socket_recv_file()

bool file_transfer::zmq_req_rep_send_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //����ļ��Ƿ����
    FILE* fp = fopen(file_name.c_str(), "rb");    //�Զ����Ʒ�ʽ���ļ�
    if (NULL == fp)
    { 
        cerr << "zmq_req_rep_send_file(): fopen(): error: " << file_name << " open failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        return false;
    }

    //���� zmq socket ����������
    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_REQ);
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //���ý��ճ�ʱʱ��
    zmq_setsockopt(socket, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                          //Set linger period for socket shutdown
    string url = "tcp://" + ipv4_address + ":" + port;
    if (-1 == zmq_connect(socket, url.c_str()))
    {
        cerr << "zmq_req_rep_send_file(): zmq_connect(): error: zmq connect " << url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //ѭ���������ݣ�ֱ���ļ���β
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        //�����ļ�����
        zmq_send(socket, buffer, count, 0);

        //���ջظ���Ϣ
        if (-1 == zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_req_rep_send_file(): zmq_recv(): error: receive file content reply message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
    }

    //�����ļ�������־
    zmq_send(socket, FILE_TRANSFER_FILE_END_FLAG, FILE_TRANSFER_FILE_END_FLAG_LEN, 0);
    if (-1 == zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
    {
        cerr << "zmq_req_rep_send_file(): zmq_recv(): error: receive file end flag reply message timeout, error code " << to_string(errno) + ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� true
    cout << "zmq_req_rep_send_file(): " << file_name << " send successfully." << endl;
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� false
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return false;
}    //zmq_req_rep_send_file()

bool file_transfer::zmq_req_rep_recv_file(IN const string& ipv4_address, IN const string& port, IN const string& file_name)
{
    //�򿪣��������ļ�
    FILE* fp = fopen(file_name.c_str(), "wb");    //�Զ����Ʒ�ʽ�򿪣��������ļ�
    if (NULL == fp)
    {
        cerr << "zmq_req_rep_recv_file(): fopen(): error: " << file_name << " create failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //���� zmq socket ����������
    void* context = zmq_ctx_new();
    void* socket = zmq_socket(context, ZMQ_REP);
    zmq_setsockopt(socket, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //���ý��ճ�ʱʱ��
    zmq_setsockopt(socket, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown
    string url = "tcp://" + ipv4_address + ":" + port;
    if (-1 == zmq_bind(socket, url.c_str()))
    {
        cerr << "zmq_req_rep_recv_file(): zmq_bind(): error: zmq bind " << url << " failed, error code " + to_string(errno) << ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //ѭ���������ݣ�ֱ���ļ���β
    char buffer[FILE_TRANSFER_BUFFER_SIZE];    //�ļ�������
    int count;
    while (true)
    {
        //������Ϣ
        count = zmq_recv(socket, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);
        if (-1 == count)
        {
            cerr << "zmq_req_rep_recv_file(): zmq_recv(): error: receive file content message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }

        //д�ļ�
        if (0 != memcmp(buffer, FILE_TRANSFER_FILE_END_FLAG, count >= FILE_TRANSFER_FILE_END_FLAG_LEN ? FILE_TRANSFER_FILE_END_FLAG_LEN : count))
        {
            fwrite(buffer, count, 1, fp);
        }
        else
        {
            //�ظ��ļ�����ȷ����Ϣ
            zmq_send(socket, "OK", 2, 0);

            //����ѭ��
            break;
        }

        //�ظ��ļ�����ȷ����Ϣ
        zmq_send(socket, "OK", 2, 0);
    }

    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� true
    cout << "zmq_req_rep_recv_file(): " << file_name << " receive successfully." << endl;
    fclose(fp);
    zmq_close(socket);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //�رղ�ɾ���ļ����ر� zmq ���ӣ����� false
    fclose(fp);
    remove(file_name.c_str());
    zmq_close(socket);
    zmq_ctx_term(context);
    return false;
}    //zmq_req_rep_recv_file()

bool file_transfer::zmq_pub_sub_send_file(IN const string& ipv4_address, IN const string& pub_port, IN const string& sync_port, IN const int subscribers_num, IN const string& file_name)
{
    //����ļ��Ƿ����
    FILE* fp = fopen(file_name.c_str(), "rb");    //�Զ����Ʒ�ʽ���ļ�
    if (NULL == fp)
    {
        cerr << "zmq_pub_sub_send_file(): fopen(): error: " << file_name << " open failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        return false;
    }

    //���� zmq context
    void* context = zmq_ctx_new();

    //���������ļ��׽���
    void* publisher = zmq_socket(context, ZMQ_PUB);
    string pub_url = "tcp://" + ipv4_address + ":" + pub_port;

    //�����ź�ͬ���׽���
    void* sync_requester = zmq_socket(context, ZMQ_REP);
    string rep_url = "tcp://" + ipv4_address + ":" + sync_port;

    //����ѡ��
    zmq_setsockopt(publisher, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));         //���ý��ճ�ʱʱ��
    zmq_setsockopt(publisher, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                     //Set linger period for socket shutdown
    zmq_setsockopt(sync_requester, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //���ý��ճ�ʱʱ��
    zmq_setsockopt(sync_requester, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown

    //�������׽���
    if (-1 == zmq_bind(publisher, pub_url.c_str()))
    {
        cerr << "zmq_pub_sub_send_file(): zmq_bind(): error: publisher bind " << pub_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }
    if (-1 == zmq_bind(sync_requester, rep_url.c_str()))
    {
        cerr << "zmq_pub_sub_send_file(): zmq_bind(): error: sync_requester bind " << rep_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //���ն����ߵľ����ź�
    cout << "zmq_pub_sub_send_file(): waiting all subscribers ready." << endl;
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
    {
        if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers ready message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
        zmq_send(sync_requester, "OK", 2, 0);
    }
    cout << "zmq_pub_sub_send_file(): all subscribers are ready." << endl;

    //ѭ�������ļ����ݣ�ֱ���ļ���β
    int count;
    while ((count = fread(buffer, 1, FILE_TRANSFER_BUFFER_SIZE, fp)) > 0)
    {
        //�����ļ�����
        zmq_send(publisher, buffer, count, 0);

        //�������ж������ļ����ݴ��������Ϣ
        for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
        {
            if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
            {
                cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers reply file content has been processed message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
                goto LABEL_FALSE;
            }
            zmq_send(sync_requester, "OK", 2, 0);
        }

        //�ȴ�һ�ᣬȷ�� subscriber ������ OK ��Ϣ
        usleep(FILE_TRANSFER_ZMQ_PUB_SLEEP_TIME);
    }

    //�����ļ�������־
    zmq_send(publisher, FILE_TRANSFER_FILE_END_FLAG, FILE_TRANSFER_FILE_END_FLAG_LEN, 0);
    for (int subscribers = 0; subscribers < subscribers_num; subscribers++)
    {
        if (-1 == zmq_recv(sync_requester, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_send_file(): zmq_recv(): error: wait subscribers reply file end flag has been processed message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
        zmq_send(sync_requester, "OK", 2, 0);
    }

    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� true
    cout << "zmq_pub_sub_send_file(): " << file_name << " send successfully." << endl;
    fclose(fp);
    zmq_close(publisher);
    zmq_close(sync_requester);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� false
    fclose(fp);
    zmq_close(publisher);
    zmq_close(sync_requester);
    zmq_ctx_term(context);
    return false;
}

bool file_transfer::zmq_pub_sub_recv_file(IN const string& ipv4_address, IN const string& sub_port, IN const string& sync_port, IN const string& file_name)
{
    //�򿪣��������ļ�
    FILE* fp = fopen(file_name.c_str(), "wb");    //�Զ����Ʒ�ʽ�򿪣��������ļ�
    if (NULL == fp)
    {
        cerr << "zmq_pub_sub_recv_file(): fopen(): error: " << file_name << " create failed, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        fclose(fp);
        remove(file_name.c_str());
        return false;
    }

    //���� zmq context
    void* context = zmq_ctx_new();

    //���������ļ��׽���
    void* subscriber = zmq_socket(context, ZMQ_SUB);
    string sub_url = "tcp://" + ipv4_address + ":" + sub_port;

    //�����ź�ͬ���׽���
    void* sync_responser = zmq_socket(context, ZMQ_REQ);
    string req_url = "tcp://" + ipv4_address + ":" + sync_port;

    //����ѡ��
    zmq_setsockopt(subscriber, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));        //���ý��ճ�ʱʱ��
    zmq_setsockopt(subscriber, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                    //Set linger period for socket shutdown
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);                                                                       //���ö���ģʽ
    zmq_setsockopt(sync_responser, ZMQ_RCVTIMEO, &FILE_TRANSFER_ZMQ_RECV_TIMOUT, sizeof(FILE_TRANSFER_ZMQ_RECV_TIMOUT));    //���ý��ճ�ʱʱ��
    zmq_setsockopt(sync_responser, ZMQ_LINGER, &FILE_TRANSFER_ZMQ_LINGER, sizeof(FILE_TRANSFER_ZMQ_LINGER));                //Set linger period for socket shutdown

    //���������׽���
    if(-1 == zmq_connect(subscriber, sub_url.c_str()))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_connect(): error: subscriber connect " << sub_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }
    if(-1 == zmq_connect(sync_responser, req_url.c_str()))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_connect(): error: sync_responser connect " << req_url << " failed, error code " + to_string(errno) + "error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //����ͬ���ź�
    zmq_send(sync_responser, "OK", 2, 0);

    //�ȴ�ͬ��Ӧ���ź�
    char buffer[FILE_TRANSFER_BUFFER_SIZE];
    if(-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
    {
        cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply ready signal confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
        goto LABEL_FALSE;
    }

    //ѭ�������ļ�����
    int count;
    while (true)
    {
        //�����ļ�����
        count = zmq_recv(subscriber, buffer, FILE_TRANSFER_BUFFER_SIZE, 0);
        if (-1 == count)
        {
            cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: receive file content message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }

        //д�ļ�
        if (0 != memcmp(buffer, FILE_TRANSFER_FILE_END_FLAG, count >= FILE_TRANSFER_FILE_END_FLAG_LEN ? FILE_TRANSFER_FILE_END_FLAG_LEN : count))
        {
            fwrite(buffer, count, 1, fp);
        }
        else
        {
            //�ظ��ļ�������־�Ѵ��������Ϣ
            zmq_send(sync_responser, "OK", 2, 0);
            if (-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
            {
                cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply file end flag has been processed confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
                goto LABEL_FALSE;
            }

            //����ѭ��
            break;
        }

        //�ظ��ļ������Ѵ��������Ϣ
        zmq_send(sync_responser, "OK", 2, 0);
        if (-1 == zmq_recv(sync_responser, buffer, FILE_TRANSFER_BUFFER_SIZE, 0))
        {
            cerr << "zmq_pub_sub_recv_file(): zmq_recv(): error: wait publisher reply file content has been processed confirm message timeout, error code " << to_string(errno) << ", error message: " << strerror(errno) << endl;
            goto LABEL_FALSE;
        }
    }

    //�ر��ļ��� zmq ���ӣ�Ȼ�󷵻� true
    cout << "zmq_pub_sub_recv_file(): " << file_name << " receive successfully." << endl;
    fclose(fp);
    zmq_close(subscriber);
    zmq_close(sync_responser);
    zmq_ctx_term(context);
    return true;

LABEL_FALSE:
    //�رղ�ɾ���ļ����ر� zmq ���ӣ����� false
    fclose(fp);
    remove(file_name.c_str());
    zmq_close(subscriber);
    zmq_close(sync_responser);
    zmq_ctx_term(context);
    return false;
}

string file_transfer::generate_file_end_flag(IN const int file_end_flag_characters_num)
{
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*+-/";

    int characters_size = characters.length();
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(0, characters_size - 1);

    string file_end_flag;
    file_end_flag.reserve(file_end_flag_characters_num);
    for (int i = 0; i < file_end_flag_characters_num; i++)
    {
        file_end_flag += characters[distribution(generator)];
    }

    return file_end_flag;
}    //generate_file_end_flag()