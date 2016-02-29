# lerdahl

A tonal abstraction external for Pd, written in C. Based on tonal theory by Lerdahl.

### `[lerdahl]`

This is an external for Pd written in C. It is largely based on ideas presented in Fred Lerdahl’s book, *“Tonal Pitch Space”*, on the construction of melodies.

Version 1 takes any MIDI input and outputs values for *pitch class*, *basic space*, *closure*, *tension*, *ratios of asymmetrical attraction*, *ratios of asymmetrical attraction* & *pitch class distance*, and *pitch class distance*. 

The user may change the configuration of the basic space to accommodate any of the 7 modes of the major scale.

The user may also offset pitch class zero if they want zero to be something other than Middle C (C3 /4, etc.).

Version 2 will have list outputs and more useful construction arguments.
