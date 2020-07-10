#include "log.h"
#include <map>
#include <functional>
#include <memory>
#include <iostream>
#include <time.h>






namespace hook{
	const char* LogLevel::ToString(LogLevel::Level level){
		switch(level){
	#define XX(name)\
		case LogLevel::name:\
			return #name;\
			break

		XX(DEBUG);
		XX(INFO);
		XX(WARN);
		XX(ERROR);
		XX(FATAL);
	#undef XX
		default:
			return "UNKNOW";
		}

	}

	LogEventWrap::LogEventWrap(LogEventWrap::ptr e):m_event(e){

	}
	LogEventWrap::~LogEventWrap(){
		m_event->getLogger()->log(m_event->getLevel(), m_event);
	}
	std::stringstream& LogEventWrap::getSS(){
		return m_event->getSS();
	}
	class MessageFormatItem : public LogFormatter::FormatItem{
	public:
		MessageFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override{
			os<<event->getContent();
		}
	};

	class LevelFormatItem : public LogFormatter::FormatItem
	{
	public:
		LevelFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<LogLevel::ToString(level);
		}
		
	};
	class ElapseFormatItem : public LogFormatter::FormatItem
	{
	public:
		ElapseFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getElapse();
		}
		
	};	
	class NameFormatItem : public LogFormatter::FormatItem
	{
	public:
		NameFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<logger->getName();
		}
		
	};
	class ThreadIdFormatItem : public LogFormatter::FormatItem
	{
	public:
		ThreadIdFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getThreadId();
		}
		
	};
	class FiberIdFormatItem : public LogFormatter::FormatItem
	{
	public:
		FiberIdFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getFiberId();
		}
		
	};
	class DateTimeFormatItem : public LogFormatter::FormatItem
	{
	public:
		DateTimeFormatItem(const std::string& format = "%y-%m-%d %H:%M:%S"):m_format(format){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			struct tm tm;
	        time_t time = event->getTime();
	        localtime_r(&time, &tm);//linux 下获取localtime   windows 下用localtime_s
	        char buf[64];
	        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
	        os << buf;
		}
	private:
		std::string m_format;
	};
	class FilenameFormatItem : public LogFormatter::FormatItem
	{
	public:
		FilenameFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getFile();
		}
		
	};
	class LineFormatItem : public LogFormatter::FormatItem
	{
	public:
		LineFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getLine();
		}
		
	};
	class NewLineFormatItem : public LogFormatter::FormatItem
	{
	public:
		NewLineFormatItem(const std::string& fmt = ""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<std::endl;
		}
		
	};
	class StringFormatItem : public LogFormatter::FormatItem
	{
	public:
		StringFormatItem(const std::string& str):m_string(str){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<m_string;
		}
	private:
		std::string m_string;
		
	};

	class TabFormatItem : public LogFormatter::FormatItem
	{
	public:
		TabFormatItem(const std::string& fmt=""){}
		void format(std::ostream& os,Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<"\t";
		}
	};

	Logger::Logger(const std::string& name):m_name(name),m_level(LogLevel::DEBUG){
		m_formatter.reset(new LogFormatter("%d [%p] %m %f"));
	}

	// void Logger::format(const char* fmt, ...){
	// 	va_list al;
	// 	va_start(al, fmt);
	// 	format(fmt, al);
	// 	va_end(al);
	// }

	// void Logger::format(const char* fmt, va_list vl){
	// 	char* buf = nullptr;
	// 	int res = vasprint(&buf, fmt, al);
	// 	if(res != -1){
	// 		m_ss << std::string(buf, len);
	// 		free(buf);
	// 	}
	// }

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

	LogEvent::LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level,const char* file,int32_t line,
		uint32_t elapse,uint32_t thread_id,uint32_t fiber_id,uint64_t time)
		:m_file(file),m_line(line),m_elapse(elapse),m_threadId(thread_id),
		m_fiberId(fiber_id),m_time(time),m_level(level),m_logger(logger){
		}

	void Logger::log(LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level){
			auto self = shared_from_this();
	        if(!m_appenders.empty()) {
	            for(auto& i : m_appenders) {
	                i->log(self, level, event);
	            }
	        }
		}
	}

	void Logger::debug(LogEvent::ptr event){
		log(LogLevel::DEBUG,event);
	}

	void Logger::info(LogEvent::ptr event){
		log(LogLevel::INFO,event);
	}

	void Logger::warn(LogEvent::ptr event){
		log(LogLevel::WARN,event);
	}

	void Logger::error(LogEvent::ptr event){
		log(LogLevel::ERROR,event);
	}

	void Logger::fatal(LogEvent::ptr event){
		log(LogLevel::FATAL,event);
	}

	FileLogAppender::FileLogAppender(const std::string& filename):m_filename(filename){}
	
	void FileLogAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level) {
			m_formatter->format(m_filename,logger,level,event);
		}
	}

	bool FileLogAppender::reopen(){
		if(m_filestream){
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}
	void StdoutLogAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level) {
			m_formatter->format(std::cout,logger,level,event);
		}
	}

	LogFormatter::LogFormatter(const std::string& pattern):m_pattern(pattern){
		init();
	}

	std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
		for(auto& i : m_items){
			i->format(ofs,logger,level,event);
		}
		return ofs;
	}

	std::string LogFormatter::format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
		std::stringstream ss;
		for(auto& i : m_items){
			i->format(ss,logger,level,event);
		}
		return ss.str();
	}
	//%xxx %xxx{xxx} %%转义
	void LogFormatter::init(){
		std::vector<std::tuple<std::string,std::string,int>> vec;
		std::string str,fmt;
		for(size_t i=0;i<m_pattern.size();i++){
			if(m_pattern[i]=='%'&&i+1<m_pattern.size()){
				if(m_pattern[i+1]=='%'){
					str.append(1,'%');
					i++;
					continue;
				}else{
					if(!str.empty()){
						vec.push_back(std::make_tuple(str,"",0));
						str.clear();
					} 
					std::vector<size_t> ss;
					size_t n = i+1;
					while(isspace(m_pattern[n])){
						n++;
					}
					str.append(1,m_pattern[n]);
					if(m_pattern[n+1]=='{'){
						fmt.append(1,'[');
						ss.push_back(n++);
						while(n<m_pattern.size()&&!ss.empty()){
							if(m_pattern[n]=='{'){
								fmt.append(1,'[');
								ss.push_back(n);
							}
							else if(m_pattern[n]=='}'){
								fmt.append(1,']');
								ss.pop_back();
							}
							else{
								fmt.append(1,m_pattern[n]);
							}
							n++;
						}
						if(!ss.empty()){
							std::cout << "pattern prase error" << m_pattern << std::endl;
							vec.push_back(std::make_tuple("<pattern_prase_error>",fmt,0));
						}
					}
					vec.push_back(std::make_tuple(str,fmt,1));
					fmt.clear();
					str.clear();
					i = n;
				}
			}else{
				str.append(1,m_pattern[i]);
			}
		}

		static std::map<std::string,std::function<LogFormatter::FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str,C) \
			{#str,[](const std::string& fmt){return LogFormatter::FormatItem::ptr(new C(fmt));}}
			XX(m,MessageFormatItem),   
			XX(p,LevelFormatItem),
			XX(r,ElapseFormatItem),
			XX(c,NameFormatItem),
			XX(t,ThreadIdFormatItem),
			XX(n,NewLineFormatItem),
			XX(d,DateTimeFormatItem),
			XX(f,FilenameFormatItem),
			XX(l,LineFormatItem),
			XX(b,TabFormatItem)
#undef XX
		};

		for(auto& i:vec){
			if(std::get<2>(i)==0){
				m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
			}else{
				auto it = s_format_items.find(std::get<0>(i));
				if(it == s_format_items.end()){
					m_items.push_back(FormatItem::ptr(new StringFormatItem("<<pattern_prase_error:"+m_pattern+">>")));
				}else{
					m_items.push_back(it->second(std::get<1>(i)));
				}
			}
		}

	}
}

