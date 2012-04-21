
HOST=utcoupe@netbook.cheuv.eu
DEST=/home/utcoupe/Bureau/coupe2012/arduino/

cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp asserv_roue/driver/ $HOST:$DEST

ssh $HOST "cd $DEST/asserv_roue/driver && scons"
