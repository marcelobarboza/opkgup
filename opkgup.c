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

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

static char *get_pkg_vs(const char *);

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
		err(1, "pkgdb null");

	/* iteramos no diretorio */
	while ((dp = readdir(pkgdb)) != NULL) {
		/* temos que ignoramos alguns pacotes */
		/* TODO: strings vazias tb temos que ignorar */
		if (dp->d_name[0] != '.' && strstr(dp->d_name, "firmware") == NULL) {
			const char *pkgf= dp->d_name;
			const char *vs = get_pkg_vs(pkgf);

			printf("Versão: %s\n", name);
			i++;
		}
		if (i == 10)
			break;
	}

	closedir(pkgdb);

	return (0);
}

static char *
get_pkg_vers(const char *s)
{
	char *save;

	for (save = NULL;; ++s) {
		if (*s == '-' && isdigit(*(++s)) != 0)
			save = (char *)s;
		if (!*s)
			return (save);

	}
}
