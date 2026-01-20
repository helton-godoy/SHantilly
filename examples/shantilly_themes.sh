#!/bin/bash
#
# shantilly Example: Themes Demo
# Demonstrates the theme manager (dark/light/system)
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
add label "<h2>shantilly Theme Demo</h2>" title
set title stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add separator

add label "The theme is automatically detected from your system." info
set info stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add frame horizontal
add checkbox "Dark Mode" darkmode
add checkbox "System Theme" systemtheme checked
end frame

add separator

add frame horizontal
add stretch
add pushbutton "&Close" close apply exit
end frame

set close default
set title "shantilly Theme Test"
show
EODEMO

echo "Exit code: $?"
