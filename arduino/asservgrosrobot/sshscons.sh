
HOST=utcoupe@fred.cheuv.eu
DEST=/home/utcoupe/Bureau/coupe2012/arduino/
ASSERV_DIR=asservgrosrobot
cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp $ASSERV_DIR/clientIRC $ASSERV_DIR/driver/ $HOST:$DEST

ssh $HOST "cd $DEST/$ASSERV_DIR/driver && scons"
