// SPDX-FileCopyrightText: 2013 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
//
// SPDX-License-Identifier: MIT

/*
 * This example shows how to push nodes into a wfstack.
 */

#include <stdio.h>
#include <stdlib.h>

#include <urcu/wfstack.h>	/* Wait-free stack */
#include <urcu/compiler.h>	/* For CAA_ARRAY_SIZE */

/*
 * Nodes populated into the stack.
 */
struct mynode {
	int value;			/* Node content */
	struct cds_wfs_node node;	/* Chaining in stack */
};

int main(void)
{
	int values[] = { -5, 42, 36, 24, };
	struct cds_wfs_stack mystack;	/* Stack */
	unsigned int i;
	int ret = 0;
	struct cds_wfs_node *snode;
	struct cds_wfs_head *shead;

	cds_wfs_init(&mystack);

	/*
	 * Push nodes.
	 */
	for (i = 0; i < CAA_ARRAY_SIZE(values); i++) {
		struct mynode *node;

		node = malloc(sizeof(*node));
		if (!node) {
			ret = -1;
			goto end;
		}

		cds_wfs_node_init(&node->node);
		node->value = values[i];
		cds_wfs_push(&mystack, &node->node);
	}

	/*
	 * Show the stack content, iterate in reverse order of push,
	 * from newest to oldest.
	 */
	printf("mystack content:");
	shead = cds_wfs_pop_all_blocking(&mystack);
	cds_wfs_for_each_blocking(shead, snode) {
		struct mynode *node =
			caa_container_of(snode, struct mynode, node);
		printf(" %d", node->value);
	}
	printf("\n");
end:
	cds_wfs_destroy(&mystack);
	return ret;
}
