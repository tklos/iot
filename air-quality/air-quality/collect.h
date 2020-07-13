#ifndef __collect_h__
#define __collect_h__


class Collect {
	public:

	Collect(const char *server, int port, const char *host, const char *api_key, const char *measurement_resource, unsigned long response_timeout, int max_data_len):
			server(server),
			port(port),
			host(host),
			api_key(api_key),
			measurement_resource(measurement_resource),
			response_timeout(response_timeout),
			max_data_len(max_data_len) {
	}

	void send_data(const String p_data);


	private:

	const char *server;
	const int port;
	const char *host;
	const char *api_key;
	const char *measurement_resource;
	const unsigned long response_timeout;
	const int max_data_len;
};


#endif
