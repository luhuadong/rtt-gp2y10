# rtt-gp2y10
Support the Particulate Matter sensor for GP2Y1010AU0F and GP2Y1014AU0F



## 1、介绍

gp2y10 软件包适用于 GP2Y10 系列光学灰尘检测传感器模块，包括 GP2Y1010AU0F 和 GP2Y1014AU0F。



### 1.1 目录结构

| 名称     | 说明                           |
| -------- | ------------------------------ |
| docs     | 文档目录                       |
| examples | 例子目录（提供两种操作示例）   |
| inc      | 头文件目录                     |
| src      | 源代码目录（提供两种驱动接口） |

驱动源代码提供两种接口，分别是自定义接口，以及 RT-Thread 设备驱动接口（open/read/control/close）。



### 1.2 许可证

gp2y10 软件包遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。



### 1.3 依赖

- RT-Thread 4.0+
- 使用动态创建方式需要开启动态内存管理模块
- 使用 sensor 设备接口需要开启 sensor 设备驱动框架模块



## 2、获取 gp2y10 软件包

使用 gp2y10 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] sensors drivers  --->
            [*] GP2Y10: dust sensor by Sharp for detect air quality.
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。



## 3、使用 gp2y10 软件包

### 3.1 版本说明

| 版本   | 说明                                                         |
| ------ | ------------------------------------------------------------ |
| v0.8.0 | 支持自定义接口和 sensor 框架，已在 DHT11 和 DHT22 上完成测试 |
| latest | 进一步优化                                                   |

目前处于公测阶段，建议开发者使用 latest 版本。



### 3.2 配置选项

- 是否使用示例程序（`PKG_USING_GP2Y10_SAMPLE`）
- 是否使用动态内存
- 是否使用 sensor 框架



## 4、注意事项

1. 为传感器对象提供静态创建和动态创建两种方式，如果使用动态创建，请记得在使用完毕释放对应的内存空间。
2. 由于 GP2Y10 模块采用光学检测原理，因此出来需要配置 ADC 引脚，还需要指定一个 GPIO 引脚（iled_pin），用于发射光脉冲。



## 5、相关文档

见 docs 目录。



## 6、联系方式

- 维护：luhuadong@163.com
- 主页：<https://github.com/luhuadong/rtt-gp2y10>