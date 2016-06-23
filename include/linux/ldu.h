#ifndef __LINUX_DEFERABLE_UPDATE
#define __LINUX_DEFERABLE_UPDATE

#include <linux/llist.h>
#include <linux/mutex.h>

#define LDU_LINKED_LIST     0
#define LDU_INTERVAL_TREE   1

#define LDU_OP_ADD 1
#define LDU_OP_DEL 2

struct ldu_head {
	struct delayed_work sync;
	struct llist_head ll_head;
};

struct ldu_node {
	void *key;
	void *key2;
	atomic_t mark;
	int op_num;
	struct rb_root *root;
	struct llist_node ll_node;
};

struct ldu_anon_node {
	unsigned long used;
	struct ldu_node node[2]; /* 0 : add op, 1 : del op */
};

struct ldu_i_mmap_node {
	unsigned long used;
	struct ldu_node node[2]; /* 0 : add op, 1 : del op */
};


struct pldu_deferred {
	struct llist_head list;
	struct delayed_work wq;
};





static inline void anon_vma_init_ldu_head(struct ldu_head *dp)
{
	init_llist_head(&dp->ll_head);
}


void i_mmap_ldu_physical_update(int op, struct vm_area_struct *vma, struct rb_root *root);
static inline void i_mmap_init_ldu_head(struct ldu_head *dp)
{
	init_llist_head(&dp->ll_head);
}


#endif /* __LINUX_DEFERABLE_UPDATE */
