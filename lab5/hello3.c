
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(howmany, "Parameter that determines how many times \"Hello world!\" will be printed");

static uint howmany = 1;
module_param(howmany, int, S_IRUGO);

static LIST_HEAD(my_list_head);

struct my_list {
	struct list_head list;
	ktime_t timestamp;
};

static int __init hello_init(void)
{
	BUG_ON(howmany > 10);
	if (howmany == 0 || (howmany >= 5 && howmany <= 10))
		pr_warn("Howmany is 0 or between 5 and 10\n");
	else {
		int i = 0;
		while (i < howmany) {
			if(i == howmany - 1){
				struct my_list *new_entry = NULL;
				INIT_LIST_HEAD(&new_entry->list);
				list_add_tail(&new_entry->list, &my_list_head);
			}else{
			struct my_list *new_entry =
					kmalloc(sizeof(*new_entry), GFP_KERNEL);
			new_entry->timestamp = ktime_get();
			INIT_LIST_HEAD(&new_entry->list);
			list_add_tail(&new_entry->list, &my_list_head);
			pr_info("Hello, world!\n");
			i++;}
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct my_list *pos, *next;
	list_for_each_entry_safe(pos, next, &my_list_head, list) {
		pr_info("Timestamp: %lld ns\n",
			ktime_to_ns(pos->timestamp));
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello_init);
module_exit(hello_exit);
