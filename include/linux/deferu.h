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
	struct llist_head ll_head;
	struct mutex mutex;
	struct deferu_operations *du_ops;
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
	int used;
	struct deferu_node defer_node[2]; /* 0 : add op, 1 : del op */
};

bool deferu_add_i_mmap(struct deferu_node *dnode);
void synchronize_deferu_i_mmap(void);

void deferu_add_i_mmap_lock(void);
void deferu_add_i_mmap_unlock(void);

void i_mmap_deferu_add(struct vm_area_struct *vma, struct rb_root *root);
void i_mmap_deferu_del(struct vm_area_struct *vma, struct rb_root *root);

#endif /* __LINUX_DEFERABLE_UPDATE */
