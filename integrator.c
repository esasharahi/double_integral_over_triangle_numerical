// esasharahi@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double A[] =
	{
		0.2955242247147529,
		0.2955242247147529,
		0.2692667193099963,
		0.2692667193099963,
		0.2190863625159820,
		0.2190863625159820,
		0.1494513491505806,
		0.1494513491505806,
		0.0666713443086881,
		0.0666713443086881,
	};
double a[] =
	{
		-0.1488743389816312,
		+0.1488743389816312,
		-0.4333953941292472,
		+0.4333953941292472,
		-0.6794095682990244,
		+0.6794095682990244,
		-0.8650633666889845,
		+0.8650633666889845,
		-0.9739065285171717,
		+0.9739065285171717,
	};

static double **w;
void w_fill (int n)
{
	w = malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++)
	{
		w[i] = malloc(n*sizeof(double*));
		for (int j = 0; j < n; j++)
		{
			w[i][j] = (1 + a[i]) * A[i] * A[j];
		}
	}
}

void free_w (int n)
{
    for (int i = 0; i < n; i++)
    {
        free(w[i]);
    }
    free(w);
    w = NULL;
}

double integrator (double *u, double *v, double f(double x, double y), int n)
{
	double J = (u[1] - u[0]) * (v[2] - v[0]) - (v[1] - v[0]) * (u[2] - u[0]);
	double **alpha, **beta;
	double sum = 0.0;
	alpha = malloc(n * sizeof(double*));
	beta = malloc(n * sizeof(double*));
	for (int i = 0; i < n; i++)
	{
		alpha[i] = malloc(n * sizeof(double*));
		beta[i] = malloc(n * sizeof(double*));
		for (int j = 0; j < n; j++)
		{
			alpha[i][j] = ((u[1] - u[0]) * (1 - a[i]) / 2) + ((u[2] - u[0]) * (1 + a[i]) * (1 - a[j]) / 4) + u[0]; 
			beta[i][j] = ((v[1] - v[0]) * (1 - a[i]) / 2) + ((v[2] - v[0]) * (1 + a[i]) * (1 - a[j]) / 4) + v[0];
			sum += w[i][j] * f(alpha[i][j], beta[i][j]);
		}
        	free(alpha[i]);
        	free(beta[i]);
	}
	sum *= J / 8;
	sum = fabs(sum);
	free(alpha);
	free(beta);
	return sum;
}

double f(double x, double y)
{
	return sin(x + 2 * y);
	//return exp(sin(x) * cos(y));
	//return sqrt(3) * exp((-2/3) * (pow((x - 5)/2, 2) - (x - 5) * (y - 9)/8 + pow((y - 9)/4, 2))) / (24 * M_PI);
	//return (1 / (2 * M_PI)) * exp(-0.5 * (x * x + y * y));
}

int main ()
{
	int n = 10;
	double x[3] = {0, 0, 1};
	double y[3] = {0, 1, 0};
	w_fill(n);
	printf("%f\n", integrator(x, y, *f, n));
	free_w(n);
}
