/* ************************************************************************** */
/*                                                                            */
/*                                  gsb_real	                              */
/*                                                                            */
/*     Copyright (C)	2000-2007 Cédric Auger (cedric@grisbi.org)	          */
/*			2003-2008 Benjamin Drieu (bdrieu@april.org)	                      */
/* 			http://www.grisbi.org				                              */
/*                                                                            */
/*  This program is free software; you can redistribute it and/or modify      */
/*  it under the terms of the GNU General Public License as published by      */
/*  the Free Software Foundation; either version 2 of the License, or         */
/*  (at your option) any later version.                                       */
/*                                                                            */
/*  This program is distributed in the hope that it will be useful,           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*  GNU General Public License for more details.                              */
/*                                                                            */
/*  You should have received a copy of the GNU General Public License         */
/*  along with this program; if not, write to the Free Software               */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                            */
/* ************************************************************************** */

/**
 * \file gsb_real.c
 * grisbi use a special structure to describe a real number
 * all is defined her
 */


#include "include.h"

/*START_INCLUDE*/
#include "gsb_real.h"
#include "./gsb_data_currency.h"
#include "./gsb_data_transaction.h"
#include "./utils_str.h"
/*END_INCLUDE*/

gsb_real null_real = { 0 , 0 };

/*START_STATIC*/
static gchar *gsb_real_format_string ( gsb_real number,
				gint currency_number,
				gboolean show_symbol );
static gsb_real gsb_real_get_from_string_normalized ( const gchar *string, gint default_exponent );
static gboolean gsb_real_normalize ( gsb_real *number_1,
			      gsb_real *number_2 );
static gdouble gsb_real_real_to_double ( gsb_real number );
/*END_STATIC*/

/*START_EXTERN*/
extern gint max;
/*END_EXTERN*/



/**
 * Return the real in a formatted string, according to the currency 
 * regarding decimal separator, thousands separator and positive or
 * negative sign.
 * this is directly the number coded in the real wich is returned
 * usually, gsb_real_get_string_with_currency is better to adapt the format
 * 	of the number to the currency format
 * 
 * \param number	Number to format.
 *
 * \return		A newly allocated string of the number (this
 *			function will never return NULL) 
*/
gchar *gsb_real_get_string ( gsb_real number )
{
    return gsb_real_format_string ( number, 0, FALSE );
}

/**
 * format a gsb_real into a string from gsb_real_get_string
 *	adapt the format of the real to the currency (nb digits after , ...)
 *	show if asked the currency symbol
 *
 * \param number		A number to format.
 * \param currency_number	Currency to use.
 *
 * \return a newly allocated string of the number
 * */
gchar *gsb_real_get_string_with_currency ( gsb_real number,
					   gint currency_number,
					   gboolean show_symbol )
{
    gchar *string;

    string = gsb_real_format_string (number,
				     currency_number,
				     show_symbol);
    return string;
}




/**
 * Return the real in a formatted string with an optional currency
 * symbol, according to the locale regarding decimal separator,
 * thousands separator and positive or negative sign.
 * 
 * \param number		Number to format.
 * \param currency_number 	the currency we want to adapt the number, 0 for no adaptation
 * \param show_symbol 		TRUE to add the currency symbol in the string
 *
 * \return		A newly allocated string of the number (this
 *			function will never return NULL) 
 */
