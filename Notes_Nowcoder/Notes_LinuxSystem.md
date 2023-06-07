# Notes

## Linux系统编程

### GCC 常用参数选项

GCC编译选项                |   说明
-------------             |   -------------
`-E`                      |   预处理指定的源文件，不进行编译
`-S`                      |   编译指定的源文件，但是不进行汇编
`-c`                      |   编译、汇编指定的源文件，但是不进行链接
`-o [file1] [file2]` / `[file2] -o [file1]` |   将文件 file2 编译成可执行文件 file1
`-I directory`            |   指定 include 包含文件的搜索目录
`-g`                      |   在编译的时候，生成调试信息，该程序可以被调试器调试
`-D`                      |   在程序编译的时候，指定一个宏
`-w`                      |   不生成任何警告信息
`-Wall`                   |   生成所有警告信息
`-On`                     |   n的取值范围：0~3。编译器的优化选项的4个级别，`-O0` 表示没有优化，`-O1` 为缺省值，`-O3` 优化级别最高
`-l`                      |   在程序编译的时候，指定使用的库
`-L`                      |   指定编译的时候，搜索的库的路径。
`-fPIC/fpic`              |   生成与位置无关的代码
`-shared`                 |   生成共享目标文件，通常用在建立共享库时
`-std`                    |   指定 C 方言，如:`-std=c99`，`gcc` 默认的方言是 GNU C

一个简单的使用样例：`gcc main.c -o app -I ./include/ -l calc -L ./lib/`

### 静态库与动态库

- 库文件是计算机上的一类文件，可以简单的把库文件看成一种代码仓库，它提供给使用者一些可以直接拿来用的变量、函数或类。
- 库是特殊的一种程序，编写库的程序和编写一般的程序区别不大，只是库不能单独运行。
- 库文件有两种，静态库和动态库（共享库），区别是：静态库在程序的链接阶段被复制到了程序中；动态库在链接阶段没有被复制到程序中，而是程序在运行时由系统动态加载到内存中供程序调用。
- 库的好处：1.代码保密 2.方便部署和分发

#### 工作原理

- 静态库：`GCC` 进行链接时，会把静态库中代码打包到可执行程序中
- 动态库：`GCC` 进行链接时，动态库的代码不会被打包到可执行程序中
  - 程序启动之后，动态库会被动态加载到内存中，通过 `ldd`（list dynamic dependencies）命令检查动态库依赖关系
  - 如何定位共享库文件呢？
当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径。此时就需要系统的动态载入器来获取该绝对路径。对于`elf` 格式的可执行程序，是由 `ld-linux.so` 来完成的，它先后搜索 `elf` 文件的 `DT_RPATH` 段 ——> 环境变量 `LD_LIBRARY_PATH` ——> `/etc/ld.so.cache` 文件列表 ——> `/lib/`，`/usr/lib` 目录找到库文件后将其载入内存。

#### 优缺点

静态库                                    |   动态库
----------                               |   --------
静态库被打包到应用程序中加载速度快          |   加载速度比静态库慢
发布程序无需提供静态库，移植方便            |   发布程序时需要提供依赖的动态库
----------                               |   --------
消耗系统资源，浪费内存                     |   可以控制何时加载动态库
更新、部署、发布麻烦                       |   更新、部署、发布简单
----------                               |   可以实现进程间资源共享（共享库）

### Makefile

```makefile
#定义变量
# add.c sub.c main.c mult.c div.c
src=$(wildcard ./*.c)
objs=$(patsubst %.c, %.o, $(src))
target=app
$(target):$(objs)
    $(CC) $(objs) -o $(target)

%.o:%.c
    $(CC) -c $< -o $@

.PHONY:clean
clean:
    rm $(objs) -f
```

预定义变量:

- `AR`：归档维护程序的名称，默认值为 `ar`；
- `CC`：C 编译器的名称，默认值为 `cc`；
- `CXX`：C++ 编译器的名称，默认值为 `g++`；
- `$@`：目标的完整名称；
- `$<`：第一个依赖文件的名称；
- `$^`：所有的依赖文件；

获取变量的值: `$(变量名)`

`%`：通配符，匹配一个字符串，两个 `%` 匹配的是同一个字符串，例：

```makefile
%.o:%.c
    gcc -c $< -o $@
```

