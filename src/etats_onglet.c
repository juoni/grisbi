/*  Fichier qui s'occupe de l'onglet �tats */
/*      etats.c */

/*     Copyright (C) 2000-2003  C�dric Auger */
/* 			cedric@grisbi.org */
/* 			http://www.grisbi.org */
/*     Copyright (C) 2002  Benjamin Drieu */
/* 			bdrieu@april.org */
/* 			http://www.grisbi.org */

/*     This program is free software; you can redistribute it and/or modify */
/*     it under the terms of the GNU General Public License as published by */
/*     the Free Software Foundation; either version 2 of the License, or */
/*     (at your option) any later version. */

/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "en_tete.h"


/*****************************************************************************************************/
GtkWidget *creation_onglet_etats ( void )
{
  GtkWidget *onglet;
  GtkWidget *frame;
  GtkWidget *vbox;

  /*   au d�part, aucun �tat n'est ouvert */

  bouton_etat_courant = NULL;
  etat_courant = NULL;


  onglet = gtk_hbox_new ( FALSE,
			  10 );
  gtk_container_set_border_width ( GTK_CONTAINER ( onglet ),
				   10 );
  gtk_widget_show ( onglet );


  /*   cr�ation de la fenetre des noms des �tats */
  /* on reprend le principe des comptes dans la fenetre des op�s */

  frame_liste_etats = gtk_frame_new ( NULL );
  gtk_frame_set_shadow_type ( GTK_FRAME ( frame_liste_etats ),
			      GTK_SHADOW_IN );
  gtk_box_pack_start ( GTK_BOX ( onglet ),
		       frame_liste_etats,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show (frame_liste_etats);

  /* on y met les rapports et les boutons */

  gtk_container_add ( GTK_CONTAINER ( frame_liste_etats ),
		      creation_liste_etats ());



  /* cr�ation du notebook contenant l'�tat et la config */

  notebook_etats = gtk_notebook_new ();
  gtk_notebook_set_show_tabs ( GTK_NOTEBOOK ( notebook_etats ),
			       FALSE );
  gtk_box_pack_start ( GTK_BOX ( onglet ),
		       notebook_etats,
		       TRUE,
		       TRUE,
		       0 );
  gtk_widget_show ( notebook_etats );


  /* cr�ation de la partie droite */

  vbox = gtk_vbox_new ( FALSE,
			10 );
  gtk_notebook_append_page ( GTK_NOTEBOOK ( notebook_etats ),
			     vbox,
			     gtk_label_new ( _("Reports display")));
  gtk_widget_show ( vbox );


  /*  Cr�ation de la partie contenant l'�tat */

  frame = gtk_frame_new ( NULL );
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       frame,
		       TRUE,
		       TRUE,
		       0 );
  gtk_frame_set_shadow_type ( GTK_FRAME ( frame ),
			      GTK_SHADOW_IN );
  gtk_widget_show (frame);

  /* on y met une scrolled window qui sera remplit par l'�tat */

  scrolled_window_etat = gtk_scrolled_window_new ( FALSE,
						   FALSE );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window_etat ),
				   GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC );
  gtk_container_add ( GTK_CONTAINER ( frame ),
		      scrolled_window_etat );
  gtk_widget_show ( scrolled_window_etat );

  /* cr�ation de la partie contenant les boutons (personnaliser ...) */

  frame = gtk_frame_new ( NULL );
  gtk_frame_set_shadow_type ( GTK_FRAME ( frame_droite_bas ),
			      GTK_SHADOW_IN );
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       frame,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show (frame);

  /* on y met les boutons */

  gtk_container_add ( GTK_CONTAINER ( frame ),
		      creation_barre_boutons_etats ());


  /* l'onglet de config sera cr�� que si n�cessaire */

  onglet_config_etat = NULL;


  return ( onglet );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
/* vontion creation_liste_etats */
/* renvoie la partie gauche de l'onglet rapports financiers */
/*****************************************************************************************************/

