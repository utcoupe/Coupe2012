
HOST=utcoupe@netbook.cheuv.eu
DEST=/home/utcoupe/Bureau/coupe2012/arduino/

cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp asserv3/driver/ $HOST:$DEST

ssh $HOST "cd $DEST/asserv3/driver && scons"
