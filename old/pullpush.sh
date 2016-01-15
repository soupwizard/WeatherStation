#!/bin/bash

APP=/opt/pullpush/pullpush.py
USER=pi
OUT=/home/pi/pullpush.log

case "$1" in

start)
	echo "starting pullpush"
	$APP > $OUT 2>$OUT &
	;;

stop)
	pkill -f $APP
	;;

*)
	echo "usage: $0 (start|stop)"
esac

exit 0
