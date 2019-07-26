#ifndef __COMM_H__
#define __COMM_H__

#include <glib.h>
#include <gio/gio.h>

//项目工程结构体,会作为私有指针在回调函数的最后一个参数出现
//方便随时检阅项目的其它数据
typedef struct{
    int xxx;
    char sss;
}private_data;

//测试用的用于序列化的结构体
typedef struct{
    int i;
    char s[3];
    double d;
    float f[3];
}test_strct;

GVariant *en_pkg(void *data, int dataLen);
void de_pkg(GVariant *var, void *data, int dataLen);

#endif