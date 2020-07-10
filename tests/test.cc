#include <iostream>
#include "../hook/log.h"

int main(int argc,char** ragv){
	hook::Logger::ptr logger(new hook::Logger);
	logger->addAppender(hook::LogAppender::ptr(new hook::StdoutLogAppender));
	hook::LogEvent::ptr event(new hook::LogEvent(logger,hook::LogLevel::Level::DEBUG,__FILE__,__LINE__,0,1,2,time(0)));

	logger->log(hook::LogLevel::DEBUG,event);

	

	return 0;

}
