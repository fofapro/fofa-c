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

#define STRING_ENCODE//�ַ���ת���Ƿ�ʼ

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
* ����JSON��ÿ���ýṹ���Ӧ���е�һ����¼
* @num����Ӧfield�ֶ���ֵ�ĸ�������Ϊmemberָ������ʵ�ʷ����ڴ�ʹ�õĸ���
* @member��ÿ����ָ��洢������¼�У�һ��fields�ֶζ�Ӧ��ֵ
*/
typedef struct{
	char *member[7];
	int num;
}ResultItem,*ResultItemPtr;

/*
* ����JSON�󣬷��صĽṹ�壬���ɸýṹ���ڲ���ָ�룬���Բ��ҷ��ؽ��
* @num�����ҷ��ص���Ŀ������¼�˹��ж��ٸ�itemsָ��
* @items�������ÿһ�������Ӧһ��Result�ṹ��
*/
typedef struct{
	ResultItem *items;
	int num;
}Result,*ResultPtr;

//��������
//client_handler���ڴ�����ڶѣ����ֶ��ͷ�
int fofa_new_client(const char *email,const char *key,FofaClientPtr *client_handler);

//���в�ѯ������JSON��ʽ�ַ���
//@query_results�����ص�JSON��洢�ڸ��ַ��������ֶ��ͷ��ڴ�
int fofa_query_as_json(const char *query,const char *fields,const int page,const FofaClientPtr client_handler,char **query_results);

//���в�ѯ����JSON��ʽ�ַ�����Resultָ��
//@query_results�����ص�JSON��洢�ڸ��ַ��������ֶ��ͷ��ڴ�
//return:���ص�ResultPtrҲ��������ڴ��ϣ������fofa_destroy_result_struct�����ֶ��ͷ�
ResultPtr fofa_query_as_array(const char *query,const char *fields,const int page,const FofaClientPtr client_handler,char **query_results);

//�ͷ�Resultsָ��ռ�õ��ڴ�
int fofa_destroy_result_struct(ResultPtr rest);

#ifdef __cplusplus 
	} 
#endif
#endif