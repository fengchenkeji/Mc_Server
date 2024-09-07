#include <iostream>
#include <cstdlib>
#include <ncurses.h>  // 包含 ncurses 头文件
extern "C" void download(const char* url, const char* filename, long fileSize, int numThreads);
void aarch() {
    std::string command = "arch";
    FILE* pipe = popen(command.c_str(), "r");  // 执行命令获取系统架构
    if (!pipe) {
        std::cerr << "popen() 失败！" << std::endl;
        return;
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;  // 读取命令输出
    }
    pclose(pipe);  // 关闭管道

    // 移除字符串末尾的换行符
    result.erase(result.find_last_not_of("\n") + 1);

    initscr();             // 初始化 ncurses
    cbreak();              // 禁用行缓冲
    noecho();              // 不显示输入字符

    // 根据系统架构输出相应的信息
    if (result == "x86_64") {
        printw("64位 x86 架构 (x86_64)\n");
    } else if (result == "i386" || result == "i686") {
        printw("32位 x86 架构 (i386/i686)\n");
    } else if (result == "aarch64") {
        printw("64位 ARM 架构 (ARM64)\n");
    } else if (result == "armv7l" || result == "armv6l" || result == "arm") {
        printw("32位 ARM 架构\n");
    } else {
        printw("未知架构: %s\n", result.c_str());
    }

    refresh();             // 刷新屏幕以显示输出
    getch();               // 等待用户输入
    endwin();              // 结束 ncurses 模式
}
void dow_java() {
  if (result == aarch64) {

  }
}
