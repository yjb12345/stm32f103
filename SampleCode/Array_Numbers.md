## Array Numbers

```c
void Task_RS485(void *arguments)
{
  // int numbers[7] = {10, 20, 30, 40, 50, 60, 70};
  int numbers[] = {10, 20, 30, 40, 50, 60, 70};
#if 1
  while (1)
  {
    //for (int i = 0; i < 9; i++)
    for (int i = 0; i < sizeof(numbers) / sizeof(int); i++)
    {
      printf("i=%d  data= %d\n", i, numbers[i]);
      osDelay(100);
    }
    osDelay(1000);
  }
}
#else
#endif
```