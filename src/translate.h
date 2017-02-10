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

#endif /* SRC_TRANSLATE_H_ */
