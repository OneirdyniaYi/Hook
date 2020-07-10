#ifndef __Hook_Util_H__
#define __Hook_Util_H__

#include <unistd.h>
#ifdef __linux__
	#include <sys/syscall.h>
	//#include <thread.h>
#elif __WIN32__
	//#include <windows.h>
#endif
#include <stdint.h>

namespace hook{
	pid_t GetThreadId(void);
	uint32_t GetFiberId();

}

#endif