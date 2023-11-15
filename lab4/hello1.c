#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include "inc/hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(howmany, "Parameter that determines how many times \"Hello world!\" will be printed");

static uint howmany = 1;
module_param(howmany, int, S_IRUGO);
struct my_list my_list_head;
static int __init hello1_init(void)
{
	INIT_LIST_HEAD(&my_list_head.list);
	if (howmany > 10) {
		pr_err("Howmany is over 10.Error\n");
		return -EINVAL;
	}
	if (howmany == 0 || (howmany >= 5 && howmany <= 10))
		pr_warn("Howmany is 0 or between 5 and 10\n");
	else {
		int i = 0;
		while (i < howmany) {
			//виділяємо пам'ять
			struct my_list *new_entry =
				kmalloc(sizeof(*new_entry), GFP_KERNEL);
			//записуємо час перед виводом
			new_entry->starttime = ktime_get();
			//вивід
			print_hello();
			//записуємо час після виводу
			new_entry->endtime = ktime_get();
			//ініціалізуємо поле list у створеній структурі
			INIT_LIST_HEAD(&new_entry->list);
			//додаємо структуру у кінець списку
			list_add_tail(&new_entry->list, &my_list_head.list);
			i++;
		}
	}
	return 0;
}

static void __exit hello1_exit(void)
{
    //INIT_LIST_HEAD(&my_list_head.list);
	//визначаємо вказівники
	struct my_list *pos, *next;
	//прохід по списку
	list_for_each_entry_safe(pos, next, &my_list_head.list, list) {
		//друкуємо час у наносукундах
		pr_info("Printing time: %lld ns\n",
			ktime_to_ns(pos->endtime - pos->starttime));
		//видаляємо поточний елемент
		list_del(&pos->list);
		//звільняємо пам'ять
		kfree(pos);
	}
}

void print_hello(void)
{
	pr_info("Hello world!");
}
EXPORT_SYMBOL(print_hello);

module_init(hello1_init);
module_exit(hello1_exit);