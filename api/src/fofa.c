// Copyright (c) 2016 baimaohui

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

#include "fofa.h"

char *res_buf = NULL;
int shift;

int str_chr(char *s,char ch)
{
	int i;
	for(i=0;s[i] != '\0';i++)
		if(ch==s[i])
			return i;
	return -1;
}

size_t fofa_curl_callback_writefunction(void *buffer,size_t size,size_t nmemb,void *userp)
{
	int res_size;
	res_size = size * nmemb;
	res_buf = (char *)realloc(res_buf,shift + res_size + 1);
	memcpy(res_buf + shift,buffer,res_size);
	shift += res_size;
	//printf("res_buf:%s\n",res_buf);
	return size * nmemb;
}

/*
* ����fofa client handler,�����ʹ�������в�ѯ����
* @email����¼ʱ��eami
* @key��API KEY
* @client_handler���ṹ�壬�洢��eamil��key���ڴ˺���в�ѯ����ʱ������ֱ�ӵ��á����ڴ�洢�����ֶ��ͷš�
*/
int fofa_new_client(const char *email,const char *key, FofaClientPtr *client_handler)
{
	if((!email) || (strlen(email) > 320)) return ERROR_INVALID_EMAIL_VALUE;
	
	if((!key) || (strlen(key) != 32)) return ERROR_INVALID_KEY_VALUE;
	if(*client_handler) return ERROR_HANDLER_NOT_NULL;
	
	*client_handler = (FofaClientPtr)malloc(sizeof(FofaClient));//FofaClientPtr��Ҫ�ֶ��ͷ��ڴ�
	if(!client_handler) return ERROR_ALLOCATE_MEMORY_FAILED;

	strcpy((*client_handler)->email,email);
	strcpy((*client_handler)->key,key);

	return SUCCESS;
}

/*
* ����ָ���ļ������������в�ѯ����JSON��ʽ���ؽ��
* @query����ѯ����
* @fields����ѯʱ���ֶ�
* @page����ѯ�ڼ�ҳ
* @client_handler���洢��email��key�Ľṹ��ָ��
* @query_requests����ѯ����ķ��أ����ڴ棬��Ҫ�ֶ��ͷ�
*/
int fofa_query_as_json(const char *query, const char *fields, const int page, const FofaClientPtr client_handler,char **query_results)
{
	char qbase64[ARRAY_MAX_LEN] = {0};
	char query_params[ARRAY_MAX_LEN *3] = {0};
	CURL *curl_handler = NULL;
	CURLcode res;
	int ress;
	size_t rc;
	char *output;
	char *old_output;
	char *old_input;
	size_t input_len;
	size_t output_len;
	iconv_t conveter;
	int index = 0;

	if(!query) return ERROR_QUERY_IS_NULL;	
	if(!fields) return ERROR_FIELDS_IS_NULL;
	if(page <= 0) return ERROR_INVALID_PAGE_VALUE;
	if(!client_handler) return ERROR_HANDLER_IS_NULL;
	//if(!query_results) return ERROR_QUERY_RESULTS_IS_NULL;
	if((strlen(query) > sizeof(qbase64)) || (strlen(query) > sizeof(query_params))) return ERROR_QUERY_STRING_LONG;
	
	//��@query����base64����
	ress = Base64Encode(qbase64,query,strlen(query));	
	if(ress == -1) return ERROR_ENCODE_FAILDED;
	ress = sprintf(query_params,"https://fofa.so/api/v1/search/all?email=%s&key=%s&qbase64=%s&page=%d&fields=%s",client_handler->email,client_handler->key,qbase64,page,fields);
	if(ress == -1) return ERROR_QUERY_STRING_LONG;
	
	//ʹ��libcurl����fofa��ѯAPI�������ݲ�ѯ��
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handler = curl_easy_init();

	if(!curl_handler) return ERROR_INIT_FAILED;

	curl_easy_setopt(curl_handler, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handler,CURLOPT_URL,query_params);

	curl_easy_setopt(curl_handler,CURLOPT_WRITEFUNCTION,fofa_curl_callback_writefunction);
	curl_easy_setopt(curl_handler,CURLOPT_WRITEDATA,(void *)query_results);

	res = curl_easy_perform(curl_handler);

	curl_easy_cleanup(curl_handler);
	curl_global_cleanup();

	*query_results = res_buf;
	shift = 0;
	res_buf = NULL;

	if(res != 0) return ERROR_CURL_PERFORM_FAILED;
		
	//���ĩβ������������ַ�������
	index = str_chr(*query_results,'}');
	if(index != -1)
	{
		(*query_results)[index+1] = '\0';
	}
#ifdef STRING_ENCODE //�ַ���ת��Ϊ�����ַ�����������������������
	conveter = iconv_open("char//IGNORE","UTF-8");
	if((int)conveter == -1) ERROR_UTF8_TO_GB2312_FAILED;
	
	output_len = (strlen(*query_results) + 1);
	input_len = strlen(*query_results) + 1;

	output = (char *)malloc(sizeof(char)*output_len);
	memset(output,0,output_len);

	old_output = output;
	old_input = *query_results;

	rc = iconv(conveter,query_results, &input_len, &output, &output_len);
	*query_results = old_input;

	strcpy(old_input,old_output);
	output=old_output;
	free(output);
	iconv_close(conveter);
#endif
	return SUCCESS;
}

