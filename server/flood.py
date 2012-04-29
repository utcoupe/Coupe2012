import sys
import socket
import string
import time 
import threading

HOST="10.42.43.1"
PORT=6667
NICK="flooder"
IDENT="flooder"
REALNAME="Flood"
readbuffer=""

def envoyer(commande):
#    print(commande) # "bracelets" autour du print (T.R.)
    commande = commande + "\r\n"
    s.send(commande.encode())

def loop():
    while 1:
        data = s.recv(1024)

        lines = data.decode().split("\n")
        
        for line in lines:
            line = line.strip()
            print(line)
            line = line.split()
            if(line and line[0] == "PING"):
                envoyer("PONG %s\r\n" % line[1])

s = socket.socket()
print("connected")
s.connect((HOST, PORT))

envoyer("NICK %s\r\n" % NICK)
envoyer("USER %s %s bla :%s\r\n" % (IDENT, HOST, REALNAME))
envoyer("JOIN #test")

print("start boucle")

# Lancement du thread
t = threading.Thread(target=loop)
t.setDaemon(True)
t.start()

while 1:
    m = input()
    if m == "flood":
        last = round(time.time())
        count = 0
        while 1:
            envoyer("privmsg #test :Je flood !") #" %f" % time.time())
            if round(time.time()) > last:
                print("%f messages par seconde" % count)
                last = round(time.time())
                count = 0
            else:
                count = count + 1
            
            time.sleep(.01)
        print(time.time())
    else:
        envoyer(m)