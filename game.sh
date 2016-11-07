#!/bin/bash

FLAG=(0 0 0 0 0)
MAX_INT=2147483647
BOARD_SIZE=10
TURN_LIMIT=100
DELAY=1
POS1=0,0
POS2=0,0
AI1_PATH=""
AI2_PATH=""
POS1_X=0
POS1_Y=0
POS2_X=0
POS2_Y=0
GUARD="79.867451263456124366124261345"


function randomNumber () {
	local A=$1
	local B=$2
	
	local X1=$((RANDOM<<15))
	X1=$((X1 + $RANDOM))
	echo $((X1%(B - A + 1) + A))
}

function checkLimits () {
	local VAR=$1
	local LOWER_LIMIT=$2
	local UPPER_LIMIT=$3
	
	if (( VAR >= LOWER_LIMIT && VAR <= UPPER_LIMIT )); then
		return 1
	else
		return 0
	fi
}

function secondPositionAvailable () {
	local X=$1
	local Y=$2
	local SIZE=$3
	
	if ! (( X + 8 <= SIZE - 3 || X - 8 >= 1 || Y - 8 >= 1 || Y + 8 <= SIZE )); then	
		error
	fi
	
}

function checkPosition () {
	local X=$1
	local Y=$2
	local LOWER_LIMIT=$3
	local UPPER_LIMIT=$4
	if (( $5 == 0 )); then
		return 1
	fi
	
	if (( Y >= LOWER_LIMIT && Y <= UPPER_LIMIT && X >= LOWER_LIMIT && X <= UPPER_LIMIT- 3 )); then
		return 1
	else
		return 0
	fi
}

function checkProcesses () {
	ID1=$1
	ID2=$2
	ID3=$3
	if (( $# == 3 )); then
		if ! kill -0 $ID1 || ! kill -0 $ID2 || ! kill -0 $ID3 ; then
			echo 1
		else
			echo 0
		fi
	else
		if ! kill -0 $ID2 || ! kill -0 $ID1; then
			echo 1
		else
			echo 0
		fi
	fi
}


function error () {
	exit 1
}

function validatePositions () {
	local X_DIFF=$3-$1
	local Y_DIFF=$4-$2
	
	if ! (( X_DIFF <= -8 || X_DIFF >= 8 || Y_DIFF <= -8 || Y_DIFF >= 8 )); then
		error
	fi
}

while (($# > 1)); do
	case "$1" in
	"-n") BOARD_SIZE="$2";;
	"-k") TURN_LIMIT="$2";;
	"-s") DELAY="$2";;
	"-p1") FLAG[0]=1; POS1="$2";;
	"-p2") FLAG[1]=1; POS2="$2";; 
	"-ai1") FLAG[2]=1; AI1_PATH="$2";;
	"-ai2") FLAG[3]=1; AI2_PATH="$2";;
	*) error
	esac
	shift 2
done

