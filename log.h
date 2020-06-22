#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>


namespace sylar{

class Logger;

//日志事件
class LogEvent{
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent(const char* file,int32_t m_line,uint32_t elapse,uint32_t thread_id,uint32_t fiber_id,uint64_t time);

	const char* getFile() const {return m_file;}
	int32_t getLine() const {return m_line;}
	uint32_t getElapse() const {return m_elapse;}
	uint32_t getThreadId() const {return m_threadId;}
	uint32_t getFiberId() const {return m_fiberId;}
	uint32_t getTime() const {return m_time;}
	std::string& getContent() const {return m_ss.str();}
	std::stringstream& getSS() {return m_ss;}
private:
	const char* m_file = nullptr; 	//filename
	int32_t m_line = 0; 			//line number
	uint32_t m_elapse = 0;			//程序启动时间
	uint32_t m_threadId = 0; 		//线程号id
	uint32_t m_fiberId = 0;			//协程号id
	uint64_t m_time = 0;
	std:stringstream m_ss; 			//msg content



};

//日志级别
class LogLevel{
public:
	enum Level{
			UNKNOW = 0;
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
	};
	static const char* ToString(LogLevel::Level level);
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

		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		LogLevel::Level getLevel() const {return m_level;}
		void setLevel(LogLevel::Level val){ m_level = val;}
		const std::string& getName() const {return m_name;}
	private:
		std::string m_name;				//日志名称
		LogLevel::level m_leve; 		//日志级别
		LogFormatter::ptr m_formatter;
		std::list<LogAppender::ptr> m_appenders;				//Appender集合
};

//日志输出地址
class LogAppender{
public: 
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
	void setFormatter(LogFormatter::ptr val) { m_formatter = val;}
	LogFormatter::ptr getFormatter() const {retrun m_formatter;}
protected:
	LogLevel::Level m_level;
	LogFormatter::ptr m_formatter;
};

//日志格式
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(const std::string& pattern);
	//%t %thread_id %m%n
	std::string format(std::shared_ptr<Logger> logger,logLevel::Level level,LogEvent::ptr event);
public:
	class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		FormatItem(const std::string& fmt = "");
		virtual ~FormatItem(){}
		virtual void format(std::ofstream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
	}
	void init();
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;
};



//输出到控制台的Appender
class StdoutLogAppender: public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	virtual void log(LogLevel::Level level,LogEvent::ptr event) override;
private:
};

//输出到文件的Appender
class FileLogAppender: public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	FileLogAppender(const std::string& filename);
	virtual void log(LogLevel::Level level,LogEvent::ptr event) override;
	void reopen();
private:
	std::string m_filename;
	std::ofstream m_filestream;
};

} 
#endif