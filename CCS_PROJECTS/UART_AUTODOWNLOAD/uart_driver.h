
#ifndef uart_driver_h
#define uart_driver_h


void UARTInit(void);
void PutChar(char data);
void PutData(char *data, uint8_t len);

#endif
