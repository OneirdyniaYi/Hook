#ifndef __Hook__Config__H__
#define __Hook__Config__H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>


namespace Hook{

class ConfigVarBase {
public:
	typedef std::shared_ptr<ConfigVirBase> ptr;
	ConfigVarBase(const std::string& name,const std::string& description = "")
		:m_name(name),
		,m_description(description){

	}
	virtual ~ConfigVarBase() {}
	const std::string& getName() {return m_name;}
	const std::string& getDescription() {return m_description;}

	virtual std::string toString() = 0;
	virtual bool parserString(const std::string& val) = 0;
protected:
	std::string m_name;
	std::string m_description;
};

template<class T,class F> class LexicalCast{
public:
	T operator(const F& f){
		return boost::lexical_cast<T>(f);
	}
};



template<class T> class ConfigVar : public ConfigVarBase{
public:
	typedef std::shared_ptr<ConfigVar> ptr;
	ConfigVar(const std::string& name,const T& defualt_setting,const std::string& description)
	:ConfigVarBase(name,description)
	,m_setting(defualt_setting){
	}

	std::string toString() override;
	bool parserString(const std::string& val) override;
private:
	T m_setting;
};

class ConfigManager{
public:
	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

private:
	static ConfigVarMap m_map;
};




}

#endif