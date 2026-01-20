#!/bin/bash
#
# shantilly Example: Table Widget
# Demonstrates the table widget with editable cells, headers and search
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

while IFS=$'=' read key value; do
	# Table cell edits are reported as table1[row][col]=value
	if [[ ${key} =~ ^table1\[([0-9]+)\]\[([0-9]+)\]$ ]]; then
		row="${BASH_REMATCH[1]}"
		col="${BASH_REMATCH[2]}"
		echo "Cell [${row}][${col}] changed to: ${value}"
	fi
done < <(

	${SHANTILLY_BIN} <<EODEMO
add label "<b>Data Table</b>" title
set title stylesheet "qproperty-textInteractionFlags: NoTextInteraction;"

add separator

add table "Employee Data" table1 headers search
set table1 headers "Name;Department;Salary"
set table1 add_line "John Doe;Engineering;75000"
set table1 add_line "Jane Smith;Marketing;65000"
set table1 add_line "Bob Johnson;Sales;70000"
set table1 add_line "Alice Brown;HR;60000"
end table

add separator

add frame horizontal
add stretch
add pushbutton "&Save" save apply exit
add pushbutton "&Cancel" cancel exit
end frame

set save default
set title "shantilly Table"
show
EODEMO

)

echo "Table demo completed"
