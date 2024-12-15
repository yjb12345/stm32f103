## ASCII 출력
```c
void Task_RS485(void *arguments);
void Task_RS485(void *arguments)
{
 char *arrStr[]={"Mod 2","Mod 5","Mod 7"}; 
  //int count = 0x30;
  int count = 48;
  int mod = 0;

  while(1){
    
    printf("count=%d\t", count++);
#if 0    
    if(count%2 ==0)  printf("%s = 0 \n\r",arrStr[0] );
    if(count%5 ==0)  printf("%s = 0 \n\r", arrStr[1]);
    if(count%6 ==0)  printf("\t%s = 0 \n\r", arrStr[2]);
#else
    printf("%X %c \n\r", count,count);
#endif
    osDelay(1000);
  }
}
```
## 출력


```text
count=48	31 1 
count=49	32 2 
count=50	33 3 
count=51	34 4 
count=52	35 5 
count=53	36 6 
count=54	37 7 
count=55	38 8 
count=56	39 9 
count=57	3A : 
count=58	3B ; 
count=59	3C < 
count=60	3D = 
count=61	3E > 
count=62	3F ? 
count=63	40 @ 
count=64	41 A 
count=65	42 B 
count=66	43 C 
count=67	44 D 
count=68	45 E 
count=69	46 F 
count=70	47 G 
count=71	48 H 
count=72	49 I 
count=73	4A J 
count=74	4B K 
count=75	4C L 
count=76	4D M 
count=77	4E N 
count=78	4F O 
count=79	50 P 
count=80	51 Q 
count=81	52 R 
count=82	53 S 
count=83	54 T 
```