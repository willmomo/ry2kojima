#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "sntpc.h"

int main() {
	sntp_socket_startup();

	{
		ntp_options opts = {0, 1, 1, 0};
		sntp("ts00", &opts);
	}

	sntp_socket_cleanup();
	return 0;
}