GtkWidget *creation_liste_etats ( void )
{
  GtkWidget *onglet;
  GtkWidget *frame;
  GtkWidget *bouton;
  GtkWidget *vbox;
  GtkWidget *scrolled_window;


  /*  Cr�ation d'une fen�tre g�n�rale*/

  onglet = gtk_vbox_new ( FALSE,
			  10);
  gtk_container_set_border_width ( GTK_CONTAINER ( onglet ),
				   10 );
  gtk_widget_show ( onglet );


  /*  Cr�ation du label contenant le rapport courant en haut */
  /*   on place le label dans une frame */

  frame = gtk_frame_new ( NULL );
  gtk_frame_set_shadow_type ( GTK_FRAME ( frame ),
			      GTK_SHADOW_ETCHED_OUT );
  gtk_box_pack_start ( GTK_BOX (onglet),
		       frame,
		       FALSE,
		       TRUE,
		       0);
  gtk_widget_show (frame);


  /*   on ne met rien dans le label, il sera rempli ensuite */

  label_etat_courant = gtk_label_new ( "" );
  gtk_misc_set_alignment ( GTK_MISC (label_etat_courant  ),
			   0.5,
			   0.5);
  gtk_container_add ( GTK_CONTAINER ( frame ),
		      label_etat_courant );
  gtk_widget_show (label_etat_courant);


  /*  Cr�ation de la fen�tre des etats */
  /*  qui peut contenir des barres de d�filement si */
  /*  n�cessaire */

  scrolled_window = gtk_scrolled_window_new ( NULL,
					      NULL);
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_NEVER,
				   GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start ( GTK_BOX ( onglet ),
		       scrolled_window,
		       TRUE,
		       TRUE,
		       0);
  gtk_widget_show ( scrolled_window );
  

  /*  cr�ation de la vbox qui contient la liste des �tats */

  vbox_liste_etats = gtk_vbox_new ( FALSE,
				    10);
  gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW (scrolled_window ),
					  vbox_liste_etats);
  gtk_viewport_set_shadow_type ( GTK_VIEWPORT ( GTK_BIN ( scrolled_window )  -> child ),
				 GTK_SHADOW_NONE );
  gtk_widget_show (vbox_liste_etats);
  

  /*  ajout des diff�rents �tats */

  remplissage_liste_etats ();

  /* ajout des boutons pour supprimer / ajouter un �tat */

  frame = gtk_frame_new ( NULL );
  gtk_frame_set_shadow_type ( GTK_FRAME ( frame ),
			      GTK_SHADOW_ETCHED_IN );
  gtk_box_pack_start ( GTK_BOX ( onglet ),
		       frame,
		       FALSE,
		       TRUE,
		       0);
  gtk_widget_show ( frame );


  vbox = gtk_vbox_new ( FALSE,
			5 );
  gtk_container_add ( GTK_CONTAINER  ( frame ),
		      vbox );
  gtk_widget_show ( vbox );


  /* mise en place du bouton ajouter */

  bouton = gtk_button_new_with_label ( _("Add a report") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE);
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       bouton,
		       FALSE,
		       TRUE,
		       0);
  gtk_signal_connect ( GTK_OBJECT (bouton),
		       "clicked",
		       GTK_SIGNAL_FUNC ( ajout_etat ),
		       NULL );
  gtk_widget_show ( bouton );

  /* on met le bouton personnaliser */

  bouton_personnaliser_etat = gtk_button_new_with_label ( _("Customise the report") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_personnaliser_etat ),
			  GTK_RELIEF_NONE );
  gtk_widget_set_sensitive ( bouton_personnaliser_etat,
			     FALSE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_personnaliser_etat ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( personnalisation_etat ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       bouton_personnaliser_etat,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_personnaliser_etat );

  /* on met le bouton dupliquer */

  bouton_dupliquer_etat = gtk_button_new_with_label ( _("Duplicate the report") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_dupliquer_etat ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_dupliquer_etat ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( dupliquer_etat ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       bouton_dupliquer_etat,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_dupliquer_etat );

  /* mise en place du bouton effacer �tat */

  bouton_effacer_etat = gtk_button_new_with_label ( _("Delete the report") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_effacer_etat ),
			  GTK_RELIEF_NONE);
  gtk_box_pack_start ( GTK_BOX ( vbox ),
		       bouton_effacer_etat,
		       FALSE,
		       TRUE,
		       0);
  gtk_signal_connect ( GTK_OBJECT (bouton_effacer_etat),
		       "clicked",
		       GTK_SIGNAL_FUNC ( efface_etat ),
		       NULL );
  gtk_widget_show ( bouton_effacer_etat );

  if ( !etat_courant )
    {
      gtk_widget_set_sensitive ( bouton_effacer_etat,
				 FALSE );
      gtk_widget_set_sensitive ( bouton_dupliquer_etat,
				 FALSE );
    }

  return ( onglet );

}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Fonction creation_barre_boutons_etats */
/* renvoie la barre des boutons de la partie droite-bas des rapports financiers */
/*****************************************************************************************************/

