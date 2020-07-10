#ifndef __HOOK_LOG_H__
#define __HOOK_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include "util.h"
#include "singleton.h"
//#include "thread.h"



#define HOOK_LOG_LEVEL(logger ,level) \
	if(logger->getLevel() <= level) \
		hook::LogEventWrap(hook::LogEvent ptr(new LogEvent(logger, level, __FILE__, __LINE__, 0, hook::GetThreadId(), \
			hook::GetFiberId(), time(0), hook::Thread::GetName()))).getSS()

#define HOOK_LOG_DEBUG(logger) HOOK_LOG_LEVEL(logger, hook::LogLevel::DEBUG)

#define HOOK_LOG_INFO(logger) HOOK_LOG_LEVEL(logger, hook::LogLevel::INFO)

#define HOOK_LOG_WARN(logger) HOOK_LOG_LEVEL(logger, hook::LogLevel::WARN)

#define HOOK_LOG_ERROR(logger) HOOK_LOG_LEVEL(logger, hook::LogLevel::ERROR)

#define HOOK_LOG_FATAL(logger) HOOK_LOG_LEVEL(logger, hook::LogLevel::FATAL)

namespace hook{

class Logger;

//日志级别
class LogLevel{
public:
	enum Level{
			UNKNOW = 0,
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
	};
	static const char* ToString(LogLevel::Level level);
};

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level,const char* file,int32_t m_line,uint32_t elapse,uint32_t thread_id,uint32_t fiber_id,uint64_t time);

	std::shared_ptr<Logger> getLogger() {return m_logger;}
	LogLevel::Level getLevel() {return m_level;}
	const char* getFile() const {return m_file;}
	int32_t getLine() const {return m_line;}
	uint32_t getElapse() const {return m_elapse;}
	uint32_t getThreadId() const {return m_threadId;}
	uint32_t getFiberId() const {return m_fiberId;}
	uint32_t getTime() const {return m_time;}
	std::string getContent() const {return m_ss.str();}
	std::stringstream& getSS() {return m_ss;}
	void format(const char* fmt, ...);
	void format(const char* fmt, va_list al);
private:
	std::shared_ptr<Logger> m_logger;
	LogLevel::Level m_level;
	const char* m_file = nullptr; 	//filename
	int32_t m_line = 0; 			//line number
	uint32_t m_elapse = 0;			//程序启动时间
	uint32_t m_threadId = 0; 		//线程号id
	uint32_t m_fiberId = 0;			//协程号id
	uint64_t m_time = 0;
	std::stringstream m_ss; 			//msg content
};

class LogEventWrap{
public:
	LogEventWrap(LogEvent::ptr e);
	~LogEventWrap();
	std::stringstream& getSS();
	LogEvent::ptr getEvent() const {return m_event;}
private:
	LogEvent::ptr m_event;
};

//日志格式
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(const std::string& pattern);
	//%t %thread_id %m%n
	std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);
	std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);
public:
	class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual ~FormatItem(){}
		virtual void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
	};
	void init();
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;
};

//日志输出地址
class LogAppender{
public: 
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
	void setFormatter(std::shared_ptr<LogFormatter> val) { m_formatter = val;}
	LogFormatter::ptr getFormatter() const {return m_formatter;}

	LogLevel::Level getLevel() const { return m_level;}
	void setLevel(LogLevel::Level val) { m_level = val;}
protected:
	LogLevel::Level m_level;
	std::shared_ptr<LogFormatter> m_formatter;
};


//日志器
class Logger : public std::enable_shared_from_this<Logger>{
	public:
		typedef std::shared_ptr<Logger> ptr;

		Logger(const std::string& name = "root");
		void log(LogLevel::Level level,LogEvent::ptr event);
		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);
		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		LogLevel::Level getLevel() const {return m_level;}
		void setLevel(LogLevel::Level val){ m_level = val;}
		const std::string& getName() const {return m_name;}
	private:
		std::string m_name;				//日志名称
		LogLevel::Level m_level; 		//日志级别
		std::shared_ptr<LogFormatter> m_formatter;
		std::list<std::shared_ptr<LogAppender>> m_appenders;				//Appender集合
};


//输出到控制台的Appender
class StdoutLogAppender: public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override;
private:
};

//输出到文件的Appender
class FileLogAppender: public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	FileLogAppender(const std::string& filename);
	void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) override;
	bool reopen();
private:
	std::string m_filename;
	std::ofstream m_filestream;
};

} 
#endif