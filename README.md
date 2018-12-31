# necho
Network echo

The client sends its standard input to the server.

The server reads from the client and writes to standard output.

# usage

```
make
```

```
./server 0.0.0.0 8080
```

```
cat /dev/urandom | head-c 100 | ./client 0.0.0.0 8080
```
