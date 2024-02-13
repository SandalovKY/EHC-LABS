#include <iostream>			
#include <random>
#include <chrono>
using namespace std;

/******* Controls *******/
#define PADDED
/************************/

#define TYPE float
const int CALL_COUNT = 100000000;
const int WIDTH = 35;

#ifdef PADDED
const int PADDED_WIDTH = WIDTH + ((32 - ((WIDTH * sizeof(TYPE)) % 32)) / sizeof(TYPE));
#else 
const int PADDED_WIDTH = WIDTH;
#endif

TYPE __attribute__((aligned(32))) table[PADDED_WIDTH];


void setup (TYPE table[PADDED_WIDTH]);

int main ()
{
	setup(table);
	auto start = chrono::high_resolution_clock::now();
	#pragma novector
	for (int x = 0; x < CALL_COUNT; x++)
	{
		#pragma nounroll
		#pragma vector
		for (int j = 0; j < PADDED_WIDTH; j++)
		{
			table[j] = (table[j] / 5.3f) * (table[j]  * table[j] + table[j]) - (12.5f / (table[j] + 0.3f))
								 + (table[j] / (14.3f / (table[j] + 1.4f))) - (table[j] / 23.0f) + (14.8f / (2.4f + table[j]));
		}
	}
	auto stop = chrono::high_resolution_clock::now();

	cout << "execution time " <<  (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << " mseconds\n";

	start = chrono::high_resolution_clock::now();
	#pragma novector
	for (int x = 0; x < CALL_COUNT; x++)
	{
		#pragma unroll(4)
		#pragma vector
		for (int j = 0; j < PADDED_WIDTH; j++)
		{
			table[j] = (table[j] / 5.3f) * (table[j]  * table[j] + table[j]) - (12.5f / (table[j] + 0.3f))
								 + (table[j] / (14.3f / (table[j] + 1.4f))) - (table[j] / 23.0f) + (14.8f / (2.4f + table[j]));
		}
	}
	stop = chrono::high_resolution_clock::now();

	cout << "execution time for unrolled loop " <<  (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << " mseconds\n";

	return EXIT_SUCCESS;
}

void setup (TYPE table[PADDED_WIDTH])
{
	for (int w = 0; w < WIDTH; w++)
	{
		table[w] = rand() % 10000 / 3;
	}

	#ifdef PADDED
	for (int w = WIDTH; w < PADDED_WIDTH; w++)
	{
		table[w] = 0;
	}
	#endif
}


