# rtt-dhtxx
Digital Humidity and Temperature sensor (communicate via single bus), including DHT11, DHT21 and DHT22



## 1、介绍

dhtxx 软件包适用于单总线式数字温湿度传感器，包括 DHT11、DHT21 和 DHT22，并提供多种操作接口。



### 1.1 目录结构

| 名称     | 说明                           |
| -------- | ------------------------------ |
| docs     | 文档目录                       |
| examples | 例子目录（提供两种操作示例）   |
| inc      | 头文件目录                     |
| src      | 源代码目录（提供两种驱动接口） |

驱动源代码提供两种接口，分别是自定义接口，以及对接sensor框架的接口，可使用RT-Thread设备驱动接口（open/read/control/close）进行操作。



### 1.2 许可证

dhtxx package 遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。



### 1.3 依赖

- RT-Thread 4.0+
- 使用动态创建方式需要开启动态内存管理模块
- 使用 sensor 设备接口需要开启 sensor 设备驱动框架模块



## 2、获取 dhtxx 软件包

使用 dhtxx package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] sensors drivers  --->
            [*] DHTxx one-wire sensor driver package
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。



## 3、使用 dhtxx 软件包

### 3.1 版本说明

目前处于公测阶段，暂时只提供 latest 版本。



### 3.2 配置选项

- 是否使用示例
- 是否使用动态内存
- 是否使用 sensor 框架



## 4、注意事项

- 为传感器对象提供静态创建和动态创建两种方式，如果使用动态创建，请在使用完毕释放对应的内存空间。
- 由于 DHTxx 模块包含一个温度传感器和一个湿度传感器，因此在 sensor 框架中会注册两个设备，内部提供1位FIFO缓存进行同步，缓存空间在调用 rt_device_open 函数时创建，因此 read 之前务必确保两个设备都开启成功。
- dhtxx 软件包为支持 DHT11、DHT22 等多种传感器，目前借用了 conf.intf.type 进行标识，在初始化时需要指定。



## 5、相关文档

见 docs 目录。



## 6、联系方式

- 维护：luhuadong@163.com
- 主页：<https://github.com/luhuadong/rtt-dhtxx>

