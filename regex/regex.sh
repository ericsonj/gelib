
echo x - grep.c 1>&2
sed 's/^X//' >grep.c <<'@@@End of grep.c'
X#ifdef vms
X#include stdio
X#else
X#include <stdio.h>
X#endif
X
X/*
X * Rudimentary grep to test regex routines.
X *
X * DEBUG is only applicable to oz version of regex. Make sure to 
X * compile regex.c with -DDEBUG as well.
X *
X */
Xextern char *re_comp();
Xextern re_exec();
X
Xmain(argc,argv)
Xchar *argv[];
X{
X	char str[512];
X	FILE *f;
X	register int n;
X	register char *p;
X
X	if (argc < 2)
X		error("usage: grep pat [file]");
X
X	if ((p = re_comp(argv[1])) != 0) {
X		printf("\t%s: %s\n", p, argv[1]);
X		exit(1);
X	}
X#ifdef DEBUG
X	symbolic(argv[1]);
X#endif
X	if (p = argv[2]) {
X		if ((f = fopen(p, "r")) == NULL) {
X			printf("cannot open %s\n", argv[2]);
X			exit(1);
X		}
X		while ((n = load(str, f)) != EOF)
X			if (re_exec(str))
X				printf("%s\n",str);
X
X	}
X	exit(0);
X}
Xload (s, f)
Xchar *s;
XFILE *f;
X{
X	register int c;
X	static int lineno = 0;
X
X	while ((c = getc(f)) != '\n' && c != EOF)
X		*s++ = c;
X	if (c == EOF)
X		return (EOF);
X	*s = (char) 0;
X	return (++lineno);
X}
X
Xerror(s) char *s ; 
X{ 
X	fprintf(stderr,"%s\n",s); 
X	exit(1); 
X}
@@@End of grep.c
echo x - makefile 1>&2
sed 's/^X//' >makefile <<'@@@End of makefile'
X#
X# public-domain regex (bsd) routines
X#
XCFLAGS = -O
X
XRSRC = regex.o re_fail.o
X
Xregex:  $(RSRC)
X#	if necessary, insert code to put these into a library
X
Xrlint:
X	lint -phc regex.c
X
Xdebug:
X	cc -O -DDEBUG -o ogrep grep.c regex.c re_fail.c
X
Xlgrep:  grep.o
X	cc -o lgrep grep.o
X
Xogrep:  grep.o $(RSRC)
X	cc -o ogrep grep.o $(RSRC)
Xclean:
X	rm -f *.o lgrep ogrep core
X
@@@End of makefile
echo x - re_fail.c 1>&2
sed 's/^X//' >re_fail.c <<'@@@End of re_fail.c'
X#ifdef vms
X#include stdio
X#else
X#include <stdio.h>
X#endif
X
X/* 
X * re_fail:
X *	default internal error handler for re_exec.
X *
X *	should probably do something like a longjump to recover
X *	gracefully.
X */ 
Xvoid	
Xre_fail(char *s, char c)
X{
X	fprintf(stderr, "%s [opcode %o]\n", s, c);
X	exit(1);
X}
@@@End of re_fail.c
echo x - regex.3 1>&2
sed 's/^X//' >regex.3 <<'@@@End of regex.3'
X.TH regex 3 local
X.DA Jun 19 1986
X.SH NAME
Xre_comp, re_exec, re_subs, re_modw, re_fail  \- regular expression handling
X.SH ORIGIN
XDept. of Computer Science
X.br
XYork University
X.SH SYNOPSIS
X.B char *re_comp(pat)
X.br
X.B char *pat;
X.PP
X.B re_exec(str)
X.br
X.B char *str;
X.PP
X.B re_subs(src, dst)
X.br
X.B char *src;
X.br
X.B char *dst;
X.PP
X.B void re_fail(msg, op)
X.br
X.B char *msg;
X.br
X.B char op;
X.PP
X.B void re_modw(str)
X.br
X.B char *str;
X
X.SH DESCRIPTION
X.PP
XThese functions implement
X.IR ed (1)-style
Xpartial regular expressions and supporting facilities.
X.PP
X.I Re_comp
Xcompiles a pattern string into an internal form (a deterministic finite-state
Xautomaton) to be executed by
X.I re_exec
Xfor pattern matching.
X.I Re_comp
Xreturns 0 if the pattern is compiled successfully, otherwise it returns an
Xerror message string. If
X.I re_comp
Xis called with a 0 or a \fInull\fR string, it returns without changing the
Xcurrently compiled regular expression.
X.sp
X.I Re_comp
Xsupports the same limited set of
X.I regular expressions
Xfound in
X.I ed
Xand Berkeley
X.IR regex (3)
Xroutines:
X.sp
X.if n .in +1.6i
X.if t .in +1i
X.de Ti
X.if n .ti -1.6i
X.if t .ti -1i
X.. 
X.if n .ta 0.8i +0.8i +0.8i
X.if t .ta 0.5i +0.5i +0.5i
X.Ti 
X[1]	\fIchar\fR	Matches itself, unless it is a special
Xcharacter (meta-character): \fB. \\ [ ] * + ^ $\fR
X
X.Ti 
X[2]	\fB.\fR	Matches \fIany\fR character.
X
X.Ti 
X[3]	\fB\\\fR	Matches the character following it, except
Xwhen followed by a digit 1 to 9, \fB(\fR, fB)\fR, \fB<\fR or \fB>\fR.
X(see [7], [8] and [9]) It is used as an escape character for all 
Xother meta-characters, and itself. When used
Xin a set ([4]), it is treated as an ordinary
Xcharacter.
X
X.Ti
X[4]	\fB[\fIset\fB]\fR	Matches one of the characters in the set.
XIf the first character in the set is \fB^\fR,
Xit matches a character NOT in the set. A
Xshorthand 
X.IR S - E
Xis used to specify a set of
Xcharacters 
X.I S 
Xup to 
X.IR E , 
Xinclusive. The special
Xcharacters \fB]\fR and \fB-\fR have no special
Xmeaning if they appear as the first chars
Xin the set.
X.nf
X	examples:	match:
X	[a-z]		any lowercase alpha
X	[^]-]		any char except ] and -
X	[^A-Z]		any char except 
X			uppercase alpha
X	[a-zA-Z0-9]	any alphanumeric
X.fi
X
X.Ti 
X[5]	\fB*\fR	Any regular expression form [1] to [4], followed by
Xclosure char (*) matches zero or more matches of
Xthat form.
X
X.Ti 
X[6]	\fB+\fR	Same as [5], except it matches one or more.
X
X.Ti 
X[7]		A regular expression in the form [1] to [10], enclosed
Xas \\(\fIform\fR\\) matches what form matches. The enclosure
Xcreates a set of tags, used for [8] and for
Xpattern substitution in
X.I re_subs. 
XThe tagged forms are numbered
Xstarting from 1.
X
X.Ti 
X[8]		A \\ followed by a digit 1 to 9 matches whatever a
Xpreviously tagged regular expression ([7]) matched.
X
X.Ti
X[9]	\fB\\<\fR	Matches the beginning of a \fIword\fR,
Xthat is, an empty string followed by a
Xletter, digit, or _ and not preceded by
Xa letter, digit, or _ .
X.Ti
X	\fB\\>\fR	Matches the end of a \fIword\fR,
Xthat is, an empty string preceded
Xby a letter, digit, or _ , and not
Xfollowed by a letter, digit, or _ .
X
X.Ti 
X[10]		A composite regular expression 
X\fIxy\fR where \fIx\fR and \fIy\fR
Xare in the form of [1] to [10] matches the longest
Xmatch of \fIx\fR followed by a match for \fIy\fR.
X
X.Ti 
X[11]	\fB^ $\fR	a regular expression starting with a \fB^\fR character
Xand/or ending with a \fB$\fR character, restricts the
Xpattern matching to the beginning of the line,
Xand/or the end of line [anchors]. Elsewhere in the
Xpattern, \fB^\fR and \fB$\fR are treated as ordinary characters.
X.if n .in -1.6i
X.if t .in -1i
X
X.PP
X.I Re_exec
Xexecutes the internal form produced by
X.I re_comp
Xand searches the argument string for the regular expression described
Xby the internal
Xform. 
X.I Re_exec
Xreturns 1 if the last regular expression pattern is matched within the string,
X0 if no match is found. In case of an internal error (corrupted internal
Xform), 
X.I re_exec 
Xcalls the user-supplied
X.I re_fail
Xand returns 0.
X.PP
XThe strings passed to both
X.I re_comp
Xand
X.I re_exec
Xmay have trailing or embedded newline characters. The strings 
Xmust be terminated by nulls.
X.PP
X.I Re_subs
Xdoes
X.IR ed -style
Xpattern substitution, after a successful match is found by
X.I re_exec.
XThe source string parameter to
X.I re_subs
Xis copied to the destination string with the following interpretation;
X.sp
X.if n .in +1.6i
X.if t .in +1i
X.Ti
X[1]	&	Substitute the entire matched string in the destination.
X
X.Ti
X[2]	\\\fIn\fR	Substitute the substring matched by a tagged subpattern
Xnumbered \fIn\fR, where \fIn\fR is between 1 to 9, inclusive.
X
X.Ti
X[3]	\\\fIchar\fR	Treat the next character literally,
Xunless the character is a digit ([2]).
X.if n .in -1.6i
X.if t .in -1i
X
X.PP
XIf the copy operation with the substitutions is successful,
X.I re_subs
Xreturns 1.
XIf the source string is corrupted, or the last call to
X.I re_exec
Xfails, it returns 0.
X
X.I Re_modw
Xis used to 
Xadd new characters into an internal table to
Xchange the re_exec's understanding of what
Xa \fIword\fR should look like, when matching with \fB\\<\fR and \fB\\>\fR
Xconstructs. If the string parameter is 0 or null string,
Xthe table is reset back to the default, which contains \fBA-Z a-z 0-9 _\fR .
X
X.I Re_fail
Xis a user-supplied routine to handle internal errors.
X.I re_exec
Xcalls
X.I re_fail
Xwith an error message string, and the opcode character that caused the error.
XThe default
X.I re_fail
Xroutine simply prints the message and the opcode character to
X.I stderr
Xand invokes
X.IR exit (2).
X.SH EXAMPLES
XIn the examples below, the
X.I nfaform
Xdescribes the internal form after the pattern is compiled. For additional
Xdetails, refer to the sources.
X.PP
X.ta 0.5i +0.5i +0.5i
X.nf
Xfoo*.*
X	nfaform:	CHR f CHR o CLO CHR o END CLO ANY END END
X	matches:	\fIfo foo fooo foobar fobar foxx ...\fR
X
Xfo[ob]a[rz]
X	nfaform:	CHR f CHR o CCL 2 o b CHR a CCL 2 r z END
X	matches:	\fIfobar fooar fobaz fooaz\fR
X
Xfoo\\\\+
X	nfaform:	CHR f CHR o CHR o CHR \\ CLO CHR \\ END END
X	matches:	\fIfoo\\ foo\\\\ foo\\\\\\  ...\fR
X
X\\(foo\\)[1-3]\\1	(same as foo[1-3]foo, but takes less internal space)
X	nfaform:	BOT 1 CHR f CHR o CHR o EOT 1 CCL 3 1 2 3 REF 1 END
X	matches:	\fIfoo1foo foo2foo foo3foo\fR
X
X\\(fo.*\\)-\\1
X	nfaform:	BOT 1 CHR f CHR o CLO ANY END EOT 1 CHR - REF 1 END
X	matches:	\fIfoo-foo fo-fo fob-fob foobar-foobar ...\fR
X.SH DIAGNOSTICS
X.I Re_comp
Xreturns one of the following strings if an error occurs:
X.PP
X.nf
X.in +0.5i
X\fINo previous regular expression,
XEmpty closure,
XIllegal closure,
XCyclical reference,
XUndetermined reference,
XUnmatched \e(,
XMissing ],
XNull pattern inside \e(\e),
XNull pattern inside \e<\e>,
XToo many \e(\e) pairs,
XUnmatched \e)\fP.
X.in -0.5i
X.fi
X.SH REFERENCES
X.if n .ta 3i
X.if t .ta 1.8i
X.nf
X\fISoftware tools\fR	Kernighan & Plauger
X\fISoftware tools in Pascal\fR	Kernighan & Plauger
X\fIGrep sources\fR [rsx-11 C dist]	David Conroy
X\fIEd - text editor\fR	Unix Programmer's Manual
X\fIAdvanced editing on Unix\fR	B. W. Kernighan
X\fIRegExp sources\fR	Henry Spencer
X.fi
X.SH "HISTORY AND NOTES"
XThese routines are derived from various implementations
Xfound in 
X.I "Software Tools"
Xbooks, and David Conroy's 
X.I grep. 
XThey are NOT derived from licensed/restricted software.
XFor more interesting/academic/complicated implementations,
Xsee Henry Spencer's 
X.I regexp 
Xroutines (V8), or 
X.I "GNU Emacs"
Xpattern
Xmatching module.
X.PP
XThe
X.I re_comp
Xand
X.I re_exec
Xroutines perform
X.I almost
Xas well as their licensed counterparts, sometimes better. 
XIn very few instances, they
Xare about 10% to 15% slower.
X.SH AUTHOR
XOzan S. Yigit (oz)
X.br
Xusenet: utzoo!yetti!oz
X.br
Xbitnet: oz@yusol || oz@yuyetti
X.SH "SEE ALSO"
Xed(1), ex(1), egrep(1), fgrep(1), grep(1), regex(3)
X.SH BUGS
XThese routines are \fIPublic Domain\fR. You can get them
Xin source.
X.br
XThe internal storage for the \fInfa form\fR is not checked for
Xoverflows. Currently, it is 1024 bytes.
X.br
XOthers, no doubt.
@@@End of regex.3
echo x - regex.c 1>&2
sed 's/^X//' >regex.c <<'@@@End of regex.c'
X/*
X * regex - Regular expression pattern matching  and replacement
X *
X * By:  Ozan S. Yigit (oz)
X *      Dept. of Computer Science
X *      York University
X *
X * These routines are the PUBLIC DOMAIN equivalents of regex
X * routines as found in 4.nBSD UN*X, with minor extensions.
X *
X * These routines are derived from various implementations found
X * in software tools books, and Conroy's grep. They are NOT derived
X * from licensed/restricted software.
X * For more interesting/academic/complicated implementations,
X * see Henry Spencer's regexp routines, or GNU Emacs pattern
X * matching module.
X *
X * Modification history:
X *
X * $Log: regex.c,v $
X * Revision 1.4  1991/10/17  03:56:42  oz
X * miscellaneous changes, small cleanups etc.
X *
X * Revision 1.3  1989/04/01  14:18:09  oz
X * Change all references to a dfa: this is actually an nfa.
X *
X * Revision 1.2  88/08/28  15:36:04  oz
X * Use a complement bitmap to represent NCL.
X * This removes the need to have seperate 
X * code in the pmatch case block - it is 
X * just CCL code now.
X * 
X * Use the actual CCL code in the CLO
X * section of pmatch. No need for a recursive
X * pmatch call.
X * 
X * Use a bitmap table to set char bits in an
X * 8-bit chunk.
X * 
X * Interfaces:
X *      re_comp:        compile a regular expression into a NFA.
X *
X *			char *re_comp(s)
X *			char *s;
X *
X *      re_exec:        execute the NFA to match a pattern.
X *
X *			int re_exec(s)
X *			char *s;
X *
X *	re_modw		change re_exec's understanding of what a "word"
X *			looks like (for \< and \>) by adding into the
X *			hidden word-syntax table.
X *
X *			void re_modw(s)
X *			char *s;
X *
X *      re_subs:	substitute the matched portions in a new string.
X *
X *			int re_subs(src, dst)
X *			char *src;
X *			char *dst;
X *
X *	re_fail:	failure routine for re_exec.
X *
X *			void re_fail(msg, op)
X *			char *msg;
X *			char op;
X *  
X * Regular Expressions:
X *
X *      [1]     char    matches itself, unless it is a special
X *                      character (metachar): . \ [ ] * + ^ $
X *
X *      [2]     .       matches any character.
X *
X *      [3]     \       matches the character following it, except
X *			when followed by a left or right round bracket,
X *			a digit 1 to 9 or a left or right angle bracket. 
X *			(see [7], [8] and [9])
X *			It is used as an escape character for all 
X *			other meta-characters, and itself. When used
X *			in a set ([4]), it is treated as an ordinary
X *			character.
X *
X *      [4]     [set]   matches one of the characters in the set.
X *                      If the first character in the set is "^",
X *                      it matches a character NOT in the set, i.e. 
X *			complements the set. A shorthand S-E is 
X *			used to specify a set of characters S upto 
X *			E, inclusive. The special characters "]" and 
X *			"-" have no special meaning if they appear 
X *			as the first chars in the set.
X *                      examples:        match:
X *
X *                              [a-z]    any lowercase alpha
X *
X *                              [^]-]    any char except ] and -
X *
X *                              [^A-Z]   any char except uppercase
X *                                       alpha
X *
X *                              [a-zA-Z] any alpha
X *
X *      [5]     *       any regular expression form [1] to [4], followed by
X *                      closure char (*) matches zero or more matches of
X *                      that form.
X *
X *      [6]     +       same as [5], except it matches one or more.
X *
X *      [7]             a regular expression in the form [1] to [10], enclosed
X *                      as \(form\) matches what form matches. The enclosure
X *                      creates a set of tags, used for [8] and for
X *                      pattern substution. The tagged forms are numbered
X *			starting from 1.
X *
X *      [8]             a \ followed by a digit 1 to 9 matches whatever a
X *                      previously tagged regular expression ([7]) matched.
X *
X *	[9]	\<	a regular expression starting with a \< construct
X *		\>	and/or ending with a \> construct, restricts the
X *			pattern matching to the beginning of a word, and/or
X *			the end of a word. A word is defined to be a character
X *			string beginning and/or ending with the characters
X *			A-Z a-z 0-9 and _. It must also be preceded and/or
X *			followed by any character outside those mentioned.
X *
X *      [10]            a composite regular expression xy where x and y
X *                      are in the form [1] to [10] matches the longest
X *                      match of x followed by a match for y.
X *
X *      [11]	^	a regular expression starting with a ^ character
X *		$	and/or ending with a $ character, restricts the
X *                      pattern matching to the beginning of the line,
X *                      or the end of line. [anchors] Elsewhere in the
X *			pattern, ^ and $ are treated as ordinary characters.
X *
X *
X * Acknowledgements:
X *
X *	HCR's Hugh Redelmeier has been most helpful in various
X *	stages of development. He convinced me to include BOW
X *	and EOW constructs, originally invented by Rob Pike at
X *	the University of Toronto.
X *
X * References:
X *              Software tools			Kernighan & Plauger
X *              Software tools in Pascal        Kernighan & Plauger
X *              Grep [rsx-11 C dist]            David Conroy
X *		ed - text editor		Un*x Programmer's Manual
X *		Advanced editing on Un*x	B. W. Kernighan
X *		RegExp routines			Henry Spencer
X *
X * Notes:
X *
X *	This implementation uses a bit-set representation for character
X *	classes for speed and compactness. Each character is represented 
X *	by one bit in a 128-bit block. Thus, CCL always takes a 
X *	constant 16 bytes in the internal nfa, and re_exec does a single
X *	bit comparison to locate the character in the set.
X *
X * Examples:
X *
X *	pattern:	foo*.*
X *	compile:	CHR f CHR o CLO CHR o END CLO ANY END END
X *	matches:	fo foo fooo foobar fobar foxx ...
X *
X *	pattern:	fo[ob]a[rz]	
X *	compile:	CHR f CHR o CCL bitset CHR a CCL bitset END
X *	matches:	fobar fooar fobaz fooaz
X *
X *	pattern:	foo\\+
X *	compile:	CHR f CHR o CHR o CHR \ CLO CHR \ END END
X *	matches:	foo\ foo\\ foo\\\  ...
X *
X *	pattern:	\(foo\)[1-3]\1	(same as foo[1-3]foo)
X *	compile:	BOT 1 CHR f CHR o CHR o EOT 1 CCL bitset REF 1 END
X *	matches:	foo1foo foo2foo foo3foo
X *
X *	pattern:	\(fo.*\)-\1
X *	compile:	BOT 1 CHR f CHR o CLO ANY END EOT 1 CHR - REF 1 END
X *	matches:	foo-foo fo-fo fob-fob foobar-foobar ...
X */
X
X#include "regex.h"
X
X#define MAXNFA  1024
X#define MAXTAG  10
X
X#define OKP     1
X#define NOP     0
X
X#define CHR     1
X#define ANY     2
X#define CCL     3
X#define BOL     4
X#define EOL     5
X#define BOT     6
X#define EOT     7
X#define BOW	8
X#define EOW	9
X#define REF     10
X#define CLO     11
X
X#define END     0
X
X/*
X * The following defines are not meant to be changeable.
X * They are for readability only.
X */
X#define MAXCHR	128
X#define CHRBIT	8
X#define BITBLK	MAXCHR/CHRBIT
X#define BLKIND	0170
X#define BITIND	07
X
X#define ASCIIB	0177
X
X#ifdef NO_UCHAR
Xtypedef char CHAR;
X#else
Xtypedef unsigned char CHAR;
X#endif
X
Xstatic int  tagstk[MAXTAG];             /* subpat tag stack..*/
Xstatic CHAR nfa[MAXNFA];		/* automaton..       */
Xstatic int  sta = NOP;               	/* status of lastpat */
X
Xstatic CHAR bittab[BITBLK];		/* bit table for CCL */
X					/* pre-set bits...   */
Xstatic CHAR bitarr[] = {1,2,4,8,16,32,64,128};
X
X
X#ifdef DEBUG
Xstatic void nfadump(CHAR *);
Xvoid symbolic(char *);
X#endif
X
Xstatic void
Xchset(CHAR c)
X{
X	bittab[(CHAR) ((c) & BLKIND) >> 3] |= bitarr[(c) & BITIND];
X}
X
X#define badpat(x)	(*nfa = END, x)
X#define store(x)	*mp++ = x
X 
Xchar *     
Xre_comp(char *pat)
X{
X	register char *p;               /* pattern pointer   */
X	register CHAR *mp=nfa;          /* nfa pointer       */
X	register CHAR *lp;              /* saved pointer..   */
X	register CHAR *sp=nfa;          /* another one..     */
X
X	register int tagi = 0;          /* tag stack index   */
X	register int tagc = 1;          /* actual tag count  */
X
X	register int n;
X	register CHAR mask;		/* xor mask -CCL/NCL */
X	int c1, c2;
X		
X	if (!pat || !*pat)
X		if (sta)
X			return 0;
X		else
X			return badpat("No previous regular expression");
X	sta = NOP;
X
X	for (p = pat; *p; p++) {
X		lp = mp;
X		switch(*p) {
X
X		case '.':               /* match any char..  */
X			store(ANY);
X			break;
X
X		case '^':               /* match beginning.. */
X			if (p == pat)
X				store(BOL);
X			else {
X				store(CHR);
X				store(*p);
X			}
X			break;
X
X		case '$':               /* match endofline.. */
X			if (!*(p+1))
X				store(EOL);
X			else {
X				store(CHR);
X				store(*p);
X			}
X			break;
X
X		case '[':               /* match char class..*/
X			store(CCL);
X
X			if (*++p == '^') {
X				mask = 0377;	
X				p++;
X			}
X			else
X				mask = 0;
X
X			if (*p == '-')		/* real dash */
X				chset(*p++);
X			if (*p == ']')		/* real brac */
X				chset(*p++);
X			while (*p && *p != ']') {
X				if (*p == '-' && *(p+1) && *(p+1) != ']') {
X					p++;
X					c1 = *(p-2) + 1;
X					c2 = *p++;
X					while (c1 <= c2)
X						chset((CHAR)c1++);
X				}
X#ifdef EXTEND
X				else if (*p == '\\' && *(p+1)) {
X					p++;
X					chset(*p++);
X				}
X#endif
X				else
X					chset(*p++);
X			}
X			if (!*p)
X				return badpat("Missing ]");
X
X			for (n = 0; n < BITBLK; bittab[n++] = (char) 0)
X				store(mask ^ bittab[n]);
X	
X			break;
X
X		case '*':               /* match 0 or more.. */
X		case '+':               /* match 1 or more.. */
X			if (p == pat)
X				return badpat("Empty closure");
X			lp = sp;		/* previous opcode */
X			if (*lp == CLO)		/* equivalence..   */
X				break;
X			switch(*lp) {
X
X			case BOL:
X			case BOT:
X			case EOT:
X			case BOW:
X			case EOW:
X			case REF:
X				return badpat("Illegal closure");
X			default:
X				break;
X			}
X
X			if (*p == '+')
X				for (sp = mp; lp < sp; lp++)
X					store(*lp);
X
X			store(END);
X			store(END);
X			sp = mp;
X			while (--mp > lp)
X				*mp = mp[-1];
X			store(CLO);
X			mp = sp;
X			break;
X
X		case '\\':              /* tags, backrefs .. */
X			switch(*++p) {
X
X			case '(':
X				if (tagc < MAXTAG) {
X					tagstk[++tagi] = tagc;
X					store(BOT);
X					store(tagc++);
X				}
X				else
X					return badpat("Too many \\(\\) pairs");
X				break;
X			case ')':
X				if (*sp == BOT)
X					return badpat("Null pattern inside \\(\\)");
X				if (tagi > 0) {
X					store(EOT);
X					store(tagstk[tagi--]);
X				}
X				else
X					return badpat("Unmatched \\)");
X				break;
X			case '<':
X				store(BOW);
X				break;
X			case '>':
X				if (*sp == BOW)
X					return badpat("Null pattern inside \\<\\>");
X				store(EOW);
X				break;
X			case '1':
X			case '2':
X			case '3':
X			case '4':
X			case '5':
X			case '6':
X			case '7':
X			case '8':
X			case '9':
X				n = *p-'0';
X				if (tagi > 0 && tagstk[tagi] == n)
X					return badpat("Cyclical reference");
X				if (tagc > n) {
X					store(REF);
X					store(n);
X				}
X				else
X					return badpat("Undetermined reference");
X				break;
X#ifdef EXTEND
X			case 'b':
X				store(CHR);
X				store('\b');
X				break;
X			case 'n':
X				store(CHR);
X				store('\n');
X				break;
X			case 'f':
X				store(CHR);
X				store('\f');
X				break;
X			case 'r':
X				store(CHR);
X				store('\r');
X				break;
X			case 't':
X				store(CHR);
X				store('\t');
X				break;
X#endif
X			default:
X				store(CHR);
X				store(*p);
X			}
X			break;
X
X		default :               /* an ordinary char  */
X			store(CHR);
X			store(*p);
X			break;
X		}
X		sp = lp;
X	}
X	if (tagi > 0)
X		return badpat("Unmatched \\(");
X	store(END);
X	sta = OKP;
X	return 0;
X}
X
X
Xstatic char *bol;
Xchar *bopat[MAXTAG];
Xchar *eopat[MAXTAG];
Xstatic char *pmatch(char *, CHAR *);
X
X/*
X * re_exec:
X * 	execute nfa to find a match.
X *
X *	special cases: (nfa[0])	
X *		BOL
X *			Match only once, starting from the
X *			beginning.
X *		CHR
X *			First locate the character without
X *			calling pmatch, and if found, call
X *			pmatch for the remaining string.
X *		END
X *			re_comp failed, poor luser did not
X *			check for it. Fail fast.
X *
X *	If a match is found, bopat[0] and eopat[0] are set
X *	to the beginning and the end of the matched fragment,
X *	respectively.
X *
X */
X
Xint
Xre_exec(char *lp)
X{
X	register CHAR c;
X	register char *ep = 0;
X	register CHAR *ap = nfa;
X
X	bol = lp;
X
X	bopat[0] = 0;
X	bopat[1] = 0;
X	bopat[2] = 0;
X	bopat[3] = 0;
X	bopat[4] = 0;
X	bopat[5] = 0;
X	bopat[6] = 0;
X	bopat[7] = 0;
X	bopat[8] = 0;
X	bopat[9] = 0;
X
X	switch(*ap) {
X
X	case BOL:			/* anchored: match from BOL only */
X		ep = pmatch(lp,ap);
X		break;
X	case CHR:			/* ordinary char: locate it fast */
X		c = *(ap+1);
X		while (*lp && *lp != c)
X			lp++;
X		if (!*lp)		/* if EOS, fail, else fall thru. */
X			return 0;
X	default:			/* regular matching all the way. */
X#ifdef OLD
X		while (*lp) {
X			if ((ep = pmatch(lp,ap)))
X				break;
X			lp++;
X		}
X#else	/* match null string */
X		do {
X			if ((ep = pmatch(lp,ap)))
X				break;
X			lp++;
X		} while (*lp);
X#endif
X		break;
X	case END:			/* munged automaton. fail always */
X		return 0;
X	}
X	if (!ep)
X		return 0;
X
X	bopat[0] = lp;
X	eopat[0] = ep;
X	return 1;
X}
X
X/* 
X * pmatch: internal routine for the hard part
X *
X * 	This code is partly snarfed from an early grep written by
X *	David Conroy. The backref and tag stuff, and various other
X *	innovations are by oz.
X *
X *	special case optimizations: (nfa[n], nfa[n+1])
X *		CLO ANY
X *			We KNOW .* will match everything upto the
X *			end of line. Thus, directly go to the end of
X *			line, without recursive pmatch calls. As in
X *			the other closure cases, the remaining pattern
X *			must be matched by moving backwards on the
X *			string recursively, to find a match for xy
X *			(x is ".*" and y is the remaining pattern)
X *			where the match satisfies the LONGEST match for
X *			x followed by a match for y.
X *		CLO CHR
X *			We can again scan the string forward for the
X *			single char and at the point of failure, we
X *			execute the remaining nfa recursively, same as
X *			above.
X *
X *	At the end of a successful match, bopat[n] and eopat[n]
X *	are set to the beginning and end of subpatterns matched
X *	by tagged expressions (n = 1 to 9).	
X *
X */
X
X#ifndef re_fail
Xextern void re_fail(char *, CHAR);
X#endif
X
X/*
X * character classification table for word boundary operators BOW
X * and EOW. the reason for not using ctype macros is that we can
X * let the user add into our own table. see re_modw. This table
X * is not in the bitset form, since we may wish to extend it in the
X * future for other character classifications. 
X *
X *	TRUE for 0-9 A-Z a-z _
X */
Xstatic CHAR chrtyp[MAXCHR] = {
X	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
X	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
X	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
X	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
X	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 
X	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
X	0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
X	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
X	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
X	1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 
X	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
X	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
X	1, 1, 1, 0, 0, 0, 0, 0
X	};
X
X#define inascii(x)	(0177&(x))
X#define iswordc(x) 	chrtyp[inascii(x)]
X#define isinset(x,y) 	((x)[((y)&BLKIND)>>3] & bitarr[(y)&BITIND])
X
X/*
X * skip values for CLO XXX to skip past the closure
X */
X
X#define ANYSKIP	2 	/* [CLO] ANY END ...	     */
X#define CHRSKIP	3	/* [CLO] CHR chr END ...     */
X#define CCLSKIP 18	/* [CLO] CCL 16bytes END ... */
X
Xstatic char *
Xpmatch(char *lp, CHAR *ap)
X{
X	register int op, c, n;
X	register char *e;		/* extra pointer for CLO */
X	register char *bp;		/* beginning of subpat.. */
X	register char *ep;		/* ending of subpat..	 */
X	char *are;			/* to save the line ptr. */
X
X	while ((op = *ap++) != END)
X		switch(op) {
X
X		case CHR:
X			if (*lp++ != *ap++)
X				return 0;
X			break;
X		case ANY:
X			if (!*lp++)
X				return 0;
X			break;
X		case CCL:
X			c = *lp++;
X			if (!isinset(ap,c))
X				return 0;
X			ap += BITBLK;
X			break;
X		case BOL:
X			if (lp != bol)
X				return 0;
X			break;
X		case EOL:
X			if (*lp)
X				return 0;
X			break;
X		case BOT:
X			bopat[*ap++] = lp;
X			break;
X		case EOT:
X			eopat[*ap++] = lp;
X			break;
X 		case BOW:
X			if (lp!=bol && iswordc(lp[-1]) || !iswordc(*lp))
X				return 0;
X			break;
X		case EOW:
X			if (lp==bol || !iswordc(lp[-1]) || iswordc(*lp))
X				return 0;
X			break;
X		case REF:
X			n = *ap++;
X			bp = bopat[n];
X			ep = eopat[n];
X			while (bp < ep)
X				if (*bp++ != *lp++)
X					return 0;
X			break;
X		case CLO:
X			are = lp;
X			switch(*ap) {
X
X			case ANY:
X				while (*lp)
X					lp++;
X				n = ANYSKIP;
X				break;
X			case CHR:
X				c = *(ap+1);
X				while (*lp && c == *lp)
X					lp++;
X				n = CHRSKIP;
X				break;
X			case CCL:
X				while ((c = *lp) && isinset(ap+1,c))
X					lp++;
X				n = CCLSKIP;
X				break;
X			default:
X				re_fail("closure: bad nfa.", *ap);
X				return 0;
X			}
X
X			ap += n;
X
X			while (lp >= are) {
X				if (e = pmatch(lp, ap))
X					return e;
X				--lp;
X			}
X			return 0;
X		default:
X			re_fail("re_exec: bad nfa.", op);
X			return 0;
X		}
X	return lp;
X}
X
X/*
X * re_modw:
X *	add new characters into the word table to change re_exec's
X *	understanding of what a word should look like. Note that we
X *	only accept additions into the word definition.
X *
X *	If the string parameter is 0 or null string, the table is
X *	reset back to the default containing A-Z a-z 0-9 _. [We use
X *	the compact bitset representation for the default table]
X */
X
Xstatic CHAR deftab[16] = {	
X	0, 0, 0, 0, 0, 0, 0377, 003, 0376, 0377, 0377, 0207,  
X	0376, 0377, 0377, 007 
X}; 
X
Xvoid
Xre_modw(char *s)
X{
X	register int i;
X
X	if (!s || !*s) {
X		for (i = 0; i < MAXCHR; i++)
X			if (!isinset(deftab,i))
X				iswordc(i) = 0;
X	}
X	else
X		while(*s)
X			iswordc(*s++) = 1;
X}
X
X/*
X * re_subs:
X *	substitute the matched portions of the src in dst.
X *
X *	&	substitute the entire matched pattern.
X *
X *	\digit	substitute a subpattern, with the given	tag number.
X *		Tags are numbered from 1 to 9. If the particular
X *		tagged subpattern does not exist, null is substituted.
X */
Xint
Xre_subs(char *src, char *dst)
X{
X	register char c;
X	register int  pin;
X	register char *bp;
X	register char *ep;
X
X	if (!*src || !bopat[0])
X		return 0;
X
X	while (c = *src++) {
X		switch(c) {
X
X		case '&':
X			pin = 0;
X			break;
X
X		case '\\':
X			c = *src++;
X			if (c >= '0' && c <= '9') {
X				pin = c - '0';
X				break;
X			}
X			
X		default:
X			*dst++ = c;
X			continue;
X		}
X
X		if ((bp = bopat[pin]) && (ep = eopat[pin])) {
X			while (*bp && bp < ep)
X				*dst++ = *bp++;
X			if (bp < ep)
X				return 0;
X		}
X	}
X	*dst = (char) 0;
X	return 1;
X}
X			
X#ifdef DEBUG
X/*
X * symbolic - produce a symbolic dump of the nfa
X */
Xsymbolic(char *s) 
X{
X	printf("pattern: %s\n", s);
X	printf("nfacode:\n");
X	nfadump(nfa);
X}
X
Xstatic	
Xnfadump(CHAR *ap)
X{
X	register int n;
X
X	while (*ap != END)
X		switch(*ap++) {
X		case CLO:
X			printf("CLOSURE");
X			nfadump(ap);
X			switch(*ap) {
X			case CHR:
X				n = CHRSKIP;
X				break;
X			case ANY:
X				n = ANYSKIP;
X				break;
X			case CCL:
X				n = CCLSKIP;
X				break;
X			}
X			ap += n;
X			break;
X		case CHR:
X			printf("\tCHR %c\n",*ap++);
X			break;
X		case ANY:
X			printf("\tANY .\n");
X			break;
X		case BOL:
X			printf("\tBOL -\n");
X			break;
X		case EOL:
X			printf("\tEOL -\n");
X			break;
X		case BOT:
X			printf("BOT: %d\n",*ap++);
X			break;
X		case EOT:
X			printf("EOT: %d\n",*ap++);
X			break;
X		case BOW:
X			printf("BOW\n");
X			break;
X		case EOW:
X			printf("EOW\n");
X			break;
X		case REF:
X			printf("REF: %d\n",*ap++);
X			break;
X		case CCL:
X			printf("\tCCL [");
X			for (n = 0; n < MAXCHR; n++)
X				if (isinset(ap,(CHAR)n)) {
X					if (n < ' ')
X						printf("^%c", n ^ 0x040);
X					else
X						printf("%c", n);
X				}
X			printf("]\n");
X			ap += BITBLK;
X			break;
X		default:
X			printf("bad nfa. opcode %o\n", ap[-1]);
X			exit(1);
X			break;
X		}
X}
X#endif
@@@End of regex.c
echo x - regex.h 1>&2
sed 's/^X//' >regex.h <<'@@@End of regex.h'
X#ifndef REGEX_H
X#define REGEX_H
X
Xextern char *re_comp(char *);
Xextern int re_exec(char *);
Xextern void re_modw(char *);
Xextern int re_subs(char *, char *);
X
X#endif /* REGEX_H */
@@@End of regex.h
