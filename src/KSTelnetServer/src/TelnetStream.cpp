
#if defined USE_KSTelnetServer || defined KSLIBRARIES_USEALL     // include File in Build only if it ist defined to use it

#include "TelnetStream.h"

TelnetStreamClass::TelnetStreamClass(uint16_t port) :server(port) {
}

void TelnetStreamClass::begin(int port) {
  	if (port) {
    	server = NetServer(port);
	}
  	server.begin();
  	client = server.available();
}

void TelnetStreamClass::stop() {
  	client.stop();
}

boolean TelnetStreamClass::disconnected() {
  	if (!server)
    	return true;

  	if (!client || !client.available()) {
    	client = server.available(); // try to get next client with data
  	}
  	return !client;
}


int TelnetStreamClass::read() {
	if (disconnected())
		return -1;
	return client.read();
}

int TelnetStreamClass::available() {
	if (disconnected())
		return 0;
	return client.available();
}

int TelnetStreamClass::peek() {
	if (disconnected())
		return -1;
	return client.peek();
}

size_t TelnetStreamClass::write(uint8_t val) {
	return server.write(val);
}

size_t TelnetStreamClass::write(const uint8_t *buf, size_t size) {
  	return server.write(buf, size);
}

void TelnetStreamClass::flush() {
  	server.flush();
}


#endif      // #if defined USE_KSTelnetServer || defined KSLIBRARIES_USEALL