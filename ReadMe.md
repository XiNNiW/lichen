
# Lichen

Lichen is an experimental music/dsp livecoding environement. It is under heavy construction right now so everything is subject to change at anytime.

## description
Lichen is a symbiotic organism. DSP code in cpp is made composable at runtime by lua scripts. Right now, there is a [plugin](https://github.com/XiNNiW/micro-lichen) for the micro text editor that lets you run lichen and send it scripts to run. Lichen then turns that script into a signal that is played on your soundcard.

## vision
### musical instrument
I hope Lichen will be a way first and formost to make music, allowing the artist to think about sound and synthesis in the same language that they think about rythm, form, and melody. I also want the artist to be able to manipulate time direcly as a means of expressing music.

### dsp playground
I also hope that it can separate concerns well enough to have a DSP library that can be used in a variety of platforms, including low power and/or embedded. Lichen will hopefully be a tool that can be used to quickly prototype ideas for instruments which can then be translated to efficient cpp code.

## getting started
1. follow build and install instructions below.
1. install the text editor, [micro](https://github.com/zyedidia/micro)
1. install the [micro-lichen](https://github.com/XiNNiW/micro-lichen) plugin
1. open micro and save a new file containing ```return osc(440.0)```
1. press ```Alt-Shift-.``` to send the code to lichen.
1. you should hear a sine tone playing an A natural.

## build

1. install libsoundio and dev headers (on ubuntu this is on apt)
1. ```git submodule update --init``` to get sol3 for lua scripting
1. ```cmake .```
1. ```make```

## test
1. ```cmake .```
1. ```make test```

## install
1. if ubuntu linux run ```make install```
1. if any other platform copy ```lichen``` from ```bin``` to where your OS stores installed programs (ex. ```/usr/bin``` on my machine.)

