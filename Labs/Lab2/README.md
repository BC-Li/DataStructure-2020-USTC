# README

本文档对实验中遇到的文件进行说明并给出使用说明

## 使用说明

可使用git下载本仓库

```bash
git clone https://git.ustc.edu.cn/ds-fa20/queue.git
```

本实验需要在Linux操作系统下完成。

需要的安装的工具有 git，CMake，Valgrind，编译器GCC或Clang。

可以在命令行界面下输入如下命令安装。

```bash
sudo apt-get install cmake
```

在实验主目录下，输入以下命令即可对项目进行编译并运行。

```
./build.sh
./run.sh
```

编译完成后，你可以进入build目录下，对某一个测试单独进行运行。

```bash
./test_queue
valgrind ./test_memory
```



## 测试说明

```bash
# 使用提供的测试文件进行测试，在实验主目录下输入
./run.sh
```

如果正确的话，则catch测试都会绿色通过，valgrind测试0 error。

通过普通队列、优先队列、排队模拟分别得3分，通过内存测试得1分。



## 文件说明

queue.h中包含了普通队列的相关操作，需要补充todo部分。

priority_queue.h包含了优先队列的相关操作，需要补充todo部分。

simulation.cc中包含了模拟的主要过程，需要补充todo部分。

**请不要修改其他部分！**

CMakeList.txt是cmake的配置文件。

