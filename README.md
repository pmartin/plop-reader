
This will be a Wallabag application for Pocketbook Touch Lux ereaders. For now, it is only in a very
early stage and still *work in progress*: it is definitely not ready for use!


# Usage

As long as the application doesn't have a screen to input some configuration informations,
you have to write those in a JSON file, pushed to the ereader via USB to:
```
/mnt/ext1/system/share/plop-reader/config.json
```

A template for this file is provided in `files/config-dist.json`


# Developping

Pre-requisites :

 * Having a good understanding of the demos and tools presented here: [pmartin/pocketbook-demo](https://github.com/pmartin/pocketbook-demo)

If you are using Eclipse, add the following includes:

```
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/include
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/include
${PBSDK}/arm-obreey-linux-gnueabi/include/c++/4.8.1
```


# Disclaimer

```
#include <std/disclaimer.h>
```

Like *they* say:

> The content of this application can be *deadly* to your ereader device!
> In short, "If it breaks, you get to keep both pieces."

This application is written by people just like you: not-quite-perfect. Which means there might be mistakes, errors, things we did not quite understand… In any case, especially if you want to fix something: PRs are welcome!

Also please note that, even if the lead developer of this project works at [TEA, The Ebook Alternative](https://www.tea-ebook.com/), which sells Pocketbook ereader devices, this application is written using a **public SDK**, with **absolutely no *insider-knowledge* at all**. This is also a **personnal project**, not linked to any specific company.
