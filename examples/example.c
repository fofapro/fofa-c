#include "include/fofa.h"

#pragma comment(lib,"fofa.lib")
/*
* 示例程序，标准流程下的函数调用
*/ 
int main(int argc,char *argv[])
{

	FofaClientPtr test_client_ptr = NULL;
	char *email = "YourEmail@email.com";
	char *key = "abcdefghigklmnopqrstuvwxyz123456";
	char *query_result = NULL;
	int i,j;
	ResultPtr result;
	fofa_new_client(email, key, &test_client_ptr);

	fofa_query_as_json("domain:\"baidu.com\"", "ip,domain,host,title", 1, test_client_ptr, &query_result);
	printf("%s\n",query_result);

	free(query_result);
	query_result = NULL;
#if 1
	result = fofa_query_as_array("domain=\"163.com\"", "ip,host,title", 1, test_client_ptr, &query_result);
	
	free(query_result);
	query_result = NULL;
	//打印输入result内存储的数据
	if(result != NULL)
	{
		for(i = 0;i < result->num;i++)
		{
			printf("No:%d:[",i);
			for(j = 0;j < result->items[i].num;j++)
			{
				printf("%s,",result->items[i].member[j]);
			}
			printf("]\n");
		}
		fofa_destroy_result_struct(result);
		free(test_client_ptr);
	}
#endif
	system("pause");
	return 0;
}