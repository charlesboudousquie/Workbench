#include <any>

enum class DATA_TYPE{BOOL, CHAR, SHORT, INT, UNSIGNED_INT, FLOAT, DOUBLE};

struct SpecialData
{
  DATA_TYPE type;
  std::string name;
  std::any data;
};

class BehaviorInterpreter
{
  
  public:
  
  static void FillJSON(typeRT& data, std::string behaviorName, JSON&);
  
  static void FillTypeRT(typeRT& data, std::string behaviorName, const rapidjson::Value &JSON);
  
  static specialData GetDefaultTypeRT(std::string behaviorName);
  
  
};



