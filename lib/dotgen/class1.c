/* $Id$ $Revision$ */
/* vim:set shiftwidth=4 ts=8: */

/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: See CVS logs. Details at http://www.graphviz.org/
 *************************************************************************/


 /*
  * Classify edges for rank assignment phase to
  * create temporary edges.
  */

#include "dot.h"


int nonconstraint_edge(edge_t * e)
{
	char *constr;

	if (E_constr && (constr = agxget(e, E_constr)) != 0) {
		if (constr[0] && mapbool(constr) == FALSE)
			return TRUE;
	}
	return FALSE;
}

static void
interclust1(graph_t * g, node_t * t, node_t * h, edge_t * e)
{
	node_t *v, *t0, *h0;
	int offset, t_len, h_len, t_rank, h_rank;
	edge_t *rt, *rh;

	if (ND_clust(agtail(e)))
		t_rank = ND_rank(agtail(e)) - ND_rank(GD_leader(ND_clust(agtail(e))));
	else
		t_rank = 0;
	if (ND_clust(aghead(e)))
		h_rank = ND_rank(aghead(e)) - ND_rank(GD_leader(ND_clust(aghead(e))));
	else
		h_rank = 0;
	offset = ED_minlen(e) + t_rank - h_rank;
	if (offset > 0) {
		t_len = 0;
		h_len = offset;
	}
	else {
		t_len = -offset;
		h_len = 0;
	}

	v = virtual_node(g);
	ND_node_type(v) = SLACKNODE;
	t0 = UF_find(t);
	h0 = UF_find(h);
	rt = make_aux_edge(v, t0, t_len, CL_BACK * ED_weight(e));
	rh = make_aux_edge(v, h0, h_len, ED_weight(e));
	ED_to_orig(rt) = ED_to_orig(rh) = e;
}
char* translate_type(int type) {
	switch (type)
	{
	case NORMAL:
		return "normal";
	case VIRTUAL:
		return "virtual";
	case SLACKNODE:
		return "slack";
	case REVERSED:
		return "reversed";
	case FLATORDER:
		return "flatorder";
	case CLUSTER_EDGE:
		return "clusteredge";
	case IGNORED:
		return "ignored";
	default:
		return "unknown";
	}
}

void printEdge(edge_t* edge, char* buffer, int length) {
	node_t* head = aghead(edge);
	node_t* tail = agtail(edge);

	textlabel_t* label = 0;

	int n = snprintf(buffer, length, "[");
	length -= n;
	buffer += n;
	label = ND_label(tail);
	if (label != 0) {
		n = snprintf(buffer, length, "(%s)", label->text);
	}
	else {
		n = snprintf(buffer, length, "(%d:%s)", ND_id(tail), translate_type(ND_node_type(tail)));
	}
	length -= n;
	buffer += n;

	n = snprintf(buffer, length, "->");
	length -= n;
	buffer += n;

	label = ND_label(head);
	if (label != 0) {
		n = snprintf(buffer, length, "(%s)", label->text);
	}
	else {
		n = snprintf(buffer, length, "(%d:%s)", ND_id(head), translate_type(ND_node_type(head)));
	}
	length -= n;
	buffer += n;

	n = snprintf(buffer, length, "]");
	length -= n;
	buffer += n;

}
void class1(graph_t * g)
{
	node_t *n, *t, *h;
	edge_t *e, *rep;
	char display_e[256] = { 0 };
	char display_f[256] = { 0 };
	mark_clusters(g);//cluster of nodes are removed and nothing elses


	for (n = agfstnode(g); n; n = agnxtnode(g, n)) {
		for (e = agfstout(g, n); e; e = agnxtout(g, e)) {
			memset(display_e, 0, sizeof(display_e));
			memset(display_f, 0, sizeof(display_f));

			printEdge(e, display_e, sizeof(display_e));

			if (ED_to_virt(e))
				continue;

			/* skip edges that we want to ignore in this phase */
			if (nonconstraint_edge(e))
				continue;

			t = UF_find(agtail(e));
			h = UF_find(aghead(e));

			/* skip self, flat, and intra-cluster edges */
			if (t == h)
				continue;


			/* inter-cluster edges require special treatment */
			if (ND_clust(t) || ND_clust(h)) {
				interclust1(g, agtail(e), aghead(e), e);
				continue;
			}

			if ((rep = find_fast_edge(t, h)) != 0)
				merge_oneway(e, rep);
			else
				virtual_edge(t, h, e);
		}
	}
}

