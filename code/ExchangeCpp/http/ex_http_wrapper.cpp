/*************************************************************************
	> File Name: ex_http_wrapper.cpp
	> Author: hello-futures
	> Mail: 717047832@qq.com 
	> Created Time: 2018年06月27日 星期三 13时59分46秒
 ************************************************************************/

#include "ex_http_wrapper.h"
#include "../utils/ex_log.h"


HttpWrapper::HttpWrapper(){

	curl_global_init(CURL_GLOBAL_DEFAULT);
	this->curl=curl_easy_init();
}

HttpWrapper::~HttpWrapper(){

	curl_easy_cleanup(this->curl);
	curl_global_cleanup();
}

void HttpWrapper::curl_api(string& url,string& result_json){

	vector<string> v;
	string action="GTE";
	string post_data="";
	curl_api_with_header(url,result_json,v,post_data,action);
}

void HttpWrapper::curl_api_with_header(string& url,string& result_json,vector<string>& header,string& post_data,string& action){

	ex_logger::write_log("<HttpWrapper::curl_api>");
	CURLcode res;
	if(this->curl){

		curl_easy_setopt(this->curl,CURLOPT_URL,url.c_str());
		curl_easy_setopt(this->curl,CURLOPT_WRITEFUNCTION,&HttpWrapper::curl_cb_write);
		curl_easy_setopt(this->curl,CURLOPT_WRITEDATA,&result_json);
		curl_easy_setopt(this->curl,CURLOPT_SSL_VERIFYPEER,false);
		curl_easy_setopt(this->curl,CURLOPT_ENCODING,"gzip");

		if(header.size()> 0){

			struct curl_slist* chunk=NULL;
			for(int i= 0;i < header.size(); ++i){

				chunk = curl_slist_append(chunk,header[i].c_str());
			}
			curl_easy_setopt(this->curl,CURLOPT_HTTPHEADER,chunk);
		}
		if(post_data.size() > 0 || action == "POST" || action =="PUT" || action =="DELETE"){

			if(action =="PUT" ||action =="DELETE"){
				curl_easy_setopt(this->curl,CURLOPT_CUSTOMREQUEST,action.c_str());
			}
			curl_easy_setopt(this->curl,CURLOPT_POSTFIELDS,post_data.c_str());
		}

		res=curl_easy_perform(this->curl);
		if(res != CURLE_OK){
			ex_logger::write_log("<HttpWrapper::curl_api> curl_esay_perform() failed:%s",curl_easy_strerror(res));
		}	
	}
	ex_logger::write_log("<HttpWrapper::curl_api> done");
}

size_t HttpWrapper::curl_cb_write(void *content,size_t size,size_t nmemb,std::string* buffer){

	ex_logger::write_log("<HttpWrapper::curl_cb_write>");
	buffer->append((char*)content,size*nmemb);
	//cout<<(char*)content<<endl;	
	ex_logger::write_log("<HttpWrapper::curl_cb_write> done");
	return size*nmemb;
}





