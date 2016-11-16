# FOFA Pro SDK 使用说明文档
## FOFA Pro API   
<a href="https://fofa.so/api"><font face="menlo">`FOFA Pro API`</font></a> 是资产搜索引擎 <a href="https://fofa.so/">`FOFA Pro`</a> 为开发者提供的 `RESTful API` 接口, 允许开发者在自己的项目中集成 `FOFA Pro` 的功能。    


## FOFA SDK
基于 `FOFA Pro API` 编写的 `C\C++` 版 `SDK`, 方便 `C\C++` 开发者快速将 `FOFA Pro` 集成到自己的项目中。
## 环境
### 开发环境
``` zsh
OS：Windows 10
IDE: Visual Studio 2012
CMake Version：3.6.3
开发语言：C
```
### 测试环境
``` zsh
操作系统：Windows 10
IDE: Visual Studio 2012、Visual Studio 2013
CMake Version：3.6.3
测试语言：C\C++
```
### 使用环境
```zsh
OS: Windows
CMake Version：3.6及以上
```

## 获取
### FOFA SDK
<strong>下载 `FOFA SDK` </strong>   
``` zsh
git clone https://github.com/fofapro/fofa-c.git
``` 
或访问<a href="https://github.com/fofapro/fofa-c">`https://github.com/fofapro/fofa-c`</a>进行下载

<strong>构建</strong>  	
Windows下使用`命令提示符`进入FOFA Pro SDK源文件根目录，执行`cmake .`,生成工程文件

<strong>编译</strong>      
打开生成文件`FOFA.sln`，执行编译，生成`fofa.lib`。
`fofa.lib`默认生成路径为`./api/build`

<strong>使用</strong>   
将编译生成的`fofa.lib`放入创建的工程文件夹，将FOFA源码文件夹中的`include`文件夹和`lib`文件夹拷贝至创建的工程文件夹，
设置相关文件夹路径，即可使用SDK。

## 依赖
### Email & API Key   
| `Email` |用户登陆 `FOFA Pro` 使用的 `Email`|
|---------|:-----------------:|
|`Key`| 前往 <a href="https://fofa.so/my/users/info" style="color:#0000ff"><strong>`个人中心`</strong></a> 查看 `API Key` |

## Example   
``` C
#include "include/fofa.h"

#pragma comment(lib,"fofa.lib")

int main(int argc,char *argv[])
{

	FofaClientPtr test_client_ptr = NULL;
	char *email = "FOFA_EAMIL@email.com";
	char *key = "FOFA_KEY";
	char *query_result = NULL;
	int i,j;
	ResultPtr result;
	fofa_new_client(email, key, &test_client_ptr);
	//以原始JSON格式字符串进行查询
	fofa_query_as_json("domain:\"fofa.so\"", "ip,domain,host,title", 1, test_client_ptr, &query_result);
	printf("%s\n",query_result);

	free(query_result);
	query_result = NULL;
	
    //获取JSON内数据的方式进行查询
	result = fofa_query_as_array("domain=\"fofa.so\"", "ip,host,title", 1, test_client_ptr, &query_result);
	
	free(query_result);
	query_result = NULL;

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

	system("pause");
	return 0;
}
```

## FOFA Tool   
使用`FOFA SDK` 开发的控制台程序,无需登陆即可进行`FOFA`查询.    


## 协议
`FOFA SDK` 遵循 `MIT` 协议 <a href="https://opensource.org/licenses/mit"><font face="menlo">https://opensource.org/licenses/mit</font></a>
```
