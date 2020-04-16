项目文档

A.触发

B.传播
This is a simple propagation model.
It has only one function : greeting.
As it is compiled and runs, it will spread to all the logical disks.
For portable disks, it will be set invisible.
 
C.服务器和客户端  
一、工作实现 客户端：client

1）弹出UI，加密文件并将encrypted_session_pri_key发给服务器

2）收到加密文件后解密

服务器：server

1）接受密钥

2）服务器在数据库中记录状态： 用户信息、应支付金额、实际支付金额、那个加密的Key解密后的密钥

3）用户扫码支付成功后，触发服务器向用户发送解密后的key

二、未解决的问题： 1）后面通讯服务器recv收不到，导致时间1时间2到后无法交互 2）时间2到时的Message Box跳出后程序崩溃 3）加解密时，在客户端和服务器端调用c++程序

三、环境 1）各种包 pip一下

2）服务器使用本地回环无需修改

3）支付宝支付请下载沙箱支付宝，并得到账号参考此链接：https://blog.csdn.net/python_striker/article/details/89423948 而后用沙箱账号里的买家账号登陆沙箱支付包支付

4）在server.py里修改mysql数据库信息，并在本地安装mysql数据库

表名为info,开三列即可

addr | varchar(25) |

d_key | varchar(25) |

price | int(11) |

四、其他说明 1.测试时打开server.py和client

2.加解密文件均为test里面的txt文件

3.本版本加解密为生成随机密钥作为aes密钥，用公钥加密发到服务器，而后服务器用私钥解密返还之前的随机密钥，并解密文件


