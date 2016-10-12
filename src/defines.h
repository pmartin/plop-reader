#ifndef DEFINES_H_
#define DEFINES_H_


#define PLOP_APPLICATION_FULLNAME "Plop! reader"
#define PLOP_APPLICATION_SHORTNAME "Plop!R"

#define PLOP_VERSION_STR "v0.2.0"
#define PLOP_VERSION_NUM 2
#define PLOP_WEBSITE_URL "http://plop-reader.pascal-martin.fr"
#define PLOP_OPENSOURCE_URL "https://github.com/pmartin/plop-reader"

// On a TL3: /mnt/ext1/system/usr/share/plop-reader
#define PLOP_BASE_DIRECTORY USERDATA "/share/plop-reader"
#define PLOP_ENTRIES_CONTENT_DIRECTORY PLOP_BASE_DIRECTORY "/entries"
#define PLOP_ENTRIES_EPUB_DIRECTORY PLOP_BASE_DIRECTORY "/entries-epub"

// WARNING: Only define this to true if you have backported a patch to add 'export' to the API -- or if your wallabag instance's version is >= 2.2
#define PLOP_FORCE_EPUB_DOWNLOAD_EVEN_IF_OLD_SERVER

#endif /* DEFINES_H_ */
