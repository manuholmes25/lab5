#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#include "utils/uartstdio.c"
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif


char data[100];
int main(void)
{
    SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE,0X03);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, 0x01);
    
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //
    UARTStdioConfig(0,9600,120000000);
    // Loop forever.
    //
    while(1)
    {
    if (UARTCharsAvail(UART0_BASE))
        {
            //UARTgets(data, sizeof(data));
            //data[sizeof(data) - 1] = '\0'; 
            memset(data, 0, sizeof(data));//limpieza del data
            UARTgets(data, sizeof(data));

            if (strncmp(data, "buzzer", 6) == 0) 
            {
                GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
                SysCtlDelay(2 * (120000000 / 3)); 
                GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
                GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
            }

            data[0] = '\0'; 
        }
    if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0){
          UARTprintf("MOTOR1 \n");
    }
    if (GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_1) == 0){
          UARTprintf("MOTOR2 \n");
    }
    }
}
