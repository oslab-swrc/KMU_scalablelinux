#ifndef __LINUX_DEFERABLE_UPDATE
#define __LINUX_DEFERABLE_UPDATE

#include <linux/llist.h>
#include <linux/mutex.h>

#define DEFERU_TYPE_1
/* #define DEFERU_TYPE_2 */
/* #define DEFERU_TYPE_3 */

#define DEFERU_LINKED_LIST		0
#define DEFERU_INTERVAL_TREE	1

struct deferu_operations {
	void (*add)(void *node, void *head);
	void (*del)(void *node, void *head);
	void (*move)(void *node, void *head);
};

struct deferu_head {
	int completed; /* preventing for next reader's read operation */
	struct llist_head ll_head;
	struct mutex mutex;
	struct deferu_operations *du_ops;
};

struct deferu_node {
	void *key;
	int op_num;
	struct llist_node *ll_node;
};

void synchronize_deferu(struct deferu_head *head);

static inline void init_deferu_head(struct deferu_head *dp,
		struct deferu_operations *ops)
{
	dp->completed = 0;
	init_llist_head(&dp->ll_head);
	mutex_init(&dp->mutex);
	dp->du_ops = ops;
}

static inline bool deferu_add(struct deferu_node *new, struct deferu_head *head)
{
	return llist_add(new, new, head);
}

#endif /* __LINUX_DEFERABLE_UPDATE */
