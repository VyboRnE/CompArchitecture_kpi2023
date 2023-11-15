#include <linux/init.h>
#include <linux/module.h>
#include "inc/hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello2, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(howmany, "Parameter that determines how many times \"Hello world!\" will be printed");

static uint howmany = 1;
module_param(howmany, int, S_IRUGO);

static int __init hello2_init(void)
{
    if (howmany > 10) {
		pr_err("Howmany is over 10.Error\n");
		return -EINVAL;
	}
	if (howmany == 0 || (howmany >= 5 && howmany <= 10))
		pr_warn("Howmany is 0 or between 5 and 10\n");
	else {
	int i = 0;
	while (i < howmany) {
		print_hello();
		i++;
		}
	}
    return 0;
}
static void __exit hello2_exit(void)
{
}

module_init(hello2_init);
module_exit(hello2_exit);
