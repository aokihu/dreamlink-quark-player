Dreamlink Quark Player 说明文档
------------------------------

> 当前版本: 1.0.1

**Dreamlink Quark Player**是运行在服务器端的播放器程序，主要实现功能如下

- 网络广播输出
- 声卡输出
- 标准输入输出通讯
- 支持通过管道输入外部音频数据


## 命令参数定义

### --silent 静默模式
***缩写 -s***

禁止输出运行时参数信息
```bash
qplayer --silent
qplayer -s
```
### --output 输出模式
***缩写 -o***

输出模式有两种，分别是通过网络输出和通过通过本地声卡输出，对应参数分别是，默认的输出方式是网络输出(net)

**--output net** 网络输出

**--output local** 本地声卡输出

```bash
qplayer --output net   # 输出到网络
qplayer --output local # 输出到本地声卡
```

### --volume 设置初始播放音量

设置播放器初始的音量，范围从**1**到**100**，默认音量数值为**60**

```bash
qplayer --volume 30
```

### --quality 这是网络输出的质量
***缩写 -q***
设置网络播放模式下输出的质量，有`low``normal`和`high`

**--quality low** 低品质输出，虽然音质最低，但是延迟也能降到最低
**--quality normal** 一般品质输出
**--quality high** 最高品质输出，对网络环境有更高的要求，有可能会发生不同步的情况

```bash
qplayer --quality low
```