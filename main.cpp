#include <stdio.h>
#include <unistd.h>       // Used for UART
#include <sys/fcntl.h>    // Used for UART
#include <termios.h>      // Used for UART
#include <string>
#include <sstream>
#include <iostream>       // User input for motors

using namespace std;

// Define Constants
const char *uart_target = "/dev/ttyTHS2";
#define     NSERIAL_CHAR   256
#define     VMINX          1
#define     BAUDRATE       B115200

int main()
{
    printf("-----------------Program Start-----------------\n\n");

    int ii, jj, kk;

    // SETUP SERIAL WORLD
    int fid = -1;
    struct termios  port_options;   // Create the structure                          

    tcgetattr(fid, &port_options);  // Get the current attributes of the Serial port 

	
    //------------------------------------------------
    //  OPEN THE UART
    //------------------------------------------------
    // The flags (defined in fcntl.h):
    //	Access modes (use 1 of these):
    //		O_RDONLY - Open for reading only.
    //		O_RDWR   - Open for reading and writing.
    //		O_WRONLY - Open for writing only.
    //	        O_NDELAY / O_NONBLOCK (same function) 
    //               - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
    //                 if there is no input immediately available (instead of blocking). Likewise, write requests can also return
    //	               immediately with a failure status if the output can't be written immediately.
    //                 Caution: VMIN and VTIME flags are ignored if O_NONBLOCK flag is set.
    //	        O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the 
    //                     controlling terminal for the process.fid = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY | O_NDELAY);		
    //Open in non blocking read/write mode

    fid = open(uart_target, O_WRONLY | O_NOCTTY );

    tcflush(fid, TCIFLUSH);
    tcflush(fid, TCIOFLUSH);
 	
    usleep(1000000);  // 1 sec delay

    if (fid == -1)
	{
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
    //------------------------------------------------
    // CONFIGURE THE UART
    //------------------------------------------------
    // flags defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html
    //	Baud rate:
    //         - B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, 
    //           B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, 
    //           B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //	CSIZE: - CS5, CS6, CS7, CS8
    //	CLOCAL - Ignore modem status lines
    //	CREAD  - Enable receiver
    //	IGNPAR = Ignore characters with parity errors
    //	ICRNL  - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //	PARENB - Parity enable
    //	PARODD - Odd parity (else even)

    port_options.c_cflag &= ~PARENB;            // Disables the Parity Enable bit(PARENB),So No Parity   
    port_options.c_cflag &= ~CSTOPB;            // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit 
    port_options.c_cflag &= ~CSIZE;	        // Clears the mask for setting the data size             
    port_options.c_cflag |=  CS8;               // Set the data bits = 8                                 	 
    port_options.c_cflag &= ~CRTSCTS;           // No Hardware flow Control                         
    port_options.c_cflag |=  CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines       				
    port_options.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both input & output
    port_options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode                            
    port_options.c_oflag &= ~OPOST;                           // No Output Processing

    //port_options.c_lflag = 0;               //  enable raw input instead of canonical,
 		
    //port_options.c_cc[VMIN]  = VMINX;       // Read at least 1 character
    //port_options.c_cc[VTIME] = 0;           // Wait indefinetly 
		
    //cfsetispeed(&port_options,BAUDRATE);    // Set Read  Speed 
    cfsetospeed(&port_options,BAUDRATE);    // Set Write Speed 

    // Set the attributes to the termios structure
    int att = tcsetattr(fid, TCSANOW, &port_options);

    if (att != 0 )
    {
        printf("\nERROR in Setting port attributes");
    }
    else
    {
        printf("\nSERIAL Port Good to Go.\n");
    }

    // Flush Buffers
    tcflush(fid, TCIFLUSH);
    tcflush(fid, TCIOFLUSH);

    usleep(500000);   // 0.5 sec delay

//--------------------------------------------------------------
    // TRANSMITTING BYTES
    //--------------------------------------------------------------
    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    int count = 0;
    int goodTX = 0;
	
    // Send dummy byte
    printf("Sending dummy byte 0x80");

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x80;
    count = write(fid, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    usleep(1000000);

    p_tx_buffer = &tx_buffer[0]; // move pointer back to start
    // Front Controller
    *p_tx_buffer++ = 0x55;	 // Header - send decimal 85 (0x55)
    *p_tx_buffer++ = 0x00;	 // Channel & Address - 0b0000 0000 - turn on left motor @ ID: 000
    *p_tx_buffer++ = 0xA0;	 // Command - set speed to 160 (0xA0) centered around 127
    *p_tx_buffer++ = 0x55 + 0x00 + 0xA0;	 // Checksum - result of Header + Address + Command
    // Back Controller
    *p_tx_buffer++ = 0x55;	 // Header - send decimal 85 (0x55)
    *p_tx_buffer++ = 0x01;	 // Channel & Address - 0b0000 0001 - turn on left motor @ ID: 001
    *p_tx_buffer++ = 0xA0;	 // Command - set speed to 160 (0xA0) centered around 127
    *p_tx_buffer++ = 0x55 + 0x01 + 0xA0;	 // Checksum - result of Header + Address + Command
    
    printf("fid 1=%d\n", fid );
	
	if (fid != -1)
	{	
		count = write(fid, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0])); //Filestream, bytes to write, number of bytes to write

        	usleep(1000);   // .001 sec delay

        	printf("Count = %d\n", count);
		printf("Sucessful Transmissions: %d\n", goodTX);

		if (count < 0)  printf("UART TX error\n");
	}

    	usleep(1000000);  // 1 sec delay
    

/*   
    string inputUser;
    int speedChange;
    int dir;
    cout << "New speed value (0-63): ";
    getline (cin, inputUser);
    stringstream(inputUser) >> speedChange;
    cout << "Direction value (0,1): ";
    getline (cin, inputUser);
    stringstream(inputUser) >> dir;
    printf("You entered: %d\n", speedChange);
    printf("You entered: %d\n", dir);
    
    unsigned char new_buffer[0];
    new_buffer[0] = speedChange & 0xFF;
    if (dir == 1) new_buffer[0] |= 0x40;    
    printf("%x\n", new_buffer[0]);
    
    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = new_buffer[0];

    count = write(fid, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    usleep(1000);   // .001 sec delay
*/


/*
//--------------------------------------------------------------
    // RECEIVING BYTES - AND BUILD MESSAGE RECEIVED
    //--------------------------------------------------------------
    unsigned char rx_buffer[VMINX];
    unsigned char serial_message[NSERIAL_CHAR];
    bool          pickup = true;
    int           rx_length;
    int           nread = 0;    

	tcflush(fid, TCIOFLUSH);

    usleep(1000);   // .001 sec delay

    printf("Ready to receive message.\n");

for (ii=0; ii<NSERIAL_CHAR; ii++)  serial_message[ii]=' ';

	while (pickup && fid != -1)
	{
        nread++;

        rx_length = read(fid, (void*)rx_buffer, VMINX);   // Filestream, buffer to store in, number of bytes to read (max)

        printf("Event %d, rx_length=%d, Read=%s\n",  nread, rx_length, rx_buffer );

		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
		}

		if (rx_length == 0)
		{
			//No data waiting
		}
		
        if (rx_length>=0)
		{
            if (nread<=NSERIAL_CHAR)   serial_message[nread-1] = rx_buffer[0];   // Build message 1 character at a time

            if (rx_buffer[0]=='#')   pickup=false;                               // # symbol is terminator
		}
	}

    printf("\nMessage Received: %s", serial_message);
*/
//-------------------------------------------
    //  CLOSE THE SERIAL PORT
    //-------------------------------------------
    close(fid);

printf("\n\nGoodbye World\n\n");

}
