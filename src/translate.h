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

#define LBL_HEADER_LONG_ENTRIES (IS_LANG_FR \
	? "Longs articles" \
	: "Long entries" \
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

#define LBL_MAINMENU_MODE_LONG_ENTRIES (IS_LANG_FR \
	? "Lecture > 10 min" \
	: "Reading > 10mn" \
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

#define LBL_SYNC_OAUTH_CREATE_TOKEN (IS_LANG_FR \
	? "Création d'un jeton OAuth" \
	: "Creating OAuth token" \
)

#define LBL_SYNC_OAUTH_ERROR_CREATE_TOKEN_INVALID_JSON (IS_LANG_FR \
	? "Échec création jeton OAuth : le serveur a retourné une chaine JSON invalide : " \
	: "Could not create OAuth token: server returned an invalid JSON string: " \
)

#define LBL_SYNC_OAUTH_ERROR_CREATE_TOKEN_STATUS_CODE (IS_LANG_FR \
	? "Échec création jeton OAuth : le serveur a retourné un code statut : " \
	: "Could not create OAuth token: server returned status code: " \
)

#define LBL_SYNC_ERROR_HINT_SHOULD_SET_HTTP_BASIC (IS_LANG_FR \
	? "Vous devriez renseigner 'http_login' et 'http_password', ou vérifier leurs valeurs, dans le fichier ce configuration JSON." \
	: "You should set 'http_login' and 'http_password', or check their value, in the JSON configuration file." \
)

#define LBL_SYNC_OUATH_REFRESH_TOKEN (IS_LANG_FR \
	? "Rafraichissement du jeton OAuth" \
	: "Refreshing OAuth token" \
)

#define LBL_SYNC_OAUTH_ERROR_REFRESH_TOKEN_INVALID_JSON (IS_LANG_FR \
	? "Échec rafraichissement jeton OAuth : le serveur a retourné une chaine JSON invalide : " \
	: "Could not refresh OAuth token: server returned an invalid JSON string: " \
)

#define LBL_SYNC_OAUTH_ERROR_REFRESH_TOKEN_STATUS_CODE (IS_LANG_FR \
	? "Échec rafraichissement jeton OAuth : le serveur a retourné un code statut : " \
	: "Could not refresh OAuth token: server returned status code: " \
)

#define LBL_SYNC_FETCH_ENTRIES_HTTP_REQUEST (IS_LANG_FR \
	? "Téléchargement des entrées : requête HTTP" \
	: "Fetching entries: HTTP request" \
)

#define LBL_SYNC_SAVE_ENTRIES_TO_LOCAL_DB (IS_LANG_FR \
	? "Enregistrement des entrées en base de données locale..." \
	: "Saving entries to local database..." \
)

#define LBL_SYNC_ERROR_DECODE_ENTRIES_INVALID_JSON (IS_LANG_FR \
	? "Échec décodage des entrées : le serveur a retourné une chaine JSON invalide : " \
	: "Could not decode entries: server returned an invalid JSON string: " \
)

#define LBL_SYNC_FETCH_ENTRIES_ERROR_STATUS_CODE (IS_LANG_FR \
	? "Échec du téléchargement des entrées : le serveur a retourné un code statut : " \
	: "Could not load entries from server: server returned status code: " \
)

#define LBL_SYNC_DOWNLOAD_EPUB_FILES (IS_LANG_FR \
	? "Téléchargement des fichiers EPUB..." \
	: "Downloading EPUB files..." \
)

#define LBL_SYNC_DOWNLOAD_EPUB_FILES_DONE (IS_LANG_FR \
	? "Téléchargement des fichiers EPUB : terminé \\o/." \
	: "Downloading EPUB files: done \\o/." \
)

#define LBL_SYNC_DOWNLOAD_EPUB_FILE_FOR_ENTRY (IS_LANG_FR \
	? "Téléchargement de l'EPUB pour %1$d/%2$s..." \
	: "Downloading EPUB for %1$d/%2$s..." \
)

#define LBL_SYNC_SEND_UPDATES_TO_SERVER (IS_LANG_FR \
	? "Envoi des mises à jour au serveur..." \
	: "Sending updated statuses to server" \
)

#define LBL_SYNC_SEND_UPDATES_TO_SERVER_ERROR_STATUS_CODE (IS_LANG_FR \
	? "Échec envoi mises à jour : le serveur a retourné un code statut : " \
	: "Could not sync entry to server: server returned status code: " \
)

