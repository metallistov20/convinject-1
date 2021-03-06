/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <libssh/libssh.h>

#include "ssh_common.h"

int verify_knownhost(ssh_session session)
{
char *hexa;

int state;

char buf[10];

unsigned char *hash = NULL;

size_t hlen;

ssh_key srv_pubkey;

int rc;

	state=ssh_is_server_known(session);

	rc = ssh_get_server_publickey(session, &srv_pubkey);

	if (rc < 0)
	{
		return -1;
	}

	rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);

	ssh_key_free(srv_pubkey);

	if (rc < 0)
	{
		return -1;
	}

	switch(state)
	{
		case SSH_SERVER_KNOWN_OK:
		/* Server found */
		break;

		case SSH_SERVER_KNOWN_CHANGED:

			fprintf(stderr,"Host key for server changed : server's one is now :\n");

			ssh_print_hexa("Public key hash",hash, hlen);

			ssh_clean_pubkey_hash(&hash);

			fprintf(stderr,"For security reason, connection will be stopped\n");

			return -1;

		case SSH_SERVER_FOUND_OTHER:

			fprintf(stderr,"The host key for this server was not found but an other type of key exists.\n");

			fprintf(stderr,"An attacker might change the default server key to confuse your client"
			"into thinking the key does not exist\n"
			"We advise you to rerun the client with -d or -r for more safety.\n");

			return -1;

		case SSH_SERVER_FILE_NOT_FOUND:

			fprintf(stderr,"Could not find known host file. If you accept the host key here,\n");

			fprintf(stderr,"the file will be automatically created.\n");

			/* fallback to SSH_SERVER_NOT_KNOWN behavior */

		case SSH_SERVER_NOT_KNOWN:

			hexa = ssh_get_hexa(hash, hlen);

			fprintf(stderr,"The server is unknown. Do you trust the host key ?\n");

			fprintf(stderr, "Public key hash: %s\n", hexa);

			ssh_string_free_char(hexa);

			if (fgets(buf, sizeof(buf), stdin) == NULL)
			{
				ssh_clean_pubkey_hash(&hash);

				return -1;
			}
			if(strncasecmp(buf,"yes",3)!=0)
			{
				ssh_clean_pubkey_hash(&hash);

				return -1;
			}

			fprintf(stderr,"This new key will be written on disk for further usage. do you agree ?\n");

			if (fgets(buf, sizeof(buf), stdin) == NULL)
			{
				ssh_clean_pubkey_hash(&hash);

				return -1;
			}

			if(strncasecmp(buf,"yes",3)==0)
			{
				if (ssh_write_knownhost(session) < 0)
				{
					ssh_clean_pubkey_hash(&hash);

					fprintf(stderr, "error %s\n", strerror(errno));

					return -1;
				}
			}
		break;

		case SSH_SERVER_ERROR:

			ssh_clean_pubkey_hash(&hash);

			fprintf(stderr,"%s",ssh_get_error(session));

		return -1;
	}

	ssh_clean_pubkey_hash(&hash);

	return 0;
} /* int verify_knownhost */
