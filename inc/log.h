#pragma once
#include <cstdio>
#define LOG_INFO(fmt, ...) fprintf(stdout, "[INFO] " fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) fprintf(stderr, "[ERR] " fmt, ##__VA_ARGS__)