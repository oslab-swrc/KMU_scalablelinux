#ifndef __LINUX_DEFERABLE_UPDATE
#define __LINUX_DEFERABLE_UPDATE

#include <linux/llist.h>
#include <linux/mutex.h>

#define DEFERU_TYPE_1
/* #define DEFERU_TYPE_2 */
/* #define DEFERU_TYPE_3 */

#define DEFERU_LINKED_LIST		0
#define DEFERU_INTERVAL_TREE	1

#define DEFERU_OP_ADD 0
#define DEFERU_OP_DEL 1

struct deferu_operations {
	void (*add)(void *node, void *head);
	void (*del)(void *node, void *head);
	void (*move)(void *node, void *head);
};

struct deferu_head {
	int completed; /* preventing for next reader's read operation */
	int init;
	struct llist_head ll_head;
	struct mutex mutex;
	struct deferu_operations *du_ops;
};

struct deferu_node {
	void *key;
	atomic_t reference;
	int op_num;
	struct llist_node ll_node;
};

struct deferu_i_mmap_node {
	struct deferu_node defer_node[2]; /* 0 : add op, 1 : del op */
};

void synchronize_deferu_i_mmap(struct deferu_head *head, struct rb_root *root);

static inline void init_deferu_head(struct deferu_head *dp,
		struct deferu_operations *ops)
{
	pr_info("@@ : init_deferu_head\n");
	dp->completed = 0;
	init_llist_head(&dp->ll_head);
	mutex_init(&dp->mutex);
	dp->du_ops = ops;
	dp->init = 0x55;
}

static inline bool deferu_add(struct deferu_node *new, struct deferu_head *head)
{
	return llist_add(&new->ll_node, &head->ll_head);
}

void i_mmap_deferu_add(struct vm_area_struct *vma, struct rb_root *root);
void i_mmap_deferu_del(struct vm_area_struct *vma, struct rb_root *root);

#endif /* __LINUX_DEFERABLE_UPDATE */
