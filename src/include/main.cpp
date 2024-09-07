#include <ncurses.h>
#include <string>
#include <locale.h>
#include <wchar.h>
#include <cstdlib> // For system
#include "check_java.h"

#define NUM_OPTIONS 3
#define BUTTONS 2

// 绘制边框函数
void drawBorder(int startY, int height, int width) {
    attron(COLOR_PAIR(1));  // 启用颜色对
    mvhline(startY, 0, ACS_HLINE, width);            // 绘制上边框
    mvhline(startY + height - 1, 0, ACS_HLINE, width);  // 绘制下边框
    mvvline(startY, 0, ACS_VLINE, height);           // 绘制左边框
    mvvline(startY, width - 1, ACS_VLINE, height);   // 绘制右边框
    mvaddch(startY, 0, ACS_ULCORNER);               // 左上角
    mvaddch(startY, width - 1, ACS_URCORNER);       // 右上角
    mvaddch(startY + height - 1, 0, ACS_LLCORNER);  // 左下角
    mvaddch(startY + height - 1, width - 1, ACS_LRCORNER); // 右下角
    attroff(COLOR_PAIR(1)); // 关闭颜色对
}

// 示例函数：检查 Java 版本
void check_java() {
    // 这里放置检查 Java 版本的逻辑代码
    // 此处示例为简单打印消息
    clear();
    printw("检查 Java 版本...\n");
    aarch();
    refresh();
    getch();  // 等待用户按键以查看消息
}

int main() {
    setlocale(LC_ALL, "");  // 设置本地化以支持宽字符

    const wchar_t *title = L"Minecraft Server Tool";  // 使用宽字符表示标题

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // 初始化颜色对

    int height, width;
    getmaxyx(stdscr, height, width);
    int borderHeight = height * 0.8;  // 修改边框高度为80%
    int borderY = 0;  // 边框从屏幕顶部开始

    int choice = 0;
    int button = 0;
    const wchar_t *options[NUM_OPTIONS] = {L"检查java", L"查看服务器列表", L"创建新的服务器"};  // 使用宽字符数组
    const char *buttons[BUTTONS] = {"OK", "Exit"};  // 按钮仍然可以使用窄字符

    while (true) {
        clear();

        // 绘制边框
        drawBorder(borderY, borderHeight, width);

        // 打印标题
        mvaddwstr(borderY + 1, (width - wcswidth(title, wcslen(title))) / 2, title);  // 使用宽字符函数

        // 打印选项
        for (int i = 0; i < NUM_OPTIONS; ++i) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvaddwstr(borderY + 3 + i, (width - wcswidth(options[i], wcslen(options[i]))) / 2, options[i]);  // 使用宽字符函数
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        // 打印按钮
        for (int j = 0; j < BUTTONS; ++j) {
            if (j == button) {
                attron(A_REVERSE);
            }
            int buttonX = (width - (BUTTONS - 1) * 9) / 2 + j * 9;  // 计算每个按钮的位置
            mvprintw(borderY + borderHeight - 1 - 1, buttonX, "%s", buttons[j]);  // 打印按钮
            if (j == button) {
                attroff(A_REVERSE);
            }
        }

        int ch = getch();

        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + NUM_OPTIONS) % NUM_OPTIONS;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % NUM_OPTIONS;
                break;
            case KEY_LEFT:
                button = (button - 1 + BUTTONS) % BUTTONS;
                break;
            case KEY_RIGHT:
                button = (button + 1) % BUTTONS;
                break;
            case 10:  // Enter key
                if (button == 0) {  // 如果 "OK" 按钮被按下
                    if (choice == 0) {
                        check_java();  // 执行检查 Java 的代码块
                    } else if (choice == 1) {
                        mvprintw(borderY + borderHeight - 1 - 15 + 2, (width - wcswidth(options[choice], wcslen(options[choice]))) / 2, "正在查看服务器列表...");
                        refresh();
                        getch();
                    } else if (choice == 2) {
                        mvprintw(borderY + borderHeight - 1 - 15 + 2, (width - wcswidth(options[choice], wcslen(options[choice]))) / 2, "正在创建新的服务器...");
                        refresh();
                        getch();
                    }
                } else if (button == 1) {  // 如果 "Exit" 按钮被按下
                    endwin();
                    return 0;
                }
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, height, width);
                borderHeight = height * 0.8;
                borderY = 0;
                break;
        }

        refresh();
    }

    endwin();
    return 0;
}
