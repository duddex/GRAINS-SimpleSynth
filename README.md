# GRAINS-SimpleSynth
Simple Synthesizer for the AEModular GRAINS module based on the "the_synth" library

Get the library here: https://github.com/dzlonline/the_synth

More information about the GRAINS module: https://wiki.aemodular.com/#/modules/grains

**NOTE:** There seems to be a problem with the tracking of notes depending on whether GRAINS is connected to
USB or not. Tracking is correct, when USB is connected. But then you hear the "USB whining" noise.

## Input
* Gate input to "A"
* Pitch input to "3"

## Output
Audio output from "OUT" (set GRAINS module to "G" mode)

## Control
* P1 = Select the waveform (from P1 left to P1 to the right: SINE, TRIANGLE, SQUARE, SAW, RAMP, NOISE)
* P2 = Length of the tone
* P3 = Select one of the four "the_synth" envelopes
