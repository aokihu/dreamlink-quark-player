## Dreamlink Quark Player 说明文档

> 当前版本: 2.1.0

**Dreamlink Quark Player**是运行在服务器端的播放器程序，主要实现功能如下

- 网络广播输出
- 声卡输出
- 标准输入输出通讯
- 支持通过管道输入外部音频数据

## 命令参数定义

### --uri 媒体资源地址

**_ 缩写 -u _**

设置媒体资源的网络地址或者本地文件的路径，如果是本地路径必须是绝对路径，**不能**使用相对路径

```bash
qplayer -u http://mp3.baidu.com/sample.mp3
qplayer -u file:///home/qplayer/sample.mp3
```

### --silent 静默模式

**_缩写 -s_**

禁止输出运行时参数信息

```bash
qplayer --silent
qplayer -s
```

### --output 输出模式

**_缩写 -o_**

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

### --quality 设置网络输出的质量

**_缩写 -q_**
设置网络播放模式下输出的质量，有` low``normal `和`high`

**--quality low** 低品质输出，虽然音质最低，但是延迟也能降到最低
**--quality normal** 一般品质输出
**--quality high** 最高品质输出，对网络环境有更高的要求，有可能会发生不同步的情况

```bash
qplayer --quality low
```

## --fade 设置声音渐入时间

设置声音渐入的时间，单位是<秒>，默认是 0 表示没有渐入效果，最大为 30

```bash
qplayer --fade 10 # 10秒时间渐入效果
```

## 输出消息前缀

通过标`准错误输出`输出播放器状态数据，数据通过`[前缀]:[数据]`这样的格式输出

| 前缀 | 解释             | 示例                         |
| ---- | ---------------- | ---------------------------- |
| E    | 错误消息         | E:invaild resource           |
| T    | 媒体总时间       | T:100000                     |
| C    | 媒体当前播放时间 | T:123                        |
| S    | 播放器状态信息   | T:0 (使用数字代替字符串)     |
| I    | 媒体信息         | I:%s=%s (使用键值的方式输出) |
