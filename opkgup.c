/*
 * Copyright (c) 2020 Murilo Ijanc' <mbsd@m0x.ru>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	/* lista de pacotes do index.txt */
	char		*pacotes_ind[20000];
	/* lista de pacotes instalados */
	char		*pacotes_ins[20000];
	/* diretorio onde fica os pacotes info dos pacotes */
	DIR 		*pkgdb = NULL;
	/* estrutura de um arquivo */
	struct	dirent	*dp = NULL;
	int i = 0;

	printf("opkgup\n");

	/* inicializamos limpando os vetores de pacotes */
	memset(pacotes_ins, '\0', sizeof(pacotes_ind));
	memset(pacotes_ind, '\0', sizeof(pacotes_ind));

	/* abrimos a pasta */
	pkgdb = opendir("/var/db/pkg");

	if (pkgdb == NULL)
		return (1);

	/* iteramos no diretorio */
	while ((dp = readdir(pkgdb)) != NULL)
		/* temos que ignoramos alguns pacotes */
		if (dp->d_name[0] != '.' && strstr(dp->d_name, "firmware") == NULL) {
			printf("Pacote: %s instalado\n", dp->d_name);

		}

	closedir(pkgdb);

	return (0);
}
