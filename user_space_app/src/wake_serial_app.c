/*
	Author : Ishaan Biswas
	Version 1.0
	Date : 08/01/2014
	Comments : User App for wakeline_validator tool
*/

#include<time.h>
#include<poll.h>
#include<fcntl.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>
#include<string.h>
#include<sys/inotify.h>
#include<sys/types.h>
#include <unistd.h>
#include "serial_app.h"


FILE *logfile ;
char *filename = NULL ;
bool response = false;
char char_val[MAX_MESSAGE_LEN][2];
double char_ts[MAX_MESSAGE_LEN];
double wakeline_ts[4]; // Global array for wakeline data. wakeline_enable,wakeline_disable, wakeline_ack_enable, wakeline_ack_disable.Populated by read_wakeline_data 
bool print_log = false ;


void chprintf( const char* format, ... ) {		
	va_list args;
	va_start(args, format);
	va_end(args);
	if (print_log ){
		vfprintf( logfile, format, args);
	} else 	{
		vprintf( format, args);
	}
}

void msec_to_time(double time, char *tmbuf) {
	char tmbuf_local[64];	
	time_t nowtime ; 
	struct tm *nowtm ;
	nowtime = (time_t) time ;
	nowtm = localtime(&nowtime);
	strftime(tmbuf_local, sizeof tmbuf_local , "%Y-%m-%d %H:%M:%S ", nowtm );
	memcpy(tmbuf, tmbuf_local, sizeof(tmbuf_local));
}


void
handle_errant_messages() {
	int fd ;
	char stop = '0';
	char start = '1';
	fd = open("/sys/class/gpio/gpio24/value", O_WRONLY);
	if( fd < 0 ){
		chprintf("Unable to Stop test due to limited permissions. Please run as root and restart tests \n");
		exit(1);
	}	
	write(fd, &stop, 1);
	chprintf("Errant message detected. Wakeline tool will be restarted in 2 seconds\n");
	chprintf("3 ...");
	sleep(1);
	chprintf("2 ...");
	sleep(1);
	chprintf("1 ...");
	sleep(1);
	chprintf("Monitoring has been restarted. \n");
	write(fd, &start, 1);
	close(fd) ;
}

// Reads wakeline data from the sysfs interface. /sys/kernel/wakeline 
// Populates wakeline_ts array
void
read_wakeline_data(FILE *logfile) {
	
	int i;
	int wake_fd[4];
	int wake_len[4];
	char wake_read_buf[4][20];
//	double fraction ;
	//init_printf();
	for (i = 0 ; i < 4 ; i++) {
		wake_fd[i] = open( wakeline_data[i] , O_RDONLY);	
		if(wake_fd[i] < 0) {
			perror("Unable to open char_value file");
			exit(1);
		}
		wake_len[i] = read(wake_fd[i], wake_read_buf[i] , 20) ;
		if(wake_len[i] < 0) {
			perror("Nothing to be read for wakeline data. Please retry");
			exit(1);
		}
		wakeline_ts[i] = atof(wake_read_buf[i]);
		//fraction = wakeline_ts[i] - ((long)wakeline_ts[i]) ;
		//chprintf("Wakeline data fraction : %f\n",fraction);
	/*	if ( (response == false) | ((response == true ) & (i < 2))){
			if (flag == 2) {
			//	printf( "%s : %9.6f \n",wakeline_data[i], wake_ts[i]);
			}	
		}
	*/
		close(wake_fd[i]);
	}
}
// Reads serial data from sysfs interface. /sys/kernel/uartstream.
// Populates global array of timestamps (char_ts) for arrival of each byte and char_val which is the value of each byte in the message
void 
read_serial_data(int no_of_bytes, int flag) {

	int i;
	int fd_val[no_of_bytes];
	int fd_ts[no_of_bytes];
	int fd_val_len[no_of_bytes];
	int fd_ts_len[no_of_bytes];
	char read_val_buf[no_of_bytes][2];
	char read_ts_buf[no_of_bytes][20];
	response = false;

	//init_printf();

	for(i = 0; i < no_of_bytes  ; i++) {
		fd_val[i] = open( char_values[i] , O_RDONLY);	
		if(fd_val[i] < 0) {
			perror("Unable to open char_value file");
			exit(1);
		}
		fd_ts[i] = open( char_timestamps[i] , O_RDONLY);	
		if(fd_ts[i] < 0) {
			perror("Unable to open char_timestamp file");
			exit(1);
		}
		fd_val_len[i] = read(fd_val[i], read_val_buf[i] , 4) ;
		if(fd_val_len[i] < 0) {
			perror("Nothing to be read from char_value file. Please retry");
			exit(1);
		}
		fd_ts_len[i] = read(fd_ts[i], read_ts_buf[i] , 20) ;
		if(fd_ts_len[i] < 0) {
			perror("Nothing to be read from char_timestamp file. Please retry");
			exit(1);
		}
		char_ts[i] = atof(read_ts_buf[i]);
		memcpy(char_val[i], read_val_buf[i],2);
		if ( i==1 ) {
			if ( (read_val_buf[i][0] == 48 ) & ( read_val_buf[i][1] == 48 ) ) {
			// byte 1 being zero means it is a response message. 
			// dont show wakeline_ack attributes for this. 
		//	chprintf("Response message dont log ack signals \n");
				response = true ;
			}
		}
		close(fd_ts[i]);
		close(fd_val[i]);
	}
}

