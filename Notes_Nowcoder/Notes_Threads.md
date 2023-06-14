# Notes of Thread

## 线程概述

与进程（process）类似，线程（thread）是允许应用程序并发执行多个任务的一种机制。一个进程可以包含多个线程。同一个程序中的所有线程均会独立执行相同程序，且共享同一份全局内存区域，其中包括初始化数据段、未初始化数据段，以及堆内存段。（传统意义上的 UNIX 进程只是多线程程序的一个特例，该进程只包含一个线程）

进程是 CPU 分配资源的最小单位，线程是操作系统调度执行的最小单位。

线程是轻量级的进程（LWP：Light Weight Process），在 Linux 环境下线程的本质仍是进程。

查看指定进程的 `LWP` 号：`ps –Lf pid`

### 线程和进程的区别

1. 进程间的信息难以共享。由于除去只读代码段外，父子进程并未共享内存，因此必须采用一些进程间通信方式，在进程间进行信息交换。
2. 调用 `fork()` 来创建进程的代价相对较高，即便利用写时复制技术，仍然需要复制诸如内存页表和文件描述符表之类的多种进程属性，这意味着 `fork()` 调用在时间上的开销依然不菲。
3. 线程之间能够方便、快速地共享信息。只需将数据复制到共享（全局或堆）变量中即可。
4. 创建线程比创建进程通常要快 10 倍甚至更多。线程间是共享虚拟地址空间的，无需采用写时复制来复制内存，也无需复制页表。

#### 线程之间共享和非共享资源

|          共享资源              |    非共享资源            |
|          -------              |    ---------            |
| 进程 ID 和父进程 ID            |    线程 ID               |
| 进程组 ID 和会话 ID            |    信号掩码              |
| 用户 ID 和 用户组 ID           |    线程特有数据           |
| 文件描述符表                   |    error 变量            |
| 信号处置                      |    实时调度策略和优先级    |
| 文件系统的相关信息：<br>文件权限掩码（umask）、当前工作目录  |    栈，本地变量和函数的调用链接信息                  |
| 虚拟地址空间（除栈、.text）    |    /    |

## 线程操作

|           函数名              |         签名            |
|          -------              |    ---------            |
|      [pthread_create](./OperatorsOfThread/pthread_create.c)           |   `int pthread_create` <br> `(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);` |
| [pthread_self](./OperatorsOfThread/pthread_exit.c) | `pthread_t pthread_self(void);`|
| [pthread_equal](./OperatorsOfThread/pthread_exit.c)   | `int pthread_equal(pthread_t t1, pthread_t t2);`  |
| [pthread_exit](./OperatorsOfThread/pthread_exit.c)   | `void pthread_exit(void *retval);`  |
| [pthread_join](./OperatorsOfThread/pthread_join.c)    | `int pthread_join(pthread_t thread, void **retval);`  |
| [pthread_detach](./OperatorsOfThread/pthread_detach.c)   | `int pthread_detach(pthread_t thread);`  |
| [pthread_cancel](./OperatorsOfThread/pthread_cancel.c)   | `int pthread_cancel(pthread_t thread);`  |

### 线程属性

线程属性 [案例](./OperatorsOfThread/pthread_attr.c)

## 线程同步（最重要）

线程的主要优势在于，能够通过全局变量来共享信息。不过，这种便捷的共享是有代价的：必须确保多个线程不会同时修改同一变量，或者某一线程不会读取正在由其他线程修改的变量。

**临界区**是指访问某一共享资源的代码片段，并且这段代码的执行应为原子操作，也就是同时访问同一共享资源的其他线程不应终端该片段的执行。

**线程同步**：即当有一个线程在对内存进行操作时，其他线程都不可以对这个内存地址进行操作，直到该线程完成操作，其他线程才能对该内存地址进行操作，而其他线程则处于等待状态。

### 互斥量

为避免线程更新共享变量时出现问题，可以使用互斥量（mutex 是 mutual exclusion 的缩写）来确保同时仅有一个线程可以访问某项共享资源。可以使用互斥量来保证对任意共享资源的原子访问。

互斥量有两种状态：已锁定（locked）和未锁定（unlocked）。任何时候，至多只有一个线程可以锁定该互斥量。试图对已经锁定的某一互斥量再次加锁，将可能阻塞线程或者报错失败，具体取决于加锁时使用的方法。一旦线程锁定互斥量，随即成为该互斥量的所有者，只有所有者才能给互斥量解锁。

#### 互斥量相关函数

互斥量相关函数 [案例](./OperatorsOfLock/mutex.c)

#### 死锁

有时，一个线程需要同时访问两个或更多不同的共享资源，而每个资源又都由不同的互斥量管理。当超过一个线程加锁同一组互斥量时，就有可能发生死锁。

两个或两个以上的进程在执行过程中，因争夺共享资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁。

死锁的几种场景：

- 忘记释放锁
- 重复加锁
- 多线程多锁，抢占锁资源

#### 读写锁

当有一个线程已经持有互斥锁时，互斥锁将所有试图进入临界区的线程都阻塞住。但是考虑一种情形，当前持有互斥锁的线程只是要读访问共享资源，而同时有其它几个线程也想读取这个共享资源，但是由于互斥锁的排它性，所有其它线程都无法获取锁，也就无法读访问共享资源了，但是实际上多个线程同时读访问共享资源并不会导致问题。

在对数据的读写操作中，更多的是读操作，写操作较少，例如对数据库数据的读写应用。为了满足当前能够允许多个读出，但只允许一个写入的需求，线程提供了读写锁来实现。

读写锁的特点：

- 如果有其它线程读数据，则允许其它线程执行读操作，但不允许写操作。
- 如果有其它线程写数据，则其它线程都不允许读、写操作。
- 写是独占的，写的优先级高。

#### 读写锁相关操作

读写锁相关操作 [案例](./OperatorsOfLock/rwlock.c)

#### 生产者消费者问题

生产者消费者问题 代码：C 实现（[条件变量](./OperatorsOfLock/prodcust.c)）（[信号量](./OperatorsOfLock/semaphore.c)）、[C++ 实现](../HandWriting/MultiThread/Producer_Consumer.cpp)