GtkWidget *creation_barre_boutons_etats ( void )
{
  GtkWidget *widget_retour;
  GtkWidget *bouton;

  widget_retour = gtk_hbox_new ( FALSE,
				 5 );
  gtk_widget_show ( widget_retour );

  /* on met le bouton rafraichir */

  bouton = gtk_button_new_with_label ( _("Refresh") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect_object ( GTK_OBJECT ( bouton ),
			      "clicked",
			      GTK_SIGNAL_FUNC ( rafraichissement_etat ),
			      NULL );
  gtk_box_pack_start ( GTK_BOX ( widget_retour ),
		       bouton,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton );

  /* on met le bouton imprimer */

  bouton_imprimer_etat = gtk_button_new_with_label ( _("Print") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_imprimer_etat ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect_object ( GTK_OBJECT ( bouton_imprimer_etat ),
			      "clicked",
			      GTK_SIGNAL_FUNC ( impression_etat ),
			      NULL );
  gtk_widget_set_sensitive ( bouton_imprimer_etat,
			     FALSE );
  gtk_box_pack_start ( GTK_BOX ( widget_retour ),
		       bouton_imprimer_etat,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_imprimer_etat );

  /* on met le bouton exporter */

  bouton_exporter_etat = gtk_button_new_with_label ( _("Export") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_exporter_etat ),
			  GTK_RELIEF_NONE );
  gtk_widget_set_sensitive ( bouton_exporter_etat,
			     FALSE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_exporter_etat ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( exporter_etat ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( widget_retour ),
		       bouton_exporter_etat,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_exporter_etat );

  /* on met le bouton importer */

  bouton_importer_etat = gtk_button_new_with_label ( _("Import") );
  gtk_button_set_relief ( GTK_BUTTON ( bouton_importer_etat ),
			  GTK_RELIEF_NONE );
  gtk_signal_connect ( GTK_OBJECT ( bouton_importer_etat ),
		       "clicked",
		       GTK_SIGNAL_FUNC ( importer_etat ),
		       NULL );
  gtk_box_pack_start ( GTK_BOX ( widget_retour ),
		       bouton_importer_etat,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( bouton_importer_etat );

  return ( widget_retour );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Fontion remplissage_liste_etats */
/* vide et remplit la liste des �tats */
/*****************************************************************************************************/

void remplissage_liste_etats ( void )
{
  GList *pointeur;
  GSList *liste_tmp;


  /* on commence par d�truire tous les enfants de la vbox */

  pointeur = GTK_BOX ( vbox_liste_etats ) -> children;

  while ( pointeur )
    {
      GtkBoxChild *child;

      child = pointeur -> data;

      pointeur = pointeur -> next;

      gtk_container_remove ( GTK_CONTAINER ( vbox_liste_etats ),
			     child -> widget );
    }

  /* on remplit maintenant avec tous les �tats */

  liste_tmp = liste_struct_etats;

  while ( liste_tmp )
    {
      struct struct_etat *etat;
      GtkWidget *hbox;
      GtkWidget *bouton;
      GtkWidget *icone;
      GtkWidget *label;

      etat = liste_tmp -> data;

      hbox = gtk_hbox_new ( FALSE,
			    10);
      gtk_box_pack_start ( GTK_BOX ( vbox_liste_etats ),
			   hbox,
			   FALSE,
			   FALSE,
			   0 );
      gtk_widget_show ( hbox );


      /*   on cr�e le bouton contenant le livre ferm� et ouvert, seul le ferm� est affich� pour l'instant */

      bouton = gtk_button_new ();
      gtk_button_set_relief ( GTK_BUTTON (bouton),
			      GTK_RELIEF_NONE);
      gtk_signal_connect ( GTK_OBJECT (bouton),
			   "clicked",
			   GTK_SIGNAL_FUNC ( changement_etat ),
			   etat );
      gtk_box_pack_start ( GTK_BOX (hbox),
			   bouton,
			   FALSE,
			   TRUE,
			   0);
      gtk_widget_show (bouton);


      /* cr�ation de l'icone ferm�e */

      if ( etat_courant
	   &&
	   etat -> no_etat == etat_courant -> no_etat )
	{
	  icone = gnome_stock_pixmap_widget ( GTK_WIDGET ( bouton ),
					      GNOME_STOCK_PIXMAP_BOOK_OPEN);
	  bouton_etat_courant = bouton;
	}
      else
	icone = gnome_stock_pixmap_widget ( GTK_WIDGET ( bouton ),
					    GNOME_STOCK_PIXMAP_BOOK_BLUE);
      gtk_container_add ( GTK_CONTAINER ( bouton ),
			  icone );
      gtk_widget_show ( icone );

      /* on cr�e le label � cot� du bouton */

      label = gtk_label_new ( etat -> nom_etat );
      gtk_box_pack_start ( GTK_BOX (hbox),
			   label,
			   FALSE,
			   TRUE,
			   0);
      gtk_widget_show (label);

      liste_tmp = liste_tmp -> next;
    }

  /*   si on a remplit la liste, c'est qu'il y a eu des modifs ( ajout, dupplication ... ) */
  /* donc on met � jour la liste des tiers */

  mise_a_jour_tiers ();

}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* on propose une liste d'�tats pr�mach�s et les remplis en fonction du choix */
/* de la personne */
/*****************************************************************************************************/

void ajout_etat ( void )
{
  struct struct_etat *etat;
  struct struct_comparaison_montants_etat *comp_montant;
  GtkWidget *dialog;
  gint resultat;
  GtkWidget *label;
  GtkWidget *frame;
  GtkWidget *option_menu;
  GtkWidget *menu;
  GtkWidget *menu_item;
  GtkWidget *label_description;
  GtkWidget *scrolled_window;


  dialog = gnome_dialog_new ( _("Create a report"),
			      GNOME_STOCK_BUTTON_OK,
			      GNOME_STOCK_BUTTON_CANCEL,
			      NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialog ),
				 GTK_WINDOW ( window ));
  gtk_widget_set_usize ( dialog,
			 400,
			 250 );
  gtk_window_set_policy ( GTK_WINDOW ( dialog ),
			  FALSE,
			  FALSE,
			  FALSE );

  label = gtk_label_new ( _("Select report type:"));
  gtk_misc_set_alignment ( GTK_MISC ( label ),
			   0,
			   0.5 );
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       label,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( label );

  /* on commence par cr�er l'option menu */

  option_menu = gtk_option_menu_new ();
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       option_menu,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( option_menu );

  /* on ajoute maintenant la frame */

  frame = gtk_frame_new ( _("Description:"));
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       frame,
		       TRUE,
		       TRUE,
		       0 );
  gtk_widget_show ( frame );

  /* on met le label dans une scrolled window */

  scrolled_window = gtk_scrolled_window_new ( FALSE,
					      FALSE );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_window ),
				   GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC );
  gtk_container_add ( GTK_CONTAINER ( frame ),
		      scrolled_window );
  gtk_widget_show ( scrolled_window );


  /* on ajoute maintenant le label */

  label_description = gtk_label_new ( "" );
  gtk_label_set_line_wrap ( GTK_LABEL ( label_description ),
			    TRUE );
  gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW ( scrolled_window ),
					  label_description );
  gtk_widget_show ( label_description );

  gtk_viewport_set_shadow_type ( GTK_VIEWPORT ( label_description -> parent ),
				 GTK_SHADOW_NONE );

  /* on cr�e ici le menu qu'on ajoute � l'option menu */

  menu = gtk_menu_new ();

  menu_item = gtk_menu_item_new_with_label ( _("Last month incomes and outgoings"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 0 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );

  /* on met le texte du 1er choix */

  change_choix_nouvel_etat ( menu_item,
			     label_description );


  menu_item = gtk_menu_item_new_with_label ( _("Current month incomes and outgoings"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 1 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
		       "activate",
		       GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
		       GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );


  menu_item = gtk_menu_item_new_with_label ( _("Annual budget"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 2 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );

  menu_item = gtk_menu_item_new_with_label ( _("Cheques deposit"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 4 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );


  menu_item = gtk_menu_item_new_with_label ( _("Monthly outgoings by third party"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 5 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );


  menu_item = gtk_menu_item_new_with_label ( _("Search"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 6 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );


  menu_item = gtk_menu_item_new_with_label ( _("Blank report"));
  gtk_menu_append ( GTK_MENU ( menu ),
		    menu_item );
  gtk_object_set_data ( GTK_OBJECT ( menu_item ),
			"no_etat",
			GINT_TO_POINTER ( 3 ));
  gtk_signal_connect ( GTK_OBJECT ( menu_item ),
			      "activate",
			      GTK_SIGNAL_FUNC ( change_choix_nouvel_etat ),
			      GTK_OBJECT ( label_description ));
  gtk_widget_show ( menu_item );

  gtk_option_menu_set_menu ( GTK_OPTION_MENU ( option_menu ),
			     menu );
  gtk_widget_show ( menu );


  /* on attend le choix de l'utilisateur */

  resultat = gnome_dialog_run ( GNOME_DIALOG ( dialog ));

  if ( resultat )
    {
      if ( GNOME_IS_DIALOG ( dialog ))
	gnome_dialog_close ( GNOME_DIALOG ( dialog ));
      return;
    }


  /* on r�cup�re le type d'�tat voulu */

  resultat = GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( GTK_OPTION_MENU ( option_menu ) -> menu_item ),
						     "no_etat" ));
  gnome_dialog_close ( GNOME_DIALOG ( dialog ));


  /* on cr�e le nouvel �tat */

  etat = calloc ( 1,
		  sizeof ( struct struct_etat ));

  etat -> no_etat = ++no_dernier_etat;


  /* on remplit maintenant l'�tat en fonction de ce qu'on a demand� */

  switch ( resultat )
    {
    case 0:
      /*  revenus et d�penses du mois pr�c�dent  */

      etat -> nom_etat = g_strdup ( _("Previous month incomes and outgoings") );

      etat -> separer_revenus_depenses = 1;
      etat -> no_plage_date = 7;


      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      etat -> type_virement = 2;
      etat -> utilise_categ = 1;
      etat -> afficher_sous_categ = 1;
      etat -> affiche_sous_total_categ = 1;
      etat -> affiche_sous_total_sous_categ = 1;
      etat -> afficher_pas_de_sous_categ = 1;
      etat -> afficher_nom_categ = 1;

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;

      break;

    case 1:
     /*  revenus et d�penses du mois courant  */

      etat -> nom_etat = g_strdup ( _("Current month incomes and outgoings") );

      etat -> separer_revenus_depenses = 1;
      etat -> no_plage_date = 3;


      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      etat -> type_virement = 2;
      etat -> utilise_categ = 1;
      etat -> afficher_sous_categ = 1;
      etat -> affiche_sous_total_categ = 1;
      etat -> affiche_sous_total_sous_categ = 1;
      etat -> afficher_pas_de_sous_categ = 1;
      etat -> afficher_nom_categ = 1;

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;

      break;


    case 2:

      /* on ajoute le budget annuel */

      etat -> nom_etat = g_strdup ( _("Annual budget") );

      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;

      etat -> separer_revenus_depenses = 1;
      etat -> no_plage_date = 4;
      etat -> utilise_categ = 1;
      etat -> exclure_ope_sans_categ = 1;
      etat -> affiche_sous_total_categ = 1;
      etat -> afficher_sous_categ = 1;
      etat -> afficher_pas_de_sous_categ = 1;
      etat -> affiche_sous_total_sous_categ = 1;
      etat -> afficher_nom_categ = 1;
      etat -> exclure_montants_nuls = 1;

      /*   tout le reste est � NULL, ce qui est tr�s bien */

      break;



    case 3:

      /* on ajoute un �tat vierge appel� nouvel �tat */

      etat -> nom_etat = g_strdup ( _("New report") );

      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;

      etat -> choix_devise_montant = 1;


      etat -> afficher_opes = 1;
      etat -> afficher_date_ope = 1;
      etat -> afficher_tiers_ope = 1;
      etat -> afficher_categ_ope = 1;
      etat -> separer_revenus_depenses = 1;
      etat -> type_virement = 2;
      etat -> utilise_categ = 1;
      etat -> exclure_ope_sans_categ = 1;
      etat -> affiche_sous_total_categ = 1;
      etat -> afficher_sous_categ = 1;
      etat -> affiche_sous_total_sous_categ = 1;
      etat -> afficher_nom_categ = 1;
      etat -> exclure_ope_sans_ib = 1;
      etat -> exclure_montants_nuls = 1;


      /*   tout le reste est � NULL, ce qui est tr�s bien */

      break;

    case 4:

      /* remise de ch�ques */

      etat -> nom_etat = g_strdup ( _("Cheques deposit") );

      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;


      etat -> afficher_opes = 1;
      etat -> afficher_nb_opes = 1;
      etat -> afficher_tiers_ope = 1;
      etat -> afficher_infobd_ope = 1;
      etat -> type_virement = 2;
      etat -> exclure_montants_nuls = 1;
      etat -> utilise_montant = 1;

      /* on doit cr�er une structure de montant qui dit que �a va �tre positif */

      comp_montant = calloc ( 1,
			      sizeof ( struct struct_comparaison_montants_etat ));
      comp_montant -> lien_struct_precedente = -1;
      comp_montant -> comparateur_1 = 8;
      comp_montant -> lien_1_2 = 3;

      etat -> liste_struct_comparaison_montants = g_slist_append ( etat -> liste_struct_comparaison_montants,
								   comp_montant );


      /*   tout le reste est � NULL, ce qui est tr�s bien */

      break;

    case 5:

      /* d�penses mensuelles par tiers */

      etat -> nom_etat = g_strdup ( _("Monthly outgoings by third party") );

      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;


      etat -> afficher_opes = 1;
      etat -> afficher_nb_opes = 1;
      etat -> afficher_tiers_ope = 1;
      etat -> type_classement_ope = 2;
      etat -> afficher_titre_colonnes = 1;
      etat -> no_plage_date = 7;
      etat -> utilise_categ = 1;
      etat -> affiche_sous_total_categ = 1;
      etat -> afficher_sous_categ = 1;
      etat -> afficher_nom_categ = 1;

      etat -> afficher_date_ope = 1;
      etat -> afficher_categ_ope = 1;
      etat -> separer_revenus_depenses = 1;
      etat -> type_virement = 2;
      etat -> exclure_ope_sans_categ = 1;
      etat -> affiche_sous_total_sous_categ = 1;
      etat -> exclure_ope_sans_ib = 1;
      etat -> exclure_montants_nuls = 1;


      /*   tout le reste est � NULL, ce qui est tr�s bien */

      break;

    case 6:

      /* d�penses mensuelles par tiers */

      etat -> nom_etat = g_strdup ( _("Search") );

      /*   le classement de base est 1-2-3-4-5-6 (cf structure.h) */

      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 1 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 2 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 3 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 4 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 5 ));
      etat -> type_classement = g_list_append ( etat -> type_classement,
						GINT_TO_POINTER ( 6 ));

      /*   les devises sont � 1 (euro) */

      etat -> devise_de_calcul_general = 1;
      etat -> devise_de_calcul_categ = 1;
      etat -> devise_de_calcul_ib = 1;
      etat -> devise_de_calcul_tiers = 1;
      etat -> choix_devise_montant = 1;


      etat -> afficher_opes = 1;
      etat -> afficher_date_ope = 1;
      etat -> afficher_tiers_ope = 1;
      etat -> afficher_categ_ope = 1;
      etat -> afficher_sous_categ_ope = 1;
      etat -> afficher_type_ope = 1;
      etat -> afficher_ib_ope = 1;
      etat -> afficher_sous_ib_ope = 1;
      etat -> afficher_cheque_ope = 1;
      etat -> afficher_notes_ope = 1;
      etat -> afficher_pc_ope = 1;
      etat -> afficher_rappr_ope = 1;
      etat -> afficher_infobd_ope = 1;
      etat -> afficher_exo_ope = 1;

      etat -> ope_clickables = 1;
      etat -> no_plage_date = 4;
      etat -> separation_par_plage = 1;
      etat -> type_separation_plage = 2;
      etat -> type_virement = 2;
 
     /*   tout le reste est � NULL, ce qui est tr�s bien */

      break;


    default :
      dialogue ( _("Unknown report type, creation cancelled"));
      return;
    }

  /* on l'ajoute � la liste */

  liste_struct_etats = g_slist_append ( liste_struct_etats,
					etat );

  /* on r�affiche la liste des �tats */

  etat_courant = etat;

  remplissage_liste_etats ();

  gtk_widget_set_sensitive ( bouton_personnaliser_etat,
			     TRUE );
  /* FIXME: r�activer �ca le jour ou on sort l'impression
	mais de toutes fa�ons, �ca sera merg� 
  gtk_widget_set_sensitive ( bouton_imprimer_etat,
			     TRUE );
  */
  gtk_widget_set_sensitive ( bouton_exporter_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_dupliquer_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_effacer_etat,
			     TRUE );

  gtk_label_set_text ( GTK_LABEL ( label_etat_courant ),
		       etat_courant -> nom_etat );

  personnalisation_etat ();
  modification_fichier ( TRUE );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
void change_choix_nouvel_etat ( GtkWidget *menu_item,
				GtkWidget *label_description )
{
  gchar *description;

  switch ( GPOINTER_TO_INT ( gtk_object_get_data ( GTK_OBJECT ( menu_item ),
						   "no_etat" )))
    {
    case 0:
      /* revenus et d�penses du mois dernier  */

      description = _("This report displays totals for last month's transactions sorted by categories and sub-categories. You just need to select the account(s). By default, all accounts are selected.");
      break;

    case 1:
      /* revenus et d�penses du mois en cours  */

      description = _("This report displays totals of current month's transactions sorted by categories and sub-categories. You just need to select the account(s). By default, all accounts are selected.");
      break;
 
    case 2:
      /* budget annuel  */

      description = _("This report displays annual budget. You just need to select the account(s). By default all accounts are selected.");
      break;
 
    case 3:
      /* etat vierge  */

      description = _("This report is an empty one. You need to customise it entirely.");
      break;
 
    case 4:
      /* remise de ch�ques  */

      description = _("This report displays the cheques deposit. You just need to select the account(s). By default all accounts are selected.");
      break;
 
    case 5:
      /* d�penses mensuelles par tiers  */

      description = _("This report displays current month's outgoings sorted by third parties. You just need to select the account(s). By default all accounts areselected.");
      break;
 
    case 6:
      /* recherche  */

      description = _("This report displays all the information for all transactions of all accounts for the current year. You just have to add the amount, date, third parties etc. criteria thant you want. By default the transactions are clickables.");
      break;
 
    default:

     description = _("???? should not be displayed...");
     }

  gtk_label_set_text ( GTK_LABEL ( label_description ),
		       description );

}
/*****************************************************************************************************/



/*****************************************************************************************************/
void efface_etat ( void )
{
  GtkWidget *dialog;
  GtkWidget *label;
  gint resultat;

  if ( !liste_struct_etats )
    return;

  if ( !etat_courant )
    return;

  dialog = gnome_dialog_new ( _("Confirm the deletion of a report"),
			      GNOME_STOCK_BUTTON_YES,
			      GNOME_STOCK_BUTTON_NO,
			      NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialog ),
				 GTK_WINDOW ( window ));

  label = gtk_label_new ( g_strdup_printf ( _("Do you really want to delete report %s?"),
					    etat_courant -> nom_etat ));
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       label,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( label );


  resultat = gnome_dialog_run ( GNOME_DIALOG ( dialog ));

  if ( !resultat )
    {
      /*   on met l'�tat courant � -1 et */
      /* le bouton � null, et le label de l'�tat en cours � rien */

      liste_struct_etats = g_slist_remove ( liste_struct_etats,
					    etat_courant );

      etat_courant = NULL;
      bouton_etat_courant = NULL;
      gtk_label_set_text ( GTK_LABEL ( label_etat_courant ),
			   "" );
      gtk_widget_set_sensitive ( bouton_personnaliser_etat,
				 FALSE );
      gtk_widget_set_sensitive ( bouton_imprimer_etat,
				 FALSE );
      gtk_widget_set_sensitive ( bouton_exporter_etat,
				 FALSE );
      gtk_widget_set_sensitive ( bouton_dupliquer_etat,
				 FALSE );
      gtk_widget_set_sensitive ( bouton_effacer_etat,
				 FALSE );

 
      if ( GTK_BIN ( scrolled_window_etat ) -> child )
	gtk_container_remove ( GTK_CONTAINER ( scrolled_window_etat ),
			       GTK_BIN ( scrolled_window_etat ) -> child );



      /* on r�affiche la liste des �tats */

      remplissage_liste_etats ();
      modification_fichier ( TRUE );
    }

  if ( GNOME_IS_DIALOG ( dialog ))
    gnome_dialog_close ( GNOME_DIALOG ( dialog ));

}
/*****************************************************************************************************/


/*****************************************************************************************************/
void changement_etat ( GtkWidget *bouton,
		       struct struct_etat *etat )
{
  GtkWidget *icone;

  /* on commence par refermer l'ancien bouton */

  if ( bouton_etat_courant )
    {
      icone = gnome_stock_pixmap_widget ( GTK_WIDGET ( bouton ),
					  GNOME_STOCK_PIXMAP_BOOK_BLUE);
      gtk_container_remove ( GTK_CONTAINER ( bouton_etat_courant ),
			     GTK_BIN ( bouton_etat_courant ) -> child );
      gtk_container_add ( GTK_CONTAINER ( bouton_etat_courant ),
			  icone );
      gtk_widget_show ( icone );
    }

  /* on ouvre le nouveau */

  bouton_etat_courant = bouton;
  etat_courant = etat;
  gtk_widget_set_sensitive ( bouton_personnaliser_etat,
			     TRUE );
/* FIXME: r�activer �ca le jour ou on sort l'impression
	mais de toutes fa�ons, �ca sera merg� 
  gtk_widget_set_sensitive ( bouton_imprimer_etat,
			     TRUE );
*/
  gtk_widget_set_sensitive ( bouton_exporter_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_dupliquer_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_effacer_etat,
			     TRUE );

  icone = gnome_stock_pixmap_widget ( GTK_WIDGET ( bouton ),
				      GNOME_STOCK_PIXMAP_BOOK_OPEN);
  gtk_container_remove ( GTK_CONTAINER ( bouton_etat_courant ),
			 GTK_BIN ( bouton_etat_courant ) -> child );
  gtk_container_add ( GTK_CONTAINER ( bouton_etat_courant ),
		      icone );
  gtk_widget_show ( icone );

  /* on met le nom de l'�tat dans la frame du haut */

  gtk_label_set_text ( GTK_LABEL ( label_etat_courant ),
		       etat -> nom_etat );

  /* on affiche l'�tat */

  rafraichissement_etat ( etat );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint recherche_etat_par_no ( struct struct_etat *etat,
			     gint *no_etat )
{
  return ( etat -> no_etat != GPOINTER_TO_INT (no_etat) );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
void exporter_etat ( void )
{
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *fenetre_nom;
  gint resultat;
  struct stat test_fichier;
  gchar *nom_etat;

  dialog = gnome_dialog_new ( _("Export a report"),
			      GNOME_STOCK_BUTTON_OK,
			      GNOME_STOCK_BUTTON_CANCEL,
			      NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialog ),
				 GTK_WINDOW ( window ));
  gnome_dialog_set_default ( GNOME_DIALOG ( dialog ),
			     0 );
  gtk_signal_connect ( GTK_OBJECT ( dialog ),
		       "destroy",
		       GTK_SIGNAL_FUNC ( gtk_signal_emit_stop_by_name ),
		       "destroy" );

  label = gtk_label_new ( COLON(_("Enter name for export")) );
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       label,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( label );

  fenetre_nom = gnome_file_entry_new ( "nom_fichier",
				       "nom_fichier" );
  gnome_file_entry_set_default_path ( GNOME_FILE_ENTRY ( fenetre_nom ),
				      dernier_chemin_de_travail );
  gtk_widget_set_usize ( gnome_file_entry_gnome_entry ( GNOME_FILE_ENTRY ( fenetre_nom )),
			 300,
			 FALSE );
  gtk_entry_set_text ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))),
		       g_strconcat ( dernier_chemin_de_travail,
				     etat_courant -> nom_etat,
				     ".egsb",
				     NULL ));
  gtk_entry_set_position ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))),
			   strlen (dernier_chemin_de_travail ));
  gnome_dialog_editable_enters ( GNOME_DIALOG ( dialog ),
				 GTK_EDITABLE ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))));
  gtk_window_set_focus ( GTK_WINDOW ( dialog ),
			 gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom )));
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       fenetre_nom,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( fenetre_nom );

  resultat = gnome_dialog_run ( GNOME_DIALOG ( dialog ));

  switch ( resultat )
    {
    case 0 :
      nom_etat = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))))));

      gnome_dialog_close ( GNOME_DIALOG ( dialog ));

      /* v�rification que c'est possible */

      if ( !strlen ( nom_etat ))
	return;

      if ( stat ( nom_etat,
		  &test_fichier ) != -1 )
	{
	  if ( S_ISREG ( test_fichier.st_mode ) )
	    {
	      GtkWidget *etes_vous_sur;
	      GtkWidget *label;

	      etes_vous_sur = gnome_dialog_new ( _("Save file"),
						 GNOME_STOCK_BUTTON_YES,
						 GNOME_STOCK_BUTTON_NO,
						 NULL );
	      label = gtk_label_new ( _("File exists. Do you want to overwrite it?") );
	      gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( etes_vous_sur ) -> vbox ),
				   label,
				   TRUE,
				   TRUE,
				   5 );
	      gtk_widget_show ( label );

	      gnome_dialog_set_default ( GNOME_DIALOG ( etes_vous_sur ),
					 1 );
	      gnome_dialog_set_parent ( GNOME_DIALOG ( etes_vous_sur ),
					GTK_WINDOW ( window ) );
	      gtk_window_set_modal ( GTK_WINDOW ( etes_vous_sur ),
				     TRUE );
	      if ( gnome_dialog_run_and_close ( GNOME_DIALOG ( etes_vous_sur ) ) )
		return;
	    }
	  else
	    {
	      dialogue ( g_strdup_printf ( _("Invalid filename \"%s\"!"),
					   nom_etat ));
	      return;
	    }
	}

      if ( !enregistre_etat ( nom_etat ))
	{
	  dialogue ( "L'enregistrement a �chou�." );
	  return;
	}

      break;

    default :
      gnome_dialog_close ( GNOME_DIALOG ( dialog ));
      return;
    }
}
/*****************************************************************************************************/


