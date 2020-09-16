#include <unistd.h>
#include <sys/syscall.h>
int main(){
	unsigned int cpu, node;
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	syscall(SYS_getcpu, &cpu, &node, NULL);
	return 0;
}

