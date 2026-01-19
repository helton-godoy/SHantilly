#!/bin/bash
#
# SHantilly Example: Calendar Widget
# Demonstrates the calendar widget with date selection
#

SHANTILLY_BIN="${SHANTILLY_BIN:-./src/code/SHantilly/bin/SHantilly}"

${SHANTILLY_BIN} <<EODEMO
add label "<b>Select a date:</b>" title
set title stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add calendar "Calendar" cal1 selection

add frame horizontal
add stretch
add pushbutton "&OK" ok apply exit
add pushbutton "&Cancel" cancel exit
end frame

set ok default
set title "SHantilly Calendar"
show
EODEMO

echo "Exit code: $?"
