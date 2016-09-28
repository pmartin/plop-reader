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


## Known limitations of the current version

Known limitations of the current version of the application:

 * Only tested on Pocketbook Touch Lux 3 "TEA" with a 5.14 firmware and Pocketbook Touch Lux 2 "TEA" with a 5.9 firmware
 * It will not work on a 4.x firmware
 * It only supports Wallabag 2.x (which means it doesn't work with *Framabag*) and has only been tested with Wallabag 2.0.6 and 2.0.8
 * At first launch, when no sync has been ever done, the screen is a bit empty.
 * Not much help included.
 * Far from perfect UI.
 * Only reads HTML content of the entries -- which means no images (they are remote and not fetched) and some encoding problems for special characters.
 * Much code-rework is needed.

This is to be considered a **beta version**, to collect useful feedback from implicated power-users.

I've used this application regularly for several weeks now and it's been tested by half-a-dozen users... But, as you surely know, there are always things the developer of an application didn't think about ;-)


### Not implemented, but planned

You can take a look at [my current roadmap](/2016/09/23/roadmap-september-2016.html).

Many things are not implemented yet, but are planned for (several) future versions. Most of them are tracked as [issues on github](https://github.com/pmartin/plop-reader/issues).
