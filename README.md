# BBC BASIC port for ez80-for-rc kit

A port of BBC Basic for eZ80 to run in ADL mode on the ez80-for-rc RC2014/RCBus kit

Implements Version BBC Basic V5 as per upstream repo https://github.com/rtrussell/BBCZ80

### Status

Currently not yet operational - very much a work in progress...

### history/changes

* The project is forked from https://github.com/rtrussell/BBCZ80.
* With changes by Dean Netherton to port to ADL mode running on the eZ80 CPU module for RC2014/RCBus kits.
* It has utilised some of changes done by Dean Belfield (https://github.com/breakintoprogram/agon-bbc-basic-adl) in his original ADL port for the Agon platform.

All original copyrights/licenses etc as per individual contributors.

## BBCZ80

> Readme from upstream: https://github.com/rtrussell/BBCZ80

BBC BASIC (Z80) v5 is an implementation of the BBC BASIC programming language for the Z80 CPU.
It is largely compatible with Acorn's ARM BASIC V but with a few language extensions based on
features of 'BBC BASIC for Windows' and 'BBC BASIC for SDL 2.0'.  These extensions include the
EXIT statement, the address-of operator (^) and byte (unsigned 8-bit) variables and arrays
(& suffix character).

More details of the features added in version 5.00 can be found in the file WHATSNEW.TXT.

The files in green constitute the generic BBC BASIC interpreter which is shared by all the
editions, it (just!) fits in 16 Kbytes so could be held in a ROM of this size.  The files in
the blue box are used to build the generic CP/M edition.  The files in the red box are used
to build the Acorn Z80 Second Processor edition.

Note that the name 'BBC BASIC' is used by permission of the British Broadcasting Corporation
and is not transferrable to a derived or forked work.