/*****************************************************************************************************/
void importer_etat ( void )
{
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *fenetre_nom;
  gint resultat;
  gchar *nom_etat;

  dialog = gnome_dialog_new ( _("Import a report"),
			      GNOME_STOCK_BUTTON_OK,
			      GNOME_STOCK_BUTTON_CANCEL,
			      NULL );
  gtk_window_set_transient_for ( GTK_WINDOW ( dialog ),
				 GTK_WINDOW ( window ));
  gnome_dialog_set_default ( GNOME_DIALOG ( dialog ),
			     0 );
  gtk_signal_connect ( GTK_OBJECT ( dialog ),
		       "destroy",
		       GTK_SIGNAL_FUNC ( gtk_signal_emit_stop_by_name ),
		       "destroy" );

  label = gtk_label_new ( COLON(_("Enter a filename")) );
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       label,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( label );

  fenetre_nom = gnome_file_entry_new ( "nom_fichier",
				       "nom_fichier" );
  gnome_file_entry_set_default_path ( GNOME_FILE_ENTRY ( fenetre_nom ),
				      dernier_chemin_de_travail );
  gtk_widget_set_usize ( gnome_file_entry_gnome_entry ( GNOME_FILE_ENTRY ( fenetre_nom )),
			 300,
			 FALSE );
  gtk_entry_set_text ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))),
		       g_strconcat ( dernier_chemin_de_travail,
				     ".egsb",
				     NULL ));
  gtk_entry_set_position ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))),
			   strlen (dernier_chemin_de_travail ));
  gnome_dialog_editable_enters ( GNOME_DIALOG ( dialog ),
				 GTK_EDITABLE ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))));
  gtk_window_set_focus ( GTK_WINDOW ( dialog ),
			 gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom )));
  gtk_box_pack_start ( GTK_BOX ( GNOME_DIALOG ( dialog ) -> vbox ),
		       fenetre_nom,
		       FALSE,
		       FALSE,
		       0 );
  gtk_widget_show ( fenetre_nom );

  resultat = gnome_dialog_run ( GNOME_DIALOG ( dialog ));

  switch ( resultat )
    {
    case 0 :
      nom_etat = g_strdup ( g_strstrip ( gtk_entry_get_text ( GTK_ENTRY ( gnome_file_entry_gtk_entry ( GNOME_FILE_ENTRY ( fenetre_nom ))))));

      gnome_dialog_close ( GNOME_DIALOG ( dialog ));

      /* v�rification que c'est possible */

      if ( !strlen ( nom_etat ))
	return;


      if ( !charge_etat ( nom_etat ))
	{
	  dialogue ( "L'importation a �chou�." );
	  return;
	}

      break;

    default :
      gnome_dialog_close ( GNOME_DIALOG ( dialog ));
      return;
    }
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* cette fonction cr�e une copie de l'�tat courant */
/*****************************************************************************************************/

