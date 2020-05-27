/*
 * emacros.h
 *
 *  Created on: May 24, 2020
 *      Author: ericson
 */

#ifndef ESTRING_EMACROS_H_
#define ESTRING_EMACROS_H_

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

#define EMAX(a, b)  (((a) > (b)) ? (a) : (b))
#define EMIN(a, b)  (((a) < (b)) ? (a) : (b))

#define E_STMT_START  do
#define E_STMT_END    while (0)

#define E_LIKELY(expr) (expr)
#define E_UNLIKELY(expr) (expr)

#define e_return_val_if_fail(expr, val) \
  E_STMT_START {						\
    if (E_LIKELY (expr))				\
    { }								 	\
    else								\
    {									\
        return (val);					\
    }									\
  } E_STMT_END

#define e_return_if_fail(expr)			\
  E_STMT_START {						\
    if (E_LIKELY (expr))				\
    { }									\
    else								\
    {									\
        return;							\
    } 									\
  } E_STMT_END


#endif /* ESTRING_EMACROS_H_ */
