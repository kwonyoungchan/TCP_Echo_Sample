#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <string>

// 내가 생각한 Use Case

// 송신자가 글을 쓴다. 
// 쓴 글을 Json 으로 만든다. 
// Json 을 String으로 변환한다. 
// TCP 소켓에 넣어서 전송한다. 
// 서버가 소켓을 받는다.
// 서버가 파싱을 한다.
// 파싱한 데이터를 출력한다. 
using namespace rapidjson;
using namespace std;
class Json
{
public: 
	string WriteJson(char * comment);
	string ReadComment(const char* json);
};