gchar *gsb_real_format_string ( gsb_real number,
                        gint currency_number,
                        gboolean show_symbol )
{
    struct lconv * conv = localeconv ( );
    gchar *string, *exponent = NULL, *mantissa;
    glong num;
    gint nbre_char;
    const gchar *currency_symbol = NULL;
    char buf[G_ASCII_DTOSTR_BUF_SIZE];

    /* as we use localeconv, all the currencies in grisbi will be formatted as the locale of 
     * the system i think it's ok like that, and to adapt the view according with the 
     * currency and not the current locale is much more complicated */
    if (currency_number && show_symbol)
        currency_symbol = gsb_data_currency_get_code (currency_number);
    
    /* First of all if number = 0 I return 0 with the symbol of the currency if necessary */
    if (number.mantissa == 0)
    {
        if (currency_symbol && conv -> p_cs_precedes)
            return g_strdup_printf ( "%s %s", currency_symbol, "0" );
        else if (currency_symbol && ! conv -> p_cs_precedes)
            return g_strdup_printf ( "%s %s", "0", currency_symbol );
        else
            return g_strdup ("0");
    }

    /* first we need to adapt the exponent to the currency */
    /* if the exponent of the real is not the same of the currency, need to adapt it */
    if ( currency_number
      &&
      number.exponent != gsb_data_currency_get_floating_point (currency_number) )
        number = gsb_real_adjust_exponent ( number, gsb_data_currency_get_floating_point (
                        currency_number) );

    /* on traite la conversion avec g_ascii_dtostr */
    num = labs(number.mantissa);

    mantissa = g_ascii_dtostr ( buf, sizeof (buf), num );
    if ( ! mantissa )
            return g_strdup ("Error");

    nbre_char = g_utf8_strlen ( mantissa, -1);

    /* on extrait la partie entière et la partie décimale */
    if ( nbre_char > number.exponent )
    {
        exponent = g_strdup ( mantissa + (nbre_char - number.exponent ) );
        mantissa = g_strndup ( mantissa, (nbre_char - number.exponent) );
    }
    else
    {
        if ( nbre_char < number.exponent )
        {
            exponent = g_strnfill ( number.exponent - nbre_char, '0' );
            exponent = g_strconcat ( exponent, mantissa, NULL );
        }
        else
            exponent = g_strdup ( mantissa );
        mantissa = g_strdup ( "0" );
    }

    /* on insère le séparateur des milliers */
    gchar *mon_thousands_sep_utf8;

    if ( nbre_char > 3 && (mon_thousands_sep_utf8 = g_locale_to_utf8 (
                        conv->mon_thousands_sep, -1, NULL, NULL, NULL )) )
    {
        gchar *reverse;
        gchar *ptr_char, *ptr_fin = NULL;
        gchar *dest = NULL;
        gchar *ch;
        gint i = 0;

        reverse = g_utf8_strreverse ( mantissa, -1 );
        dest = g_malloc0 ( 30 * sizeof (gunichar));

        nbre_char = g_utf8_strlen ( reverse, -1);
        ptr_char = reverse;
        ptr_fin = dest;
        for (i = 0; i < nbre_char; i++)
        {
            ch = g_strndup (ptr_char, 1);
            if ( i == 3 )
                ptr_fin = g_stpcpy (ptr_fin, mon_thousands_sep_utf8 );
            ptr_fin = g_stpcpy ( ptr_fin, ch );
            ptr_char = g_utf8_next_char ( ptr_char );

            if (ptr_char == NULL) break;
        }
        mantissa = g_utf8_strreverse ( dest, -1 );
        g_free ( reverse );
        g_free ( dest );
    }
    
    /* Add the sign at the end of the string just before to reverse it to avoid
       to have to insert it at the begin just after... */
    string = g_strdup_printf ( "%s%s%s%s%s%s%s%s", 
                    ( currency_symbol && conv -> p_cs_precedes ? currency_symbol : "" ),
                    ( currency_symbol && conv -> p_sep_by_space ? " " : "" ),
                    number.mantissa < 0 ? conv -> negative_sign : conv -> positive_sign,
                    mantissa,
                    ( * conv -> mon_decimal_point ? conv -> mon_decimal_point : "." ),
                    strlen (exponent) ? exponent : "0",
                    ( currency_symbol && ! conv -> p_cs_precedes && conv -> p_sep_by_space ? 
                    " " : "" ),
                    ( currency_symbol && ! conv -> p_cs_precedes ? currency_symbol : "" ) );

    g_free ( exponent );
    g_free ( mantissa );

    return string;
}


/**
 * get a real number from a string
 * the string can be formatted :
 * - handle , or . as separator
 * - spaces are ignored
 * - another character makes a 0 return
 *
 *   there is no ask for any exponent, so the gsb_real will be exactly the
 *   same as the string
 *
 * \param string
 *
 * \return the number in the string transformed to gsb_real
 * */
gsb_real gsb_real_get_from_string ( const gchar *string )
{
    return gsb_real_get_from_string_normalized ( string, -1 );
}



/**
 * get a real number from an integer
 *
 * \param mantissa
 * \param exponent -1 for no limit
 *
 * \return a gsb_real from the integer
 */
