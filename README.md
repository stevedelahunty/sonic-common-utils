# sonic-common-utils
This repo contains a utilities library used by the SONiC project for thread queues, mutex support, and  other items. If the code is a utility and used by more then one SONiC component,  the code utility should be placed here.

Examples of this are the event service used by the sonic-object-library. The API that starts, stops, and the infrastructure for events are in the "event" sources while the user is in the sonic-object-library.

## Build
See [sonic-nas-manifest](https://github.com/Azure/sonic-nas-manifest) for complete information on common build tools.

### Build dependencies
* `sonic-logging`
* `libxml2-dev`

### Package dependencies
* `libsonic-logging1` 
* `libsonic-logging-dev`

BUILD CMS: sonic_build libsonic-logging1 libsonic-logging-dev -- clean binary

(c) Dell 2016
