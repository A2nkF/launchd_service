//  server.c
//  Mach_Messages
//
//  Created by A2nkF on 12/14/18.
//  Copyright © 2018 A2nkF. All rights reserved.
//

#include "tfp.c"
#include <servers/bootstrap.h>

int main(){
  char msg[1024];
  char service_name[] = "com.a2nkf.hackme";

  mach_port_t service_port, bs_port;
  kern_return_t kr;

  task_get_bootstrap_port(mach_task_self(), &bs_port);
  kr = bootstrap_check_in(bs_port, service_name, &service_port);
  pend();
  printf("Registring %s service... ", service_name);
  if(kr != KERN_SUCCESS){
    fail();
    printf("bootstrap_check_in failed: %s\n", mach_error_string(kr));
    return kr;
  }
  goodn();

  while(1){
    info();
    puts("Awaiting message... ");
    fflush(stdout);
    kr = rcvMessage(msg, service_port);
    good();
    puts("Receiving message...\n");
    if(kr != KERN_SUCCESS){
      printf("Failed to recieve message! Error: %s\n", mach_error_string(kr));
      return kr;
    }

    good();
    puts("Received message! Content: ");
    printf(msg);
  }

}
