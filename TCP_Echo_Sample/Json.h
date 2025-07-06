#pragma once
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <string>

// ���� ������ Use Case

// �۽��ڰ� ���� ����. 
// �� ���� Json ���� �����. 
// Json �� String���� ��ȯ�Ѵ�. 
// TCP ���Ͽ� �־ �����Ѵ�. 
// ������ ������ �޴´�.
// ������ �Ľ��� �Ѵ�.
// �Ľ��� �����͸� ����Ѵ�. 
using namespace rapidjson;
using namespace std;
class Json
{
public: 
	string WriteJson(char * comment);
	string ReadComment(const char* json);
};

