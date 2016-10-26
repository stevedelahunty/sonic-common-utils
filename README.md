# sonic-common-utils
Common utilities for the SONiC project.

## Description
This repo contains a utilities library used by the sonic project for thread queues, mutex support and many many other items.

If the code is a utility and used by more then one SONiC component, then the code utility should be placed here.

Some examples of this are the event service used by the sonic-object-library.  The API that starts, stops and the infrastructure for events are in the "event" sources while the user is in the sonic-object-library.


Building
--------
Please see the instructions in the sonic-nas-manifest repo for more details on the common build tools.  [Sonic-nas-manifest](https://github.com/Azure/sonic-nas-manifest)

Build Dependencies
 - sonic-logging
 - libxml2-dev

Package Dependencies:
  libsonic-logging1 libsonic-logging-dev

BUILD CMD: sonic_build libsonic-logging1 libsonic-logging-dev -- clean binary

(c) Dell 2016
