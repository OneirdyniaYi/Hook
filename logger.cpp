#include "log.h"

namespace sylar{
	Logger::Logger(const std::string& name):m_name(name){}

	void Logger::addAppender(LogAppender::ptr appender){
		m_appenders.push_back(appender);
	}

	void Logger::delAppender(LogAppender::ptr appender){
		for(auto it = m_appenders.begin();it != m_appenders.end();it++){
			if(*it == appender) {
				m_appenders.erase(it);
				return;
			}
		}
		//error
	}
	void Logger::log(LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level){
			for(auto& i: m_appenders){
				i->log(level,event);
			}
		}
	}

	void Logger::debug(LogEvenet::ptr event){
		debug(LogLevel::DEBUG,event);
	}

	void Logger::info(LogEvenet::ptr event){
		debug(LogLevel::INFO,event);
	}

	void Logger::warn(LogEvenet::ptr event){
		debug(LogLevel::WARN,event);
	}

	void Logger::error(LogEvenet::ptr event){
		debug(LogLevel::ERROR,event);
	}

	void Logger::fatal(LogEvenet::ptr event){
		debug(LogLevel::FATAL,event);
	}


}
