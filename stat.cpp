#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

double distance(int *a, int *b, int N)
{
	double sum = 0;
	for(int i = 0; i < N; i++)
	{
		//sum += 0.5*(a[i]*(log(2*a[i])-log(a[i]+b[i]))+b[i]*(log(2*b[i])-log(a[i]+b[i])));
		sum += a[i]*(log(a[i])-log(b[i]));
	}
	return sum;
}

double mean(double *a, int N)
{
	double sum = 0;
	for(int i = 0; i < N; i++)
		sum += a[i];
	return sum/N;
}

double var(double *a, int N)
{
	double sum = 0;
	double m = mean(a,N);
	for(int i = 0; i < N; i++)
		sum += (a[i]-m)*(a[i]-m);
	return sqrt(sum/N);
}

int main()
{
	srand((unsigned)time(0));
	const int N = 90, n = 800, C = 1000;
	int bin1[N],bin2[N];
	double dist[C];
	for(int c = 0; c < C; c++)
	{
		for(int i = 0; i < N; i++)
			bin1[N] = bin2[N] = 0;
		for(int i = 0; i < n; i++)
		{
			bin1[rand()%N]++;
			bin2[rand()%N]++;
		}
		dist[c] = distance(bin1,bin2,N);
	}
	printf("%g %g\n",mean(dist,C),var(dist,C));
	for(int c = 0; c < C; c++)
	{
		int offset = rand()%17-8;
		for(int i = 0; i < N; i++)
			bin1[N] = bin2[N] = 0;
		for(int i = 0; i < n/5; i++)
		{
			int k = rand()%N;
			bin1[k]++;
			bin2[(k+offset+N+rand()%3-1)%N]++;
		}
		for(int i = n/5; i < n; i++)
		{
			bin1[rand()%N]++;
			bin2[rand()%N]++;
		}
		dist[c] = distance(bin1,bin2,N);
	}
	printf("%g %g\n",mean(dist,C),var(dist,C));
	return 0;
}
