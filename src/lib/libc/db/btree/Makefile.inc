#	Makefile.inc,v 1.6 1996/05/03 21:50:36 cgd Exp
#	@(#)Makefile.inc	8.2 (Berkeley) 7/14/94

.PATH: ${.CURDIR}/db/btree

SRCS+=	bt_close.c bt_conv.c bt_debug.c bt_delete.c bt_get.c bt_open.c \
	bt_overflow.c bt_page.c bt_put.c bt_search.c bt_seq.c bt_split.c \
	bt_utils.c