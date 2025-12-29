#include "my_string.h"

// 解决 Keil 自带 atof 可能无效的问题
float my_atof(char *s) {
    float val = 0.0f;
    float power = 1.0f;
    int i = 0;
    int sign = 1;
    // 处理负号
    if (s[i] == '-') {
        sign = -1;
        i++;
    }
    // 处理整数部分
    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        val = 10.0f * val + (s[i] - '0');
    }
    // 处理小数部分
    if (s[i] == '.') {
        i++;
        for (; s[i] >= '0' && s[i] <= '9'; i++) {
            val = 10.0f * val + (s[i] - '0');
            power *= 10.0f;
        }
    }
    return sign * val / power;
}

// 简单的子串查找
uint8 my_strstr(char* s1, char* s2) {
    int i, j;
    int len1 = 0; 
	int len2 = 0; 
	while(s1[len1]) len1++;
	while(s2[len2]) len2++;
    if(len2 > len1) return 0;
    for(i = 0; i <= len1 - len2; i++) {
        for(j = 0; j < len2; j++) {
            if(s1[i+j] != s2[j]) break;
        }
        if(j == len2) return 1;
    }
    return 0;
}