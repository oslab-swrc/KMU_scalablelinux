#ifndef _LOCKFREE_LIST_H_
#define _LOCKFREE_LIST_H_

#include <linux/kernel.h>
#include <asm/cmpxchg.h>

struct lockfree_list_head {
    struct lockfree_list_node *head, *tail;
};

struct lockfree_list_node {
    struct lockfree_list_node *next;
    void *key;
    int garbage;
};

#define LOCKFREE_LIST_HEAD_INIT(name)   { NULL, NULL }
#define LOCKFREE_LIST_HEAD(name) \
		struct lockfree_list_head name = LOCKFREE_LIST_HEAD_INIT(name)

#define LOCKFREE_LIST_HEAD_NODE_INIT(name)   { NULL, NULL }
#define LOCKFREE_LIST_HEAD_NODE(name) \
		struct lockfree_list_node name = LOCKFREE_LIST_HEAD_NODE_INIT(name)

#define LOCKFREE_LIST_TAIL_NODE_INIT(name)   { NULL, NULL }
#define LOCKFREE_LIST_TAIL_NODE(name) \
		struct lockfree_list_node name = LOCKFREE_LIST_TAIL_NODE_INIT(name)

#define LOCKFREE_LIST_SAVE_KEY(name, member) \
		(name)->member.key = (void *)(name)

#define LOCKFREE_LIST_CLEAR_GC(name, member) \
		(name)->member.garbage = 0

static inline void init_lockfree_list_head(struct lockfree_list_head *list,
        struct lockfree_list_node *head, struct lockfree_list_node *tail)
{
	tail->key = (void *)LONG_MAX;
	tail->next = NULL;
	head->key = 0;
	head->next = tail;
	list->head = head;
	list->tail = tail;
}

#define lockfree_list_entry(ptr, type, member)      \
    container_of(ptr, type, member)

#define lockfree_list_for_each(pos, node)           \
    for ((pos) = (node); pos; (pos) = (pos)->next)

#define lockfree_list_for_each_entry(pos, node, member)             \
    for ((pos) = lockfree_list_entry((node), typeof(*(pos)), member);   \
         &(pos)->member != NULL;                    \
         (pos) = lockfree_list_entry((pos)->member.next, typeof(*(pos)), member))

#define lockfree_list_for_each_entry_safe(pos, n, node, member)               \
    for (pos = lockfree_list_entry((node), typeof(*pos), member);             \
         &pos->member != NULL &&                           \
         (n = lockfree_list_entry(pos->member.next, typeof(*n), member), true); \
         pos = n)

static inline bool lockfree_list_empty(const struct lockfree_list_head *head)
{
	return ACCESS_ONCE(head->head->next) == ACCESS_ONCE(head->tail);
}

static inline struct lockfree_list_node *lockfree_list_next(
		struct lockfree_list_node *node)
{
	return node->next;
}

extern bool lockfree_list_add_batch(struct lockfree_list_node *new,
		struct lockfree_list_head *head);
static inline bool lockfree_list_add(struct lockfree_list_node *new,
		struct lockfree_list_head *head)
{
	return lockfree_list_add_batch(new, head);
}

struct lockfree_list_node *lockfree_list_del_batch(
		struct lockfree_list_node *node, struct lockfree_list_head *head);
static inline struct lockfree_list_node *lockfree_list_del(
		struct lockfree_list_node *node, struct lockfree_list_head *head)
{
	return lockfree_list_del_batch(node, head);
}

static inline int lockfree_list_is_singular(const struct lockfree_list_head *head)
{
	return !lockfree_list_empty(head) && (head->head->next->next == head->tail);
}


#endif /* #ifndef _LOCKFREE_LIST_H_ */
