#!/bin/bash
#
# Demo2 - Input de dados com shantilly
# Teste de compatibilidade - widgets de entrada
#

# Detect shantilly binary
if [ -z "${SHANTILLY_BIN}" ]; then
    if [ -f "../../build/bin/shantilly" ]; then
        SHANTILLY_BIN="../../build/bin/shantilly"
    elif [ -f "../build/bin/shantilly" ]; then
        SHANTILLY_BIN="../build/bin/shantilly"
    else
        SHANTILLY_BIN="shantilly"
    fi
fi

while IFS=$'=' read key value; do
	case ${key} in
	cb1)
		if [[ ${value} == "1" ]]; then
			echo "Option 1 is checked"
		else
			echo "Option 1 is unchecked"
		fi
		;;
	txt1)
		echo "Text entered: ${value}"
		;;
	okay)
		echo "User clicked Ok pushbutton"
		;;
	cancel)
		echo "User clicked Cancel pushbutton"
		;;
	esac
done < <(

	${SHANTILLY_BIN} <<EODEMO
add checkbox "&Option 1" cb1
add textbox "&Text field" txt1 "text to edit"
add frame horizontal
add stretch
add pushbutton O&k okay apply exit
add pushbutton &Cancel cancel exit
end frame
set okay default
set cb1 focus
set title "shantilly Demo 2"
show
EODEMO

)

echo "Demo2 completed"
