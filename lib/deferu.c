#include <linux/deferu.h>

//#define DEFERU_OPT

#ifdef DEFERU_OPT
void optimization_deferu_i_mmap(struct llist *entry,
		struct vm_area_struct *svma)
{
	struct vm_area_struct *dvma;

	llist_for_each_entry(dvma, entry, deferu_node.ll_node) {
		if (dvma->deferu_node.op_num == DEFERU_OP_DEL) {
			if (svma->deferu_node.key == dvma->deferu_node.key)
				break;
			else
				continue;
		}
		/* In case of ADD operation */
		if (dvma->deferu_node.op_num == DEFERU_OP_ADD &&
				svma->deferu_node.key == dvma->deferu_node.key) {
			svma->deferu_node.used = 1;
			dvma->deferu_node.used = 1;
			break;
		}
	}
}

void optimization_work(struct llist *entry,
		struct vm_area_struct *svma)
{
	/* Optimization */
	llist_for_each_entry(vma, entry, deferu_node.ll_node) {
		if (vma->deferu_node.op_num == DEFERU_OP_DEL) {
			optimization_deferu_i_mmap(entry, vma);
		}
	}
}
#endif
