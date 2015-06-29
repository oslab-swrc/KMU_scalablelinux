#include <linux/module.h>
#include <linux/lockfree_list.h>
#include <linux/random.h>
#include <asm/timex.h>

#define NODES       100
#define PERF_LOOPS  1
#define CHECK_LOOPS 100

struct test_node {
	struct lockfree_list_node node;
	u32 val;
};

static struct test_node nodes[NODES];
LOCKFREE_LIST_HEAD(test_head);
LOCKFREE_LIST_HEAD_NODE(test_head_node);
LOCKFREE_LIST_TAIL_NODE(test_tail_node);
static struct rnd_state rnd;

static void insert(struct test_node *node, struct lockfree_list_head *head)
{
	if (!lockfree_list_add(&node->node, head))
		printk(KERN_ALERT "lockfree_list fail due to add");
}

static inline void delete(struct test_node *node, struct lockfree_list_head *head)
{
	struct lockfree_list_node *lockfree_node;

	lockfree_node = lockfree_list_del(&node->node, head);
	if (!lockfree_node) {
		printk(KERN_ALERT "lockfree_list fail due to del");
	}
}

static void init(void)
{
	int i;

	init_lockfree_list_head(&test_head, &test_head_node, &test_tail_node);
	for (i = 0; i < NODES; i++) {
		nodes[i].val = prandom_u32_state(&rnd);
		LOCKFREE_LIST_SAVE_KEY(&nodes[i], node);
	}
}

static int __init lockfree_list_test_init(void)
{
	int i, j;
	cycles_t time1, time2, time;
	struct test_node *temp;

	printk(KERN_ALERT "lockfree_list testing");

	prandom_seed_state(&rnd, 3141592653589793238ULL);
	init();

	time1 = get_cycles();

	for (i = 0; i < PERF_LOOPS; i++) {
		for (j = 0; j < NODES; j++)
			insert(nodes + j, &test_head);
		lockfree_list_for_each_entry(temp, &test_head_node, node){
			if (&temp->node == &test_head_node ||
					&temp->node == &test_tail_node)
				continue;
			pr_info("list : key %lx\n", temp->node.key);
}

		for (j = 0; j < NODES; j++)
			delete(nodes + j, &test_head);
	}

	time2 = get_cycles();
	time = time2 - time1;

	time = div_u64(time, PERF_LOOPS);
	printk(" -> %llu cycles\n", (unsigned long long)time);

	return -EAGAIN; /* Fail will directly unload the module */
}

static void __exit lockfree_list_test_exit(void)
{
	printk(KERN_ALERT "test exit\n");
}

module_init(lockfree_list_test_init)
module_exit(lockfree_list_test_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Joohyun Kyong");
MODULE_DESCRIPTION("Lockfree List test");
