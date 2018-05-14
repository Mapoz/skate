/*****************************************************************************/
/*  BOARD.H - Header file for board.c                                        */
/*                                                                           */
/*                                                                           */
/*  FUNCTIONS:                                                               */
/*                                                                           */
/*  brd_init()                                                               */
/*  cio_open()                                                               */
/*  cio_close()                                                              */
/*  cio_read()                                                               */
/*  cio_write()                                                              */
/*  brd_fifo_read()                                                          */
/*  brd_fifo_write()                                                         */
/*  brd_led_enable()                                                         */
/*  brd_led_disable()                                                        */
/*                                                                           */
/*                                                                           */
/*  STATIC FUNCTIONS:                                                        */
/*    None                                                                   */
/*                                                                           */
/*                                                                           */
/*  GLOBAL VARIABLES DEFINED                                                 */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#ifndef _BOARD_H
#define _BOARD_H

#include <type.h>

#undef  OK
#define OK                       0

#undef  ERROR
#define ERROR                   -1

//port declarations used to access CPLD registers
volatile ioport unsigned char port0;
volatile ioport unsigned char port1;
volatile ioport unsigned char port2;
volatile ioport unsigned char port3;
volatile ioport unsigned char port4;
volatile ioport unsigned char port5;
volatile ioport unsigned char port6;

/*----------------------------------------------------------------------------*/
/* MACRO DEFINITIONS                                                          */
/*----------------------------------------------------------------------------*/
#define MIN_CPU_FREQ             10
#define MAX_CPU_FREQ             100
#define DELAY_TIMER_PORT         0

#define LED0_MASK                0x1
#define LED1_MASK                0x2
#define LED2_MASK                0x4

#define CPLD_CTRL1_DEFAULT       0x10 //NMIEN=1
#define CPLD_DMCTRL_DEFAULT      0x00
#define CPLD_DBIO_DEFAULT        0x30
#define CPLD_CTRL2_DEFAULT       0x03
#define CPLD_SEM0_DEFAULT        0x00
#define CPLD_SEM1_DEFAULT        0x00

#define CPLD_CTRL1_REG           port0
#define CPLD_STAT_REG            port1
#define CPLD_DMCTRL_REG          port2
#define CPLD_DBIO_REG            port3
#define CPLD_CTRL2_REG           port4
#define CPLD_SEM0_REG            port5
#define CPLD_SEM1_REG            port6

//CPLD register addresses used the C-callable portio routines
//DO NOT use the above defines with the c-callable portio
//routines prototyped in portio.h
typedef enum
{
    CpldCtrl1Reg   = 0x00,
    CpldStatReg    = 0x01,
    CpldDmctrlReg  = 0x02,
    CpldDbioReg    = 0x03,
    CpldCtrl2Reg   = 0x04,
    CpldSem0Reg    = 0x05,
    CpldSem1Reg    = 0x06
} CpldReg, *PCpldReg;

//dsk semaphores
typedef enum
{
    BRD_SEM0,
    BRD_SEM1
} BrdSem;

typedef enum
{
    BRD_HINT,
     BRD_HINT2
} BrdIntr;

typedef enum
{
    BRD_LED0,
    BRD_LED1,
    BRD_LED2
} BrdLed;

typedef void (*Fp)(void);

/*****************************************************************************/
/* FUNCTIONS                                                                 */
/*****************************************************************************/

/******************************************************************************/
/* brd_init(u16 freq) - Initialize DSK board for use                          */
/*                                                                            */
/*  Parameters:                                                               */
/*      - freq: Desired board operating frequency.                            */
/*                                                                            */
/*  Return:                                                                   */
/*  - OK success                                                              */
/*  - ERROR failure                                                           */
/*                                                                            */
/*  Notes:                                                                    */
/*                                                                            */
/*  Board frequencies must be a multiple of 10 with the lowest being 20 MHz   */
/*  and the highest being 100 MHz.  Board frequencies supported are:          */
/*  20,30,40,50,60,70,                                                        */
/*                                                                            */
/******************************************************************************/
s16 brd_init(unsigned int cpuFreq);

/*****************************************************************************/
/*  s16 brd_interrupt_host(EvmInt intr)                                      */
/*                                                                           */
/*  This routine interrupts the host application .                           */
/*                                                                           */
/*  Parameters:                                                              */
/*      - intr: specified interrupt                                          */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_interrupt_host(BrdIntr intr);

/*****************************************************************************/
/*  s16 brd_led_enable(BrdLed led)                                            */
/*                                                                           */
/*  This routine enables the dsk led.             .                          */
/*                                                                           */
/*  Parameters:                                                              */
/*      - led - specific led                                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_led_enable(BrdLed led);

/*****************************************************************************/
/*  s16 brd_led_disable(BrdLed led)                                           */
/*                                                                           */
/*  This routine disables the dsk led.            .                          */
/*                                                                           */
/*  Parameters:                                                              */
/*      - led - specific led                                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_led_disable(BrdLed led);

/*****************************************************************************/
/*  s16 brd_toggle_led(BrdLed led)                                            */
/*                                                                           */
/*  This routine enables the dsk led.             .                          */
/*                                                                           */
/*  Parameters:                                                              */
/*      - led - specific led                                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_led_toggle(BrdLed led);

/*****************************************************************************/
/*  s16 brd_sem_get(void)                                                    */
/*                                                                           */
/*  This routine tries to obtain the specified user semaphore.               */
/*                                                                           */
/*                                                                           */
/*  Parameters:                                                              */
/*      - sem - desired semaphore                                            */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_sem_get(BrdSem sem);

