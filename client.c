//
//  client.c
//  Mach_Messages
//
//  Created by A2nkF on 8/29/18.
//  Copyright © 2018 A2nkF. All rights reserved.
//

#include <stdio.h>
#include <mach/mach.h>

#include "tfp.c"

int main() {
  char service_name[] = "com.a2nkf.hackme";
  mach_port_t local;
  kern_return_t kr;
  char msg[1024];

  bzero(&msg, sizeof(msg));

  kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &local);

  if (kr != KERN_SUCCESS) {
      fail()
      printf("Could not allocate local port: %s \n", mach_error_string(kr));
      return kr;
  }

  mach_port_t service_port = connect_to_service(service_name);

  printf("Enter a message: ");
  fflush(stdout); // Force the text to be shown
  fgets((char*) &msg, sizeof(msg)-1, stdin);

  kr = sendMessage(msg, local, service_port);
  puts("Sending message...\n");
  if (kr != MACH_MSG_SUCCESS) {
      printf("Failed to send message! Error: %s\n", mach_error_string(kr));
      return -1;
  }

  puts("Message sent.");

  return 0;
}
