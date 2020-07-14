# $OpenBSD$

PROG=		opkgup
MAN=
SRCS= 		opkgup.c
CFLAGS+=	-Wall -Werror -std=c89

.include <bsd.prog.mk>