/*****************************************************************************/
/*  s16 brd_sem_wait(void)                                                   */
/*                                                                           */
/*  This routine tries to obtain the specified user semaphore. It blocks till*/
/*  the semaphore is obtained.                                               */
/*                                                                           */
/*  Parameters:                                                              */
/*      - sem - desired semaphore                                            */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_sem_wait(BrdSem sem);

/*****************************************************************************/
/*  s16 brd_sem_release(void)                                                */
/*                                                                           */
/*  This routine releases the previously obtained semaphore.                 */
/*                                                                           */
/*  Parameters:                                                              */
/*      - sem - semaphore to release                                         */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_sem_release(BrdSem sem);

/*****************************************************************************/
/*  s16 brd_fifo_read (s8 *bufptr, u16 cnt)                                  */
/*                                                                           */
/*  This routine begins a synchronous read from the fifo channel.            */
/*                                                                           */
/*  Parameters:                                                              */
/*  - p_buffer is the location of the data to be written to the FIFO         */
/*  - num_bytes is the number of bytes of data to be written to the FIFO,    */
/*                                                                           */
/*  Return:                                                                  */
/*  - returns OK or ERROR,                                                   */
/*      possible error conditions include: another send in progress,         */
/*      chan not open, no DMA chan available                                 */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_fifo_read(char *bufptr, u16 cnt);

/*****************************************************************************/
/*  s16 brd_fifo_write (s8 *bufptr, u16 cnt)                                 */
/*                                                                           */
/*  This routine begins a synchronous read from the fifo channel.            */
/*                                                                           */
/*  Parameters:                                                              */
/*  - p_buffer is the location of the data to be written to the FIFO         */
/*  - num_bytes is the number of bytes of data to be written to the FIFO,    */
/*                                                                           */
/*  Return:                                                                  */
/*  - returns OK or ERROR,                                                   */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_fifo_write(char *bufptr, u16 cnt);

/*****************************************************************************/
/*  s16 brd_delay_msec(u16 msec)                                             */
/*                                                                           */
/*  This routine delays execution by specified milliseconds.                 */
/*                                                                           */
/*  Parameters:                                                              */
/*      - msec - time delay in milliseconds.                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - TRUE success                                                           */
/*  - FALSE failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*  The delay is implemented using timer 0 at port (0x24). Care must be      */
/*  taken not to interrupt timer operation by way of interrupts.             */
/*  Maximum supported delay is 6553 usec or 6.5 secs.                        */
/*                                                                           */
/*****************************************************************************/
s16 brd_delay_msec(u16 msec);

/*****************************************************************************/
/*  s16 brd_delay_usec(u16 usec)                                             */
/*                                                                           */
/*  This routine delays execution by specified microseconds.                 */
/*                                                                           */
/*  Parameters:                                                              */
/*      - usec - time delay in microseconds.                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - TRUE success                                                           */
/*  - FALSE failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*  The delay is implemented using timer 0 at port (0x24). Care must be      */
/*  taken not to interrupt timer operation by way of interrupts.             */
/*  Maximum supported delay is 65536 usec or 65.536 msecs.                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_delay_usec(u16 usec);

/*****************************************************************************/
/*  s16 brd_set_wait_states(u16 flashDbProg,                                 */
/*                          u16 data32_64,                                   */
/*                          u16 io)                                          */
/*                                                                           */
/*  This routine sets the wait states for program, data & I/O spaces         */
/*                                                                           */
/*  Parameters:                                                              */
/*      - extendedProg - dsk onboard external program memory wait states     */
/*      - flashDbProg - flash/daughter board memory wait states              */
/*      - data0_32 - first 32K data memory wait states                       */
/*      - data32_64 - Second 32K data memory wait states                     */
/*      - io - I/O wait states                                               */
/*                                                                           */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*****************************************************************************/
s16 brd_set_wait_states(u16 flashDbProg,
                        u16 data32_64,
                        u16 io);

/*****************************************************************************/
/*   brd_set_cpu_freq()                                                      */
/*                                                                           */
/*  This routine sets the CPU frequency                                      */
/*                                                                           */
/*  Parameters:                                                              */
/*  - freq frequency in MHz to set cpu clock                                 */
/*                                                                           */
/*  Return:                                                                  */
/*  - void                                                                   */
/*                                                                           */
/*  Notes:                                                                   */
/*                                                                           */
/*  Board frequencies must be a multiple of 10 with the lowest being 20 MHz  */
/*  and the highest being 100 MHz.  Board frequencies supported are:         */
/*  20,30,40,50,60,70,                                                       */
/*                                                                           */
/*****************************************************************************/
void brd_set_cpu_freq(u16 freq);

