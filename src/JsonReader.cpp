#include "marco/JsonReader.h"
#include "marco/JsonError.h"

Marco::JsonReader::JsonReader(): m_valid(false) { }

// TODO: Finish implementing

Marco::JsonReader::JsonReader(const std::string& jsonString)
{
	if (jsonString.empty() || jsonString.front() != '{' || jsonString.back() != '}')
	{
		this->m_valid = false;
		this->m_error = JsonError::InvalidFormat;
		return;
	}
	
	m_valid = true;

	m_json = jsonString;
}

Marco::JsonReader::JsonReader(const std::fstream& jsonFile)
{
	
}