#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>

namespace sylar{

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();
private:
	const char* m_file = nullptr; 	//filename
	int32_t m_line = 0; 			//line number
	uint32_t m_threadId = 0; 		//线程号id
	uint32_t m_fiberId = 0;			//协程号id
	uint64_t m_time;
	uint32_t m_elapse = 0;			//程序启动时间
	std:string m_content; 			//msg content



};

//日志级别
class LogLevel{
public:
	enum level{
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
	};
}
//日志器

class Logger{
	public:
		typedef std::shared_ptr<Logger> ptr;

		logger(const std::string& name = "root");
		void log(LogLevel::Level level,LogEvenet::ptr event);
		void debug(LogEvnet::ptr event);
		void info(LogEvnet::ptr event);
		void warn(LogEvnet::ptr event);
		void error(LogEvnet::ptr event);
		void fatal(LogEvnet::ptr event);
	private:
		std::string m_name;				//日志名称
		LogLevel::level m_leve; 		//日志级别
		std::list<LogAppender::ptr> m_appenders;				//Appender集合
};

//日志输出地址
class LogAppender{
public: 
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	void log(LogLevel::Level level,LogEvent::ptr event);
private:
	LogLevel::Level m_level;
};

//日志格式
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	std::string format(LogEvent::ptr event);
private:
};


}

//输出到控制台的Appender
class StdoutLogAppender: public LogAppender{

};

//输出到文件的Appender
class StdoutLogAppender: public LogAppender{

};

#endif