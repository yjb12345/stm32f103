## Bit Operator

```c
void bitOperator()
{
  unsigned char a = 60; // 이진수: 0011 1100
  unsigned char b = 13; // 이진수: 0000 1101

  printf("a = %02X\n", a);
  printf("b = %02X\n", b);

  // & (AND) 연산자
  printf("\n& Operator:\n");
  printf("%02X\n", a & b); // 결과: 0000 1100 (12)

  // | (OR) 연산자
  printf("\n| Operator:\n");
  printf("%02X\n", a | b); // 결과: 0011 1101 (61)

  // ^ (XOR) 연산자
  printf("\n^ Operator:\n");
  printf("%02X\n", a ^ b); // 결과: 0011 0001 (49)

  // ~ (NOT) 연산자
  printf("\n~ Operator:\n");
  printf("~a = %02X\n", ~a); // 결과: 1100 0011

  // << (왼쪽 시프트) 연산자
  printf("\n<< Operator:\n");
  printf("a << 2 = %02X\n", a << 2); // 결과: 1111 0000

  // >> (오른쪽 시프트) 연산자
  printf("\n>> Operator:\n");
  printf("a >> 2 = %02X\n", a >> 2); // 결과: 0000 1111
/*
a = 3C             // 0011 1100
b = 0D             // 0000 1101

AND = a & b : 0x0C     // 0000 1100
OR  = a | b : 0x3D     // 0011 1101
XOR = a ^ b : 0x31     // 0011 0001
NOT =  ~a   : 0xC3     // 1100 0011
L-SHIFT = a << 2 : 0xF0 // 1111 0000
R-SHIFT = a >> 2 : 0x0F
  return 0;
}
```

## 결과정리
- a = 0x3C
- b = 0x0D

| Name | Operator | 연산 | 연산 결과(HEX) | 연산 결과(Bits) | 
|:---:|:---:|:---:|:---:|:---:|
| AND | & | a & b | 0x0C | 0000 1100|
| OR | \| | a \| b | 0x3D | 0011 1101|
| XOR | ^ | a ^ b | 0x31 | 0011 0001|
| NOT | ~ | ~a | 0xC3 | 1100 0011|
| L-SHITFT | << | a << 2 | 0xF0 | 1111 0000|
| R-SHITFT | >> | a >> 2 | 0x0F | 0000 1111|

## Bit Operator2

```c
void bitOperator2();
void bitOperator2()
{
  unsigned char a = 0X01; // 이진수: 0000 0001

  printf("a = %02X\n\r", a);

  // 0X00 0X01 0X02 0X03    0x04 0x05 0x06 0x07

  printf("\n\r& Operator: ");
  for (int i = 0; i < 8; i++)
    printf(" %02x", a & i); // 0x00 0x01 0x00 0x01     0x00 0x01 0x00 0x01

  printf(" \n\r| Operator: ");
  for (int i = 0; i < 8; i++)
    printf(" %02x", a | i); // 0x01 0x01 0x03 0x03     0x05 0x05 0x07 0x07

  printf("\n\r^ Operator:");
  for (int i = 0; i < 8; i++)
    printf(" %02x", a ^ i); // 0x01 0x00 0x03 0x02     0x05 0x04 0x07 0x06

  printf("\n\r~ Operator:");
  for (int i = 0; i < 8; i++)
    printf(" %02x", (uint8_t)~a); // 0xFE 0xFE 0xFE 0xFE     0xFE 0xFE 0xFE 0xFE

  printf("\n\r<< Operator:");
  for (int i = 0; i < 8; i++)
    printf(" %02x", a << i); // 0x01 0x02 0x04 0x08  0x10 0x20 0x40 0x80

  printf("\n\r>> Operator:");
  for (int i = 0; i < 8; i++)            // 1111 1110 = 0xFE
    printf(" %02x", (uint8_t)(~a) >> i); // 0xFE 0x7F 0x3F 0x1F   0x0F 0x07 0x03 0x01
  /*
    1111 1110 = >>0  1111 1110
    1111 1110 = >>1  0111 1111
    1111 1110 = >>2  0011 1111
    1111 1110 = >>3  0001 1111
    1111 1110 = >>4  0000 1111
    1111 1110 = >>5  0000 0111
    1111 1110 = >>6  0000 0011
    1111 1110 = >>7  0000 0001
    */
}
```


```c
void readHodlingregister(uint16_t addrStart, uint16_t addrEnd);
void readHodlingregister(uint16_t addrStart, uint16_t addrEnd)
{
#if 1
  uint8_t txBuf[8];
  txBuf[0] = 0x11; // slave address
  txBuf[1] = 3;    // FC

  txBuf[2] = (uint8_t)(addrStart & 0xFF); // Start Addr Hi
  txBuf[3] = (uint8_t)(addrStart >> 8);   // Start Addr Lo, actual addr = 40001
  txBuf[4] = (uint8_t)(addrEnd & 0xFF);   // End Addr Hi
  txBuf[5] = (uint8_t)(addrEnd >> 8);     // End Addr Lo

  uint16_t crc = crc16(txBuf, 6);
  txBuf[6] = crc & 0xFF;
  txBuf[7] = (crc >> 8) & 0xFF;

  printf("----Start= %04X End= %04X\n\r", addrStart, addrEnd);
  for (int i = 0; i < sizeof(txBuf); i++)
    printf(" %02x", txBuf[i]);

  printf("\n\r");
  for (int i = 0; i < sizeof(txBuf); i++)
  {

    for (int j = 0; j < 8; j++)
    {
    	// 1000 0000
    	// 7 6 5 4 3 2 1 0
    	uint8_t bit=  (txBuf[i] << j) & 0x80;
        printf(" %d", bit==0x80 ? 1 : 0);
    }
  }
  printf("\n\r");
#endif
}
```