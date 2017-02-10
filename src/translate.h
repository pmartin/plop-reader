#ifndef SRC_TRANSLATE_H_
#define SRC_TRANSLATE_H_

#define IS_LANG_FR (true)

#define LBL_HEADER_UNREAD_ENTRIES (IS_LANG_FR \
	? "entrées non lues" \
	: "unread entries" \
)

#define LBL_HEADER_ARCHIVED_ENTRIES (IS_LANG_FR \
	? "entrées archivées" \
	: "archives entries" \
)

#define LBL_HEADER_STARRED_ENTRIES (IS_LANG_FR \
	? "entrées étoilées" \
	: "starred entries" \
)

#define LBL_HEADER_PAGE_NUM (IS_LANG_FR \
	? "Page %1$d / %2$d (%3$d entrées)" \
	: "Page %1$d / %2$d (%3$d entries)" \
)

#define LBL_STATUSBAR_NO_UNREAD_ENTRIES_USE_SYNC (IS_LANG_FR \
	? "Vous n'avez aucune entrée non lue. Utilisez [SYNC] pour en obtenir depuis le serveur." \
	: "You don't have any unread entries. Use [SYNC] to fetch some from the server." \
)

#define LBL_STATUSBAR_NO_ARCHIVED_ENTRIES_USE_MENU (IS_LANG_FR \
	? "Vous n'avez aucune entrée archivée. Utilisez [MENU] pour voir les entrées non lues." \
	: "You don't have any archived entries. Use [MENU] to view unread entries." \
)

#define LBL_STATUSBAR_NO_STARRED_ENTRIES_USE_MENU (IS_LANG_FR \
	? "Vous n'avez aucune entrée étoilée. Utilisez [MENU] pour voir les entrées non lues." \
	: "You don't have any starred entries. Use [MENU] to view unread entries." \
)

#define LBL_STATUSBAR_USE_KEYS_OR_TOUCH (IS_LANG_FR \
	? "Utilisez les touches [<] et [>] pour naviguer. Touchez une entrée pour la lire." \
	: "Use [<] and [>] keys to navigate. Touch an entry to read it." \
)

#define LBL_MAINMENU_MODE_UNREAD_ENTRIES (IS_LANG_FR \
	? "Afficher les entrées non lues" \
	: "Display unread entries" \
)

#define LBL_MAINMENU_MODE_ARCHIVED_ENTRIES (IS_LANG_FR \
	? "Afficher les entrées archivées" \
	: "Display archived entries" \
)

#define LBL_MAINMENU_MODE_STARRED_ENTRIES (IS_LANG_FR \
	? "Afficher les entrées étoilées" \
	: "Display starred entries" \
)

#define LBL_MAINMENU_DELETE_ALL_LOCAL_DATA (IS_LANG_FR \
	? "Supprimer toutes les données locales" \
	: "Delete all local data" \
)

#define LBL_MAINMENU_ABOUT (IS_LANG_FR \
	? "À propos" \
	: "About" \
)

#define LBL_STATUSBAR_MAINMENU (IS_LANG_FR \
	? "Choisissez une action dans le menu, ou fermez-le..." \
	: "Choose an action in the menu, or close it..." \
)

#define LBL_ABOUT_MAIN_DESCRIPTION (IS_LANG_FR \
	? "Une application Wallabag pour liseuses Pocketbook Touch Lux." \
	: "A Wallabag application for Pocketbook Touch Lux ereaders." \
)

#define LBL_ABOUT_DEVELOPED_BY_PM (IS_LANG_FR \
	? "Développé par Pascal MARTIN." \
	: "Developed by Pascal MARTIN." \
)

#define LBL_ABOUT_CONTRIBUTE (IS_LANG_FR \
	? "Contribuer (GPL-3.0) :" \
	: "Contribute (GPL-3.0):" \
)

#define LBL_STATUSBAR_FEEL_FREE_TO_CONTRIBUTE (IS_LANG_FR \
	? "N'hésitez pas à contribuer sur %s ;-)" \
	: "Feel free to contribute on %s ;-)" \
)

#define LBL_DELETEALL_DIALOG_TITLE (IS_LANG_FR \
	? "Supprimer toutes les données locales ?" \
	: "Delete all local data?" \
)

#define LBL_DELETEALL_DIALOG_CONTENT (IS_LANG_FR \
	? "Voulez-vous vraiment supprimer toutes les données locales ?\nVous devrez synchroniser depuis le serveur pour obtenir de nouvelles données.\nLes données modifiées localement et non synchronisées vers le serveur seront perdues.." \
	: "Do you really want to delete all local data?\nYou will need to sync from server to fetch new data.\nData updated locally and not already synced to server will be lost." \
)

#define LBL_DELETEALL_DIALOG_BTN_OK (IS_LANG_FR \
	? "Supprimer les données" \
	: "Delete local data" \
)

#define LBL_DELETEALL_DIALOG_BTN_CANCEL (IS_LANG_FR \
	? "Annuler" \
	: "Cancel" \
)

#define LBL_STATUSBAR_DELETEALL_DONE (IS_LANG_FR \
	? "Données locales supprimées. Lance une synchro pour obtenir des données depuis le serveur ;-)" \
	: "Local data deleted. You should now run a sync to fetch data from server ;-)" \
)

#define LBL_STATUSBAR_DELETEALL_CANCELED (IS_LANG_FR \
	? "Données locales non modifiées." \
	: "Local data has been left untouched." \
)

#define LBL_STATUSBAR_CLOSING_APPLICATION_BYE (IS_LANG_FR \
	? "Fermeture de l'application. À bientôt ;-)" \
	: "Closing the application. Bye, see you soon ;-)" \
)

#define LBL_STATUSBAR_LAUNCHING_SYNC (IS_LANG_FR \
	? "Lancement de la synchronisation avec le serveur..." \
	: "Launching synchronization with server..." \
)

#define LBL_STATUSBAR_OPENING_MAIN_MENU (IS_LANG_FR \
	? "Ouverture du menu principal de l'application..." \
	: "Opening the application's main menu..." \
)

#define LBL_STATUSBAR_LOADING_READER_FOR_ENTRY (IS_LANG_FR \
	? "Chargement de l'appli de lecture pour #%1$d - %2$s..." \
	: "Loading reader app for entry#%1$d - %2$s..." \
)

#endif /* SRC_TRANSLATE_H_ */
