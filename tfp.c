//
//  tfp.c
//  Mach_Messages
//
//  Created by A2nkF on 8/29/18.
//  Copyright © 2018 A2nkF. All rights reserved.
//
#include <stdio.h>
#include <mach/mach.h>
#include <spawn.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <servers/bootstrap.h>

#include "tfp.h"


mach_msg_return_t sendMessage(char *content, mach_port_t from, mach_port_t to){
  our_msg msg;
  bzero(&msg, sizeof(msg));

  // Setup header
  msg.header.msgh_remote_port = to;
  msg.header.msgh_local_port = from;
  msg.header.msgh_voucher_port = MACH_PORT_NULL;
  msg.header.msgh_id = 0x1535; // 0x9456 // 0x1535
  msg.header.msgh_bits = MACH_MSGH_BITS_SET(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND, 0, 0);

  // Send message
  strncpy((char *) &msg.content, content, 1024);

  printc(7, "%s\n", "======================== Details ========================");
  printf("local port: %d\n", from);
  printf("remote port: %d\n", to);
  printf("message id: 0x%x\n", msg.header.msgh_id);
  printf("message size: %lu\n", sizeof(msg));
  printf("message content: %s\n", msg.content);
  printc(7, "%s\n","========================== End ==========================");

  return mach_msg(&msg.header, MACH_SEND_MSG, sizeof(msg), 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, mach_task_self());

}

int sendPort(mach_port_t dest, mach_port_t port){
  kern_return_t kr;
  mach_message msg;

  msg.header.msgh_remote_port = dest;
  msg.header.msgh_local_port = MACH_PORT_NULL;
  msg.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MAKE_SEND, 0) | MACH_MSGH_BITS_COMPLEX;
  msg.header.msgh_size = sizeof(mach_message);
  msg.body.msgh_descriptor_count = 1;
  msg.task_port.name = port;
  msg.task_port.disposition = MACH_MSG_TYPE_COPY_SEND;
  msg.task_port.type = MACH_MSG_PORT_DESCRIPTOR;

  kr = mach_msg_send(&msg.header);
  if(kr != KERN_SUCCESS){
    fail();
    printf("Send failed: %s\n", mach_error_string(kr));
    return -1;
  }
  return 0;

}

mach_msg_return_t rcvMessage(char *outputContent, mach_port_t rcvPort) {
    rcv_msg msg;
    bzero(&msg, sizeof(msg));

    // Receive the message!
    // Parameters:
    // buffer: Buffer were the received message should be stored
    // options: MACH_SEND_MSG/MACH_RCV_MSG depending if we want to send or receive messages.
    // sendSize: Size of the message we want to send
    // rcvSize: Maximum size of the message we want to receive
    // rcvName: Port on which we want to receive the message
    // timeout: Maximum time mach_msg should wait for a message
    // notify: Port to which a notification should be sent once a message has been received
    mach_msg_return_t ret = mach_msg(&msg.header, MACH_RCV_MSG, 0, sizeof(msg), rcvPort, MACH_MSG_TIMEOUT_NONE, mach_task_self());
    if (ret != MACH_MSG_SUCCESS) {
        printf("Failed to receive message! Error: %s\n", mach_error_string(ret));
        return ret;
    }

    // Check the message id and size
    if (msg.header.msgh_id != 0x1337) {
      warn();
      printf("Received unexpected message id: %x\n", msg.header.msgh_id);
      return KERN_FAILURE;
    }

    if (msg.header.msgh_size != (sizeof(msg) - sizeof(mach_msg_trailer_t))) {
      warn();
      printf("Received unexpected message size: %d\n", msg.header.msgh_size);
      return KERN_FAILURE;
    }

    // Id and size match? Then copy the string!
    strncpy(outputContent, msg.content, 1024);


    return MACH_MSG_SUCCESS;
}

mach_msg_id_t rcvMessageWithID(char *outputContent, mach_port_t rcvPort) {
    rcv_msg msg;
    bzero(&msg, sizeof(msg));

    // Receive the message!
    // Parameters:
    // buffer: Buffer were the received message should be stored
    // options: MACH_SEND_MSG/MACH_RCV_MSG depending if we want to send or receive messages.
    // sendSize: Size of the message we want to send
    // rcvSize: Maximum size of the message we want to receive
    // rcvName: Port on which we want to receive the message
    // timeout: Maximum time mach_msg should wait for a message
    // notify: Port to which a notification should be sent once a message has been received
    mach_msg_return_t ret = mach_msg(&msg.header, MACH_RCV_MSG, 0, sizeof(msg), rcvPort, MACH_MSG_TIMEOUT_NONE, mach_task_self());
    if (ret != MACH_MSG_SUCCESS) {
        printf("Failed to receive message! Error: %s\n", mach_error_string(ret));
        return -1;
    }

    if (msg.header.msgh_size != (sizeof(msg) - sizeof(mach_msg_trailer_t))) {
      warn();
      printf("Received unexpected message size: %d\n", msg.header.msgh_size);
      return -1;
    }

    // Id and size match? Then copy the string!
    strncpy(outputContent, msg.content, 1024);
    return msg.header.msgh_id;
}


mach_port_t receivePort(mach_port_t recvPort) {
    kern_return_t kr;
    mach_message_recv rmsg;

    kr = mach_msg(&rmsg.header, MACH_RCV_MSG, 0, sizeof(mach_message_recv), recvPort, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        return MACH_PORT_NULL;
    }

    return rmsg.task_port.name;
}

mach_port_t connect_to_service(const char* service_name) {
  mach_port_t bs_port, service_port;
  kern_return_t err;

  task_get_bootstrap_port(mach_task_self(), &bs_port);
  info();
  printf("Getting service port for %s\n", service_name);
  err = bootstrap_look_up(bs_port, service_name, &service_port);
  if(err == KERN_SUCCESS) {
    info()
    printf("service port: 0x%x\n", service_port);

    return service_port;
  }
  else {
    fail();
    printf("Could not get service port for %s\n", service_name);

    return -1;
  }
}
