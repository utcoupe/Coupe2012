
HOST=utcoupe@10.42.43.94
DEST=/home/utcoupe/Bureau/coupe2012/arduino/

cd /home/thomas/utcoupe/coupe2012/arduino
rsync -avR protocole.h message.h message.cpp asserv/driver/ $HOST:$DEST

ssh $HOST "cd $DEST/asserv/driver && scons"