//void display_info(int flag , double *wake_data , double *char_timestamps, int no_of_bytes) {
void display_info(int flag ,  int no_of_bytes ) {

	double wake_life ;
	double serial_start , serial_start_diff ;
	double serial_end , serial_end_diff ;
	int i;	
	char tmbuf[64] ;
	
	//init_printf();
	msec_to_time(wakeline_ts[0], tmbuf);
	chprintf("\n**********Start of new Message ********************************\n");	
	chprintf("A Message was sent from to the Client MCU from ACM at %s \n",tmbuf);
	
	chprintf("Message was %d bytes long \n", no_of_bytes);
	chprintf("Relative time between each byte received (Start byte relative to wake enable) : \n");
	if(flag == 2) {
		chprintf("Byte  , Relative Arrival time : \n");	
		for (i=0; i < no_of_bytes ; i++) {
			if (i==0) {
				chprintf("0x%c%c , %9.6f ms \n", char_val[i][0],char_val[i][1],(char_ts[i]- wakeline_ts[0])*1000);
			} else {
				chprintf("0x%c%c , %9.6f ms \n", char_val[i][0],char_val[i][1], ( char_ts[i] -char_ts[i-1])*1000 );
			}
		}
	} else if (flag ==1 ) {
		chprintf(" Message received was : 0x");
		for (i=0; i < no_of_bytes ; i++) {
			chprintf("%c%c", char_val[i][0],char_val[i][1]);
		}
	} 	
	chprintf("\n**********End of Byte level detail*********** \n \n");
//	if (flag == 1) {	
	if (1) {	
		wake_life = (wakeline_ts[1] - wakeline_ts[0]) ;
		chprintf("Wakeline data timestamps as received : \n ");
		chprintf("Wake line enabled at %1.6f  \n",wakeline_ts[0]);
		chprintf("Wake line enabled for %1.6f ms \n",wake_life*1000);
		chprintf("Wake line disabled at %1.6f  \n",wakeline_ts[1]);
		serial_start = char_ts[0] - 0.000260 ; // Since the serial byte arrives 10*1/baud_rate secs later 
		serial_end = char_ts[no_of_bytes-1]  ;
		serial_start_diff = (serial_start - wakeline_ts[0])*1000; 
		serial_end_diff = (serial_end - wakeline_ts[1])*1000; 

		if (serial_start_diff < 0 ){
			chprintf("serial data started %1.6f ms before wakeline was enabled \n", fabs(serial_start_diff));
		} else {
			chprintf("Serial data started %1.6f ms after wakeline was enabled \n", serial_start_diff);
		}

		if (serial_end_diff < 0 ){
			chprintf("serial data ended %1.6f ms before wakeline disabled \n", fabs(serial_end_diff));
		} else {
			chprintf("Serial data ended %1.6f ms after wakeline was disabled \n", serial_end_diff);
		}
	} 
	chprintf("\n**********End of message details ************\n");
}


