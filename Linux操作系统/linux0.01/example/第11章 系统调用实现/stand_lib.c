size_t strlen(const char *str)
{
    size_t ret_val;

    for(ret_val = 0; *str != '\0'; str++)
        ret_val++;
    return ret_val;
}

int strcmp(const char *str1, const char *str2)
{
    while ((*str2 != '\0') && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *str1 -  *str2;
}

const char *strstr(const char *s1, const char *s2)
{
    const char *start_s1 = NULL;
    const char *in_s2 = NULL;

    for( ; *s1 != '\0'; s1++)
    {
        if(start_s1 == NULL)
        {
            /* first char of match */
            if(*s1 == *s2)
            {
                /* remember start of matching substring in s1 */
                start_s1 = s1;
                in_s2 = s2 + 1;
                /* done already? */
                if(*in_s2 == '\0')
                    return start_s1;
            }
            /*	continued mis-match
            else
            	nothing ; */
        }
        else
        {
            /* continued match */
            if(*s1 == *in_s2)
            {
                in_s2++;
                /* done */
                if(*in_s2 == '\0')
                    return start_s1;
            }
            else
                /* first char of mis-match */
                start_s1 = NULL;
        }
    }
    return NULL;
}

void *memcpy(void *dst_ptr, const void *src_ptr, size_t count)
{
    const char *src = (const char *)src_ptr;
    char *dst = (char *)dst_ptr;
    void *ret_val = dst_ptr;

    for(; count != 0; count--)
    {
        *dst = *src;
        dst++;
        src++;
    }
    return ret_val;
}

void *memmove(void *dst_p, const void *src_p, size_t count)
{
    const char *src = (const char *)src_p;
    char *dst = (char *)dst_p;

    if(dst_p < src_p)	/* copy up */
    {
        for(; count != 0; count--)
            *dst++ = *src++;
    }
    else			/* copy down */
    {
        dst += (count - 1);
        src += (count - 1);
        for(; count != 0; count--)
            *dst-- = *src--;
    }
    return dst_p;
}

int fputc(int c, FILE *stream)
{
    char one_char;
    int ret_val;

    ret_val = c;
    if(stream->flags == _IONBF || stream->size == 0) /* unbuffered */
    {
        one_char = (char)c;
        if(write(stream->handle, &one_char, 1) != 1)
            ret_val = EOF;
    }
    else										/* buffered */
    {
        if(stream->room == 0)
        {
            if(fflush(stream) != 0)
                return EOF;
        }
        *stream->buf_ptr = (char)c;
        stream->buf_ptr++;
        stream->room--;
        if((stream->flags & _IOLBF) && (c == '\n'))
        {
            if(fflush(stream) != 0)
                ret_val = EOF;
        }
    }
    return ret_val;
}

int fflush(FILE *stream)
{
    int bytes_to_write, ret_val;

    ret_val = 0;
    if(stream->size != 0)						/* buffered? */
    {
        bytes_to_write = stream->size - stream->room;
        if(bytes_to_write != 0)
        {
            if(write(stream->handle, stream->buf_base,
                     bytes_to_write) != bytes_to_write)
                ret_val = EOF;
            stream->buf_ptr = stream->buf_base;
            stream->room = stream->size;
        }
    }
    return ret_val;
}


#include <_printf.h>		/* fnptr_t */
#include <string.h>			/* strlen() */
#include <stdarg.h>		/* va_list, va_arg() */
/*******************************************************************
	name:	do_printf
	action:	minimal subfunction for ?printf, calls function
		'fn' with arg 'ptr' for each character to be output
	returns:total number of characters output

	%[flag][width][.prec][mod][conv]
	flag:	-	left justify, pad right w/ blanks	DONE
		0	pad left w/ 0 for numerics		DONE
		+	always print sign, + or -		xxx
		' '	(blank)					xxx
		#	(???)					xxx

	width:		(field width)				DONE

	prec:		(precision)				xxx

	conv:	d,i	decimal int				DONE
		u	decimal unsigned			DONE
		o	octal					DONE
		x,X	hex					DONE
		f,e,g,E,G float					xxx
		c	char					DONE
		s	string					DONE
		p	ptr					DONE

	mod:	N	near ptr				DONE
		F	far ptr					xxx
		h	short (16-bit) int			DONE
		l	long (32-bit) int			DONE
		L	long long (64-bit) int			xxx
*******************************************************************/
/* flags used in processing format string */
#define		PR_LJ	0x01	/* left justify */
#define		PR_CA	0x02	/* use A-F instead of a-f for hex */
#define		PR_SG	0x04	/* signed numeric conversion (%d vs. %u) */
#define		PR_32	0x08	/* long (32-bit) numeric conversion */
#define		PR_16	0x10	/* short (16-bit) numeric conversion */
#define		PR_WS	0x20	/* PR_SG set and num was < 0 */
#define		PR_LZ	0x40	/* pad left with '0' instead of ' ' */
#define		PR_FP	0x80	/* pointers are far */

/* largest number handled is 2^32-1, lowest radix handled is 8.
2^32-1 in base 8 has 11 digits (add 5 for trailing NUL and for slop) */
#define		PR_BUFLEN	16

int do_printf(const char *fmt, va_list args, fnptr_t fn, void *ptr)
{
    unsigned char state, flags, radix, actual_wd;
    unsigned short count, given_wd;
    char *where, buf[PR_BUFLEN];
    long num;

    state = flags = count = given_wd = 0;
    /* begin scanning format specifier list */
    for(; *fmt; fmt++)
    {
        switch(state)
        {
            /* STATE 0: AWAITING % */
        case 0:
            if(*fmt != '%')	/* not %... */
            {
                fn(*fmt, &ptr);	/* ...just echo it */
                count++;
                break;
            }
            /* found %, get next char and advance state to check if next char is a flag */
            state++;
            fmt++;
            /* FALL THROUGH */
            /* STATE 1: AWAITING FLAGS (%-0) */
        case 1:
            if(*fmt == '%')	/* %% */
            {
                fn(*fmt, &ptr);
                count++;
                state = flags = given_wd = 0;
                break;
            }
            if(*fmt == '-')
            {
                if(flags & PR_LJ)/* %-- is illegal */
                    state = flags = given_wd = 0;
                else
                    flags |= PR_LJ;
                break;
            }
            /* not a flag char: advance state to check if it's field width */
            state++;
            /* check now for '%0...' */
            if(*fmt == '0')
            {
                flags |= PR_LZ;
                fmt++;
            }
            /* FALL THROUGH */
            /* STATE 2: AWAITING (NUMERIC) FIELD WIDTH */
        case 2:
            if(*fmt >= '0' && *fmt <= '9')
            {
                given_wd = 10 * given_wd +
                           (*fmt - '0');
                break;
            }
            /* not field width: advance state to check if it's a modifier */
            state++;
            /* FALL THROUGH */
            /* STATE 3: AWAITING MODIFIER CHARS (FNlh) */
        case 3:
            if(*fmt == 'F')
            {
                flags |= PR_FP;
                break;
            }
            if(*fmt == 'N')
                break;
            if(*fmt == 'l')
            {
                flags |= PR_32;
                break;
            }
            if(*fmt == 'h')
            {
                flags |= PR_16;
                break;
            }
            /* not modifier: advance state to check if it's a conversion char */
            state++;
            /* FALL THROUGH */
            /* STATE 4: AWAITING CONVERSION CHARS (Xxpndiuocs) */
        case 4:
            where = buf + PR_BUFLEN - 1;
            *where = '\0';
            switch(*fmt)
            {
            case 'X':
                flags |= PR_CA;
                /* FALL THROUGH */
                /* xxx - far pointers (%Fp, %Fn) not yet supported */
            case 'x':
            case 'p':
            case 'n':
                radix = 16;
                goto DO_NUM;
            case 'd':
            case 'i':
                flags |= PR_SG;
                /* FALL THROUGH */
            case 'u':
                radix = 10;
                goto DO_NUM;
            case 'o':
                radix = 8;
                /* load the value to be printed. l=long=32 bits: */
DO_NUM:				if(flags & PR_32)
                num = va_arg(args, unsigned long);
                /* h=short=16 bits (signed or unsigned) */
                else if(flags & PR_16)
                {
                    if(flags & PR_SG)
                        num = va_arg(args, short);
                    else
                        num = va_arg(args, unsigned short);
                }
                /* no h nor l: sizeof(int) bits (signed or unsigned) */
                else
                {
                    if(flags & PR_SG)
                        num = va_arg(args, int);
                    else
                        num = va_arg(args, unsigned int);
                }
                /* take care of sign */
                if(flags & PR_SG)
                {
                    if(num < 0)
                    {
                        flags |= PR_WS;
                        num = -num;
                    }
                }
                /* convert binary to octal/decimal/hex ASCII
                OK, I found my mistake. The math here is _always_ unsigned */
                do
                {
                    unsigned long temp;

                    temp = (unsigned long)num % radix;
                    where--;
                    if(temp < 10)
                        *where = temp + '0';
                    else if(flags & PR_CA)
                        *where = temp - 10 + 'A';
                    else
                        *where = temp - 10 + 'a';
                    num = (unsigned long)num / radix;
                }
                while(num != 0);
                goto EMIT;
            case 'c':
                /* disallow pad-left-with-zeroes for %c */
                flags &= ~PR_LZ;
                where--;
                *where = (char)va_arg(args, char);
                actual_wd = 1;
                goto EMIT2;
            case 's':
                /* disallow pad-left-with-zeroes for %s */
                flags &= ~PR_LZ;
                where = va_arg(args, char *);
EMIT:
                actual_wd = strlen(where);
                if(flags & PR_WS)
                    actual_wd++;
                /* if we pad left with ZEROES, do the sign now */
                if((flags & (PR_WS | PR_LZ)) ==
                        (PR_WS | PR_LZ))
                {
                    fn('-', &ptr);
                    count++;
                }
                /* pad on left with spaces or zeroes (for right justify) */
EMIT2:				if((flags & PR_LJ) == 0)
                {
                    while(given_wd > actual_wd)
                    {
                        fn(flags & PR_LZ ? '0' :
                           ' ', &ptr);
                        count++;
                        given_wd--;
                    }
                }
                /* if we pad left with SPACES, do the sign now */
                if((flags & (PR_WS | PR_LZ)) == PR_WS)
                {
                    fn('-', &ptr);
                    count++;
                }
                /* emit string/char/converted number */
                while(*where != '\0')
                {
                    fn(*where++, &ptr);
                    count++;
                }
                /* pad on right with spaces (for left justify) */
                if(given_wd < actual_wd)
                    given_wd = 0;
                else given_wd -= actual_wd;
                for(; given_wd; given_wd--)
                {
                    fn(' ', &ptr);
                    count++;
                }
                break;
            default:
                break;
            }
        default:
            state = flags = given_wd = 0;
            break;
        }
    }
    return count;
}

#if 0 /* testing */
int vsprintf_help(char c, void **ptr)
{
    char *dst;

    dst = *ptr;
    *dst++ = c;
    *ptr = dst;
    return 0 ;
}
int vsprintf(char *buffer, const char *fmt, va_list args)
{
    int ret_val;

    ret_val = do_printf(fmt, args, vsprintf_help, (void *)buffer);
    buffer[ret_val] = '\0';
    return ret_val;
}
int sprintf(char *buffer, const char *fmt, ...)
{
    va_list args;
    int ret_val;

    va_start(args, fmt);
    ret_val = vsprintf(buffer, fmt, args);
    va_end(args);
    return ret_val;
}

int vprintf_help(char c, void **ptr)
{
    putchar(c);
    return 0 ;
}
int vprintf(const char *fmt, va_list args)
{
    return do_printf(fmt, args, vprintf_help, NULL);
}

int printf(const char *fmt, ...)
{
    va_list args;
    int ret_val;

    va_start(args, fmt);
    ret_val = vprintf(fmt, args);
    va_end(args);
    return ret_val;
}

int main(void)
{
    char buf[64];

    sprintf(buf, "%u score and %i years ago...\n", 4, -7);
    puts(buf);

    sprintf(buf, "-1L == 0x%lX == octal %lo\n", -1L, -1L);
    puts(buf);

    printf("<%-08s> and <%08s> justified strings\n", "left", "right");
    return 0;
}
#endif

unsigned _seed;
unsigned rand(void) /* stdlib.h */
{
    if(_seed == 0)
        _seed = 1;
    if((((_seed << 3) ^ _seed) & 0x80000000L) != 0)
        _seed = (_seed << 1) | 1;
    else
        _seed <<= 1;
    return _seed - 1;
}

extern unsigned _seed; /* in rand.c */
void srand(unsigned new_seed) /* stdlib.h */
{
    _seed = new_seed;
}
