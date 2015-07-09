#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/interrupt.h>
#include <linux/lockfree_list.h>

static inline int is_marked_ref(long i)
{
	return (i & 0x1L);
}

static inline long get_unmarked_ref(long w)
{
	return (w & ~0x1L);
}

static inline long get_marked_ref(long w)
{
	return (w | 0x1L);
}

static struct lockfree_list_node *list_search_for_add(struct lockfree_list_head *head, void *key,
		struct lockfree_list_node **left_node)
{
	struct lockfree_list_node *left_node_next;
	struct lockfree_list_node *right_node;

	left_node_next = head->head;
search_again:
	do {
		struct lockfree_list_node *t = head->head;
		struct lockfree_list_node *t_next = head->head->next;

		/* Find left_node and right_node */
		do {
			if (!is_marked_ref((long)t_next)) {
				(*left_node) = t;
				left_node_next = t_next;
			}
			t = (struct lockfree_list_node *) get_unmarked_ref((long)t_next);
			if (t == head->tail)
				break;
			t_next = t->next;
		} while (is_marked_ref((long)t_next));
		right_node = t;

		/* Check that nodes are adjacent */
		if (left_node_next == right_node) {
			if ((right_node != head->tail) && is_marked_ref((long) right_node->next))
				goto search_again;
			else {
				return right_node;
			}
		}

		/* Remove one or more marked nodes */
		if (cmpxchg(&(*left_node)->next, left_node_next, right_node) == left_node_next) {
			if ((right_node != head->tail) && is_marked_ref((long) right_node->next))
				goto search_again;
			else {
				return right_node;
			}
		}

	} while (1);
}


static struct lockfree_list_node *list_search_for_del(struct lockfree_list_head *head, void *key,
		struct lockfree_list_node **left_node)
{
	struct lockfree_list_node *left_node_next;
	struct lockfree_list_node *right_node;

	left_node_next = head->head;
search_again:
	do {
		struct lockfree_list_node *t = head->head;
		struct lockfree_list_node *t_next = head->head->next;

		/* Find left_node and right_node */
		do {
			if (!is_marked_ref((long)t_next)) {
				(*left_node) = t;
				left_node_next = t_next;
			}
			t = (struct lockfree_list_node *) get_unmarked_ref((long)t_next);
			if (t == head->tail)
				break;
			t_next = t->next;
		} while (is_marked_ref((long)t_next) || (t->key != key));
		right_node = t;

		/* Check that nodes are adjacent */
		if (left_node_next == right_node) {
			if ((right_node != head->tail) && is_marked_ref((long) right_node->next))
				goto search_again;
			else
				return right_node;
		}

		/* Remove one or more marked nodes */
		if (cmpxchg(&(*left_node)->next, left_node_next, right_node) == left_node_next) {
			if ((right_node != head->tail) && is_marked_ref((long) right_node->next))
				goto search_again;
			else
				return right_node;
		}

	} while (1);
}

bool lockfree_list_add_batch(struct lockfree_list_node *new,
		struct lockfree_list_head *head)
{
	struct lockfree_list_node *right_node, *left_node;
	left_node = head->head;
	do {
		right_node = list_search_for_add(head, new->key, &left_node);

		new->next = right_node;
		smp_mb();
	} while (cmpxchg(&left_node->next, right_node, new) != right_node);

	return true;
}
EXPORT_SYMBOL_GPL(lockfree_list_add_batch);

struct lockfree_list_node *lockfree_list_del_batch(
		struct lockfree_list_node *node, struct lockfree_list_head *head)
{
	struct lockfree_list_node *right_node, *right_node_next, *left_node;
	left_node = head->head;
	do {
		right_node = list_search_for_del(head, node->key, &left_node);
		if ((right_node == head->tail) || (right_node->key != node->key)) {
			printk(KERN_ALERT "lockfree_list del fail");
			return NULL;
		}
		right_node_next = right_node->next;
		if (!is_marked_ref((long) right_node_next))
			if (cmpxchg(&right_node->next, right_node_next,
					(struct lockfree_list_node *)get_marked_ref((long) right_node_next)) == right_node_next)
				break;
	} while (1);
	if (cmpxchg(&left_node->next, right_node, right_node_next) != right_node)
		right_node = list_search_for_del(head, right_node->key, &left_node);

	return right_node;
}
EXPORT_SYMBOL_GPL(lockfree_list_del_batch);
