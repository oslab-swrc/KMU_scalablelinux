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

struct deferu_operations {
	void (*add)(void *node, void *head);
	void (*del)(void *node, void *head);
	void (*move)(void *node, void *head);
};

struct deferu_head {
	int completed; /* preventing for next reader's read operation */
	int init;
	struct delayed_work sync;
	struct llist_head ll_head;
};

struct deferu_node {
	void *key;
	atomic_t reference;
	int op_num;
	int garbage;
	struct rb_root *root;
	struct llist_node ll_node;
};

struct deferu_i_mmap_node {
	unsigned long used;
	struct deferu_node defer_node[2]; /* 0 : add op, 1 : del op */
};

void i_mmap_free_work_func(struct work_struct *work);

static inline void init_deferu_head(struct deferu_head *dp)
{
	dp->completed = 0;
	init_llist_head(&dp->ll_head);
	dp->init = 0;
	INIT_DELAYED_WORK(&dp->sync, i_mmap_free_work_func);
}

void i_mmap_deferu_add(struct vm_area_struct *vma, struct rb_root *root);
void i_mmap_deferu_del(struct vm_area_struct *vma, struct rb_root *root);

#endif /* __LINUX_DEFERABLE_UPDATE */
