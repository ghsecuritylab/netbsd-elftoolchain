/*	authkeys.c,v 1.2 2010/12/04 23:08:34 christos Exp	*/

/*
 * authkeys.c - routines to manage the storage of authentication keys
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include "ntp_types.h"
#include "ntp_fp.h"
#include "ntp.h"
#include "ntpd.h"
#include "ntp_string.h"
#include "ntp_malloc.h"
#include "ntp_stdlib.h"

/*
 * Structure to store keys in in the hash table.
 */
struct savekey {
	struct savekey *next;
	union {
		u_char MD5_key[64];	/* for keys up to to 512 bits */
	} k;
	keyid_t keyid;		/* key identifier */
	int	type;		/* key type */
	u_short flags;		/* flags that wave */
	u_long lifetime;	/* remaining lifetime */
	int keylen;		/* key length */
};

#define	KEY_TRUSTED	0x001	/* this key is trusted */

/*
 * The hash table. This is indexed by the low order bits of the
 * keyid. We make this fairly big for potentially busy servers.
 */
#define	HASHSIZE	64
#define	HASHMASK	((HASHSIZE)-1)
#define	KEYHASH(keyid)	((keyid) & HASHMASK)

struct savekey *key_hash[HASHSIZE];

u_long authkeynotfound;		/* keys not found */
u_long authkeylookups;		/* calls to lookup keys */
u_long authnumkeys;		/* number of active keys */
u_long authkeyexpired;		/* key lifetime expirations */
u_long authkeyuncached;		/* cache misses */
u_long authnokey;		/* calls to encrypt with no key */
u_long authencryptions;		/* calls to encrypt */
u_long authdecryptions;		/* calls to decrypt */

/*
 * Storage for free key structures.  We malloc() such things but
 * never free them.
 */
struct savekey *authfreekeys;
int authnumfreekeys;

#define	MEMINC	12		/* number of new free ones to get */

/*
 * The key cache. We cache the last key we looked at here.
 */
keyid_t	cache_keyid;		/* key identifier */
u_char	*cache_key;		/* key pointer */
u_int	cache_keylen;		/* key length */
int	cache_type;		/* key type */
u_short cache_flags;		/* flags that wave */


/*
 * init_auth - initialize internal data
 */
void
init_auth(void)
{
	/*
	 * Initialize hash table and free list
	 */
	memset((char *)key_hash, 0, sizeof key_hash);
}


/*
 * auth_findkey - find a key in the hash table
 */
struct savekey *
auth_findkey(
	keyid_t keyno
	)
{
	struct savekey *sk;

	sk = key_hash[KEYHASH(keyno)];
	while (sk != 0) {
		if (keyno == sk->keyid)
			return (sk);

		sk = sk->next;
	}
	return (0);
}


/*
 * auth_havekey - return one if the key is known
 */
int
auth_havekey(
	keyid_t keyno
	)
{
	struct savekey *sk;

	if (keyno == 0 || (keyno == cache_keyid))
		return (1);

	sk = key_hash[KEYHASH(keyno)];
	while (sk != 0) {
		if (keyno == sk->keyid)
			return (1);

		sk = sk->next;
	}
	return (0);
}


/*
 * authhavekey - return one and cache the key, if known and trusted.
 */
int
authhavekey(
	keyid_t keyno
	)
{
	struct savekey *sk;

	authkeylookups++;
	if (keyno == 0 || keyno == cache_keyid)
		return (1);

	/*
	 * Seach the bin for the key. If found and the key type
	 * is zero, somebody marked it trusted without specifying
	 * a key or key type. In this case consider the key missing.
	 */
	authkeyuncached++;
	sk = key_hash[KEYHASH(keyno)];
	while (sk != NULL) {
		if (keyno == sk->keyid) {
			if (sk->type == 0) {
				authkeynotfound++;
				return (0);
			}
			break;
		}
		sk = sk->next;
	}

	/*
	 * If the key is not found, or if it is found but not trusted,
	 * the key is not considered found.
	 */
	if (sk == NULL) {
		authkeynotfound++;
		return (0);

	}
	if (!(sk->flags & KEY_TRUSTED)) {
		authnokey++;
		return (0);
	}

	/*
	 * The key is found and trusted. Initialize the key cache.
	 */
	cache_keyid = sk->keyid;
	cache_type = sk->type;
	cache_flags = sk->flags;
	cache_key = sk->k.MD5_key;
	cache_keylen = sk->keylen;
	return (1);
}


