# necho
network-echo: a simple client-server application to put sockets into practice.

The client sends its standard input to the server.

The server reads from each client and writes to standard output.

# usage

Build:
```
make
```

Run the server:
```
./server [ port=8080 ]
```

Run the client:
```
./client [ port=8080 [ ip=0.0.0.0 ]]

cat /dev/urandom | head -c 100 | ./client 8080 0.0.0.0
```
