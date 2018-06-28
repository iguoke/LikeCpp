/*************************************************************************
	> File Name: ex_http_wrapper.h
	> Author: hello-futures
	> Mail: 717047832@qq.com 
	> Created Time: 2018年06月27日 星期三 13时31分51秒
 ************************************************************************/

#ifndef EX_HTTP_WRAPPER_H
#define EX_HTTP_WRAPPER_H

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#include<iostream>
#include<sstream>
#include<string>
#include<map>
#include<vector>
#include<exception>

#include <curl/curl.h>
#include <json/json.h>

using namespace std;

class HttpWrapper{

	private:
		CURL* curl;

	public:
		void curl_api(string& url,string& result_json);
		void curl_api_with_header(string& url,string& result_json,vector<string>& extra_http_header,string& post_data,string& action);
		static size_t curl_cb_write(void* url,size_t size,size_t nmemb,string* buffer);
		HttpWrapper();
		~HttpWrapper();


};
#endif
