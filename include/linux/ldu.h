#ifndef __LINUX_DEFERABLE_UPDATE
#define __LINUX_DEFERABLE_UPDATE

#include <linux/llist.h>
#include <linux/mutex.h>

#define LDU_LINKED_LIST     0
#define LDU_INTERVAL_TREE   1

#define LDU_OP_ADD 0
#define LDU_OP_DEL 1

struct ldu_head {
	struct delayed_work sync;
	struct llist_head ll_head;
};

struct ldu_node {
	void *key;
	atomic_t mark;
	int op_num;
	struct rb_root *root;
	struct llist_node ll_node;
};

struct ldu_anon_node {
	unsigned long used;
	struct ldu_node node[2]; /* 0 : add op, 1 : del op */
};

void avc_free_work_func(struct work_struct *work);

void synchronize_ldu_anon(struct anon_vma *anon);

static inline void init_ldu_head(struct ldu_head *dp)
{
	init_llist_head(&dp->ll_head);
	INIT_DELAYED_WORK(&dp->sync, avc_free_work_func);
}

#endif /* __LINUX_DEFERABLE_UPDATE */
