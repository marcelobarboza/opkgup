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

#include <sys/queue.h>

#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXT "tgz"
#define INDEX "index.txt"

/*
 * package
 */
typedef struct pkg {
	char 			*fullname;
	char 			*name;
	char 			*ver;

	SLIST_ENTRY(pkg) 	 entries;
} pkg_t;

typedef struct opkg {
	char 				*arch;
	char 				*pkg_path;
	SLIST_HEAD(package_dir, pkg) 	 pkgd;
	SLIST_HEAD(package_index, pkg) 	 pkgi;
} opkg_t;

static pkg_t	*pkg_new(const char *);
static void 	 pkg_free(pkg_t *);
static void	 pkg_print(pkg_t *);
static char	*pkg_get_ver(const char *);
static char	*pkg_get_name(const char *);
static int	 ext_exist(const char *);
opkg_t		*opkg_new(void);
void		 opkg_free(opkg_t *);
int		 opkg_push_pkg(opkg_t *, const char *);

int
main(int argc, char *argv[])
{
	pkg_t 	*p = NULL;
	pkg_t 	*pn = NULL;
	opkg_t 	*o = opkg_new();

	if (opkg_push_pkg(o, "pacote-1.0.0p1.tgz") != 0)
		goto clean;

	for (p = SLIST_FIRST(&o->pkgd); p != NULL; p = pn) {
		pn = SLIST_NEXT(p, entries);
		pkg_print(p);
	}

clean:
	opkg_free(o);

	return (0);
}

/*
 * TODO: option to push multiple packages type
 */
int
opkg_push_pkg(opkg_t *o, const char *n)
{
	pkg_t *pf = NULL;
	pkg_t *p = pkg_new(n);

	if (p == NULL)
		return (-1);

	if (SLIST_EMPTY(&o->pkgd))
		SLIST_INSERT_HEAD(&o->pkgd, p, entries);
	else {
		pf = SLIST_FIRST(&o->pkgd);
		SLIST_INSERT_AFTER(pf, p, entries);
	}
	return (0);
}

opkg_t *
opkg_new(void)
{
	opkg_t *o = NULL;

	o = malloc(sizeof(o));
	if (o == NULL)
		err(2, "malloc");

	o->arch = NULL;
	o->pkg_path = NULL;

	SLIST_INIT(&o->pkgd);
	SLIST_INIT(&o->pkgi);

	return (o);
}

void
opkg_free(opkg_t *o)
{
	pkg_t *p = NULL;
	pkg_t *pn = NULL;

	if (o == NULL)
		return;
	if (o->arch != NULL)
		free(o->arch);
	if (o->pkg_path != NULL)
		free(o->pkg_path);

	for (p = SLIST_FIRST(&o->pkgd); p != NULL; p = pn) {
		pn = SLIST_NEXT(p, entries);
		pkg_free(p);
	}

	for (p = SLIST_FIRST(&o->pkgi); p != NULL; p = pn) {
		pn = SLIST_NEXT(p, entries);
		pkg_free(p);
	}

	free(o);
}

static char *
pkg_get_name(const char *st)
{
	char *copy;
	const char *s = st;
	int c = 0;
	size_t siz;

	siz = strlen(s) + 1;
	if ((copy = malloc(siz)) == NULL)
		return (NULL);

	for (;; ++s) {
		if (*s == '-' && isdigit(*(s+1))) {
			break;
		}
		if (!*s)
			break;
		c++;

	}

	memcpy(copy, st, c);
	copy[c+1] = '\0';
	return (copy);
}

static char *
pkg_get_ver(const char *s)
{
	char *save;

	for (save = NULL;; ++s) {
		if (*s == '-' && isdigit(*(++s)) != 0)
			save = (char *)s;
		if (!*s)
			return (save);
	}
}

/*
 * The ext_exist test if 'tgz' extension exist.
 * return zero if extension not exist or non-zero if exist
 */
static int
ext_exist(const char *fullname)
{
	const char *sp;

	if ((sp = strrchr(fullname, '.')) == NULL)
		return (0);
	sp++;

	if (strcmp(EXT, sp) == 0)
		return (1);
	return (0);
}

static void
pkg_print(pkg_t *p)
{
	if (p == NULL) return;

	printf("===========================================\n");
	printf("Name:\t\t%s\nVersion:\t%s\nFullName:\t%s\n", p->name != NULL ?
	    p->name : "", p->ver != NULL ? p->ver : "", p->fullname != NULL ?
	    p->fullname : "");
	printf("===========================================\n");
}

static pkg_t *
pkg_new(const char *f)
{
	pkg_t *p = NULL;
	size_t pos = strlen(f);

	p = (pkg_t *)malloc(sizeof(pkg_t));
	if (p == NULL)
		err(2, "malloc");

	if (f == NULL) {
		free(p);
		err(2, "fullname null");
	}

	/* TODO: checking is null */
	p->fullname = strdup(f);

	if ((ext_exist(f)) == 1)
		p->fullname[pos-4] = '\0';

	p->name = pkg_get_name(p->fullname);
	p->ver = pkg_get_ver(p->fullname);

	return (p);
}

static void
pkg_free(pkg_t *p)
{
	if (p == NULL) return;

	if (p->fullname != NULL)
		free(p->fullname);
	if (p->name != NULL)
		free(p->name);
	free(p);
}
