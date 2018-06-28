/*************************************************************************
	> File Name: async.c
	> Author: hello-futures
	> Mail: 717047832@qq.com 
	> Created Time: 2018年06月26日 星期二 18时17分07秒
 ************************************************************************/
#include<string>
#include<iostream>

#include<curl/curl.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
size_t curl_write(void *buffer,size_t size,size_t count,void *stream){
	std::string *pstream=static_cast<std::string*>(stream);
	(*pstream).append((char*)buffer,size*count);
	return size*count;
}

CURL *curl_easy_handler(const std::string& surl,const std::string& sproxy,std::string& srsp,unsigned int uiTimeout){
	CURL *curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,surl.c_str());
	curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);

	if(0 < uiTimeout){
		curl_easy_setopt(curl,CURLOPT_TIMEOUT_MS,uiTimeout);
	}
	if(!sproxy.empty()){
		curl_easy_setopt(curl,CURLOPT_PROXY,sproxy.c_str());
	}
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_write);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&srsp);
	return curl;
}

int curl_multi_select(CURLM* curl_m){
	
	int ret=0;
	struct timeval timeout_tv;
	fd_set fd_read;
	fd_set fd_write;
	fd_set fd_except;
	int max_fd = -1;

	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_except);

	timeout_tv.tv_sec = 1;
	timeout_tv.tv_usec = 0;

	curl_multi_fdset(curl_m,&fd_read,&fd_write,&fd_except,&max_fd);

	if(-1 ==max_fd){
		return -1;
	}
	
	int ret_code=::select(max_fd+1,&fd_read,&fd_write,&fd_except,&timeout_tv);
	switch(ret_code){

		case -1:
			ret =-1;
			break;
		case 0:
		default:
			ret=0;
			break;
	}

	return ret;
}

#define MULTI_CURL_NUM 3
std::string URL = "http://website.com";
std::string PROXY = "ip:port";

unsigned int TIMEOUT =2000;

int curl_multi_demo(int num){

	CURLM *curl_m=curl_multi_init();
	std::string RspArray[num];
	CURL * CurlArray[num];

	for(int idx =0;idx < num; ++idx){

		CurlArray[idx] = NULL;
		CurlArray[idx] = curl_easy_handler(URL,PROXY,RspArray[idx],TIMEOUT);
		if(NULL == CurlArray[idx]){
			
			return -1;
		}
		curl_multi_add_handle(curl_m,CurlArray[idx]);
	}

	int running_handles;
	while(CURLM_CALL_MULTI_PERFORM ==curl_multi_perform(curl_m,&running_handles)){
	
		cout<<running_handles<<endl;
	}

	while(running_handles){

		if(-1 ==curl_multi_select(curl_m)){

			cerr<<"select error"<<endl;
			//break;
		}else {
			while(CURLM_CALL_MULTI_PERFORM ==curl_multi_perform(curl_m,&running_handles)){

				cout<<"select:"<<running_handles<<endl;
			}
		}
		cout<<"select:"<<running_handles<<endl;
	}

	int msgs_left;
	CURLMsg *msg;
	while((msg = curl_multi_info_read(curl_m,&msgs_left))){

		if(CURLMSG_DONE ==msg->msg){

			int idx;
			for(idx = 0; idx<num; ++idx){

				if(msg->easy_handle ==CurlArray[idx]) break;
			}

			if(idx == num){

				cerr<<"crul not found"<<endl;
			}else {

				cout<<"curl []"<<idx<<"] completed with status:"<<msg->data.result<<endl;
				cout<<"rsp:"<<RspArray[idx]<<endl;
			}
		}
	}
	
	for(int idx = 0; idx < num; ++idx){

		curl_multi_remove_handle(curl_m,CurlArray[idx]);
	}

	for(int idx = 0; idx < num; ++idx){

		curl_easy_cleanup(CurlArray[idx]);
	}

	curl_multi_cleanup(curl_m);
	return 0;
}

int curl_easy_demo(int num){

	std::string RspArray[num];
	for(int idx = 0;idx < num;++idx){

		CURL *curl = curl_easy_handler(URL,PROXY,RspArray[idx],TIMEOUT);
		CURLcode code=curl_easy_perform(curl);
		cout<<"curl ["<<idx<<"] completed with status:"<<code<<endl;
		cout<<"rsp:"<<RspArray[idx]<<endl;
		curl_easy_cleanup(curl);
	}
	return 0;
}

#define USE_MULTI_CURL
struct timeval begin_tv,end_tv;

int main(int argc,char *argv[]){

	if(argc < 2){
		return -1;
	}
	int num=atoi(argv[1]);

	gettimeofday(&begin_tv,NULL);
#ifdef USE_MULTI_CURL
	curl_multi_demo(num);
#else
	curl_easy_demo(num);
#endif
	
	struct timeval end_tv;
	gettimeofday(&end_tv,NULL);

	int eclapsed=(end_tv.tv_sec-begin_tv.tv_sec)*1000+(end_tv.tv_usec-begin_tv.tv_usec)/1000;
	cout<<"eclapsed time:"<<eclapsed<<"ms"<<endl;
	return 0;
}






















































		