/*
 * auth_moremem - get some more free key structures
 */
int
auth_moremem(void)
{
	struct savekey *sk;
	int i;

	sk = (struct savekey *)calloc(MEMINC, sizeof(struct savekey));
	if (sk == 0)
		return (0);
	
	for (i = MEMINC; i > 0; i--) {
		sk->next = authfreekeys;
		authfreekeys = sk++;
	}
	authnumfreekeys += MEMINC;
	return (authnumfreekeys);
}


/*
 * authtrust - declare a key to be trusted/untrusted
 */
void
authtrust(
	keyid_t keyno,
	u_long trust
	)
{
	struct savekey *sk;

	/*
	 * Search bin for key; if it does not exist and is untrusted,
	 * forget it.
	 */
	sk = key_hash[KEYHASH(keyno)];
	while (sk != 0) {
		if (keyno == sk->keyid)
		    break;

		sk = sk->next;
	}
	if (sk == 0 && !trust)
		return;

	/*
	 * There are two conditions remaining. Either it does not
	 * exist and is to be trusted or it does exist and is or is
	 * not to be trusted.
	 */	
	if (sk != 0) {
		if (cache_keyid == keyno) {
			cache_flags = 0;
			cache_keyid = 0;
		}

		/*
		 * Key exists. If it is to be trusted, say so and
		 * update its lifetime. If not, return it to the
		 * free list.
		 */
		if (trust > 0) {
			sk->flags |= KEY_TRUSTED;
			if (trust > 1)
				sk->lifetime = current_time + trust;
			else
				sk->lifetime = 0;
			return;
		}
		sk->flags &= ~KEY_TRUSTED; {
			struct savekey *skp;

			skp = key_hash[KEYHASH(keyno)];
			if (skp == sk) {
				key_hash[KEYHASH(keyno)] = sk->next;
			} else {
				while (skp->next != sk)
				    skp = skp->next;
				skp->next = sk->next;
			}
			authnumkeys--;

			sk->next = authfreekeys;
			authfreekeys = sk;
			authnumfreekeys++;
		}
		return;
	}

	/*
	 * Here there is not key, but the key is to be trusted. There
	 * seems to be a disconnect here. Here we allocate a new key,
	 * but do not specify a key type, key or key length.
	 */ 
	if (authnumfreekeys == 0)
	    if (auth_moremem() == 0)
		return;

	sk = authfreekeys;
	authfreekeys = sk->next;
	authnumfreekeys--;
	sk->keyid = keyno;
	sk->type = 0;
	sk->keylen = 0;
	sk->flags = KEY_TRUSTED;
	sk->next = key_hash[KEYHASH(keyno)];
	key_hash[KEYHASH(keyno)] = sk;
	authnumkeys++;
	return;
}


/*
 * authistrusted - determine whether a key is trusted
 */
int
authistrusted(
	keyid_t keyno
	)
{
	struct savekey *sk;

	if (keyno == cache_keyid)
	    return ((cache_flags & KEY_TRUSTED) != 0);

	authkeyuncached++;
	sk = key_hash[KEYHASH(keyno)];
	while (sk != 0) {
		if (keyno == sk->keyid)
		    break;
		sk = sk->next;
	}
	if (sk == 0) {
		authkeynotfound++;
		return (0);

	} else if (!(sk->flags & KEY_TRUSTED)) {
		authkeynotfound++;
		return (0);
	}
	return (1);
}


