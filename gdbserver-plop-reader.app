#!/bin/sh

echo "Activating wifi"
/mnt/ext1/applications/devutils.app 'wifi:activate'

if [[ -f "/mnt/ext1/applications/Plop! reader.app" ]]
then
    /mnt/ext1/system/usr/bin/gdbserver 0.0.0.0:10002 "/mnt/ext1/applications/Plop! reader.app"
else
    /mnt/ext1/system/usr/bin/gdbserver 0.0.0.0:10002 "/mnt/ext1/applications/Plop!reader.app"
fi