gsb_real gsb_real_new ( gint mantissa, gint exponent )
{
    gsb_real number = null_real;
    number.mantissa = mantissa;
    number.exponent = exponent;
    return number;
}



/**
 * get a real number from a string
 * the string can be formatted :
 * - handle , or . as separator
 * - spaces are ignored
 * - another character makes a 0 return
 *
 *   the gsb_real will have the exponent given in default_exponent, except if default_exponent = -1
 *
 * \param string
 * \param default_exponent -1 for no limit
 *
 * \return the number in the string transformed to gsb_real
 */
gsb_real gsb_real_get_from_string_normalized ( const gchar *string, gint default_exponent )
{
    struct lconv * conv = localeconv ( );
    gsb_real number = null_real;
    gchar *new_str;
    gchar *mon_thousands_sep_utf8;
    gchar *ptr;
    gchar**	tab;
    gint sign;

    if ( !string
	 ||
	 !strlen (string))
        return number;

    new_str = my_strdup (string);

    /* on enlève les séparateurs des milliers */
    mon_thousands_sep_utf8 = g_locale_to_utf8 (
                        conv->mon_thousands_sep, -1, NULL, NULL, NULL );
    if ( g_utf8_strchr (new_str, -1, g_utf8_get_char (mon_thousands_sep_utf8)) )
    {
        tab = g_strsplit ( new_str, mon_thousands_sep_utf8, 0 );
        g_free ( new_str );
        new_str = g_strjoinv ( "", tab );
        g_strfreev ( tab );
    }

    /* on extrait le signe */
    if ( new_str[0] == *(conv -> negative_sign) )
        sign = -1;
    else
        sign = 1;

    /* On détermine l'exponent */
    if ( (ptr = g_strrstr (new_str, conv -> mon_decimal_point)) )
        number.exponent = strlen ( ptr ) - 1;

    /* on détermine la mantisse on supprime tous les séparateurs et autres signes */
    tab = g_strsplit_set ( new_str, g_strconcat (
                        "+", "-", conv -> mon_decimal_point, NULL), 0 );
    g_free ( new_str );
    new_str = g_strjoinv ( "", tab );
    g_strfreev ( tab );

    number.mantissa = (glong) g_ascii_strtod ( new_str, NULL );
    number.mantissa = sign * number.mantissa;
    g_free ( new_str );

    return number;
}



/**
 * compare 2 gsb_real and return the result (-1, 0, 1)
 *
 * \param number_1
 * \param number_2
 *
 * \return -1 if number_1 < number_2 ; 0 if number_1 = number_2 ; 1 if number_1 > number_2
 * */
gint gsb_real_cmp ( gsb_real number_1,
		    gsb_real number_2 )
{
    gsb_real_normalize ( &number_1,
			 &number_2 );
    if (number_1.mantissa < number_2.mantissa)
	return -1;
    if (number_1.mantissa == number_2.mantissa)
	return 0;

    return 1;
}

/**
 * normalize the 2 numbers to be able to work with them later
 * for that transform the 2 numbers to have the same exponent
 * and after that we can work on the mantissa
 *
 * \param number_1 a pointer to gsb_real wich contains the number_1 to transform
 * \param number_2 a pointer to gsb_real wich contains the number_2 to transform
 *
 * \return TRUE
 * */
gboolean gsb_real_normalize ( gsb_real *number_1,
			      gsb_real *number_2 )
{
    glong limit_number;
    gboolean invert = FALSE;

    limit_number = G_MAXLONG / 10;

    while ( number_1 -> exponent != number_2 -> exponent )
    {
	/* if we go over the upper limit we change the sense */
	if ( !invert
	     &&
	     ( labs (number_1 -> mantissa) > limit_number
	      ||
	      labs (number_2 -> mantissa) > limit_number ))
	    invert = TRUE;

	if (number_1 -> exponent > number_2 -> exponent)
	{
	    if (invert)
	    {
		number_1 -> exponent--;
		number_1 -> mantissa = number_1 -> mantissa / 10;
	    }
	    else
	    {
		number_2 -> exponent++;
		number_2 -> mantissa = number_2 -> mantissa * 10;
	    }
	}
	else
	{
	    if (invert)
	    {
		number_2 -> exponent--;
		number_2 -> mantissa = number_2 -> mantissa / 10;
	    }
	    else
	    {
		number_1 -> exponent++;
		number_1 -> mantissa = number_1 -> mantissa * 10;
	    }
	}
    }
    return TRUE;
}

