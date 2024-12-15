## Struct Ptr
```c
// struct RS485_DATA
// {
//   uint8_t id;
//   char data[8];
// };

typedef struct RS485_DATA_TAG
{
  uint8_t id;
  char data[8];
}
RS485_DATA;

void Task_RS485(void *arguments);
int send485(char *str);

int send485(char *str)
{
  printf("send485 called data = %s\n\r", str);
};

int recve485();

int recve485()
{
  printf("\trecve484 called\n\r");
};

// void send485_ST(struct RS485_DATA *stData)
void send485_ST(RS485_DATA *stData)
{
  printf("send id= %d  data= %s\n\r", stData->id, stData->data);
};

void Task_RS485(void *arguments)
{
  char *arrStr[] = {"Mod 2", "Mod 5", "Mod 7"};
  // int count = 0x30;
  int count = 48;
  int mod = 0;
  // struct RS485_DATA stData;
  RS485_DATA stData;

  while (1)
  {

    // printf("count=%d\t", count++);

    if (count % 2 == 0)
    {
      stData.id = 0x01;
      strcpy(stData.data, "R10");
    }

    else
    {
      stData.id = 0x02;
      strcpy(stData.data, "R20");
    }

    send485_ST(&stData);

    recve485();
    osDelay(1000);
    count++;
  }
}

```