// Copyright (c) 2016 baimaohui

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

#ifndef FOFA_API_FOFA_H_
#define FOFA_API_FOFA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "cJSON.h"
#include "iconv/iconv.h"

#define CURL_STATICLIB
#include "curl/curl.h"

#pragma comment(lib,"libcurl_imp.lib")
#pragma comment(lib,"libiconv.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")

#define ARRAY_MAX_LEN 1024
#define KEY_LEN 33
#define EMAIL_MAX_LEN 320

#define STRING_ENCODE//字符集转换是否开始

#ifdef __cplusplus 
	extern "C"{ 
#endif


enum ReturnCode{
	SUCCESS = 0,
	ERROR_INVALID_KEY_VALUE,
	ERROR_INVALID_EMAIL_VALUE,
	ERROR_INVALID_PAGE_VALUE,

	ERROR_HANDLER_NOT_NULL,
	ERROR_HANDLER_IS_NULL,
	ERROR_QUERY_IS_NULL,
	ERROR_FIELDS_IS_NULL,
	ERROR_QUERY_RESULTS_IS_NULL,
	ERROR_RESULT_IS_NULL,

	ERROR_ALLOCATE_MEMORY_FAILED,
	ERROR_INIT_FAILED,
	ERROR_PERFORM_FAILED,
	ERROR_CURL_PERFORM_FAILED,
	ERROR_ENCODE_FAILDED,
	ERROR_UTF8_TO_GB2312_FAILED,

	ERROR_QUERY_STRING_LONG,
};

typedef struct{
	char email[EMAIL_MAX_LEN];
	char key[KEY_LEN];
}FofaClient,*FofaClientPtr;

/*
* 解析JSON后，每个该结构体对应其中的一条记录
* @num：对应field字段中值的个数，其为member指针数组实际分配内存使用的个数
* @member：每个该指针存储单条记录中，一条fields字段对应的值
*/
typedef struct{
	char *member[7];
	int num;
}ResultItem,*ResultItemPtr;

/*
* 解析JSON后，返回的结构体，经由该结构体内部的指针，可以查找返回结果
* @num：查找返回的条目数，记录了共有多少个items指针
* @items：结果中每一条结果对应一个Result结构体
*/
typedef struct{
	ResultItem *items;
	int num;
}Result,*ResultPtr;

//创建数组
//client_handler：内存分配于堆，需手动释放
int fofa_new_client(const char *email,const char *key,FofaClientPtr *client_handler);

//进行查询并返回JSON格式字符串
//@query_results：返回的JSON会存储于该字符串，需手动释放内存
int fofa_query_as_json(const char *query,const char *fields,const int page,const FofaClientPtr client_handler,char **query_results);

//进行查询返回JSON格式字符串和Result指针
//@query_results：返回的JSON会存储于该字符串，需手动释放内存
//return:返回的ResultPtr也分配与堆内存上，需调用fofa_destroy_result_struct进行手动释放
ResultPtr fofa_query_as_array(const char *query,const char *fields,const int page,const FofaClientPtr client_handler,char **query_results);

//释放Results指针占用的内存
int fofa_destroy_result_struct(ResultPtr rest);

#ifdef __cplusplus 
	} 
#endif
#endif