/**
 * give the absolute value of the number
 *
 * \param number
 *
 * \return a gsb_real, the absolute value of the given number
 * */
gsb_real gsb_real_abs ( gsb_real number )
{
    number.mantissa = labs (number.mantissa);
    return number;
}


/**
 * modify the number to adjust the exponent wanted
 *
 * \param number
 * \param return_exponent the exponent we want to have for the returned number, or -1 if no limit
 *
 * \return the transformed number
 * */
gsb_real gsb_real_adjust_exponent ( gsb_real number,
				    gint return_exponent )
{
    gdouble tmp;

    if (return_exponent == -1)
	return number;

    tmp = number.mantissa;

    while ( number.exponent != return_exponent )
    {
	if (number.exponent < return_exponent)
	{
	    tmp = tmp * 10;
	    number.exponent++;
	}
	else
	{
	    tmp = tmp / 10;
	    number.exponent--;
	}
    }
    number.mantissa = lrint (tmp);
    return number;
}



/**
 * add 2 gsb_real
 * !! CARREFULL : that function can create an overflow and return something invalid
 * 	to avoid that, don't cass the fuctions gsb_data_transaction_get_adjusted_amount and similar with -1
 * 		as return_exponent, but the exponent of the currency
 * 	if someone find how to detect the overflow on the line number.mantissa + number_2.mantissa, please teach me !!
 *
 * \param number_1
 * \param number_2
 *
 * \return a gsb_real = number_1 + number_2
 * */
gsb_real gsb_real_add ( gsb_real number_1,
			gsb_real number_2 )
{
    gsb_real number = number_1;
    
    gsb_real_normalize ( &number,
			 &number_2 );

    number.mantissa = number.mantissa + number_2.mantissa;
    return number;
}

/**
 * substract between 2 gsb_real : number_1 - number_2
 *
 * \param number_1
 * \param number_2
 *
 * \return a gsb_real = number_1 - number_2
 * */
gsb_real gsb_real_sub ( gsb_real number_1,
			gsb_real number_2 )
{
    gsb_real number = number_1;
    
    gsb_real_normalize ( &number,
			 &number_2 );

    number.mantissa = number.mantissa - number_2.mantissa;
    return number;
}


/**
 * return the opposite of the number
 * ie 5 returns -5 in gsb_real number
 *
 * \param number
 *
 * \return its opposite
 * */
gsb_real gsb_real_opposite ( gsb_real number )
{
    number.mantissa = -number.mantissa;
    return number;
}


/**
 * multiply 2 gsb_reals
 *
 * \param number_1
 * \param number_2
 *
 * \return the multiplication between the 2
 * */
gsb_real gsb_real_mul ( gsb_real number_1,
			gsb_real number_2 )
{
    number_1.mantissa *= number_2.mantissa;
    number_1.exponent += number_2.exponent;
    return number_1;
}


/**
 * divide 2 gsb_reals
 *
 * \param number_1
 * \param number_2
 *
 * \return the multiplication between the 2
 * */
gsb_real gsb_real_div ( gsb_real number_1,
			gsb_real number_2 )
{
    gsb_real number;

    number = gsb_real_double_to_real ( gsb_real_real_to_double (number_1)
				       /
				       gsb_real_real_to_double (number_2));

    return number;
}


/**
 * convert a gsb_real to a double
 *
 * \param number a gsb_real number
 *
 * \return the number in double format
 * */
gdouble gsb_real_real_to_double ( gsb_real number )
{
    gdouble double_number;
    gint i;

    double_number = number.mantissa;
    for ( i=0 ; i<number.exponent ; i++ )
	double_number = double_number / 10;
    return double_number;
}


/**
 * convert a double to a gsb_real
 *
 * \param number a gdouble number
 *
 * \return the number in gsb_real format
 * */
gsb_real gsb_real_double_to_real ( gdouble number )
{
    gdouble tmp_double;
    gdouble maxlong;

    gsb_real real_number = null_real;
    maxlong = G_MAXLONG / 10;

    while (modf (number, &tmp_double))
    {
	number = number * 10;
	real_number.exponent++;

	if (fabs (number) > maxlong)
	    number = rint (number);
    }
    real_number.mantissa = number;
    return real_number;
}




