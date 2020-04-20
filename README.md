# IS217-Ransomware-Lab

Lab program for IS217, spring 2020.

***Academic communication only. DO NOT use in practice. Authors do not take responsibility for any usage related to the codes.***

---
## 功能分工

### 一、触发
宏病毒触发

### 二、传播
1. 感染磁盘，包括任何时候插入的移动硬盘和U盘

2. 感染同类所有doc文档

3. 利用outlook邮件传播

### 三、加解密
#### 设计思路 （RSA-RSA-AES）三层加密算法
在加密算法中使用了两对RSA非对称加密算法密钥和一对AES的对称加密算法密钥，两对RSA非对称加密算法密钥分别为：

1.（Master_Pub_Key，Master_Pri_Key）由软件内置生成，是固有不变的。Master_Pri_Key保存在服务器上，Master_Pub_Key内置于软件中。

2.（Session_Pub_Key，Session_Pri_Key）是程序运行时随机生成的。
AES对称加密采用的是128位密钥，ECB模式。

#### 加密过程：
1.程序运行时，首先生成Session密钥对（Session_Pub_Key，Session_Pri_Key），其中Session_Pri_Key使用Master_Pub_Key进行非对称加密获得Encrypted_Sesson_Pri_Key，保存至本地。

2.创建随机数，并对随机数进行Hash，经过处理后获得AES对称加密密钥Aes_Key，使用Aes_Key用AES算法对文件进行加密，获得加密后的文件。同时使用Session_Pub_Key对Aes_Key进行非对称加密获得Encrypted_Aes_Key，并将Encrypted_Aes_Key保存到加密文件开头。

3.遍历下一个指定后缀格式的文件，重复2中操作，直到遍历完所有文件。

#### 解密过程：
1.待客户端支付后，将Encrypted_Session_Pri_Key通过匿名网络或者邮件提交给服务器，服务器端用Master_Pri_Key解密Encrypted_Session_Pri_Key，把解密后得到的Session_Pri_Key发给客户端。

2.解密时提取每个加密文件开头的Encypted_Key，通过使用Session_Pri_Key解密获得Aes_Key，对文件进行解密。

### 四、服务器和客户端  
#### 工作实现
客户端：client

1. 弹出UI，加密文件并将encrypted_session_pri_key发给服务器

1. 收到加密文件后解密

服务器：server

1. 接受密钥

1. 服务器在数据库中记录状态： 用户信息、应支付金额、实际支付金额、那个加密的Key解密后的密钥

1. 用户扫码支付成功后，触发服务器向用户发送解密后的key

#### 未解决的问题

* [ ] c++编译成动态链接库，被Py调用

#### 环境

1. 各种包 pip一下

1. 服务器使用本地回环无需修改

1. 支付宝支付请下载沙箱支付宝，并得到账号参考此链接：https://blog.csdn.net/python_striker/article/details/89423948 而后用沙箱账号里的买家账号登陆沙箱支付包支付

1. 在server.py里修改mysql数据库信息，并在本地安装mysql数据库，表名为info,开三列即可

```
addr | varchar(25) |

d_key | varchar(25) |

price | int(11) |
```

#### 其他说明

1. 测试时打开server.py和client

2. 加解密文件均为test里面的txt文件

3. 本版本加解密为生成随机密钥作为aes密钥，用公钥加密发到服务器，而后服务器用私钥解密返还之前的随机密钥，并解密文件

### 五、其它功能

修改桌面







