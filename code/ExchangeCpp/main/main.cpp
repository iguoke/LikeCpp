/*************************************************************************
	> File Name: main.cpp
	> Author: hello-futures
	> Mail: 717047832@qq.com 
	> Created Time: 2018年06月26日 星期二 17时07分42秒
 ************************************************************************/

#include "../utils/ex_log.h"
#include "../http/ex_http_wrapper.h"
#include <iostream>
#include <string>

using namespace std;

int main(void){

	string logfile("./bin/log.txt");
	ex_logger::enable_logfile(1);
	ex_logger::set_debug_logfile(logfile);
	ex_logger::write_log("this is main() func!\n");
	string key("");
	string sec_key("");
	string url("http://www.sina.com.cn");	
	HttpWrapper http;
	string result;
	http.curl_api(url,result);
	cout<<result<<endl;	
	return 0;
}
