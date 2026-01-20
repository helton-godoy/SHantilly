#!/bin/bash
#
# shantilly Example: ProgressBar Widget
# Demonstrates the progressbar widget with normal and busy modes
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

# Demo with static progress bar
${SHANTILLY_BIN} <<EODEMO
add label "<b>Download Progress</b>" title
set title stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add separator

add label "Downloading file..." status
set status stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add progressbar "Progress" progress1
set progress1 value 65

add separator

add label "<small>Processing in background...</small>" busy_label
set busy_label stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add progressbar "Busy" progress2 busy

add separator

add frame horizontal
add stretch
add pushbutton "&Close" close exit
end frame

set close default
set title "shantilly ProgressBar"
show
EODEMO

echo "Exit code: $?"
