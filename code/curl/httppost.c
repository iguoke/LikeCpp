/*************************************************************************
	> File Name: httppost.c
	> Author: hello-futures
	> Mail: 717047832@qq.com 
	> Created Time: 2018年06月26日 星期二 13时26分39秒
 ************************************************************************/

#include<stdio.h>
#include<curl/curl.h>
int main(void){
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl=curl_easy_init();
	if(curl){
		curl_easy_setopt(curl,CURLOPT_URL,"http://postit.example.com/moo.cgi");
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"name=daniel&project=curl");
		res=curl_easy_perform(curl);
		if(res!=CURLE_OK){
			fprintf(stderr,"curl_easy_perfrom() failed:%s\n",curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;
}
