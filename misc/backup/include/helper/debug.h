/*	Functions for debugging in umps3.
	This header's functions count on the test to define them.
*/


#ifndef DEBUG_H
#define DEBUG_H




/* This function placess the specified character string in okbuf and
 *	causes the string to be written out to terminal0 */
extern void addokbuf(char *strp);


/* This function placess the specified character string in errbuf and
 *	causes the string to be written out to terminal0.  After this is done
 *	the system shuts down with a panic message */
extern void adderrbuf(char *strp);


#endif /* DEBUG_H */