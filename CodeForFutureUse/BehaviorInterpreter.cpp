
// table that links a class type to general information about its specialized data
std::unordered_map<std::string, SpecialData> specialDataTable
{
  {"EnemySpotted", EnemySpotted::SpecialData},
};


enum class DATA_TYPE{BOOL, CHAR, SHORT, INT, UNSIGNED_INT, FLOAT, DOUBLE};


static std::map<DATA_TYPE, 
                std::function<void(rapidjson::Document&, 
                                   typeRT&, const std::string&)>> toJSON
{
  
  
  {DATA_TYPE::BOOL, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<float>( render_data.member(name).getBool() );
  }},
  
  {DATA_TYPE::CHAR, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<char>( render_data.member(name).getChar() );
  }},
  
  {DATA_TYPE::SHORT, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<short>( render_data.member(name).getShort() );
  }},
  
  {DATA_TYPE::INT, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<int>( render_data.member(name).getInt() );
  }},
  
  {DATA_TYPE::UNSIGNED_INT, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<unsigned int>( render_data.member(name).getUint() );
  }},
  
  {DATA_TYPE::FLOAT, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<float>( render_data.member(name).getFloat() );
  }},
  
  {DATA_TYPE::DOUBLE, [](rapidjson::Document& j, typeRT& render_data, const std::string& name)
  {
    
    j[name].Set<double>( render_data.member(name).getDouble() );
  }}
};



static std::map<DATA_TYPE, std::function<void(rapidjson::Document&, 
                                              typeRT&, const std::string&)>> addToJson
{
  {DATA_TYPE::BOOL, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
  {
    doc.AddMember(name, render_data.getBool(), doc.GetAllocator());
  }
  },
  
  {DATA_TYPE::CHAR, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
  {
    doc.AddMember(name, render_data.getChar(), doc.GetAllocator());
  }
  },
  
  {DATA_TYPE::SHORT, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
    {
      doc.AddMember(name, render_data.getShort(), doc.GetAllocator());
    }
  },
  
  {DATA_TYPE::INT, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
    {
      doc.AddMember(name, render_data.getInt(), doc.GetAllocator());
    }
  },
  
  {DATA_TYPE::UNSIGNED_INT, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
    {
      doc.AddMember(name, render_data.getUint(), doc.GetAllocator());
    }
  },
  
  {DATA_TYPE::FLOAT, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
    {
      doc.AddMember(name, render_data.getFloat(), doc.GetAllocator());
    }
  },
  
  {DATA_TYPE::DOUBLE, [](rapidjson::Document& doc, typeRT& render_data, const std::string& name)
    {
      doc.AddMember(name, render_data.getDouble(), doc.GetAllocator());
    }
  }
};


static std::map<DATA_TYPE, std::function<void(const rapidjson::Value&, 
                                              typeRT&, const std::string&)>> toTypeRT
{
  {BOOL, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setBool(doc.GetBool());
  }}, 
  
  {CHAR, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setChar(doc.GetChar());
  }},
  
  {SHORT, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setShort(doc.GetShort());
  }}, 
  
  {INT, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setInt(doc.GetInt());
  }},
  
  {UNSIGNED_INT, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setUint(doc.Getuint());
  }},
  
  {FLOAT, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setFloat(doc.GetFloat());
  }},
  
  {DOUBLE, [](const rapidjson::Value& doc, typeRT& render_data, const std::string& name)
  {
    render_data.setDouble(doc.GetDouble());
  }}
};


void FillJSON(typeRT& render_data, std::string behaviorName, 
              rapidjson::Document& doc)
{
  assert(data.getVariableName() == "Node Render Data");
  
  std::vector<SpecialData> dataList = GetList of special data based on behaviorName;
  
  // for each special member that the behavior has
  for (auto entry : dataList)
  {
    
      // if json has member
      if(doc.HasMember(entry.name))
      {
        // insert value of entry into the json
        toJSON[entry.type](doc, render_data.member(entry.name), entry.name);
      }
      else
      {
        addToJson[entry.type](doc, render_data.member(entry.name), entry.name);
      }
  }
}

void FillTypeRT(typeRT& data, std::string behaviorName, const rapidjson::Value &JSON)
{
  assert(data.getVariableName() == "Node Render Data");
  
  std::vector<SpecialData> dataList = GetList of special data based on behaviorName;
  
  for(auto entry : dataList)
  {
    if(JSON.HasMember(entry.name))
    {
      toTypeRT[entry.type](JSON[entry.name], data.member(entry.name), entry.name)
    }
    else
    {
      throw std::runtime_error("Missing" + entry.name);
    }
  }
  
}

specialData GetDefaultTypeRT(std::string behaviorName);