/*****************************************************************************/
/*   brd_get_cpu_freq()                                                      */
/*                                                                           */
/*  This routine returns the CPU frequency                                   */
/*                                                                           */
/*  Parameters:                                                              */
/*  - None                                                                   */
/*                                                                           */
/*  Return:                                                                  */
/*  - void                                                                   */
/*                                                                           */
/*  Notes:                                                                   */
/*****************************************************************************/
u16 brd_get_cpu_freq();

/*****************************************************************************/
/*   brd_io_redir()                                                          */
/*                                                                           */
/*  This routine redirects printfs to the host terminal.                     */
/*                                                                           */
/*  Parameters:                                                              */
/*  - None                                                                   */
/*                                                                           */
/*  Return:                                                                  */
/*  - void                                                                   */
/*                                                                           */
/*  Notes:                                                                   */
/*****************************************************************************/
void brd_io_redir(BOOL enable);

/*****************************************************************************/
/*  s16 brd_mem_transfer()                                                   */
/*                                                                           */
/*  This routine uses the DMA to transfer data from one mem location to the  */
/*  the other.                                                               */
/*                                                                           */
/*  Parameters:                                                              */
/*      - ch : DMA channel to use.                                           */
/*      - src_addr - source address.                                         */
/*      - src_space - source memory space. Values in dma5410.h               */
/*      - dst_addr - destination address.                                    */
/*      - dst_space - destination memory space. Values in dma5410.h          */
/*      - cnt - number of words to transfer.                                 */
/*      - sync - wait for transfer to complete.                              */
/*  Return:                                                                  */
/*  - OK success                                                             */
/*  - ERROR failure                                                          */
/*                                                                           */
/*  Notes:                                                                   */
/*  - If channel is busy call waits for channel to be available.             */
/*                                                                           */
/*****************************************************************************/
s16 brd_mem_transfer(u16 ch, u16 *src_addr, u16 src_space,
                             u16 *dst_addr, u16 dst_space, u16 cnt, BOOL sync);

/*****************************************************************************/
/*  s16 cio_open(const char *path, unsigned flags, s16 mode)                 */
/*                                                                           */
/*  This routine opens the I/O channel to the host.                          */
/*                                                                           */
/*  Parameters:                                                              */
/*  - None.                                                                  */
/*                                                                           */
/*  Return:                                                                  */
/*  - a stream number on success or 0 on failure                             */
/*                                                                           */
/*  Notes:                                                                   */
/*  - only one open stream is supported                                      */
/*                                                                           */
/*****************************************************************************/
s16 cio_open();

/*****************************************************************************/
/*  s16 cio_read(s16 fildes, char *bufptr, unsigned cnt)                     */
/*                                                                           */
/*  This routine reads 'cnt' bytes of data from the host.                    */
/*                                                                           */
/*  Parameters:                                                              */
/*  - fildes parameter is the file descriptor returned from a successful     */
/*      open call                                                            */
/*  - bufptr parameter is the location of the buffer where read data         */
/*      is placed.                                                           */
/*  - cnt parameter is the number of characters to be read from the host.    */
/*                                                                           */
/*  Return:                                                                  */
/*  - the number of characters read or -1 on failure                         */
/*                                                                           */
/*  Notes:                                                                   */
/*****************************************************************************/
s16 cio_read(int fildes, char *bufptr, unsigned int cnt);

/*****************************************************************************/
/*  s16 cio_write(s16 fildes, char *bufptr, unsigned cnt)                    */
/*                                                                           */
/*  This routine writes 'cnt' bytes of data to the host.                     */
/*                                                                           */
/*  Parameters:                                                              */
/*  - fildes parameter is the file descriptor returned from a successful     */
/*      open call                                                            */
/*  - bufptr parameter is the location of the buffer where data is read from */
/*                                                                           */
/*  - cnt parameter is the number of characters to write to the host.        */
/*                                                                           */
/*  Return:                                                                  */
/*  - the number of characters written or -1 on failure                      */
/*                                                                           */
/*  Notes:                                                                   */
/*****************************************************************************/
s16 cio_write(int fildes, char *bufptr, unsigned int cnt);

/*****************************************************************************/
/*  s16 cio_close(s16 fildes)                                                */
/*                                                                           */
/*  This routine closes the FIFO device.                                     */
/*                                                                           */
/*  Parameters:                                                              */
/*  - fildes parameter is the file descriptor returned from a successful     */
/*      open call                                                            */
/*                                                                           */
/*  Return:                                                                  */
/*  - returns 0 on success or -1 on failure                                  */
/*                                                                           */
/*  Notes:                                                                   */
/*  - this function will utilize the pci_fifo_close routine                  */
/*                                                                           */
/*****************************************************************************/
s16 cio_close(int fildes);

#endif /* end of #ifndef _BOARD_H */

