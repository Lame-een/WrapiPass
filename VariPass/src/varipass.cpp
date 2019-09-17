#include "varipass.h"

void VariPass::sendRequest(std::string& str)
{
	request.setOpt(new curlpp::options::PostFields(str));
	request.setOpt(new curlpp::options::PostFieldSize(str.length()));
	request.perform();
}

std::string VariPass::getValFromJSON(std::string& json, std::string key)
{
	int index = json.find(key);
	std::string out("");


	index += key.size() + 2;

	for(; json[index] != '}' && json[index] != ','; index++)
	{
		out.push_back(json[index]);
	}

	return out;
}

std::string VariPass::sendPOST(std::string& id, std::string& apiCall)
{

	//{"key":"key","id":"id"
	std::string POSTData = "{\"key\":\"" + key + "\"" + ",\"id\":\"" + id + "\"," + apiCall;
	POSTData.push_back('}');

	sendRequest(POSTData);

	std::string rawJSON = ssResult.str();
	ssResult.str("");


	std::string result = getValFromJSON(rawJSON, "result");

	if(responseMap[result] == VPResponse::SUCCESS)
	{
		return rawJSON;
	}
	else
	{
		std::cout << errorList[responseMap[result]] << std::endl;
		return "";
	}
}

std::string VariPass::boolToString(bool& a)
{
	return (a) ? std::string("true") : std::string("false");
}

bool VariPass::stringToBool(std::string& a)
{
	return (a == "true") ? 1 : 0;
}

std::string VariPass::VPStringToString(std::string& a)
{
	if(a == "null")
	{
		return std::string("");
	}
	else
	{
		std::string out = a;
		if(out.size() > 1)
		{
			out.erase(out.begin());
			out.erase(out.begin() + out.size() - 1);
		}
		return out;
	}
}

bool VariPass::write(std::string& id, std::string& val)
{
	std::string aux = "\"action\":\"write\",\"value\":\"";
	aux += val;
	aux.push_back('\"');

	if(sendPOST(id, aux) != "")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool VariPass::write(std::string& id, int& val)
{
	std::string aux = "\"action\":\"write\",\"value\":";
	aux += std::to_string(val);

	if(sendPOST(id, aux) != "")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool VariPass::write(std::string& id, float& val)
{
	std::string aux = "\"action\":\"write\",\"value\":";
	aux += std::to_string(val);

	if(sendPOST(id, aux) != "")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool VariPass::write(std::string& id, bool& val)
{
	std::string aux = "\"action\":\"write\",\"value\":";
	aux += boolToString(val);

	if(sendPOST(id, aux) != "")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

VariPass::ValType VariPass::checkValueType(std::string& val)
{
	if(val[0] == '"')
	{
		return ValType::STRING;
	}

	if(val == "null")
	{
		return ValType::STRING;
	}

	if(val == "true" || val == "false")
	{
		return ValType::BOOL;
	}

	bool isFloat = false;
	for(int i = 0; i < (int)val.size(); i++)
	{
		if(val[i] == '.')
		{
			isFloat = true;
			break;
		}
	}

	if(isFloat)
	{
		return ValType::FLOAT;
	}
	else
	{
		return ValType::INT;
	}
}

std::string VariPass::readRaw(std::string& id)
{
	std::string aux = "\"action\":\"read\"";
	std::string json = sendPOST(id, aux);

	if(json == "")
	{
		std::cout << errorList[VPResponse::ERROR_WRAP_RAWREAD] << std::endl;
	}

	return json;
}

VariPass::VPVar<int> VariPass::readRawInt(std::string& json)
{
	VPVar<int> out;

	std::string value = getValFromJSON(json, "value");
	ValType valType = checkValueType(value);
	if(valType == ValType::INT)
	{
		out.valid = true;
		out.value = std::stoi(value);
		out.time = std::stoull(getValFromJSON(json, "time"));
		out.current = std::stoull(getValFromJSON(json, "current"));
	}
	else
	{
		std::cout << errorList[VPResponse::WARNING_WRAP_WRONG_TYPE] << "int; recieved: " << ValTypeString[(int)valType] << "." << std::endl;
	}
	return out;
}

VariPass::VPVar<float> VariPass::readRawFloat(std::string& json)
{
	VPVar<float> out;

	std::string value = getValFromJSON(json, "value");
	ValType valType = checkValueType(value);
	if(valType == ValType::FLOAT)
	{
		out.valid = true;
		out.value = std::stof(value);
		out.time = std::stoull(getValFromJSON(json, "time"));
		out.current = std::stoull(getValFromJSON(json, "current"));
	}
	else
	{
		std::cout << errorList[VPResponse::WARNING_WRAP_WRONG_TYPE] << "float; recieved: " << ValTypeString[(int)valType] << "." << std::endl;
	}
	return out;
}

VariPass::VPVar<bool> VariPass::readRawBool(std::string& json)
{
	VPVar<bool> out;

	std::string value = getValFromJSON(json, "value");
	ValType valType = checkValueType(value);
	if(valType == ValType::BOOL)
	{
		out.valid = true;
		out.value = stringToBool(value);
		out.time = std::stoull(getValFromJSON(json, "time"));
		out.current = std::stoull(getValFromJSON(json, "current"));
	}
	else
	{
		std::cout << errorList[VPResponse::WARNING_WRAP_WRONG_TYPE] << "bool; recieved: " << ValTypeString[(int)valType] << "." << std::endl;
	}
	return out;
}

VariPass::VPVar<std::string> VariPass::readRawString(std::string& json)
{
	VPVar<std::string> out;

	std::string value = getValFromJSON(json, "value");
	ValType valType = checkValueType(value);
	if(valType == ValType::STRING)
	{
		out.valid = true;
		out.value = VPStringToString(value);
		out.time = std::stoull(getValFromJSON(json, "time"));
		out.current = std::stoull(getValFromJSON(json, "current"));
	}
	else
	{
		std::cout << errorList[VPResponse::WARNING_WRAP_WRONG_TYPE] << "string; recieved: " << ValTypeString[(int)valType] << "." << std::endl;
	}
	return out;
}

VariPass::VPVar<int> VariPass::readInt(std::string& id)
{
	std::string json = readRaw(id);
	return readRawInt(json);
}

VariPass::VPVar<float> VariPass::readFloat(std::string& id)
{
	std::string json = readRaw(id);
	return readRawFloat(json);
}

VariPass::VPVar<bool> VariPass::readBool(std::string& id)
{
	std::string json = readRaw(id);
	return readRawBool(json);
}

VariPass::VPVar<std::string> VariPass::readString(std::string& id)
{
	std::string json = readRaw(id);
	return readRawString(json);
}