test/*.cc是4个测试文件，分别会测试普通队列、优先队列、模拟结果的正确性。

test/catch.hpp是C++单元测试框架。

util中是与排队模拟相关的工具函数。



## 实验要求

填写所有的todo部分，**请不要修改其他部分！**

### 数据结构部分

#### 实现操作

在**queue.h** 和**priority_queue.h**中需要填写这两个数据结构的操作。

1. **init**函数对数据结构进行初始化。

2. **destroy**函数负责清除分配堆内存的空间。

3. **enqueue**函数会将一个元素加入到队列中。如果队列已满，返回false；否则，返回true。

4. **dequeue**函数将队头元素从队列中取出。如果队列中没有数据，返回false；否则，返回true。

5. **top**函数会访问队列中的元素，但不会改变队列。如果队列中没有数据，返回false；否则，返回true。

6. **length**函数返回队列中元素的个数。

7. **full**和**empty**函数分别判断队列是否为满或者为空。

#### 实现细节

1. queue 和 priority queue 最大的不同点在于：queue 要求先进入队列的元素最先出来，而 priority queue 要求把在队列数据中最小的元素放入队头，让其成为最先出来的元素。
2. 两个数据结构中的 data 域是存放数据的数组， capacity 域表示队列能容纳的最大元素个数。由于队空和队满之间存在 capacity + 1 种状态，data 也需要分配 capacity + 1 的容量以区分。
3. queue 的 front 指向队头元素，而 rear 指向下一个元素插入的位置。
4. priority queue 的实现：数据只能存放在 data[1] 和 data[rear] 之间，data[1] 一定是优先队列中元素最小的，且满足 data[i/2] < data[i] (i > 1)。元素入队时，放入队列末尾，判断是否满足 data[i/2] < data[i]。若满足可返回；若不满足则交换 data[i/2] 和 data[i] 的位置，并继续判断。元素出队时，将队尾元素放入队头，交换所有不满足 data[i/2] < data[i] 的元素。

5. 为了通用性，priority queue 需要比较元素大小的函数指针 cmp。cmp(a, b) > 0 表示 a > b。

### 队列模拟部分

你需要在**simulation.cc**中填写相关操作。最终得到**办理业务总人数**，和他们在**排队等待的总时间**。返回排队等待的平均时间。

Customer中分别有3个数据，分别是：arrive_time，service_time, leave_time。它们分别表示客户到达银行的时间点，客户办理业务需要的时间，客户最终离开的时间点。

simulate函数中有3个队列数据结构，都存储Customer类型，分别是：arrive_flow、queue、leave_flow。

arrive_flow已生成好并作为simulate函数的参数，已经**根据进入银行的时间做了从小到大的排序**，且arrive_time，service_time已经根据指数分布随机生成。

queue是客户在银行内排队等待的一个队列。如果所有的服务台都被占用，就会有客户在等待。如果等待人数超过队列容量queue_capacity，新来的客户无法排队，直接离开。

leave_flow是所有正在办理业务的客户的优先队列，根据客户离开时间进行排序。容量大小就是服务台数量num_servers。你需要自己**设定好leave_time和Customer在优先队列中的比较函数**。

从客户视角上看。客户在arrive_time进入银行（arrive_flow 出队）。如果银行排队的人太多了，就直接走了；否则加入排队（queue 入队）。如果有一个服务台没有人办业务，自己在队头，就能结束排队（queue 出队），用service_time的时间办业务（leave_flow 入队）。到了leave_time后就离开银行 （leave_flow 出队）。

从~~上帝~~编程视角上看。只会发生3种事件，其中只有2种事件会改变系统，一个是**客户进入银行**，另一个是**客户离开银行**。也只有在这两个事件发生后，**排队等待中的客户开始办理业务**的事情才会发生。因此~~作为膜法师~~，只需要~~操控时间~~，选择做最近的这两类事件，然后判断能不能做第三种事件。

## 问题与回答

问：什么是 .cc 文件格式？

答 ：Linux/Uinx 环境下的 C++ 程序通常使用 .cc 结尾。

问：为什么按照这样的方式组织文件而不是把所有的写在一个文件中？

答：把所有代码写到一个文件下是工程上的灾难！另外，每个测试文件都是可执行文件，而main函数只有1个，为了避免冲突也只能拆分。

问：我是 C++ 大佬，为什么要用 struct 而不是 class ？我是 C++ 小白，每个函数里的::是什么意思？

答：因为我们既要保证易用性 （数据结构既能装int数据，又能装Customer），又要保证所填代码 C++ 特性极可能的少，使得不会 C++ 的同学也能填写。不会 C++ 的同学只需要知道教程上的引用`&`，以及在 simulation.cc 中调用数据结构函数时在前面加上`类型::`即可。

问：为什么在 simulation.cc 里用 queue.data 会报错？

答：排队模拟是应用层的，而 queue.data 是抽象的数据结构层的，已被设置为private。它们之间的交互请使用我们提供的函数。计算机科学很重要的一个思想就是抽象。不然，程序员就该操作物理层电子的流动了。


## 附录

### Linux环境配置

- 可以下载虚拟机与镜像进行安装

  - virtualbox下载链接: https://rec.ustc.edu.cn/share/49792820-0967-11eb-8e75-2f662354f3be

  - ubuntu16.04 下载链接：https://rec.ustc.edu.cn/share/6af39cd0-0967-11eb-994e-7d08e505ac68

- 也可以使用学校提供的vlab
  - https://vlab.ustc.edu.cn/

### 实验中用到的c++特性

- 类模板 可以理解成对书本上使用elemtype作为元素类型的ADT的一种具体实现，利用类模板生成类的代码为` Queue<Customer> queue`也即`模板名<元素类型名> 实例化得到的对象名 `
- 访问控制 可以简单地理解为private成员只能在该类的成员函数中使用，public可以在其它类的成员函数中使用。（关于友元函数和protect此处并未做更进一步地描述）
- 引用 引用是一个别名`int & a= b`, 则 a是b的一个别名，对`a`的修改会直接作用在b上。

### 概率统计的背景

可以了解，对于写实验并没有太大帮助。

蒙特卡洛模拟

https://en.wikipedia.org/wiki/Monte_Carlo_method

置信区间~~（你们做大物实验应该知道）~~

https://en.wikipedia.org/wiki/Confidence_interval

排队论 （我们模拟的是M/M/c/N模型）

https://www2.isye.gatech.edu/~sman/courses/6644/Module05Q-QueueingSlides_171025.pdf



