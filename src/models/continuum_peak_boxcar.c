/* Copyright 2010-2013 Christopher Waters and Julie Hollek */
#include "../robo.h"

void generate_model_continuum_peak_boxcar(opts *options, data *D, model *M) {
  int i,j;
  stats *S1 = 0;
  double *v1 = malloc(sizeof(double) * D->N);
  int n1;

  for (i = 0; i < D->N; i += 1) {
    n1 = 0;
    for (j = 0; j < D->N; j++) {
      if ((D->x[j] > D->x[i] - options->continuum_box)&&
	  (D->x[j] < D->x[i] + options->continuum_box)) { /* Inside the box to consider */
	if (
	    ((j == 0)&&(D->y[j] > D->y[j+1]))|| /* at left edge and higher than neighbor */
	    ((j == D->N - 1)&&(D->y[j] > D->y[j-1] ))|| /* at right edge and higher than neighbor */
	    ((j != 0)&&(j != D->N - 1)&& /* not at an edge */
	     (D->y[j]  > D->y[j-1])&& /* higher than left neighbor */
	     (D->y[j]  > D->y[j+1]))) { /* highter than right neighbor */
	  v1[n1] = (D->y[j]); /* Append to test array */
	  n1++;
	}
      }
      if (D->x[j] > D->x[i] + options->continuum_box) { /* skip out if we've covered the box */
	j = D->N + 10;
      }
    }
    if (n1 > 0) { /* If this sample has more than zero values */
      S1 = array_stats(v1,n1);   /* Do stats and take teh median */
      M->continuum[i] = S1->med;
      free(S1);
    }
    else {
      M->continuum[i] = M->continuum[i-1];  /* Else take the last value and assume continuity */
    }    

    log_comment(options,ROBO_VERBOSE_CONTINUUM,
		"gmcPb: (%d/%d) (%f %f %f) (%f %f %ld) %f",
		i,D->N,
		D->x[i],D->y[i],D->e[i],
		S1->med,1.4826 * S1->MAD,S1->N,
		M->continuum[i]
		);
  }
  free(v1);
}
