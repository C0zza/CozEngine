#include "SerializableTest.h"

void to_json(nlohmann::json& J, const LSerializableTest& ST)
{
	J = nlohmann::json{ {"SomeInt", ST.SomeInt} };
}

void from_json(const nlohmann::json& J, LSerializableTest& ST)
{
	J.at("SomeInt").get_to(ST.SomeInt);
}

void LSerializableTest::Test()
{
	LoadAssetFromDisk("Test.json", *this);
	SomeInt++;
	SaveAssetToDisk("Test.json", *this);
}
