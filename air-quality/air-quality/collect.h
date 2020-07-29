#ifndef __collect_h__
#define __collect_h__


class Collect {
	public:

	Collect(const char *server, int port, const char *host, const char *api_key, const char *measurement_resource, unsigned long response_timeout):
			server(server),
			port(port),
			host(host),
			api_key(api_key),
			measurement_resource(measurement_resource),
			response_timeout(response_timeout) {
	}

	void send_data(const String p_data);


	private:

	const char *server;
	const int port;
	const char *host;
	const char *api_key;
	const char *measurement_resource;
	const unsigned long response_timeout;
};


#endif
