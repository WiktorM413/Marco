#include <string>
namespace Marco
{
	class JsonReader
	{
	public:
		JsonReader();
		JsonReader(std::string jsonString);
		JsonReader(std::fstream jsonFile);
	};
}