#define LBL_SYNC_FETCH_SERVER_VERSION (IS_LANG_FR \
	? "Obtention de la version du serveur..." \
	: "Fetching server version" \
)

#define LBL_SYNC_FETCH_SERVER_VERSION_ERROR_STATUS_CODE (IS_LANG_FR \
	? "Échec obtention version serveur : le serveur a retourné un code statut : " \
	: "Could not load entries from server: server returned status code: " \
)

#define LBL_SYNC_DIALOG_TITLE (IS_LANG_FR \
	? "Synchronisation avec le serveur" \
	: "Synchronizing with server" \
)

#define LBL_SYNC_START_SYNC (IS_LANG_FR \
	? "Lancement de la synchronisation..." \
	: "Starting synchronization" \
)

#define LBL_SYNC_FETCHING_RECENT_ENTRIES (IS_LANG_FR \
	? "Téléchargement des entrées récentes..." \
	: "Fetching recent entries from server" \
)

#define LBL_SYNC_FAILED_TOO_BAD (IS_LANG_FR \
	? "Dommage ;-(" \
	: "Too bad ;-(" \
)

#define LBL_SYNC_SUCCESS_DONE (IS_LANG_FR \
	? "Synchronisation terminée \\o/" \
	: "All done \\o/" \
)

#define LBL_ONBOARDING_EXIT (IS_LANG_FR \
	? "Quitter" \
	: "Exit" \
)

#define LBL_ONBOARDING_EXIT_01 (IS_LANG_FR \
	? "Utilisez ce bouton" \
	: "Use this button" \
)

#define LBL_ONBOARDING_EXIT_02 (IS_LANG_FR \
	? "pour fermer" \
	: "to exit the" \
)

#define LBL_ONBOARDING_EXIT_03 (IS_LANG_FR \
	? "l'application." \
	: "application." \
)

#define LBL_ONBOARDING_SYNC (IS_LANG_FR \
	? "Synchronisation" \
	: "Synchronization" \
)

#define LBL_ONBOARDING_SYNC_01 (IS_LANG_FR \
	? "Utilisez ce bouton pour" \
	: "Use this button to" \
)

#define LBL_ONBOARDING_SYNC_02 (IS_LANG_FR \
	? "obtenir les données depuis" \
	: "get data from your" \
)

#define LBL_ONBOARDING_SYNC_03 (IS_LANG_FR \
	? "votre serveur wallabag ;-)" \
	: "wallabag server ;-)" \
)

#define LBL_ONBOARDING_MENU (IS_LANG_FR \
	? "Menu" \
	: "Open Menu" \
)

#define LBL_ONBOARDING_MENU_01 (IS_LANG_FR \
	? "Utilisez ce bouton pour" \
	: "Use this button to" \
)

#define LBL_ONBOARDING_MENU_02 (IS_LANG_FR \
	? "ouvrir le menu principal" \
	: "open the main menu" \
)

#define LBL_ONBOARDING_MENU_03 (IS_LANG_FR \
	? "de l'application." \
	: "of the application." \
)

#define LBL_ONBOARDING_STATUSBAR (IS_LANG_FR \
	? "Barre d'état" \
	: "Status Bar" \
)

#define LBL_ONBOARDING_STATUSBAR_01 (IS_LANG_FR \
	? "Vous trouverez souvent de l'aide" \
	: "You'll often find some help" \
)

#define LBL_ONBOARDING_STATUSBAR_02 (IS_LANG_FR \
	? "ou un indicateur d'avancement" \
	: "or progress indicator" \
)

#define LBL_ONBOARDING_STATUSBAR_03 (IS_LANG_FR \
	? "dans la barre de statut." \
	: "in the status bar." \
)

#define LBL_ONBOARDING_INFO_NO_ENTRIES_01 (IS_LANG_FR \
	? "Vous n'avez aucune entrée sur cette liseuse." \
	: "You don't have any entries on this ereader." \
)

#define LBL_ONBOARDING_INFO_NO_ENTRIES_02 (IS_LANG_FR \
	? "Pour plus d'informations (configuration, utilisation), lisez :" \
	: "For more informations about configuration and usage, read:" \
)

#define LBL_ONBOARDING_INFO_NO_ENTRIES_03 (IS_LANG_FR \
	? "http://plop-reader.pascal-martin.fr/install-and-configure.html" \
	: "http://plop-reader.pascal-martin.fr/install-and-configure.html" \
)

#endif /* SRC_TRANSLATE_H_ */
