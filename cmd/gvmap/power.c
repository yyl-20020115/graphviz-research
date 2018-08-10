/* $Id$Revision: */
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

#include "power.h"
#include "SparseMatrix.h"

void power_method(void (*matvec)(void *, int, int, real*, real **, int, int*),
		  void *A, int n, int K, int random_seed, int maxit, real tol, real **eigv, real **eigs){

  real **v, *u, *vv;
  int iter = 0;
  real res, unorm;
  int i, j, k;
  real uij;
  int flag;

  K = MAX(0, MIN(n, K));
  assert(K <= n && K > 0);

  if (!(*eigv)) *eigv = MALLOC(sizeof(real)*n*K);
  if (!(*eigs)) *eigs = MALLOC(sizeof(real)*K);
  v = MALLOC(sizeof(real*)*K);

  vv = MALLOC(sizeof(real)*n);
  u = MALLOC(sizeof(real)*n);

  srand(random_seed);

  for (k = 0; k < K; k++){
    //fprintf(stderr,"calculating eig k ==================== %d\n",k);
    v[k] = &((*eigv)[k*n]);
    for (i = 0; i < n; i++) u[i] = drand();
    res = sqrt(vector_product(n, u, u));
    if (res > 0) res =  1/res;
    for (i = 0; i < n; i++) {
      u[i] = u[i]*res;
      v[k][i] = u[i];
    }
    /*
    fprintf(stderr,"initial vec=");
    for (i = 0; i < n; i++) fprintf(stderr,"%f,",u[i]);fprintf(stderr,"\n");
    */
    iter = 0;
    do {


      /* normalize against previous eigens */
      for (j = 0; j < k; j++){
	uij = vector_product(n, u, v[j]);
	for (i = 0; i < n; i++) {
	  u[i] = u[i] - uij *v[j][i];
	}
      }
      matvec(A, n, n, u, &vv, FALSE, &flag);
      assert(!flag);

      /*
      fprintf(stderr,"normalized against prev vec=");
      for (i = 0; i < n; i++) fprintf(stderr,"%f,",u[i]);fprintf(stderr,"\n");
      */

      unorm = vector_product(n, vv, vv);/* ||u||^2 */    
      unorm = sqrt(unorm);
      (*eigs)[k] = unorm;
      if (unorm > 0) {
	unorm = 1/unorm;
      } else {
	// ||A.v||=0, so v must be an eigenvec correspond to eigenvalue zero
	for (i = 0; i < n; i++) vv[i] = u[i];
	unorm = sqrt(vector_product(n, vv, vv));
	if (unorm > 0) unorm = 1/unorm;
      }
      res = 0.;

      for (i = 0; i < n; i++) {
	//res = MAX(res, ABS(vv[i]-(*eigs)[k]*u[i]));
	u[i] = vv[i]*unorm;
	res = res + u[i]*v[k][i];
	v[k][i] = u[i];
      }
      //fprintf(stderr,"res=%g, tol = %g, res < 1-tol=%d\n",res, tol,res < 1 - tol);
    } while (res < 1 - tol && iter++ < maxit);
    //} while (iter++ < maxit);
    //fprintf(stderr,"iter= %d, res=%f\n",iter, res);
  }
  FREE(u);
  FREE(vv);  
}


void matvec_sparse(void *M, int m, int n, real *u, real **v, int transpose,
		   int *flag){
  SparseMatrix A;
  m;
  n;
  *flag = 0;
  A = (SparseMatrix) M;
  SparseMatrix_multiply_vector(A, u, v, transpose);
  return;
}

void mat_print_dense(real *M, int m, int n){
  int i, j;
  fprintf(stderr,"{");
  for (i = 0; i < m; i++){
    fprintf(stderr,"{");
    for (j = 0; j < n; j++){
      if (j != 0) fprintf(stderr,",");
      fprintf(stderr,"%f",M[i*n+j]);
    }
    if (i != m-1){
      fprintf(stderr,"},\n");
    } else {
      fprintf(stderr,"}");
    }
  }
  fprintf(stderr,"}\n");
}

void matvec_dense(void *M, int m, int n, real *u, real **v, int transpose,
		  int *flag){
  /* M.u or M^T.u */
  real *A;
  int i, j;

  A = (real*) M;
  *flag = 0;
  

  if (!transpose){
    if (!(*v)) *v = MALLOC(sizeof(real)*m);
    for (i = 0; i < m; i++){
      (*v)[i] = 0;
      for (j = 0; j < n; j++){
	(*v)[i] += A[i*n+j]*u[j];
      }
    }
  } else {
    if (!(*v)) *v = MALLOC(sizeof(real)*n);
    for (i = 0; i < n; i++){
      (*v)[i] = 0;
    }
    for (i = 0; i < m; i++){
      for (j = 0; j < n; j++){
	(*v)[j] += A[i*n+j]*u[i];
      }
    }
  }

  return;
}
