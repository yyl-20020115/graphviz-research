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

#ifndef DOTPROCS_H
#define DOTPROCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "aspect.h"
	extern void dot_layout(Agraph_t * g);

    extern void acyclic(graph_t *);
    extern void allocate_ranks(graph_t *);
    extern void build_ranks(graph_t *, int);
    extern void build_skeleton(graph_t *, graph_t *);
    extern void checkLabelOrder (graph_t* g);
    extern void class1(graph_t *);
    extern void class2(graph_t *);
    extern void decompose(graph_t *, int);
    extern void delete_fast_edge(edge_t *);
    extern void delete_fast_node(graph_t *, node_t *);
    extern void delete_flat_edge(edge_t *);
    extern void dot_cleanup(graph_t * g);
    extern void dot_init_node_edge(graph_t * g);
    extern void dot_scan_ranks(graph_t * g);
    extern void enqueue_neighbors(nodequeue * q, node_t * n0, int pass);
    extern void expand_cluster(graph_t *);
    extern edge_t *fast_edge(edge_t *);
    extern void fast_node(graph_t *, node_t *);
    extern void fast_nodeapp(node_t *, node_t *);
    extern edge_t *find_fast_edge(node_t *, node_t *);
    extern edge_t *find_flat_edge(node_t *, node_t *);
    extern void flat_edge(graph_t *, edge_t *);
    extern int flat_edges(graph_t *);
    extern void install_cluster(graph_t *, node_t *, int, nodequeue *);
    extern void install_in_rank(graph_t *, node_t *);
    extern int is_cluster(graph_t *);
    extern void dot_compoundEdges(graph_t *);
    extern edge_t *make_aux_edge(node_t *, node_t *, double, int);
    extern void mark_clusters(graph_t *);
    extern void mark_lowclusters(graph_t *);
    extern int mergeable(edge_t * e, edge_t * f);
    extern void merge_chain(graph_t *, edge_t *, edge_t *, int);
    extern void merge_oneway(edge_t *, edge_t *);
    extern int ncross(graph_t *);
    extern edge_t *new_virtual_edge(node_t *, node_t *, edge_t *);
    extern int nonconstraint_edge(edge_t *);
    extern void other_edge(edge_t *);
    extern void rank1(graph_t * g);
    extern int portcmp(port p0, port p1);
    extern int ports_eq(edge_t *, edge_t *);
    extern void rec_reset_vlists(graph_t *);
    extern void rec_save_vlists(graph_t *);
    extern void reverse_edge(edge_t *);
    extern void safe_other_edge(edge_t *);
    extern void save_vlist(graph_t *);
    extern void unmerge_oneway(edge_t *);
    extern edge_t *virtual_edge(node_t *, node_t *, edge_t *);
    extern node_t *virtual_node(graph_t *);
    extern void virtual_weight(edge_t *);
    extern void zapinlist(elist *, edge_t *);

#if defined(_BLD_dot) && defined(_DLL)
#   define extern __EXPORT__
#endif
    extern graph_t* dot_root(void *);
    extern void dot_concentrate(graph_t *);
    extern void dot_mincross(graph_t *, int);
    extern void dot_position(graph_t *, aspect_t*);
    extern void dot_rank(graph_t *, aspect_t*);
    extern void dot_sameports(graph_t *);
    extern void dot_splines(graph_t *);
#undef extern

#ifdef __cplusplus
}
#endif
#endif
