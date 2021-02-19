float calculateNewEf(float oldEf, unsigned int q)
{
    float newEf = oldEf - 0.8+0.28*q-0.02*q*q; //   (0.1-(5-q)*(0.08+(5-q)*0.02));

    if(newEf < 1.3)
    {
        newEf = 1.3;
    }

    if(newEf > 2.5)
    {
        newEf = 2.5;
    }

    return newEf;
}