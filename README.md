
This application allows you to read entries from your [Wallabag v2](https://www.wallabag.org/) instance, on your [Pocketbook Touch Lux](http://www.pocketbook-int.com/fr/products/pocketbook-touch-lux-3).


# Usage

To install and configure this application, read [Installation and configuration](http://plop-reader.pascal-martin.fr/install-and-configure.html).


# Developping

Pre-requisites :

 * Having a good understanding of the demos and tools presented here: [pmartin/pocketbook-demo](https://github.com/pmartin/pocketbook-demo)

If you are using Eclipse, add the following includes:

```
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/include
${PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/include
${PBSDK}/arm-obreey-linux-gnueabi/include/c++/4.8.1
```

# Used libraries

This project uses the following libraries:

 * Lib Inkview, bundled with Pocketbook SDK
 * [Pithikos/C-Thread-Pool](https://github.com/Pithikos/C-Thread-Pool), MIT


# Disclaimer

```
#include <std/disclaimer.h>
```

Like *they* say:

> The content of this application can be *deadly* to your ereader device!
> In short, "If it breaks, you get to keep both pieces."

This application is written by people just like you: not-quite-perfect. Which means there might be mistakes, errors, things we did not quite understand… In any case, especially if you want to fix something: PRs are welcome!

Also please note that, even if the lead developer of this project works at [TEA, The Ebook Alternative](https://www.tea-ebook.com/), which sells Pocketbook ereader devices, this application is written using a **public SDK**, with **absolutely no *insider-knowledge* at all**. This is also a **personnal project**, not linked to any specific company.