/*
* ����ָ�������������в�ѯ������Խṹ��������ʽ����
* @query����ѯ����
* @fields����ѯ�ֶ�
* @page����ѯ�ڼ�ҳ
* @client_handler:�洢��email��key�Ľṹ��ָ��
* @query_results: ��ѯ����ķ��أ����ڴ棬��Ҫ�ֶ��ͷ�
* return : JSON������Ľṹ�����飬�������������ݣ����ڴ棬��Ҫ�ֶ��ͷš�
*/
ResultPtr fofa_query_as_array(const char *query,const char *fields,const int page,const FofaClientPtr client_handler,char **query_results)
{
	char *str= NULL;
	int i = 0;
	int j = 0;

	cJSON *answer_json = NULL;
	cJSON *answer_exter_array = NULL;
	cJSON *answer_inter_array = NULL;
	cJSON *answer_item = NULL;
	int answer_exter_array_size = 0;
	int answer_inter_array_size = 0;
	int is_array = 0;
	ResultPtr ptr = (ResultPtr)malloc(sizeof(Result));

	
	int res = fofa_query_as_json(query,fields,page,client_handler,query_results);
	if(res != 0) return NULL;
#if 0
	for(i = 0;i < strlen(fields);i++)
	{
		if(fields[i] == ',')
			is_array = 1;
	}
#endif 
	answer_json = cJSON_Parse(*query_results);
	if(!answer_json) return NULL;

	//�����жϣ������ѯ���ִ������˳�
	i = cJSON_GetObjectItem(answer_json,"error")->valueint;
	if(i) 
	{
		cJSON_Delete(answer_json);
		return NULL;
	}

	//������ѯ
	//�洢�߼���fields�ֶ��ж��ֵʱ��JSON��Ϊ��ά����
	//Result����ݵ�һά�Ĵ�С����ResultItemPtrָ��ĸ���
	//Result | ->num:��һά��ָ�����,��������¼,�˺�ResultItem�ĸ���
	//	     | ->ResultItem[0] | -> num:����fields�ֶ�ֵ�ĸ���ȷ����ֵ,˵���˸ýṹ����memberֵ�ĸ���
	//						   | -> member[0]:�洢ÿ����¼�У������ֶζ�Ӧֵ
	//		 | ->ResultItem[1] | -> num
	//						   | -> member[0]
	answer_exter_array = cJSON_GetObjectItem(answer_json,"results");//һά����
	answer_exter_array_size = cJSON_GetArraySize(answer_exter_array);//һά�����С
	ptr->items = (ResultItemPtr)malloc(sizeof(ResultItem)*answer_exter_array_size + sizeof(int));
	
	ptr->num = answer_exter_array_size;
	for(i = 0;i < answer_exter_array_size;i++)
	{
		answer_inter_array = cJSON_GetArrayItem(answer_exter_array,i);//��ά
		answer_inter_array_size = cJSON_GetArraySize(answer_inter_array);//��ά�����С
		ptr->items[i].num = answer_inter_array_size;
		if(answer_inter_array->type == 5)//fields�ֶ�Ϊ���ֵʱ
		{
			answer_inter_array = cJSON_GetArrayItem(answer_exter_array,i);//��ά
			answer_inter_array_size = cJSON_GetArraySize(answer_inter_array);//��ά�����С
			ptr->items[i].num = answer_inter_array_size;
			for(j = 0;j < ptr->items[i].num;j++)
			{
				answer_item = cJSON_GetArrayItem(answer_inter_array,j);//��ά�е�ÿһ����Ա
				if(answer_item == NULL) return NULL;
				str = answer_item->valuestring;
				ptr->items[i].member[j] = (char *)malloc((strlen(str)+1)*sizeof(char));
				strcpy(ptr->items[i].member[j],str);
			}
		}
		else
		{
			answer_item = answer_inter_array;
			ptr->items[i].num = 1;
			if(answer_item->type != 4) continue;
			str = answer_item->valuestring;
			ptr->items[i].member[0] = (char *)malloc((strlen(str)+1)*sizeof(char));
			strcpy(ptr->items[i].member[0],str);
		}	
	}
	cJSON_Delete(answer_json);
	return ptr;
}

/*
* �ͷ�Result�ṹ��
* @rest���Ѿ������ڴ��Result�ṹ��ָ��
*/
int fofa_destroy_result_struct(ResultPtr rest)
{
	int i,j;
	if(rest == NULL) return ERROR_RESULT_IS_NULL;
	for(i = 0;i < rest->num;i++)
	{
		for(j = 0;j < rest->items[i].num;j++)
		{
			free(rest->items[i].member[j]);
		}
	}
	free(rest->items);
	free(rest);
	return SUCCESS;
}