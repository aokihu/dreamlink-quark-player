Change Log

# 3.0.0

播放输出类型增加混合输出模式`fusion`,在该模式下播放器可以同时输出到`local`和`net`,不做具体输出区分

## 命令行参数

增加命令行参数`--output-list`,这是一个字符串类型的参数,表示输出端口的数组,使用`,`作为分隔符。

本地设备参数格式:`dev://$dev_name`
网络输出参数格式:`udp://$address:$port`

## 启动命令数据结构

`QP_CmdParam`数据结构中增加`fusion_output`字段，用于保存**fusion**输出参数字符串