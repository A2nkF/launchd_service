# launchd_service

The server registers a launchd service and waits for mach messages which the client can send.
I'm using the [`bootstrap_check_in`](http://www.manpagez.com/man/3/bootstrap_check_in/) function to register my service.

## NOTE

1. I intentionally included a format-string vulnerability in `server.c`. 
If you aren't trying to exploit the service, you should change the `printf(msg)` in the last line to 
`printf("%s", msg)`  

2. I think this is obvious, but in case it isn't: **you will need MacOS to run this**.

## Build

`gcc server.c -o server && gcc client.c -o client`
