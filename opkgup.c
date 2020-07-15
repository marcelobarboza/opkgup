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
static char *get_pkg_name(const char *);

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
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;

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
			/* obtenhho a versão */
			const char *vs = get_pkg_vs(pkgf);
			/* obtenho o nome */
			char *name = get_pkg_name(pkgf);
			
			printf("Pacote: %s Versão: %s\n", name, vs);

			free(name);
			i++;
		}
		/*
		if (i == 10)
			break;
		*/
	}

	/* abrir o arquivo index.txt */
	fp = fopen("./index.txt", "r");
	if (fp == NULL) {
		closedir(pkgdb);
		err(1, "fopen");
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		/* removemos \n */
		line[read-1] = '\0';
		/* procuro pelo ultimo espaco */
		char *l = strrchr(line, ' ');
		/* pulo para o endereco */
		l++;
		printf("%s\n", l);
	}

	/* fecho o descritor */
	fclose(fp);
	/* limpo a linha caso ela exista */
	if (line != NULL) free(line);

	closedir(pkgdb);

	return (0);
}

static char *
get_pkg_name(const char *st)
{
	char *copy;
	int c = 0;
	size_t siz;
	const char *s = st;

	siz = strlen(s) + 1;
	if ((copy = malloc(siz)) == NULL)
		return (NULL);

	for (;; ++s) {
		if (*s == '-' && isdigit(*(++s)) != 0)
			break;
		if (!*s)
			break;
		c++;

	}

	memcpy(copy, st, c);
	copy[c+1] = '\0';
	return (copy);
}

static char *
get_pkg_vs(const char *s)
{
	char *save;

	for (save = NULL;; ++s) {
		if (*s == '-' && isdigit(*(++s)) != 0)
			save = (char *)s;
		if (!*s)
			return (save);

	}
}
