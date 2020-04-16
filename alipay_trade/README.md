# alipay_trade.trade

客户端支付宝支付生成及验证接口

* 根据金额参数生成支付宝预支付交易，生成二维码

* 根据交易单号参数查询交易是否完成

## Usage
```python
from alipay_trade.trade import tradeManager

# Initialize
trade = tradeManager()
# Generate new trade
trade.gen_trade(amount=1.)
# Verify trade
if trade.paid_query():
    print('Paid!')
```

---

## Note

* 先按照 [requirements.txt](requirements.txt) 安装依赖

* ~~调试时，请使用自己支付宝账号生成并覆盖公钥~~

* 生成交易单号默认为当前时间，也可调用时指定，需要注意统一对象连续多次生成交易时，单号属性会被覆盖，由主进程自行进行单号管理

* 二维码有效期为 2 小时，若超时，或需要更新二维码/交易信息，由主进程调用 `del_trade()` 取消交易并重新生成 `gen_trade()`

* 使用【支付宝沙箱钱包（[安卓](https://sandbox.alipaydev.com/user/downloadApp.htm)）】而不是真的【支付宝】进行支付测试


## Possible Todo

* 自动更新二维码

* 安全性提升/服务端逻辑

* 使用 [异步通知](https://opendocs.alipay.com/open/194/103296/) 接口查询交易信息


## Reference

* [支付宝开放平台文档-网页&移动应用-当面付-扫码支付快速接入实现](https://opendocs.alipay.com/open/194/106078)

* [支付宝开放平台文档-支付API-统一收单线下交易预创建](https://opendocs.alipay.com/apis/api_1/alipay.trade.precreate)

* [支付宝开放平台文档-网页&移动应用-沙箱环境](https://opendocs.alipay.com/open/200/105311)

* [支付宝 Python SDK](https://github.com/fzlee/alipay/blob/master/README.zh-hans.md)

* [python接入支付宝接口样例](https://blog.csdn.net/zsx1314lovezyf/article/details/90677788)
