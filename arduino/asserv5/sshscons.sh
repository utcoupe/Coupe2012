
HOST=utcoupe@fred.cheuv.eu
DEST=/home/utcoupe/Bureau/coupe2012/arduino/

cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp asserv5/clientIRC asserv5/driver/ $HOST:$DEST

#ssh $HOST "cd $DEST/asserv5/driver && scons"
