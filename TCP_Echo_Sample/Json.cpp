#include "Json.h"


string Json::WriteJson(char* comment)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("Comment");
	writer.String(comment);
	writer.EndObject();

	return  s.GetString();
}

string Json::ReadComment(const char* json)
{
	Document doc;
	doc.Parse(json);
	if (doc.HasMember("Comment"))
	{
		return doc["Comment"].GetString();
	}
	return string();
}
