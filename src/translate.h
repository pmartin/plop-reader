#ifndef SRC_TRANSLATE_H_
#define SRC_TRANSLATE_H_

#define LANG_EN 0
#define LANG_FR 1

extern int global_lang;

#define IS_LANG_FR (global_lang == LANG_FR)

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

#define LBL_STATUSBAR_OPENING_CONTEXTMENU_FOR_ENTRY (IS_LANG_FR \
	? "Ouverture du menu contextuel pour #%1$d - %2$s..." \
	: "Opening context menu for entry#%1$d - %2$s..." \
)

#define LBL_ENTRY_CONTEXTMENU_READ_HTML (IS_LANG_FR \
	? "Lire le contenu HTML" \
	: "Read HTML content" \
)

#define LBL_ENTRY_CONTEXTMENU_READ_EPUB (IS_LANG_FR \
	? "Lire la version EPUB" \
	: "Read EPUB version" \
)

#define LBL_STATUSBAR_OPENING_READERAPP_FOR_ENTRY_FORMAT (IS_LANG_FR \
	? "Chargement de l'appli de lecture pour #%1$d (%3$s) - %2$s..." \
	: "Loading reader app for entry#%1$d (%3$s) - %2$s..." \
)

#define LBL_AFTERREAD_ACTIONONENTRY_WHAT_TO_DO_WITH_ENTRY_CONTENT (IS_LANG_FR \
	? "Que voulez-vous faire de #%d?\n%.128s?" \
	: "What do you want to do with entry #%d?\n%.128s?" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_WHAT_TO_DO_WITH_ENTRY_TITLE (IS_LANG_FR \
	? "Et maintenant ?" \
	: "What now with this entry?" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE (IS_LANG_FR \
	? "Archiver" \
	: "Archive" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE_STAR (IS_LANG_FR \
	? "Archiver + Étoiler" \
	: "Archive + Star" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_ARCHIVE_UNSTAR (IS_LANG_FR \
	? "Archiver + Dés-étoiler" \
	: "Archive + Un-star" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE (IS_LANG_FR \
	? "Dés-archiver" \
	: "Un-archive" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE_STAR (IS_LANG_FR \
	? "Dés-archiver + Étoiler" \
	: "Un-archive + Star" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_UNARCHIVE_UNSTAR (IS_LANG_FR \
	? "Dés-archiver + Dés-étoiler" \
	: "Un-archive + Un-star" \
)

#define LBL_AFTERREAD_ACTIONONENTRY_BUTTON_DONOTHING (IS_LANG_FR \
	? "Ne rien faire" \
	: "Do nothing" \
)

#define LBL_STATUSBAR_AFTERREAD_THANKS_READ_ANOTHER (IS_LANG_FR \
	? "Merci ! Vous pouvez maintenant lire une autre entrée ;-)" \
	: "Thanks! You can read another entry, now ;-)" \
)

#define LBL_STATUSBAR_AFTERREAD_CHANGE_SENT_ON_NEXT_SYNC (IS_LANG_FR \
	? "Ce changement sera envoyé au serveur à la prochaine synchronisation." \
	: "This change will be sent to the server next time you synchronize." \
)

#define LBL_STATUSBAR_AFTERREAD_WHAT_TO_DO_WITH_ENTRY (IS_LANG_FR \
	? "Choisissez quoi faire de l'entrée que vous venez de lire..." \
	: "Choose what to do with the entry you've just read..." \
)

#endif /* SRC_TRANSLATE_H_ */
