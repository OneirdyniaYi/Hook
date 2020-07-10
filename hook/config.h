#ifndef __Hook__Config__H__
#define __Hook__Config__H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>


namespace hook{

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

	std::string toString() override {
		try{
			return boost::lexical_cast<str::string>(m_setting);
		}catch(std::exception& e){
			HOOK_LOG_ERROR(HOOK_LOG_ROOT()) << "ConfigVar::toString exception" << e.what() << "convert:"
			<< typeid(m_setting).name() << "to string";
		}
		return "";
	}
	bool parserString(const std::string& val) override{
		try{
			m_setting = boost::lexical_cast<T>(val);
		}catch{
			HOOK_LOG_ERROR(HOOK_LOG_ROOT()) << "ConfigVar::toString exception" << e.what() << "convert: string to"
			<< typeid(m_setting).name();
		}
		return false;
	};
private:
	T m_setting;
};



class ConfigManager{
public:
	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;
	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
		const T& defualt_setting,const std::string& description = ""){
		auto tmp = Lookup<T>(name);
		if(tmp){
			HOOK_LOG_INFO(HOOK_LOG_ROOT())<<"Lookup name "<<name<<" has existed.";
			return tmp;
		}
		if(name.find_first_not_of("")){
			HOOK_LOG_ERROR(HOOK_LOG_ROOT())<<"Lookup name "<<name<<" invalid";
			throw std::invalid_argument(name);
		}
		typename ConfigVar<T>::ptr pptr(new ConfigVar<T>(name,defualt_setting,description));
		m_map[name] = pptr;
	}
	typename ConfigVar<T>::ptr Lookup(const std::string& name){
		auto it = m_map.find(name);
		if(it == m_map.end()) return nullptr;
		return std::dynamic_pointer_cast<ConfigVar>()
	}
private:
	static ConfigVarMap m_map;
};




}

#endif