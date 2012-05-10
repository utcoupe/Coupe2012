
HOST=utcoupe@netbook.cheuv.eu
DEST=/home/utcoupe/Bureau/coupe2012/arduino/

cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp asserv4/clientIRC asserv4/driver/ $HOST:$DEST

ssh $HOST "cd $DEST/asserv4/driver && scons"
