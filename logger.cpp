#include "log.h"
#include <map>


namespace sylar{
	const char* LogLevel::ToString(LogLevel::Level level){
		switch(level){
	#define XX(name)\
		case LogLevel::#name:\
			return #name;\
			break;

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
	class MessageFormatItem : public LogFormatter::FormatItem{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override{
			os<<event->getContent();
		}
	};

	class LevelFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<LogLevel::ToString(level);
		}
		
	};
	class ElapseFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<LogLevel::getElapse();
		}
		
	};	
	class NameFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<logger->getName();
		}
		
	};
	class ThreadIdFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getThreadId();
		}
		
	};
	class FiberIdFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getFiberId();
		}
		
	};
	class DateTimeFormatItem : public LogFormatter::FormatItem
	{
	public:
		DateTimeFormatItem(const std::string& format = "%y:%m:%d %H:%M:%S"):m_format(format){}
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getTime();
		}
	private:
		std::string m_format;
	};
	class FilenameFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getFile();
		}
		
	};
	class LineFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<event->getLine();
		}
		
	};
	class NewLineFormatItem : public LogFormatter::FormatItem
	{
	public:
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<std::endl;
		}
		
	};
	class StringFormatItem : public LogFormatter::FormatItem
	{
	public:
		StringFormatItem(const std::string& str):FormatItem(str),m_string(str){}
		void format(std::ostream& os,logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override {
			os<<m_string;
		}
	private:
		std::string m_string;
		
	};
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

	FileLogAppender::FileLogAppender(const std::string& filename):m_filename(filename){}
	
	void FileLogAppender::log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level) {
			m_filestream << m_formatter.format(logger,level,event);
		}
	}

	bool FileLogAppender::reopen(){
		if(m_filestream){
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}
	void StdoutLogAppender::logstd::shared_ptr<Logger> logger,(LogLevel::Level level,LogEvent::ptr event){
		if(level >= m_level) {
			std::cout << m_formatter.format(logger,level,event);
		}
	}

	LogFormatter::LogFormatter(const std::string& pattern):m_pattern(pattern){}
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
					std::vector<size_t> ss;
					size_t n = i+1;
					while(isspace(m_pattern[n])){
						n++;
					}
					if(m_pattern[n]=='{'){
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
					}else{
						str.append(1,m_pattern[n]);
					}
					vec.push_back(std::make_tuple(str,fmt,))>
				}
			}else{
				std::cout<<"The format of " << m_pattern << " error"<<" - "<<m_pattern.substr(i) << std::endl;
			}
		}

		static std::map<std::string,std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str,C) \
			{#str,[](const std::string& fmt){return FormatItem::ptr(new C(fmt))};},
			XX(m,MessageFormatItem),   
			XX(p,LevelFormatItem),
			XX(r,ElapseFormatItem),
			XX(c,NameFormatItem),
			XX(t,ThreadIdFormatItem),
			XX(n,NewLineFormatItem),
			XX(d,DateTimeFormatItem),
			XX(f,FilenameFormatItem),
			XX(l,LineFormatItem)
#undef XX
		};

		for(auto& i:vec){
			if(std::get<2>(i)==0){
				m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
			}
		}

	}
}

