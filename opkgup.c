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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int 		i = 0;
	int 		j = 0;
	/* pacote 1 (antigo) */
	const char 	*pkgv1 = "pacote1-123";
	/* pacote 2 (atualizado) */
	const char 	*pkgv2 = "pacote1-124";
	/* pacote 3 */
	const char	*pkgv3 = "pacote2-123";
	/* lista de pacotes do index.txt */
	char		*pacotes_ind[20000];
	/* lista de pacotes instalados */
	char		*pacotes_ins[20000];

	printf("opkgup\n");

	memset(pacotes_ins, '\0', sizeof(pacotes_ind));
	memset(pacotes_ind, '\0', sizeof(pacotes_ind));
	
	/* inserimos os pacotes do index.txt */
	pacotes_ind[0] = strdup(pkgv1);
	pacotes_ind[1] = strdup(pkgv2);
	pacotes_ind[2] = strdup(pkgv3);

	/* inserimos os pacotes instalados (troquei a ordem ) */
	pacotes_ins[0] = strdup(pkgv2);
	pacotes_ins[1] = strdup(pkgv1);
	pacotes_ins[2] = strdup(pkgv3);

	/* iteramos na lista de pacotes do index.txt */
	for (i = 0; pacotes_ind[i] != NULL; i++) {

		/* iteramos na lista de pacotes instalados */
		for (j = 0; pacotes_ins[j] != NULL; j++) {
			/* comparamos as versoes obtidas */
			if (strcmp(pacotes_ind[i], pacotes_ins[j]) == 0)
				printf("%s == %s\n", pacotes_ind[i], 
				    pacotes_ins[j]);
		else
				printf("%s != %s\n", pacotes_ind[i],
				    pacotes_ins[j]);
		}
	}

	/* liberamos os nomes de pacotes index.txt */
	free(pacotes_ind[0]);
	free(pacotes_ind[1]);
	free(pacotes_ind[2]);

	/* liberamos os nomes de pacotes instalados */
	free(pacotes_ins[0]);
	free(pacotes_ins[1]);
	free(pacotes_ins[2]);

	return (0);
}
