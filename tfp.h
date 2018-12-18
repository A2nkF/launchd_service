//
//  tfp.h
//  Mach_Messages
//
//  Created by A2nkF on 8/29/18.
//  Copyright © 2018 A2nkF. All rights reserved.

#ifndef tfp_h
#define tfp_h
// Make logs colorful
#define printc(c,f,s) printf ("\033[%dm" f "\033[0m", 30 + c, s);
#define fail() printc(1, "%s","[-] ")
#define good() printc(2, "%s","[+] ")
#define goodn() printc(2, "%s","[+]\n")
#define warn() printc(3, "%s","[!] ")
#define que() printc(4, "%s","[?] ")
#define pend() printc(5, "%s","[~] ")
#define input() printc(6,  "%s","[>] ")
#define info() printc(7, "%s","[i] ")
#define brackets(c,f,s) printf("[\033[%dm" f "\033[0m] ", 30 + c, s);
#define debug() brackets(1, "%s","DEBUG")

typedef struct {
    mach_msg_header_t header;
    char content[1024];
} our_msg;

typedef struct {
    mach_msg_header_t header;
    char content[1024];
    mach_msg_trailer_t trailer;
} rcv_msg;

typedef struct {
    mach_msg_header_t          header;
    mach_msg_body_t            body;
    mach_msg_port_descriptor_t task_port;
} mach_message;

typedef struct {
    mach_msg_header_t          header;
    mach_msg_body_t            body;
    mach_msg_port_descriptor_t task_port;
    mach_msg_trailer_t         trailer;
} mach_message_recv;


mach_msg_return_t sendMessage(char *content, mach_port_t from, mach_port_t to);
mach_msg_return_t rcvMessage(char *outputContent, mach_port_t rcvPort);
mach_port_t connect_to_service(const char* service_name);
mach_port_t receivePort(mach_port_t recvPort);
int sendPort(mach_port_t dest, mach_port_t port);
#endif
