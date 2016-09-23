---
layout: post
title:  "Roadmap (September 2016)"
date:   2016-09-23
---

At the end of September 2016, this project's development has not gone far yet, but here the idea I have in mind for the first couple of versions.

Note this is a couple of ideas I have on my mind now, but, as always, the plan might well change ;-)


## v0.1: first private beta

This version should only be released to a few power-user.

Goal: collect some feedback, to determine if this is useful and if the project should continue, or this is an idea that doesn't bring anything useful to the table and should be terminated.

This version has an ugly UI, doesn't have much error-handling (it'll crash if you don't do everything *the right way*), but bring synchronization between the device and the server and offers reading of the entries in HTML format.


## v0.2: first public beta

In this version, there is more error-handling and more logging. Enough to prevent crashes in the most common error-cases and get useful feedback from a larger base of users.

This is still a beta version, and still to be considered as unstable; it also still targets power-users.


## v0.3: EPUB reading

Until now, the application only offered reading of HTML version of entries, which especially means no images.

With this release, depending on the availability of the EPUB-export feature in wallabag itself (in its API), EPUB files for entries will be downloaded when syncing from server and should provide for a much better reading experience.

This is still an un-finished version, still targetting power-users.


## v0.4: a better UI

This release focuses on a better UI and improvements on UX.

This should be the first version targetting less *advanced* users.


## v0.5: technical improvements

The application now has a few recurrent users, and it's time to think a bit more about improving some underlying technical details.

This version should not bring much chances to the end-user, but will rework a few internal components, which should help in the long run.
