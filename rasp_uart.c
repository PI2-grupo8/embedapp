#include "rasp_uart.h"

int uart0_filestream = -1;
struct termios oldtio, newtio;

int uart_init(const char* device)
{
    struct stat dev_status;

    if((uart0_filestream = open(device, O_RDWR | O_NOCTTY)) < 0){
        perror("uart_init");
        return errno;
    }

    if(fstat(uart0_filestream, &dev_status) < 0){
        perror("uart_init");
        return errno;
    }

    if((dev_status.st_mode & 0444) != 0444)
    {
        fprintf(stderr, "uart_init: O dispositivo %s não possui as permissões necessárias.\n", device);
        fprintf(stderr, "Permissões: %lo.\n", (unsigned long) dev_status.st_mode & 0777);

        close(uart0_filestream);

        return -1;
    }

    tcgetattr(uart0_filestream, &oldtio);

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 1;     /* inter-character timer unused */
    newtio.c_cc[VMIN]  = 1;     /* blocking read until 1 character arrives */

    tcflush(uart0_filestream, TCIOFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &newtio);

    return 0;
}

int uart_send(struct uart_data* data_out){
    write(uart0_filestream, data_out->data, data_out->length);

    return 0;
}

int uart_recv(struct uart_data* buffer){
    int total_received = 0;
    int current_received;

    while(total_received < buffer->length){
        if((current_received = read(uart0_filestream, (buffer->data + total_received), (buffer->length - total_received))) < 0){
            perror(strerror(errno));
            return errno;
        }

        total_received += current_received;
    }

    return 0;
}

void uart_end(){
    tcflush(uart0_filestream, TCIOFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &oldtio);
    close(uart0_filestream);
}
