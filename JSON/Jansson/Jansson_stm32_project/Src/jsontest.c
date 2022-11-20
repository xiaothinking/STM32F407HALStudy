#include "jsontest.h"


//jansson Test
void jansson_pack_test(void)
{
	json_t *root;
	char *out;
	int id=30;
	
	/* Build an empty JSON object */
	root = json_pack("{}");
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/* Build the JSON object {"foo": 42, "bar": 7} */
	root = json_pack("{sisi}", "foo", 42, "bar", 7);

	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/* Like above, ':', ',' and whitespace are ignored */
	root = json_pack("{s:i, s:i}", "foo", 42, "bar", 7);

	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/* Build the JSON array [[1, 2], {"cool": true}] */
	root = json_pack("[[i,i],{s:b}]", 1, 2, "cool", 1);

	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/* Build a string from a non-null terminated buffer */
	char buffer[4] = {'t', 'e', 's', 't'};
	root = json_pack("[s#]", buffer, 4);

	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/* Concatenate strings together to build the JSON string "foobarbaz" */
	root = json_pack("[s++]", "foo", "bar", "baz");
	
	out = json_dumps(root, JSON_ENCODE_ANY);
	printf("out:%s\r\n", out);
	json_decref(root);
	free(out);
	
	/***************************************************************/

	//char buf[10]={128,128,0,24,25};
	//char buf[10]={'t', 'e', 's', 't'};
  root=json_pack("{s:i,s:s,s:f,s:i,s:{s:i,s:s},s:[i,i,i,i]}", "id", 1, "heartbeat", "on","num",128.4,"price",24,"status","zhekou",80,"zhangsan","12587","shangpin",128,128,0,24);
	out = json_dumps(root, JSON_PRESERVE_ORDER);
//s_repon = json_dumps(root, JSON_COMPACT);
	printf("str = %s \n",out);
	free(out);
	json_decref(root);

	
}


uint8_t Jansson1_Demo(void)
{
    const char *text = "{\"status\":true,\"age\":20,\"score\":78.4,\"name\":\"whik1194\",\"blog\":\"https://blog.csdn.net/whik1194\",\"wechat\":\"mcu149\"}";
    
    json_error_t error;
    json_t *root;
    json_t *status;
    
    char *name, *blog, *wechat;
    int age;
    double score;
    
    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        printf("root format correct\r\n");
        
        /* string ????? */
        name = (char *)json_string_value(json_object_get(root, "name"));
        blog = (char *)json_string_value(json_object_get(root, "blog"));
        wechat = (char *)json_string_value(json_object_get(root, "wechat"));
        
        printf("name: %s\r\n", name);
        printf("blog: %s\r\n", blog);
        printf("wechat: %s\r\n", wechat);
        
        /* int ????? */
        age = json_integer_value(json_object_get(root, "age"));
        printf("age: %d\r\n", age);
        
        /* double ????? */
        score = json_real_value(json_object_get(root, "score"));
        printf("score: %.1f\r\n", score);
        
        /* bool ????? */
        status = json_object_get(root, "status");
        if(json_is_true(status))
            printf("status: true\r\n");
        else if(json_is_false(status))
            printf("status: false\r\n");
    }
    else
    {
        printf("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}   

uint8_t Jansson2_Demo(void)
{
    const char *text = "{\"success\":\"1\",\"result\":{\"timestamp\":\"1592640249\",\"datetime_1\":\"2020-06-20 16:04:09\",\"week_1\":\"6\",\"week_2\":\"Saturday\"}}";
    json_error_t error;
    json_t *root;
    json_t *result;
    
    char *success;
    char *timestamp;
    char *datetime_1;
    char *week_1;

    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        printf("root format correct\r\n");
        success = (char *)json_string_value(json_object_get(root, "success"));
        printf("success:%s\r\n", success);
        
        result = json_object_get(root, "result");
        if(json_is_object(result))
        {
            timestamp = (char *)json_string_value(json_object_get(result, "timestamp"));
            datetime_1 = (char *)json_string_value(json_object_get(result, "datetime_1"));
            week_1 = (char *)json_string_value(json_object_get(result, "week_1"));
            
            printf("timestamp:%s\r\n", timestamp);
            printf("datetime_1:%s\r\n", datetime_1);
            printf("week_1:%s\r\n", week_1);
        }
        else 
        {
            printf("reuslt format error\r\n");
            return 1;
        }
    }
    else
    {
        printf("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}

uint8_t Jansson3_Demo(void)
{
    const char *text = "{\"location\":[{\"name\":\"Faye\",\"address\":\"Beijing\"},{\"name\":\"Andy\",\"address\":\"ShangHai\"},{\"name\":\"Lisa\",\"address\":\"ShenZhen\"}],\"time\":\"2018-11-17\"}";
   
    json_error_t error;
    json_t *root;
    json_t *location_arr;
    json_t *location;
    
    char *name, *address;
    char *time;
    int idx;
    
    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        printf("root format correct\r\n");
        time = (char *)json_string_value(json_object_get(root, "time"));
        location_arr = json_object_get(root, "location");
        if(json_is_array(location_arr))
        {
            for(idx = 0; idx < json_array_size(location_arr); idx++)
            {
                location = json_array_get(location_arr, idx);
                name = (char *)json_string_value(json_object_get(location, "name"));
                address = (char *)json_string_value(json_object_get(location, "address"));
                printf("%d: %s-%s\r\n", idx, name, address);
            }
        }
    }
    else
    {
        printf("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}

