#ifndef __FILTER_NEW_MEAN__
#define __FILTER_NEW_MEAN__

int filterMean()
{
  int sum = 0;
  for (int i = 0; i < DATA_SIZE; ++i)
  {
    sum += data[i];  
  }
  return sum / DATA_SIZE;
} 
#endif

