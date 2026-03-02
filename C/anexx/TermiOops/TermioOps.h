#ifndef TERMIOOPS_H
#define TERMIOOPS_H

#include <termios.h>

#ifdef TERMIOOPS
#undef TERMIOOPS
#endif
#define TERMIOOPS "TermioOps"

#ifndef TERMIOS_REG_MAX_NAME
#define TERMIOS_REG_MAX_NAME 32
#endif

/** Retrieve current termios attributes from STDIN. */
struct termios termiosget(void);

/** Read termios attributes and update internal states. */
char termiosread(struct termios Termios);

/** Write current termios attributes to a new structure. */
struct termios termioswrite(void);

/** Assign a value to a termios flag using its label. */
char termiosassign(char *label, char value);

/** Check if all termios flags in a structure are zero. */
char termiosiszero(struct termios Termios);

/** Register a new named termios preset. */
char termiosregister(char *name, struct termios Termios);

/** Load a registered termios preset by name. */
char termiosload(char *name);

/** Swap current and previous termios configurations. */
void termiosswap(void);

/** Print current terminal configuration. */
void termiosprtcurrent(void);

/** Print attributes of a given termios structure. */
void termiosprt(struct termios Termios);

/** Compare two termios structures and show matches. */
void termiosmatch(struct termios TermiosA, struct termios TermiosB);

/** Restore default terminal state and release resources. */
void termiosexit(void);

#endif // TERMIOOPS_H