---
layout: page
title: Features
permalink: /features.html
---

This application allows you to read entries from your [Wallabag v2](https://www.wallabag.org/) instance, on your [Pocketbook Touch Lux](http://www.pocketbook-int.com/fr/products/pocketbook-touch-lux-3).

Current version of the app is **v0.1.0**.


## Implemented features

Here's a quick short list of features that are currently implemented and should work in this v0.1.0 release:

 * Fetching the 200 most recent entries from the Wallabag server and synchronizing them to the ereader.
 * Marking an entry as *archived* or *starred* on the ereader.
 * Synchronizing this change to the server.
 * Works offline and only accesses the network when you touch the *sync* button (reactivating wifi if need be).
 * Three distinct lists: unread, archived, starred; and *menu* button to switch between lists.
 * Uses the internal ereader's reading application to open contents, which means you have all the usual *reading* features and options.
 * Logs (current builds are quite verbose, as they are early versions) to `plop-reader-log.html` at the root the device; which means this log can be read from the library and you can get it via USB.


### Not implemented, but planned

You can take a look at [my current roadmap](/2016/09/23/roadmap-september-2016.html).

Many things are not implemented yet, but are planned for (several) future versions. Most of them are tracked as [issues on github](https://github.com/pmartin/plop-reader/issues).
