/* ************************************************************************** */
/*  Fichier qui s'occupe de former les diff�rentes fen�tres de travail        */
/*      fenetre_principale.c                                                  */
/*                                                                            */
/*     Copyright (C)	2000-2003 C�dric Auger (cedric@grisbi.org)	      */
/*			2003 Alain Portal (dionysos@grisbi.org)		      */
/* 			http://www.grisbi.org				      */
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



#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "en_tete.h"


/**********************************************************************************************************
** Cr�ation de la fen�tre du gestionnaire de comptes **
** Renvoie le notebook **
***********************************************************************************************************/

GtkWidget *creation_fenetre_principale (void )
{

/* cr�ation du notebook de base */

  notebook_general = gtk_notebook_new();

  gtk_signal_connect ( GTK_OBJECT ( notebook_general ),
		       "switch_page",
		       GTK_SIGNAL_FUNC ( change_page_notebook),
		       NULL );


/* Cr�ation de la page d'accueil */

  page_accueil = creation_onglet_accueil();

  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_accueil,
			     gtk_label_new (SPACIFY(_("Main page"))) );

  /*  C�ation de la fen�tre principale qui contient d'un c�t� */
  /*  les comptes, et de l'autre les op�rations */

  page_operations = creation_onglet_operations ();

  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_operations,
			     gtk_label_new (SPACIFY(_("Transactions"))) );

/*   cr�ation de la fenetre des ech�ances */

  page_echeancier = creation_onglet_echeancier();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_echeancier,
			     gtk_label_new (SPACIFY(_("Scheduler"))) );

/*   cr�ation de la fenetre des comptes */

  page_comptes = creation_onglet_comptes ();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_comptes,
			     gtk_label_new (SPACIFY(_("Accounts"))) );

/* Cr�ation de la fenetre des tiers */

  page_tiers = onglet_tiers();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_tiers,
			     gtk_label_new (SPACIFY(_("Third party"))) );

  /* cr�ation de la fenetre des categories */

  page_categories = onglet_categories();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_categories,
			     gtk_label_new (SPACIFY(_("Categories"))) );

  /* cr�ation de la fenetre des imputations budg�taires */

  page_imputations = onglet_imputations();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_imputations,
			     gtk_label_new (SPACIFY(_("Budgetary lines"))) );

  gtk_widget_set_sensitive ( page_imputations,
			     etat.utilise_imputation_budgetaire );

  /* cr�ation de la fenetre des �tats */

  page_etats = creation_onglet_etats ();
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_general ),
			     page_etats,
			     gtk_label_new (SPACIFY(_("Reports"))) );




  /* change les titres des colonnes si n�cessaire */

  if ( etat.affiche_nb_ecritures_listes )
    {
      gtk_clist_set_column_title ( GTK_CLIST ( arbre_tiers ),
				   0,
				   _("Third parties list (transactions)") );
      gtk_clist_set_column_title ( GTK_CLIST ( arbre_categ ),
				   0,
				   _("Categories list (transactions)") );
      gtk_clist_set_column_title ( GTK_CLIST ( arbre_imputation ),
				   0,
				   _("Budgetary lines list (transactions)") );
    }


  return ( notebook_general );
}
/***********************************************************************************************************/



/***********************************************************************************************************/
void change_page_notebook ( GtkNotebook *notebook,
			    GtkNotebookPage *page,
			    guint numero_page,
			    gpointer null )
{

  /* retire l'horloge si part de l'accueil */
  if ( id_temps )
    {
      gtk_timeout_remove ( id_temps );
      id_temps = 0;
    }

  switch ( numero_page )
    {
      /* remet l'horloge si revient � l'accueil */

    case 0:
      id_temps = gtk_timeout_add ( 1000,
				   (GtkFunction) change_temps,
				   GTK_WIDGET ( label_temps ));
      break;
      /* si on va sur la fen�tre des comptes => focus � la liste */

    case 1:
      p_tab_nom_de_compte_variable = p_tab_nom_de_compte_courant;

      if ( GTK_IS_WIDGET ( CLIST_OPERATIONS ))
	gtk_widget_grab_focus ( GTK_WIDGET ( CLIST_OPERATIONS ) );
      break;

      /*   pour les listes, si aucune ligne n'est affich�e ( c'est le cas au d�part ), */
      /* on le fait ici */

    case 4:
      if ( modif_tiers )
	remplit_arbre_tiers ();
      break;

    case 5:
      if ( modif_categ )
	remplit_arbre_categ ();
      break;

    case 6:
      if ( modif_imputation )
	remplit_arbre_imputation ();
      break;
    }

}
/***********************************************************************************************************/
