cd ~/coupe2012/executables/hokuyo/clientIRC/

if [ $1 == red ]
then
	./hokuyobot.py -S 10.42.43.1 --exec-params "-col,2"
else
	./hokuyobot.py -S 10.42.43.1 --exec-params "-col,1"
fi

