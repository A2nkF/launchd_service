//  server.c
//  Mach_Messages
//
//  Created by A2nkF on 12/14/18.
//  Copyright © 2018 A2nkF. All rights reserved.
//

#include "tfp.c"
#include "vuln.c"
#include <servers/bootstrap.h>

int main(){
  char msg[1024];
  char con1[1024];
  char con2[1024];
  char service_name[] = "com.a2nkf.hackme";
  int id1 = 0;
  int id2 = 0;
  mach_msg_id_t tmp = NULL;
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
    tmp = rcvMessageWithID(msg, service_port);
    good();
    puts("Received message! Content: ");
    printf(msg);

    if(id1 && id2){
      magic(con1, con2);
      id1 = id2 = 0;
    } else if(tmp == 0x1535){
      strcpy(con1, msg);
      info();
      printf("ID1 set.\n");
      id1 = 1;
    } else if(tmp == 0x9456){
      strcpy(con2, msg);
      info();
      printf("ID2 set.\n");
      id2 = 1;
    }
  }

}