`$(wildcard PATTERN...)`

- 功能：获取指定目录下指定类型的文件列表；
- 参数：`PATTERN` 指的是某个或多个目录下的对应的某种类型的文件，如果有多个目录，一般使用空格间隔；
- 返回：得到的若干个文件的文件列表，文件名之间使用空格间隔；
- 示例：`makefilesrc=$(wildcard ./*.c)`

`$(patsubst <pattern>,<replacement>,<text>)`

- 功能：功能：查找 `<text>` 中的单词(单词以“空格”、“Tab”或“回车”“换行”分隔)是否符合模式 `<pattern>`，如果匹配的话，则以 `<replacement>` 替换；
- 参数：`<pattern>` 可以包括通配符`%`，表示任意长度的字串。如果 `<replacement>` 中也包含`%`，那么，`<replacement>` 中的这个 `%` 将是 `<pattern>` 中的那个 `%` 所代表的字串。(可以用 `\` 来转义，以 `\%` 来表示真实含义的 `%` 字符)；
- 返回：函数返回被替换过后的字符串；
- 示例：`objs=$(patsubst %.c, %.o, $(src))`

### 系统调用函数

#### open()

```cpp
/*
    // 打开一个已经存在的文件
    int open(const char *pathname, int flags);
        参数：
            - pathname：要打开的文件路径
            - flags：对文件的操作权限设置还有其他的设置
              O_RDONLY,  O_WRONLY,  O_RDWR  这三个设置是互斥的
        返回值：返回一个新的文件描述符，如果调用失败，返回-1

    errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号。

    #include <stdio.h>
    void perror(const char *s);作用：打印errno对应的错误描述
        s参数：用户描述，比如hello,最终输出的内容是  hello:xxx(实际的错误描述)
        
    // 创建一个新的文件
    int open(const char *pathname, int flags, mode_t mode);
        参数：
            - pathname：要创建的文件的路径
            - flags：对文件的操作权限和其他的设置
                - 必选项：O_RDONLY,  O_WRONLY, O_RDWR  这三个之间是互斥的
                - 可选项：O_CREAT 文件不存在，创建新文件
            - mode：八进制的数，表示创建出的新的文件的操作权限，比如：0775
            最终的权限是：mode & ~umask
            0777   ->   111111111
        &   0775   ->   111111101
        ----------------------------
                        111111101
        按位与：0和任何数都为0
        umask的作用就是抹去某些权限。

        flags参数是一个int类型的数据，占4个字节，32位。
        flags 32个位，每一位就是一个标志位。
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // 打开一个文件
    int fd = open("a.txt", O_RDONLY);

    // // 创建一个新的文件
    // int fd = open("create.txt", O_RDWR | O_CREAT, 0777);

    if(fd == -1) {
        perror("open");
    }
    // 读写操作

    // 关闭
    close(fd);

    return 0;
}
```

#### read() & write()

```cpp
/*  
    #include <unistd.h> 
    ssize_t read(int fd, void *buf, size_t count);
        参数：
            - fd：文件描述符，open得到的，通过这个文件描述符操作某个文件
            - buf：需要读取数据存放的地方，数组的地址（传出参数）
            - count：指定的数组的大小
        返回值：
            - 成功：
                >0: 返回实际的读取到的字节数
                =0：文件已经读取完了
            - 失败：-1 ，并且设置errno

    #include <unistd.h>
    ssize_t write(int fd, const void *buf, size_t count);
        参数：
            - fd：文件描述符，open得到的，通过这个文件描述符操作某个文件
            - buf：要往磁盘写入的数据，数据
            - count：要写的数据的实际的大小
        返回值：
            成功：实际写入的字节数
            失败：返回-1，并设置errno
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // 1.通过open打开english.txt文件
    int srcfd = open("english.txt", O_RDONLY);
    if(srcfd == -1) {
        perror("open");
        return -1;
    }

    // 2.创建一个新的文件（拷贝文件）
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
    if(destfd == -1) {
        perror("open");
        return -1;
    }

    // 3.频繁的读写操作
    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }

    // 4.关闭文件
    close(destfd);
    close(srcfd);

    return 0;
}
```

#### lseek()

```cpp
/*  
    标准C库的函数
    #include <stdio.h>
    int fseek(FILE *stream, long offset, int whence);

    Linux系统函数
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd, off_t offset, int whence);
        参数：
            - fd：文件描述符，通过open得到的，通过这个fd操作某个文件
            - offset：偏移量
            - whence:
                SEEK_SET
                    设置文件指针的偏移量
                SEEK_CUR
                    设置偏移量：当前位置 + 第二个参数offset的值
                SEEK_END
                    设置偏移量：文件大小 + 第二个参数offset的值
        返回值：返回文件指针的位置

    作用：
        1.移动文件指针到文件头
        lseek(fd, 0, SEEK_SET);

        2.获取当前文件指针的位置
        lseek(fd, 0, SEEK_CUR);

        3.获取文件长度
        lseek(fd, 0, SEEK_END);

        4.拓展文件的长度，当前文件10b, 110b, 增加了100个字节
        lseek(fd, 100, SEEK_END)
        注意：需要写一次数据
*/
```

#### stat() & lstat()

```cpp
struct stat {
    dev_t st_dev; // 文件的设备编号
    ino_t st_ino; // 节点
    mode_t st_mode; // 文件的类型和存取的权限
    nlink_t st_nlink; // 连到该文件的硬连接数目
    uid_t st_uid; // 用户ID
    gid_t st_gid; // 组ID
    dev_t st_rdev; // 设备文件的设备编号
    off_t st_size; // 文件字节数(文件大小)
    blksize_t st_blksize; // 块大小
    blkcnt_t st_blocks; // 块数
    time_t st_atime; // 最后一次访问时间
    time_t st_mtime; // 最后一次修改时间
    time_t st_ctime; // 最后一次改变时间(指属性)
};
/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
        作用：获取一个文件相关的一些信息
        参数:
            - pathname：操作的文件的路径
            - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
        返回值：
            成功：返回0
            失败：返回-1 设置errno

    int lstat(const char *pathname, struct stat *statbuf);
        作用：获取软链接文件相关的一些信息
        参数:
            - pathname：操作的文件的路径
            - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
        返回值：
            成功：返回0
            失败：返回-1 设置errno
*/
```

模拟实现 `ls -l` 指令（[c file](./ls-l.c)）

#### 文件属性操作符 `access()`, `chmod()`, `truncate()`

```cpp
/*
    #include <unistd.h>
    int access(const char *pathname, int mode);
        作用：判断某个文件是否有某个权限，或者判断文件是否存在
        参数：
            - pathname: 判断的文件路径
            - mode:
                R_OK: 判断是否有读权限
                W_OK: 判断是否有写权限
                X_OK: 判断是否有执行权限
                F_OK: 判断文件是否存在
        返回值：成功返回0， 失败返回-1

    #include <sys/stat.h>
    int chmod(const char *pathname, mode_t mode);
        修改文件的权限
        参数：
            - pathname: 需要修改的文件的路径
            - mode:需要修改的权限值，八进制的数
        返回值：成功返回0，失败返回-1

    #include <unistd.h>
    #include <sys/types.h>
    int truncate(const char *path, off_t length);
        作用：缩减或者扩展文件的尺寸至指定的大小
        参数：
            - path: 需要修改的文件的路径
            - length: 需要最终文件变成的大小
        返回值：
            成功返回0， 失败返回-1
*/
```

#### 目录操作符 `mkdir()`, `rename()`, `chdir()`, `getcwd()`

```cpp
/*
    #include <sys/stat.h>
    #include <sys/types.h>
    int mkdir(const char *pathname, mode_t mode);
        作用：创建一个目录
        参数：
            pathname: 创建的目录的路径
            mode: 权限，八进制的数
        返回值：
            成功返回0， 失败返回-1
    
    #include <stdio.h>
    int rename(const char *oldpath, const char *newpath);

    #include <unistd.h>
    int chdir(const char *path);
        作用：修改进程的工作目录
            比如在/home/nowcoder 启动了一个可执行程序a.out, 进程的工作目录 /home/nowcoder
        参数：
            path : 需要修改的工作目录

    #include <unistd.h>
    char *getcwd(char *buf, size_t size);
        作用：获取当前工作目录
        参数：
            - buf : 存储的路径，指向的是一个数组（传出参数）
            - size: 数组的大小
        返回值：
            返回的指向的一块内存，这个数据就是第一个参数
*/
```