POS1_X=${POS1%,*}
POS1_Y=${POS1#*,}

POS2_X=${POS2%,*}
POS2_Y=${POS2#*,}

if [[ $# != 0 ]]; then
	error
elif ! [[ $BOARD_SIZE =~ ^[0-9]+$ ]] || checkLimits $BOARD_SIZE 9 $MAX_INT; then
	error
elif ! [[ $TURN_LIMIT =~ ^[0-9]+$ ]] || checkLimits $TURN_LIMIT 1 $MAX_INT; then
	error
elif ! [[ $DELAY =~ ^[0-9]+$ ]]; then
	error
elif ! [[ $POS1 =~ ^[0-9]+,[0-9]+$ ]] || checkPosition $POS1_X $POS1_Y 1 $BOARD_SIZE ${FLAG[0]}; then
	error
elif ! [[ $POS2 =~ ^[0-9]+,[0-9]+$ ]] || checkPosition $POS2_X $POS2_Y 1 $BOARD_SIZE ${FLAG[1]}; then
	error
elif ! [ -x $AI1_PATH ]; then
	error
elif ! [ -x $AI2_PATH ]; then
	error
fi

CNT=0
ARRL=(0 0)
ARRU=(0 0)

if (( ${FLAG[0]} == 1 )); then
	if (( ${FLAG[1]} == 1 )); then
		validatePositions $POS1_X $POS1_Y $POS2_X $POS2_Y
	else
		secondPositionAvailable $POS1_X $POS1_Y $BOARD_SIZE
		POS2_X=$(randomNumber 1 $BOARD_SIZE-3)
		if (( POS2_X >= POS1_X - 8 && POS2_X <= POS1_X + 8 )); then
			if (( POS1_Y - 8 >= 1 )); then
				ARRL[0]=1
				ARRU[0]=$(( POS1_Y - 8 ))
				CNT=$(( CNT + 1 ))
			fi
			
			if (( POS1_Y + 8 <= BOARD_SIZE )); then
				ARRL[$CNT]=$(( POS1_Y + 8 ))
				ARRU[$CNT]=$BOARD_SIZE
				CNT=$(( CNT + 1 ))
			fi
			
			TMP=$(( RANDOM % CNT ))
			POS2_Y=$(randomNumber ARRL[$TMP] ARRU[$TMP]) 
		else
			POS2_Y=$(randomNumber 1 $BOARD_SIZE)
		fi
	fi
else
	if (( ${FLAG[1]} == 1 )); then
		secondPositionAvailable $POS2_X $POS2_Y $BOARD_SIZE
		POS1_X=$(randomNumber 1 $BOARD_SIZE-3)
		CNT=0
		if (( POS1_X >= POS2_X - 8 && POS1_X <= POS2_X + 8 )); then
			if (( POS2_Y - 8 >= 1 )); then
				ARRL[0]=1
				ARRU[0]=$(( POS2_Y - 8 ))
				CNT=$(( CNT + 1 ))
			fi
			
			if (( POS2_Y + 8 <= BOARD_SIZE )); then
				ARRL[$CNT]=$(( POS2_Y + 8 ))
				ARRU[$CNT]=$BOARD_SIZE
				CNT=$(( CNT + 1 ))
			fi
			
			TMP=$(( RANDOM % CNT ))
			POS1_Y=$(randomNumber ARRL[$TMP] ARRU[$TMP]) 
		else
			POS1_Y=$(randomNumber 1 $BOARD_SIZE)
		fi
	else
		POS1_X=$(randomNumber 1 $((BOARD_SIZE-3)))
		TMP=$(randomNumber 0 1)
		if ((TMP == 1)); then
			POS1_Y=$(randomNumber 1 $BOARD_SIZE-8)
		else
			POS1_Y=$(randomNumber 9 $BOARD_SIZE)
		fi
		POS2_X=$(randomNumber 1 $BOARD_SIZE-3)
		CNT=0
		if (( POS2_X >= POS1_X - 8 && POS2_X <= POS1_X + 8 )); then
			if (( POS1_Y - 8 >= 1 )); then
				ARRL[0]=1
				ARRU[0]=$(( POS1_Y - 8 ))
				CNT=$(( CNT + 1 ))
			fi
			
			if (( POS1_Y + 8 <= BOARD_SIZE )); then
				ARRL[$CNT]=$(( POS1_Y + 8 ))
				ARRU[$CNT]=$BOARD_SIZE
				CNT=$(( CNT + 1 ))
			fi
			
			TMP=$(( RANDOM % CNT ))
			POS2_Y=$(randomNumber ARRL[$TMP] ARRU[$TMP]) 
		else
			POS2_Y=$(randomNumber 1 $BOARD_SIZE)
		fi
	fi
fi

GUI_IN=3
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${GUI_IN}<>${PIPE}"
rm $PIPE

GUI_OUT=4
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${GUI_OUT}<>${PIPE}"
rm $PIPE

AI1_IN=5
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${AI1_IN}<>${PIPE}"
rm $PIPE

AI1_OUT=6
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${AI1_OUT}<>${PIPE}"
rm $PIPE

AI2_IN=7
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${AI2_IN}<>${PIPE}"
rm $PIPE

AI2_OUT=8
PIPE=$(mktemp -u)
mkfifo $PIPE
eval "exec ${AI2_OUT}<>${PIPE}"
rm $PIPE

if (( ${FLAG[2]} == 0 && ${FLAG[3]} == 0 )); then
	./sredniowiecze_gui_with_libs.sh -human1 -human2 <&${GUI_IN} >&${GUI_OUT} &
	GUI_ID=$!
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	wait $GUI_ID
	GUI_CODE=$?
	if (( GUI_CODE != 0 )); then
		error
	fi
	
elif (( ${FLAG[2]} == 1 && ${FLAG[3]} == 1 )); then
	./sredniowiecze_gui_with_libs.sh <&${GUI_IN} >&${GUI_OUT} &
	GUI_ID=$!
	
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	
	$AI1_PATH <&${AI1_IN} >&${AI1_OUT} &
	AI1_ID=$!
	
	$AI2_PATH <&${AI2_IN} >&${AI2_OUT} &
	AI2_ID=$!
	
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${AI1_IN}
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${AI2_IN} 
	
	while (( 1 )); do
		
		sleep $DELAY
		
		echo $GUARD >&${AI1_OUT}
		while read line <&${AI1_OUT}; do	
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${GUI_OUT}
			else
				echo $line >&${GUI_IN}
				echo $line >&${AI2_IN}
			fi
			STATUS=$(checkProcesses $AI1_ID $GUI_ID $AI2_ID)
			if [[ $line == "END_TURN" ]]; then
				break
			fi
			if (( $STATUS == 1 )); then
				break
			fi
		done
		
		sleep $DELAY
		
		echo $GUARD >&${AI2_OUT}
		while read line <&${AI2_OUT}; do
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${AI2_OUT}
			else
				echo $line >&${GUI_IN}
				echo $line >&${AI1_IN}
			fi
			STATUS=$(checkProcesses $AI1_ID $GUI_ID $AI2_ID)
			if (( $STATUS == 1 )); then
				break
			fi
			if [[ $line == "END_TURN" ]]; then
				break
			fi
		done
		
		STATUS=$(checkProcesses $AI1_ID $AI2_ID $GUI_ID)
		if (( $STATUS == 1 )); then
			echo $GUARD >&${AI2_OUT}
			while read line <&${AI2_OUT}; do
				if [[ $line == $GUARD ]]; then
					break
				fi
				echo $line >&${GUI_IN}
				echo $line >&${AI1_IN}
				if [[ $line == "END_TURN" ]]; then
					break
				fi
			done;
			
			echo $GUARD >&${AI1_OUT}
			while read line <&${AI1_OUT}; do
				if [[ $line == $GUARD ]]; then
					break
				fi
				echo $line >&${GUI_IN}
				echo $line >&${AI2_IN}
				if [[ $line == "END_TURN" ]]; then
					break
				fi
			done;
			break
		fi
		
	done
	
	if ! kill -0 $AI2_ID; then
		wait $AI2_ID
		AI_CODE=$?
		if (( AI_CODE != 0  && AI_CODE != 1 && AI_CODE != 2)); then
			error
		fi
	else
		kill $AI2_ID
	fi
	
	if ! kill -0 $AI1_ID; then
		wait $AI1_ID
		AI_CODE=$?
		if (( AI_CODE != 0  && AI_CODE != 1 && AI_CODE != 2)); then
			error
		fi
	else
		kill $AI1_ID
	fi
	
	if ! kill -0 $GUI_ID; then
		wait $GUI_ID
		GUI_CODE=$?
		if (( GUI_CODE != 0 )); then
			error
		fi
	fi
	
elif (( ${FLAG[3]} == 1 )); then
	./sredniowiecze_gui_with_libs.sh -human1 <&${GUI_IN} >&${GUI_OUT} &
	GUI_ID=$!
	$AI2_PATH <&${AI2_IN} >&${AI2_OUT} &
	AI2_ID=$!
	
	
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${AI2_IN} 
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	
	while (( 1 )); do
		
		echo $GUARD >&${GUI_OUT}
		while read line <&${GUI_OUT}; do
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${GUI_OUT}
			else
				echo $line >&${AI2_IN}
			fi
			
			if [[ $line == "END_TURN" ]]; then
				break
			fi
				
			STATUS=$(checkProcesses $AI2_ID $GUI_ID)
			if (( $STATUS == 1 )); then
				break
			fi
		done
		
		echo $GUARD >&${AI2_OUT}
		while read line <&${AI2_OUT}; do
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${AI2_OUT}
			else
				echo $line >&${GUI_IN}
			fi
			STATUS=$(checkProcesses $AI2_ID $GUI_ID)
			if (( $STATUS == 1 )); then
				break
			fi
			if [[ $line == "END_TURN" ]]; then
				break
			fi
		done
		
		STATUS=$(checkProcesses $AI2_ID $GUI_ID)
		if (( STATUS == 1 )); then
			echo $GUARD >&${AI2_OUT}
			while read line <&${AI2_OUT}; do
				if [[ $line == $GUARD ]]; then
					break
				fi
				echo $line >&${GUI_IN}
				if [[ $line == "END_TURN" ]]; then
					break
				fi
			done;
			break
		fi
		
	done
	
	
	if ! kill -0 $AI2_ID; then
		wait $AI2_ID
		AI_CODE=$?
		if (( AI_CODE != 0  && AI_CODE != 1 && AI_CODE != 2)); then
			error
		fi
	else
		kill $AI2_ID
	fi
	
	if ! kill -0 $GUI_ID; then
		wait $GUI_ID
		GUI_CODE=$?
		if (( GUI_CODE != 0 )); then
			error
		fi
	fi
	
else
	./sredniowiecze_gui_with_libs.sh -human2 <&${GUI_IN} >&${GUI_OUT} &
	GUI_ID=$!
	$AI1_PATH <&${AI1_IN} >&${AI1_OUT} &
	AI1_ID=$!
	
	
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${AI1_IN} 
	echo "INIT $BOARD_SIZE $TURN_LIMIT 1 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	echo "INIT $BOARD_SIZE $TURN_LIMIT 2 $POS1_X $POS1_Y $POS2_X $POS2_Y" >&${GUI_IN}
	
	while (( 1 )); do
		
		echo $GUARD >&${AI1_OUT}
		while read line <&${AI1_OUT}; do	
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${AI1_OUT}
			else
				echo $line >&${GUI_IN}
			fi
			if [[ $line == "END_TURN" ]]; then
				break
			fi
			
			STATUS=$(checkProcesses $AI1_ID $GUI_ID)
			if (( STATUS == 1 )); then
				break
			fi
		done
		
		echo $GUARD >&${GUI_OUT}
		
		while read line <&${GUI_OUT}; do
			if [[ $line == $GUARD ]]; then
				echo $GUARD >&${GUI_OUT}
			else
				echo $line >&${AI1_IN}
			fi
			
			if [[ $line == "END_TURN" ]]; then
				break
			fi
				
			STATUS=$(checkProcesses $AI1_ID $GUI_ID)
			if (( $STATUS == 1 )); then
				break
			fi
		done
		
		STATUS=$(checkProcesses $AI1_ID $GUI_ID)
		if (( STATUS == 1 )); then
			echo $GUARD >&${AI1_OUT}
			while read line <&${AI1_OUT}; do
				if [[ $line == $GUARD ]]; then
					break
				fi
				echo $line >&${GUI_IN}
				if [[ $line == "END_TURN" ]]; then
					break
				fi
			done;	
			break
		fi
		
	done

	if ! kill -0 $AI1_ID; then
		wait $AI1_ID
		AI_CODE=$?
		if (( AI_CODE != 0  && AI_CODE != 1 && AI_CODE != 2)); then
			error
		fi
	else
		kill $AI1_ID
	fi
	
	if ! kill -0 $GUI_ID; then
		wait $GUI_ID
		GUI_CODE=$?
		if (( GUI_CODE != 0 )); then
			error
		fi
	fi
	
fi

exit 0