void dupliquer_etat ( void )
{
  struct struct_etat *etat;
  GSList *liste_tmp;

  etat = calloc ( 1,
		  sizeof ( struct struct_etat ));


  /* on recopie les donn�es de l'�tat courant */

  memcpy ( etat,
	   etat_courant,
	   sizeof ( struct struct_etat ));

  /* il reste juste � faire une copie des listes et des chaines pour terminer */

  etat -> no_etat = ++no_dernier_etat;

  etat -> nom_etat = g_strdup ( etat_courant -> nom_etat );
  etat -> no_exercices = g_slist_copy ( etat_courant -> no_exercices );

  if ( etat_courant -> date_perso_debut )
    etat -> date_perso_debut = g_date_new_dmy ( g_date_day ( etat_courant -> date_perso_debut ),
						g_date_month ( etat_courant -> date_perso_debut ),
						g_date_year ( etat_courant -> date_perso_debut ));
						
  if ( etat_courant -> date_perso_fin )
    etat -> date_perso_fin = g_date_new_dmy ( g_date_day ( etat_courant -> date_perso_fin ),
						g_date_month ( etat_courant -> date_perso_fin ),
						g_date_year ( etat_courant -> date_perso_fin ));
						
  etat -> type_classement = g_list_copy ( etat_courant -> type_classement );
  etat -> no_comptes = g_slist_copy ( etat_courant -> no_comptes );
  etat -> no_comptes_virements = g_slist_copy ( etat_courant -> no_comptes_virements );
  etat -> no_categ = g_slist_copy ( etat_courant -> no_categ );
  etat -> no_ib = g_slist_copy ( etat_courant -> no_ib );
  etat -> no_tiers = g_slist_copy ( etat_courant -> no_tiers );


  /* on fait une copie de la liste des textes */

  if ( etat -> liste_struct_comparaison_textes )
    {
      etat -> liste_struct_comparaison_textes = NULL;

      liste_tmp = etat_courant -> liste_struct_comparaison_textes;

      while ( liste_tmp )
	{
	  struct struct_comparaison_textes_etat *ancien_comp_textes;
	  struct struct_comparaison_textes_etat *comp_textes;

	  ancien_comp_textes = liste_tmp -> data;

	  comp_textes = calloc ( 1,
				   sizeof ( struct struct_comparaison_textes_etat ));

	  memcpy ( comp_textes,
		   ancien_comp_textes,
		   sizeof ( struct struct_comparaison_textes_etat ));

	  comp_textes -> texte = g_strdup ( comp_textes -> texte );

	  etat -> liste_struct_comparaison_textes = g_slist_append ( etat -> liste_struct_comparaison_textes,
								     comp_textes );
	  liste_tmp = liste_tmp -> next;
	}
    }

  /* on fait une copie de la liste des montants */

  if ( etat -> liste_struct_comparaison_montants )
    {
      etat -> liste_struct_comparaison_montants = NULL;

      liste_tmp = etat_courant -> liste_struct_comparaison_montants;

      while ( liste_tmp )
	{
	  struct struct_comparaison_montants_etat *ancien_comp_montants;
	  struct struct_comparaison_montants_etat *comp_montants;

	  ancien_comp_montants = liste_tmp -> data;

	  comp_montants = calloc ( 1,
				   sizeof ( struct struct_comparaison_montants_etat ));

	  memcpy ( comp_montants,
		   ancien_comp_montants,
		   sizeof ( struct struct_comparaison_montants_etat ));

	  etat -> liste_struct_comparaison_montants = g_slist_append ( etat -> liste_struct_comparaison_montants,
								       comp_montants );
	  liste_tmp = liste_tmp -> next;
	}
    }



  /* on l'ajoute � la liste */

  liste_struct_etats = g_slist_append ( liste_struct_etats,
					etat );

  /* on r�affiche la liste des �tats */

  etat_courant = etat;

  remplissage_liste_etats ();

  gtk_widget_set_sensitive ( bouton_personnaliser_etat,
			     TRUE );
/* FIXME: r�activer �ca le jour ou on sort l'impression
	mais de toutes fa�ons, �ca sera merg� 
  gtk_widget_set_sensitive ( bouton_imprimer_etat,
			     TRUE );
*/
  gtk_widget_set_sensitive ( bouton_exporter_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_dupliquer_etat,
			     TRUE );
  gtk_widget_set_sensitive ( bouton_effacer_etat,
			     TRUE );

  gtk_label_set_text ( GTK_LABEL ( label_etat_courant ),
		       etat_courant -> nom_etat );

  gtk_widget_set_sensitive ( bouton_effacer_etat,
			     TRUE );

  personnalisation_etat ();
  modification_fichier ( TRUE );
}
/*****************************************************************************************************/
