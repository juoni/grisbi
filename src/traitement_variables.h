#ifndef _TRAITEMENT_VARIABLES_H
#define _TRAITEMENT_VARIABLES_H (1)

#include <glib.h>

/* START_INCLUDE_H */
/* END_INCLUDE_H */


/*START_DECLARATION*/
void	free_variables									(void);
void	init_variables									(void);
void	initialise_largeur_colonnes_prev_tab			(void);
void	initialise_largeur_colonnes_tab_affichage_ope	(gint type_operation,
														 const gchar *description);
/*END_DECLARATION*/


#endif

