#include <gtest/gtest.h>
#include "include/fofa.h"

#if 1
FofaClientPtr test_client_ptr = NULL;
char *email = "test@eamil.com";
char *key = "abcdefghijklmnopqrstuvwxyz123456";

TEST(FofaNewClinetFuncTEST, HandleInit)
{
	char unvalid_email[322] = {0};
	int i;
	for (i = 0;i < 322;i++)
		unvalid_email[i] = 'a';
	char *null_ptr = NULL;
	char *short_key = "abcdefghijklmnopqrstuvwxyz12345";
	char *long_key = "abcdefghijklmnopqrstuvwxyz1234567";
	EXPECT_EQ(SUCCESS, fofa_new_client(email, key, &test_client_ptr));
	free(test_client_ptr);
//Test Params FofaClientPtr
	EXPECT_EQ(ERROR_HANDLER_NOT_NULL, fofa_new_client(email, key, &test_client_ptr));
	test_client_ptr = NULL;
	EXPECT_EQ(SUCCESS, fofa_new_client(email, key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
//Test Params Email
	EXPECT_EQ(ERROR_INVALID_EMAIL_VALUE, fofa_new_client(null_ptr, key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
	EXPECT_EQ(ERROR_INVALID_EMAIL_VALUE, fofa_new_client(unvalid_email, key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
//Test Params KEY
	EXPECT_EQ(SUCCESS, fofa_new_client(email, key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
	EXPECT_EQ(ERROR_INVALID_KEY_VALUE, fofa_new_client(email, null_ptr, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
	EXPECT_EQ(ERROR_INVALID_KEY_VALUE, fofa_new_client(email, short_key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
	EXPECT_EQ(ERROR_INVALID_KEY_VALUE, fofa_new_client(email, long_key, &test_client_ptr));
	free(test_client_ptr);
	test_client_ptr = NULL;
}

TEST(FofaQueryAsJSONFuncTest, QueryAsJSON)
{
	char *str;
	char str_long[4000];
	int i;
	for(i = 0;i < 4000;i++)
	{
		str_long[i] = 'a';
	}
//Test Params FofaClientPtr
	EXPECT_EQ(ERROR_HANDLER_IS_NULL, fofa_query_as_json("domain=\"fofa.so\"","host",1,test_client_ptr, &str));
	ASSERT_EQ(SUCCESS, fofa_new_client(email, key, &test_client_ptr));
	EXPECT_EQ(SUCCESS, fofa_query_as_json("domain=\"fofa.so\"", "host", 1, test_client_ptr, &str));
//Test Params Query
	EXPECT_EQ(ERROR_QUERY_IS_NULL, fofa_query_as_json(NULL, "host", 1, test_client_ptr ,&str));
//Test Params Fields
	EXPECT_EQ(ERROR_FIELDS_IS_NULL, fofa_query_as_json("domain=\"fofa.so\"", NULL, 1, test_client_ptr, &str));
//Test Params Page
	EXPECT_EQ(ERROR_INVALID_PAGE_VALUE, fofa_query_as_json("domain=\"fofa.so\"", "host", -1, test_client_ptr, &str));
	EXPECT_EQ(SUCCESS, fofa_query_as_json("domain=\"fofa.so\"", "host", 1.1, test_client_ptr, &str));
//Test Params Query_results
	EXPECT_EQ(ERROR_QUERY_STRING_LONG, fofa_query_as_json(str_long,"host",1,test_client_ptr,&str));
	free(test_client_ptr);
	test_client_ptr = NULL;
}

TEST(FofaQueryAsArrayFuncTest, QueryAsArray)
{
	char *str;
	char str_long[4000];
	int i;
	for(i = 0;i < 4000;i++)
	{
		str_long[i] = 'a';
	}
	EXPECT_FALSE(fofa_query_as_array("domain=\"fofa.so\"","host",1,test_client_ptr, &str));
	ASSERT_EQ(SUCCESS, fofa_new_client(email, key, &test_client_ptr));
	EXPECT_TRUE(fofa_query_as_array("domain=\"fofa.so\"","host",1,test_client_ptr, &str));
	EXPECT_FALSE(fofa_query_as_array(NULL, "host", 1, test_client_ptr ,&str));
	EXPECT_FALSE(fofa_query_as_array("domain=\"fofa.so\"", NULL, 1, test_client_ptr, &str));
	EXPECT_FALSE(fofa_query_as_array("domain=\"fofa.so\"", "host", -1, test_client_ptr, &str));
	EXPECT_TRUE(fofa_query_as_array("domain=\"fofa.so\"", "host", 1.1, test_client_ptr, &str));
	EXPECT_FALSE(fofa_query_as_array(str_long,"host",1,test_client_ptr,&str));
	free(test_client_ptr);
	test_client_ptr = NULL;
}
#endif
#if 1
int main(int argc,char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}
#endif