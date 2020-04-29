#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage int sys_printkk(char *output)
{
	printk(output);
	return 0;
}
