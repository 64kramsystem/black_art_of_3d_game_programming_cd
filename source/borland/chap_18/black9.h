
// D E F I N E S  ////////////////////////////////////////////////////////////

// modem responses

#define MODEM_USER_ABORT      -1

#define MODEM_OK              0
#define MODEM_CONNECT         1
#define MODEM_RING            2
#define MODEM_NO_CARRIER      3
#define MODEM_ERROR           4
#define MODEM_CONNECT_1200    5
#define MODEM_NO_DIALTONE     6
#define MODEM_BUSY            7
#define MODEM_NO_ANSWER       8
#define MODEM_CONNECT_0600    9
#define MODEM_CONNECT_2400    10
#define MODEM_CARRIER_2400    11
#define MODEM_CONNECT_4800    12
#define MODEM_CONNECT_9600    13

#define NUM_MODEM_RESPONSES   14

// standard delay times

#define DELAY_1_SECOND  18
#define DELAY_2_SECOND  38
#define DELAY_3_SECOND  48

// registers indices in 16450/16550 UART

#define SERIAL_RBF        0    // the read buffer
#define SERIAL_THR        0    // the write buffer
#define SERIAL_IER        1    // the int. enable register
#define SERIAL_IIR        2    // the int. identification register
#define SERIAL_LCR        3    // control data config. and divisor latch
#define SERIAL_MCR        4    // modem control reg.
#define SERIAL_LSR        5    // line status reg.
#define SERIAL_MSR        6    // modem status of cts, ring etc.
#define SERIAL_DLL        0    // the low byte of baud rate divisor
#define SERIAL_DLH        1    // the hi byte of divisor latch

// baud rate divisors

#define SERIAL_BAUD_1200  96     // baud rate divisors for 1200 baud - 19200
#define SERIAL_BAUD_2400  48
#define SERIAL_BAUD_4800  24
#define SERIAL_BAUD_9600  12
#define SERIAL_BAUD_19200  6

// bit masks for different comm settings

#define SERIAL_STOP_1      0     // 1 stop bit per character
#define SERIAL_STOP_2      4     // 2 stop bits per character

#define SERIAL_BITS_5      0     // send 5 bit characters
#define SERIAL_BITS_6      1     // send 6 bit characters
#define SERIAL_BITS_7      2     // send 7 bit characters
#define SERIAL_BITS_8      3     // send 8 bit characters

#define SERIAL_PARITY_NONE 0     // no parity
#define SERIAL_PARITY_ODD  8     // odd parity
#define SERIAL_PARITY_EVEN 24    // even parity

#define SERIAL_DIV_LATCH_ON 128  // used to turn reg 0,1 into divisor latch

#define SERIAL_GP02        8     // enable interrupt

// comm port I/O port base addresses

#define COM_1              0x3F8 // base port address of port 0
#define COM_2              0x2F8 // base port address of port 1

// programmable interrupt controller addresses

#define PIC_IMR    0x21   // pic's interrupt mask reg.
#define PIC_ICR    0x20   // pic's interupt control reg.

#define INT_SERIAL_PORT_0    0x0C  // port 0 interrupt com 1 & 3
#define INT_SERIAL_PORT_1    0x0B  // port 1 interrupt com 2 & 4

#define SERIAL_BUFF_SIZE 1024    // size of re-circulating receive buffer

// modem control commands

#define MODEM_DTR_ON      0
#define MODEM_DTR_OFF     1

// E X T E R N A L S //////////////////////////////////////////////////////////

extern void (_interrupt _far *Old_Serial_ISR)();  // holds old comm port interrupt handler


extern char serial_buffer[SERIAL_BUFF_SIZE];  // the receive buffer

extern int serial_end;       // indexes into receive buffer
extern int serial_start;
extern int serial_ch;
extern int char_ready;       // current character and ready flag
extern int old_int_mask;     // the old interrupt mask on the PIC
extern int open_port;        // the currently open port
extern int serial_lock;      // serial ISR semaphore so the buffer
                             // isn't altered will it is being written
                             // to by the ISR

extern char *modem_strings[];

// P R O T O T Y P E S ///////////////////////////////////////////////////////

// serial communications

void _interrupt _far Serial_ISR(void);

int Serial_Ready(void);

int Serial_Read(void);

int Serial_Read_Wait(void);

void Serial_Write(char ch);

void Serial_Print(char *string,int cr);

int Serial_Open(int port_base, int baud, int configuration);

int Serial_Close(void);

void Serial_Flush();

// modem interface

void Modem_Control(int command);

void Modem_Send_Command(char *buffer);

int Initialize_Modem(char *extra_init);

int Make_Connection(char *number);

int Wait_For_Connection(void);

int Modem_Result(char *result,int exit_enable);

int Hang_Up(void);




