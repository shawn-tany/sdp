#ifndef MFTP_SERIAL_TRANSMISSION
#define MFTP_SERIAL_TRANSMISSION

typedef struct 
{
    
} MFTP_SERIAL_T;

int serial_init();

int serial_recv();

int serial_send();

int serial_uninit();

#endif
