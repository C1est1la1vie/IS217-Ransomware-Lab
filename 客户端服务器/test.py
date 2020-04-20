import mysql.connector
mydb = mysql.connector.connect(
            host="localhost",  # 数据库主机地址
            user="root",  # 数据库用户名
            passwd="123"  # 数据库密码
        )
mycursor = mydb.cursor()
mycursor.execute("use clients;")
tmp="127.0.0.1:49184"
sql = "delete from info where addr=%s"
mycursor.execute(sql,(tmp,))
mydb.commit()