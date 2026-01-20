#!/bin/bash
#
# shantilly Example: Calendar Widget
# Demonstrates the calendar widget with date selection
#

# Detect shantilly binary
if [ -z "${SHANTILLY_BIN}" ]; then
    if [ -f "../build/bin/shantilly" ]; then
        SHANTILLY_BIN="../build/bin/shantilly"
    elif [ -f "./build/bin/shantilly" ]; then
        SHANTILLY_BIN="./build/bin/shantilly"
    else
        SHANTILLY_BIN="shantilly"
    fi
fi

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
set title "shantilly Calendar"
show
EODEMO

echo "Exit code: $?"
