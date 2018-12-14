# launchd_service

The server registers a launchd service and waits for mach messages which the client can send.

## NOTE
I intentionally included a format-string vulnerability in `server.c`. 
If you aren't trying to exploit the service, you should change the `printf(msg)` in the last line to 
`printf("%s", msg)`  


## Build

`gcc server.c -o server && gcc client.c -o client`