void
MD5auth_setkey(
	keyid_t keyno,
	int	keytype,
	const u_char *key,
	size_t len
	)
{
	struct savekey *sk;
	
	/*
	 * See if we already have the key.  If so just stick in the
	 * new value.
	 */
	sk = key_hash[KEYHASH(keyno)];
	while (sk != NULL) {
		if (keyno == sk->keyid) {
			sk->type = keytype;
			sk->keylen = min(len, sizeof(sk->k.MD5_key));
#ifndef DISABLE_BUG1243_FIX
			memcpy(sk->k.MD5_key, key, sk->keylen);
#else
			strncpy((char *)sk->k.MD5_key, (const char *)key,
			    sizeof(sk->k.MD5_key));
#endif
			if (cache_keyid == keyno) {
				cache_flags = 0;
				cache_keyid = 0;
			}
			return;
		}
		sk = sk->next;
	}

	/*
	 * Need to allocate new structure.  Do it.
	 */
	if (0 == authnumfreekeys && !auth_moremem())
		return;

	sk = authfreekeys;
	authfreekeys = sk->next;
	authnumfreekeys--;

	sk->keyid = keyno;
	sk->type = keytype;
	sk->flags = 0;
	sk->lifetime = 0;
	sk->keylen = min(len, sizeof(sk->k.MD5_key));
#ifndef DISABLE_BUG1243_FIX
	memcpy(sk->k.MD5_key, key, sk->keylen);
#else
	strncpy((char *)sk->k.MD5_key, (const char *)key,
	    sizeof(sk->k.MD5_key));
#endif
	sk->next = key_hash[KEYHASH(keyno)];
	key_hash[KEYHASH(keyno)] = sk;
#ifdef DEBUG
	if (debug > 1) {
		char	hex[] = "0123456789abcdef";
		int	j;

		printf("auth_setkey: key %d type %d len %d ", sk->keyid,
		    sk->type, sk->keylen);
		for (j = 0; j < sk->keylen; j++)
				printf("%c%c", hex[key[j] >> 4],
				    hex[key[j] & 0xf]);
		printf("\n");
	}	
#endif
	authnumkeys++;
}


/*
 * auth_delkeys - delete all known keys, in preparation for rereading
 *		  the keys file (presumably)
 */
void
auth_delkeys(void)
{
	struct savekey *sk;
	struct savekey **skp;
	int i;

	for (i = 0; i < HASHSIZE; i++) {
		skp = &(key_hash[i]);
		sk = key_hash[i];
		/*
		 * Leave autokey keys alone.
		 */
		while (sk != 0 && sk->keyid <= NTP_MAXKEY) {
			/*
			 * Don't lose info as to which keys are trusted.
			 */
			if (sk->flags & KEY_TRUSTED) {
				skp = &(sk->next);
				memset(&sk->k, 0, sizeof(sk->k));
				sk->lifetime = 0;
				sk->keylen = 0;
				sk = sk->next;
			} else {
				*skp = sk->next;
				authnumkeys--;
				sk->next = authfreekeys;
				authfreekeys = sk;
				authnumfreekeys++;
				sk = *skp;
			}
		}
	}
}

/*
 * auth_agekeys - delete keys whose lifetimes have expired
 */
void
auth_agekeys(void)
{
	struct savekey *sk;
	struct savekey *skp;
	int i;

	for (i = 0; i < HASHSIZE; i++) {
		sk = skp = key_hash[i];
		while (sk != 0) {
			skp = sk->next;
			if (sk->lifetime > 0 && current_time >
			    sk->lifetime) {
				authtrust(sk->keyid, 0);
				authkeyexpired++;
			}
			sk = skp;
		}
	}
#ifdef DEBUG
	if (debug)
		printf("auth_agekeys: at %lu keys %lu expired %lu\n",
		    current_time, authnumkeys, authkeyexpired);
#endif
}

/*
 * authencrypt - generate message authenticator
 *
 * Returns length of authenticator field, zero if key not found.
 */
int
authencrypt(
	keyid_t keyno,
	u_int32 *pkt,
	int length
	)
{

	/*
	 * A zero key identifier means the sender has not verified
	 * the last message was correctly authenticated. The MAC
	 * consists of a single word with value zero.
	 */
	authencryptions++;
	pkt[length / 4] = htonl(keyno);
	if (keyno == 0) {
		return (4);
	}
	if (!authhavekey(keyno))
		return (0);

	return (MD5authencrypt(cache_type, cache_key, pkt, length));
}

/*
 * authdecrypt - verify message authenticator
 *
 * Returns one if authenticator valid, zero if invalid or key not found.
 */
int
authdecrypt(
	keyid_t keyno,
	u_int32 *pkt,
	int length,
	int size
	)
{

	/*
	 * A zero key identifier means the sender has not verified
	 * the last message was correctly authenticated. Nevertheless,
	 * the authenticator itself is considered valid.
	 */
	authdecryptions++;
	if (keyno == 0)
		return (0);

	if (!authhavekey(keyno) || size < 4)
		return (0);

	return (MD5authdecrypt(cache_type, cache_key, pkt, length,
	   size));
}