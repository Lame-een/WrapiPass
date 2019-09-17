#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <utility>
#include <cctype>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

class VariPass
{
public:
	enum VPResponse : int
	{
		SUCCESS = 0,
		ERROR_VP_NO_REQUEST,
		ERROR_VP_MISSING_KEY,
		ERROR_VP_INVALID_KEY,
		ERROR_VP_INVALID_ACTION,
		ERROR_VP_INVALID_ID,
		ERROR_VP_INVALID_PARAMETERS,
		ERROR_VP_INVALID_TYPE,
		ERROR_VP_NOT_BOOL,
		ERROR_VP_ORDER_FIRST,
		ERROR_VP_ORDER_LAST,
		ERROR_VP_LOCKED,
		ERROR_VP_COOLDOWN,
		ERROR_VP_MAX_VARIABLES,
		ERROR_VP_USER_NOT_FOUND,
		ERROR_VP_TRANSFER_DISABLED,
		ERROR_VP_UNCONFIRMED,
		ERROR_VP_BANNED,
		ERROR_VP_EMPTY_VARIABLE,
		ERROR_VP_DB,

		//wrapper responses
		ERROR_WRAP_RAWREAD,
		WARNING_WRAP_WRONG_TYPE
	};

	std::map<std::string, VariPass::VPResponse> responseMap = {
		{"\"success\"", SUCCESS},
		{"\"error_no_request\"", ERROR_VP_NO_REQUEST},
		{"\"error_missing_key\"", ERROR_VP_MISSING_KEY},
		{"\"error_invalid_key\"", ERROR_VP_INVALID_KEY},
		{"\"error_invalid_action", ERROR_VP_INVALID_ACTION},
		{"\"error_invalid_id\"", ERROR_VP_INVALID_ID},
		{"\"error_invalid_parameters", ERROR_VP_INVALID_PARAMETERS},
		{"\"error_invalid_type\"", ERROR_VP_INVALID_TYPE},
		{"\"error_not_bool\"", ERROR_VP_NOT_BOOL},
		{"\"error_order_first\"", ERROR_VP_ORDER_FIRST},
		{"\"error_order_last\"", ERROR_VP_ORDER_LAST},
		{"\"error_locked\"", ERROR_VP_LOCKED},
		{"\"error_cooldown\"", ERROR_VP_COOLDOWN},
		{"\"error_max_variables", ERROR_VP_MAX_VARIABLES},
		{"\"error_user_not_found", ERROR_VP_USER_NOT_FOUND},
		{"\"error_transfer_disabled", ERROR_VP_TRANSFER_DISABLED},
		{"\"error_unconfirmed\"", ERROR_VP_UNCONFIRMED},
		{"\"error_banned\"", ERROR_VP_BANNED},
		{"\"error_empty_variable", ERROR_VP_EMPTY_VARIABLE},
		{"\"error_db\"", ERROR_VP_DB}
	};

	std::map<VariPass::VPResponse, std::string> errorList = {
		{ERROR_VP_NO_REQUEST, "error_no_request: no API action specified."},
		{ERROR_VP_MISSING_KEY, "error_missing_key: API key not provided."},
		{ERROR_VP_INVALID_KEY, "error_invalid_key: provided API key doesn't exist."}, //Similar to the above error, but it is fired if the specified API key doesn't exit. NOTE: This error may also pop up if your IP address has been banned from the API. If this happens, please contact support@varipass.org
		{ERROR_VP_INVALID_ACTION, "error_invalid_action: API action doesn't exist."},
		{ERROR_VP_INVALID_ID, "error_invalid_id: variable ID doesn't exist."},
		{ERROR_VP_INVALID_PARAMETERS, "error_invalid_parameters: parameters missing from API request."},
		{ERROR_VP_INVALID_TYPE, "error_invalid_type: specified variable not supported."},
		{ERROR_VP_NOT_BOOL, "error_not_bool: 'toggle' API has been provided with non bool variable."},
		{ERROR_VP_ORDER_FIRST, "error_order_first: variable is already first."},
		{ERROR_VP_ORDER_LAST, "error_oder_last: variable si already last."},
		{ERROR_VP_LOCKED, "error_locked: variable is locked - unable to edit/delete"},
		{ERROR_VP_COOLDOWN, "error_cooldown: too quick API requests."},
		{ERROR_VP_MAX_VARIABLES, "error_max_variables: max variable amount, unable to create more."},
		{ERROR_VP_USER_NOT_FOUND, "error_user_not_found: target transfer user not found."},
		{ERROR_VP_TRANSFER_DISABLED, "error_transfer_disabled: target user has variable transfer disabled."},
		{ERROR_VP_UNCONFIRMED, "error_unconfirmed: user account not activated."},
		{ERROR_VP_BANNED, "error_banned: account banned."},
		{ERROR_VP_EMPTY_VARIABLE, "error_empty_variable: database error - should never happen contact 'support@varipass.org' immediately."},
		{ERROR_VP_DB, "error_db: database error - should never happen contact 'support@varipass.org' immediately."},
		{ERROR_WRAP_RAWREAD, "error_wrapper_rawread: error in reading the variable - unable to read from JSON."},
		{WARNING_WRAP_WRONG_TYPE, "warning_wrapper_wrong_type: expected: "}
	};

private:
	char *url = "http://api.varipass.org";
	curlpp::Easy request;
	std::list<std::string> header;
	std::stringstream ssResult;


	//account key
	std::string key;

	//curl request sender
	void sendRequest(std::string& str);

	//sends info request to the server and returns a JSON string
	std::string sendPOST(std::string& id, std::string& apiCall);

	//function returning the value from a key:value pair located in the json string
	std::string getValFromJSON(std::string& json, std::string key);


	bool stringToBool(std::string& a);
	std::string boolToString(bool& a);
	//returns string fully interpreted and parsed from the raw json value
	std::string VPStringToString(std::string& a);

	std::string ValTypeString[4] = {"int", "float", "bool", "string"};
	enum ValType : int
	{
		INT = 0, FLOAT, BOOL, STRING
	};

	ValType checkValueType(std::string& val);

public:
	VariPass(std::string& key)
		: key(key)
	{
		header.push_back("Content-type: application/json");

		request.setOpt(new curlpp::options::Url(url));
		request.setOpt(new curlpp::options::HttpHeader(header));
		request.setOpt(cURLpp::Options::WriteStream(&ssResult));
	}

	template<typename T>
	struct VPVar
	{
		bool valid = false;
		T value;
		unsigned long long int time;
		unsigned long long int current;
	};

	bool write(std::string& id, std::string& val);
	bool write(std::string& id, int& val);
	bool write(std::string& id, float& val);
	bool write(std::string& id, bool& val);

	//sends 'read' action request and returns the json if successful; "" if unsuccessful
	std::string readRaw(std::string& id);

	//takes in the json returned from readRaw(std::string& id) and returns the variable
	VPVar<int> readRawInt(std::string& json);
	VPVar<float> readRawFloat(std::string& json);
	VPVar<bool> readRawBool(std::string& json);
	VPVar<std::string> readRawString(std::string& json);

	//takes in a variable id and returns the variable
	VPVar<int> readInt(std::string& id);
	VPVar<float> readFloat(std::string& id);
	VPVar<bool> readBool(std::string& id);
	VPVar<std::string> readString(std::string& id);

};