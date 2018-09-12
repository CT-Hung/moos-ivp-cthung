#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
VNAME="ual"         
VPORT="9001"
START_POS="0,0"         
M200_IP="192.168.2.1"
SHORE_IP="192.168.1.114"
SHORE_LISTEN="9300"
SHARE_LISTEN="9301"

for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	printf "%s [SWITCHES] [time_warp]   \n" $0
	printf "  --just_make, -j    \n" 
	printf "  --help, -h         \n" 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
      	JUST_MAKE="yes"
    elif [ "${ARGI}" = "--sim" -o "${ARGI}" = "-s" ] ; then
        SIM="SIM"
        echo "Simulation mode ON"
    elif [ "${ARGI}" = "--field" -o "${ARGI}" = "-f" ] ; then
        VMODEL="M300"
        echo "Field mode ON"
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done

#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------

nsplug meta_vehicle.moos targ_$VNAME.moos -f WARP=$TIME_WARP \
    VNAME=$VNAME                     \
    SHARE_LISTEN=$SHARE_LISTEN       \
    SHORE_LISTEN=$SHORE_LISTEN       \
    SHORE_IP=$SHORE_IP               \
    START_POS=$START_POS             \
    M200_IP=$M200_IP                 \
    VPORT=$VPORT                     \
    $SIM                             \
    VMODEL=$VMODEL                     \

nsplug meta_vehicle.bhv targ_$VNAME.bhv -f VNAME=$VNAME     \
    VNAME=$VNAME                     \
    START_POS=$START_POS             \

if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $VNAME MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME.moos >& /dev/null &
printf "Done \n"

uMAC targ_$VNAME.moos

printf "Killing all processes ... \n"
kill %1 %2 %3
printf "Done killing processes.   \n"
