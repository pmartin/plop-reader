---
layout: post
title:  "The informations displayed for an entry, in v0.2"
date:   2016-09-28
---

The main screen of the application, the list of entries, looks like this, for v0.2:

![The main screen](/images/v0.2.0/screenshot-01-main-screen.png)

I'll admit, this is not looking really great (for now, work on UI is planned for [v0.4](https://github.com/pmartin/plop-reader/milestone/4)), but each entry displays a set of informations that could be of interest:

![One entry](/images/v0.2.0/screenshot-02-one-entry.png)

The second line of each entry is composed of the following data:

 * `#X/Y`: `X` is the id of the entry in the local database, and `Y` is the id of the entry in your Wallabag instance.
 * `aX/Y`: indicates if the entry is *archived* in the local database on your ereader (`X`) and in your wallabag instance (`Y`, fetched during the last synchronization).
 * `*X/Y`: indicates if the entry is *starred* in the local database on your ereader (`X`) and in your wallabag instance (`Y`, fetched during the last synchronization).
 * `uX/Y`: UTC timestamp of the last change on the entry in the local database on your ereader (`X`) and in your wallabag instance (`Y`, fetched during the last synchronization).
 * `tX`: estimated reading time of the entry, in minutes.

In the future, when work has been done on the UI, these informations will be displayed in a more visually-pleasing way - or not displayed, for some that are only useful now for debugging purposes.
