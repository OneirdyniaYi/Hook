#include "util.h"

namespace hook{
	pid_t GetThreadId(void){
		return syscall(SYS_gettid);
	}

	uint32_t GetFiberId(){
		return 0;
	}
}