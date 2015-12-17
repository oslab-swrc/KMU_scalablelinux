#ifndef __LINUX_DEFERABLE_UPDATE
#define __LINUX_DEFERABLE_UPDATE

#include <linux/llist.h>
#include <linux/mutex.h>

#define DEFERU_TYPE_1
/* #define DEFERU_TYPE_2 */
/* #define DEFERU_TYPE_3 */

#define DEFERU_LINKED_LIST		0
#define DEFERU_INTERVAL_TREE	1

#define DEFERU_OP_ADD 1
#define DEFERU_OP_DEL 2
#define DEFERU_OP_ADD_AFTER 3

struct deferu_head {
	struct delayed_work sync;
	struct llist_head ll_head;
};

struct deferu_node {
	void *key;
	atomic_t mark;
	int op_num;
	struct rb_root *root;
	struct llist_node ll_node;
};

struct deferu_i_mmap_node {
	unsigned long used;
	unsigned long exist;
	struct deferu_node defer_node[2]; /* 0 : add op, 1 : del op */
};

void i_mmap_free_work_func(struct work_struct *work);

void deferu_physical_update(int op, struct vm_area_struct *vma, struct rb_root *root);
static inline void init_deferu_head(struct deferu_head *dp)
{
	init_llist_head(&dp->ll_head);
	INIT_DELAYED_WORK(&dp->sync, i_mmap_free_work_func);
}

#endif /* __LINUX_DEFERABLE_UPDATE */
