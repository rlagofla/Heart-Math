#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// 判断操作系统，使用不同的清屏操作
#ifdef _WIN32
#define clean system("cls")
#else
#define clean printf("\033c")
#endif

// parameter list
#define PI 3.1415926536
#define COL 40  // 40; 91
#define ROW 11  // 12; 26
#define yFctr 0.70  // 0.70; 1.5
#define xFctr 1.40  // 1.40; 3
#define GRDNT 0.03  // gradient
#define PERIOD 4  // 实际上是5，0开始计数
#define O_MAX 6
#define O_MIN 4
#define I_MAX 10
#define I_MIN 7
#define SLEEP 100000  // 100000; 120000

int picture[2 * ROW][2 * COL] = {0};
char pixel[] = " @$#*!=;:~-,.";

// 一些会复用的全局变量
// 使用全局变量不是一个好习惯，但我这就一个单文件，管他呢，这样函数的封装也好看点
double t, x, y;
double dxt, dyt, dist;
int isShrink = 1, cnt = -1;

// Heart Function and Its Derivative
double xt(double t) { return 16 * sin(t) * sin(t) * sin(t); }

double yt(double t) { return 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t) + 2; }

double dxdt(double t) { return 16 * 3 * sin(t) * sin(t) * cos(t); }

double dydt(double t) { return -13 * sin(t) + 5 * 2 * sin(2 * t) + 2 * 3 * sin(3 * t) + 4 * sin(4 * t); }

// Curve
double xFactor() {
    if (isShrink)
        return -GRDNT * cnt + xFctr;
    else
        return GRDNT * (cnt - 1) + (xFctr - GRDNT * PERIOD);
}

double yFactor() {
    if (isShrink)
        return -GRDNT * cnt + yFctr;
    else
        return GRDNT * (cnt - 1) + (yFctr - GRDNT * PERIOD);
}

double curveOuter() {
    if (isShrink)
        return -fabs((O_MAX - O_MIN) / (PERIOD / 2.0) * (cnt - (PERIOD / 2.0))) + O_MAX;
    else
        return O_MIN;
}

double curveInner() {
    if (isShrink)
        return (I_MAX - I_MIN) / (PERIOD * PERIOD * PERIOD * 1.0) * cnt * cnt * cnt + I_MIN;
    else
        return (I_MIN - I_MAX) / (PERIOD * PERIOD * PERIOD * 1.0) * cnt * cnt * cnt + I_MAX;
}

int isDotProdPost(double x1, double y1, double x2, double y2) { return x1 * x2 + y1 * y2 > 0 ? 1 : 0; }

void outer();

void inner();

int main() {
    while (1) {
        ++cnt;
        memset(picture, 0, sizeof(picture));
        // calculate pixels
        for (t = 0; t < 2 * PI; t += 0.01) {
            x = xFactor() * xt(t);
            y = yFactor() * yt(t);
            picture[ROW - (int) y][COL + (int) x] = 1;

            dyt = dydt(t);
            dxt = dxdt(t);
            dist = sqrt(dyt * dyt + dxt * dxt);

            inner();
            if (t < 0.5 || t > 2 * PI - 0.5) continue;  // -> 注释
            outer();
        }
        // print current heart
        for (int i = 0; i < 2 * ROW; ++i) {
            for (int j = 0; j < 2 * COL; ++j) {
                printf("%c", pixel[picture[i][j]]);
            }
            printf("\n");
        }
        // flush
        if (cnt == 4) {  // a period
            isShrink = -isShrink + 1;  // toggle between 1 & 0
            cnt = -1;  // count from 0
        }
        // stay and clean screen
        usleep(SLEEP);
        clean;
    }
    return 0;
}

void outer() {
    if (!isDotProdPost(x, y, dyt, -dxt)) {
        dyt = -dyt;
        dxt = -dxt;
    }
    for (double id = 1; id < curveOuter(); id += 0.5) {
        int a = x + id * dyt / dist;
        int b = y - yFactor() * id * dxt / dist;
        if (a < COL && b < ROW && a > -COL && b > -ROW &&
            picture[ROW - b][COL + a] != 1)
            picture[ROW - b][COL + a] = 12;
    }
}

void inner() {
    if (isDotProdPost(x, y, dyt, -dxt)) {
        dyt = -dyt;
        dxt = -dxt;
    }
    for (double id = 1; id < curveInner(); id += 0.5) {
        int a = x + id * dyt / dist;
        int b = y - id * dxt / dist;
        if (a < COL && b < ROW && a > -COL && b > -ROW &&
            (picture[ROW - b][COL + a] == 0 || picture[ROW - b][COL + a] > id) &&
            picture[ROW - b][COL + a] != 12)
            picture[ROW - b][COL + a] = id + 1;
        if (t < PI + 0.5 && t > PI - 0.5 && picture[ROW - b][COL + a] > id) return;  // -> 注释
    }
}