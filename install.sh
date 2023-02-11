#!/bin/bash
install -m 755 -s kgcalc /usr/bin/
mkdir -p /usr/share/kgcalc
install -m 644  kgcalc.png /usr/share/kgcalc/kgcalc.png
install -m 644  kgcalc.desktop /usr/share/applications
