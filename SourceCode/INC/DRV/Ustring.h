#include "..\inc\def.h"

//int 到Unicode字符串的转换
void Int2Unicode(int number, U16 str[]);
//Unicode字符串到int的转换
int Unicode2Int(U16 str[]);
//char类型包括GB编码,到Unicode的编码转换
void strChar2Unicode(U16 ch2[], const char ch1[]);
void UstrCpy(U16 ch1[],U16 ch2[]);
