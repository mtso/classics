/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2008
 *
 * Authors:
 * Daniel Lezcano <dlezcano at fr.ibm.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

#include <lxc.h>
#include <state.h>
#include <list.h>
#include <conf.h>

void usage(char *cmd)
{
	fprintf(stderr, "%s <command>\n", basename(cmd));
	fprintf(stderr, "\t -n <name>   : name of the container\n");
	_exit(1);
}

static int prestart(const char *name, int argc, char *argv[], void *data)
{
	printf("%s hook has been called with data %s\n",
		__FUNCTION__, (char *)data);
	return 0;
}

int main(int argc, char *argv[])
{
	char opt;
	char *name = NULL;
	char **args;
	int nbargs = 0;

	while ((opt = getopt(argc, argv, "n:")) != -1) {
		switch (opt) {
		case 'n':
			name = optarg;
			break;
		}

		nbargs++;
	}

	if (!name || !argv[optind] || !strlen(argv[optind]))
		usage(argv[0]);

	args = &argv[optind];
	argc -= nbargs;

	if (lxc_start(name, argc, args, prestart, "hello world")) {
		fprintf(stderr, "failed to start %s\n", name);
		return 1;
	}

	return 0;
}
