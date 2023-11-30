all: test_server.cpp tcp_server.cpp ./head_files/tcp_server.h test_client.cpp tcp_client.cpp ./head_files/tcp_client.h
	g++ -o test_client test_client.cpp tcp_client.cpp -lpthread
	g++ -o test_server test_server.cpp tcp_server.cpp -lpthread
test_server: test_server.cpp tcp_server.cpp tcp_server.h
	g++ -o test_server test_server.cpp tcp_server.cpp -lpthread
test_client: test_client.cpp tcp_client.cpp tcp_client.h
	g++ -o test_client test_client.cpp tcp_client.cpp -lpthread
clean:
	rm test_server
	rm test_client