int 
main (int argc, char **argv) {
	
	char buf_end[10] ;
	char buf_len[10] ;
	char buf_error[10] ;
	struct pollfd sysfd ;
	bool print = false;
	int no_of_bytes, msg_error;
	int opt = 0;
	int dflag = 0;
	int fd_end, fd_len, fd_error, ret, end_len, msg_len , error_len ;
	bool option_chosen = false ;
	while (( opt = getopt(argc , argv, "svo:")) != -1 ) {
		switch(opt) {
		case 's' :
			chprintf("Stripped version selected \n");
			dflag = 1 ;
			option_chosen = true;
			break ;
		case 'v' :
			chprintf("Verbose version selected \n");
			dflag = 2 ;
			option_chosen = true;
			break ;
		case 'o' :
			filename = optarg ;
			chprintf("Messages will be logged in %s \n",filename);
			print_log = true ;
			option_chosen = true;
			break ;
		case '?' :
			option_chosen = false;
		}	
	}
	if (  ( option_chosen == false ) ) {
	     chprintf("\n Please read the Wakeline Validator document before trying the tool out. This is not for the uninitiated! \n");
	    chprintf(  "Usage : monitor_wake_serial -s for stripped report \n monitor_wake_serial -v for verbose report \n monitor_wake_serial -o <logoutput.log>  to output information to a file. \n");
	    chprintf("\n \n Please read the Wakeline_Validator document available on Google Drive. \n Contact <kelly.schwarzhoff@arrayent.com> for more details.\n");
	   exit(EXIT_FAILURE);
	}
	if (print_log ){
		logfile = fopen(filename, "a+");
		chprintf( "Opened file and printing into %s. Oh yes the printf prints to a file.\n", filename);
		chprintf("dflag is %d",dflag);
		if (logfile == NULL) {
			printf(  "Error Opening Logfile : %s ! \n", filename);
			exit(1);
		}
		fclose(logfile);
	}
	if(1) {
	do {
		if (print_log) {
			logfile = fopen(filename, "a");
		}
		fd_end = open ( MESSAGE_END ,O_RDONLY);
		if(fd_end < 0) {
			perror("Unable to open message_end file");
			exit(1);
		}	
		fd_len = open (MESSAGE_LEN , O_RDONLY);
		if(fd_len < 0) {
			perror("Unable to open message_length file");
			exit(1);
		}	
		fd_error = open (MESSAGE_ERROR , O_RDONLY);
		if(fd_error < 0) {
			perror("Unable to open message_error file");
			exit(1);
		}	
		end_len = read(fd_end, buf_end, 4);
		if(end_len <= 0) {
			perror("Nothing read from message_end file");
			exit(1);
		}	
		msg_len = read(fd_len, buf_len, 4);
		if(msg_len <= 0) {
			perror("Nothing read from message_length file");
			exit(1);
		}	
		error_len = read(fd_error, buf_error, 4);
		if(error_len <= 0) {
			perror("Nothing read from message_error file");
			exit(1);
		}	
		no_of_bytes = atoi(buf_len);
		msg_error = atoi(buf_error);
		//msg_end = atoi(buf_end);
		if(print ) {
		// This is where new data needs to be read from sysfs	
			if (msg_error == 1) {
				chprintf(  "Maximum message length exceeded for message. Please ensure message sent is less than 105 bytes long.Refer to ACM Developers Guide for more details.Error code : %d\n", msg_error);
			} else if (msg_error == 2) {
				chprintf(  "Interrupts got missed. Previous message not logged. Restarting tests. Error code : %d \n",msg_error);
				handle_errant_messages();
			} else {
				//printf(logfile , "Well atleast something got printed\n");
				read_wakeline_data(logfile);
				read_serial_data(no_of_bytes, dflag);
				// Read all characters here
				display_info(dflag , no_of_bytes );
			}
			print = false;
		}
		sysfd.fd = fd_end ;
		sysfd.events = POLLPRI ; //High priority data to be read. Set when sysfs_notify is called in the kernel
		ret = poll (&sysfd, 1 , -1);
		if (ret < 0) {
			perror("Error in polling message_end. Please retry");
		} else if (ret == 0) {
//			printf("Timeout Occured");
		} else if (sysfd.revents & POLLPRI){
			print = true;
		}
		close(fd_end);
		close(fd_len);
		close(fd_error);
		if(print_log) {
			fclose(logfile);
		}
	}  while (1) ;
	}
}
