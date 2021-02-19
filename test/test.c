#include <stdio.h>

#include <memAlgo.h>


int main()
{
    printf("\n\nTEST\n\n");

    float oldEf = 1.3;
    float newEf = 0.0;

    newEf = calculateNewEf(oldEf, 4);

    printf("Old ef=%f newEf=%f", oldEf, newEf);

    return 0;
 
}