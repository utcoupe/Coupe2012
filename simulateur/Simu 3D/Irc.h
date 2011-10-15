#ifndef IRC_H
#define IRC_H

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <time.h>
#include <cctype>

using namespace std;

void ThreadFunction(void* UserData);

class stringHandler {

public:

    void splitString(const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters = " ") {
        std::string::size_type lastPos = str.find_first_not_of(delimiters,0);
        std::string::size_type pos = str.find_first_of(delimiters,lastPos);

        while (std::string::npos != pos || std::string::npos != lastPos) {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters,pos);
            pos = str.find_first_of(delimiters,lastPos);
        }
    }

    std::string replaceString(std::string str,std::string l,std::string rep) {
        std::string::size_type pos = str.find(l);

        while(std::string::npos != pos) {
            str.replace(pos,l.length(),rep);
            pos = str.find(l);
        }

        return str;
    }

    std::string mergeLast(std::vector<std::string>& arr,int at) {
        std::string end = "";
        for (unsigned int i = at; i < arr.size(); i++) {
            end += arr[i];
            end += " ";
        }

        end.erase(0,1);
        end.erase(end.length() - 1,1);
        return end;
    }

    std::string getNick(std::string hmask) {
        hmask = hmask.substr(0,hmask.find_first_of("!"));
        hmask.erase(0,1);
        return hmask;
    }

    std::string toLower(std::string str) {
        char uni[] = {'Å','Ä','Ö'};
        char to[] = {'å','ä','ö'};
        for (unsigned int i = 0; i < str.length(); i++) {
            for(unsigned int t = 0; t < 3; t++) {
                if (str[i] == uni[t]) {
                    str[i] = to[t];
                }
            }

            str[i] = std::tolower(str[i]);
        }

        return str;
    }

    std::string generateWhitespace(std::string chanstr,std::string nickstr,std::string msgstr) {
        unsigned int whitespaces = 18 + chanstr.length() + nickstr.length();
        unsigned int curSize = 0;
        std::string endStr = "";

        std::vector<std::string> words;
        splitString(msgstr,words," ");

        for (unsigned int i = 0; i < words.size(); i++) {
            if (words[i].length() > 80 - whitespaces) {
                std::vector<std::string>::iterator it(words.begin());
                words.insert(it + i + 1,words[i].substr((79 - whitespaces),words[i].length() - (79 - whitespaces)));
                words[i] = words[i].substr(0,(79 - whitespaces));
            }

            if (words[i].length() + curSize < 80 - whitespaces) {
                endStr += words[i].append(" ");
                curSize += words[i].length();
            } else {
#ifndef _WIN32
                endStr.append("\n");
#endif

                endStr.append(((80 - whitespaces) - curSize) + whitespaces,' ');
                curSize = 0;
                i--;
            }
        }

        return endStr;
    }

    std::string applyUniFix(std::string str) {
        str = replaceString(str,std::string("\xC3").append("\xA5"),"å");
        str = replaceString(str,std::string("\xC3").append("\xA4"),"ä");
        str = replaceString(str,std::string("\xC3").append("\xB6"),"ö");
        str = replaceString(str,std::string("\xC3").append("\x85"),"Å");
        str = replaceString(str,std::string("\xC3").append("\x84"),"Ä");
        str = replaceString(str,std::string("\xC3").append("\x96"),"Ö");
        return str;
    }

    std::string parseUni(std::string str) {
        str = replaceString(str,"å","\x86");
        str = replaceString(str,"ä","\x84");
        str = replaceString(str,"ö","\x94");
        str = replaceString(str,"Å","\x8F");
        str = replaceString(str,"Ä","\x8E");
        str = replaceString(str,"Ö","\x99");
        return str;
    }
};


class IRCConnection {

    sf::Thread * thread;
    sf::SocketTCP mainSocket;
    char recv[1024];
    size_t r;
    std::string chan, nick, srecv;
    std::vector<std::string> recvArr, recvArr2;
    std::vector< std::vector<std::string> > trigger;
    stringHandler * sH;
    char timeBuffer[10];

public:

    bool channelSendMsg(std::string channel, std::string message) {
        if (!socketSend(&mainSocket,std::string("PRIVMSG ").append(channel).append(" :").append(message)))
            return false;

        std::cout << " [" << timeBuffer << "] [" << channel << "] <" << nick << "> " << sH -> generateWhitespace(channel,nick,sH -> parseUni(message)) << "\n";
        return true;
    }

    virtual void doCmd(std::string time,std::string chan,std::string user,std::string msg) = 0;

    IRCConnection(stringHandler * ss = NULL) {
        if(ss==NULL) {
            ss = new stringHandler;
        }
        sH = ss;
    }

    bool channelJoin(std::string channel) {
        if (!socketSend(&mainSocket,std::string("JOIN "+channel)))
            return false;

        return true;
    }

    bool connect(sf::IPAddress * hostAddr,int port,std::string nickname,const char * name) {

        std::cout << "Retrieving triggers... ";
        triggerInit(trigger,getTriggersFrom("sverker.burbruee.se","/api.php"));
        std::cout << "DONE!\n[Loaded " << trigger.size() << " triggers.]\n\n";

        chan = "";
        nick = nickname;

        std::cout << "Checking host... ";

        if (!hostAddr -> IsValid()) {
            std::cout << "Could not validate host.";
            return false;
        }

        std::cout << "SUCCESS!\n" << "Connecting to " << hostAddr -> ToString() << "... ";

        if (!sockConnect(&mainSocket,port,hostAddr -> ToString())) {
            std::cout << "Could not connect to host.";
            return false;
        }

        std::cout << "SUCCESS!\nSending initial data to server... ";

        // Création d'un thread avec notre fonction
        thread = new sf::Thread(&ThreadFunction,this);

        if (!socketSend(&mainSocket,std::string("NICK ").insert(5,nick)))
            return false;
        listen();

        if (!socketSend(&mainSocket,std::string("USER  ignored irc.quakenet.org :sverker").insert(5,name)))
            return false;
        listen();
        listen();


        // Lancement du thread !
        thread->Launch();


        std::cout << "DONE!\n\n <<  LISTENING  >>\n\n";
        return true;
    }

public :
    bool listen() {
        if (mainSocket.Receive(recv,sizeof(recv),r) != sf::Socket::Done) {
            std::cout << "Host has disconnected...";
            return false;
        } else {
            std::string srecv(recv);
            //std::cout<<srecv;
            srecv = sH -> applyUniFix(srecv);
            time_t myTime = time(NULL);
            strftime(timeBuffer, 10, "%H:%M:%S", localtime(&myTime));

            sH -> splitString(srecv,recvArr,"\r\n");
            for (unsigned int i = 0; i < recvArr.size(); i++) {
                sH -> splitString(recvArr[i],recvArr2);

                if (recvArr2.size() > 1) {
                    if (recvArr2[1] == "AUTH")
                        std::cout << " >> " << sH -> mergeLast(recvArr2,2) << "\n";

                    if (recvArr2[0] == "PING")
                        socketSend(&mainSocket,std::string("PONG ").insert(5,recvArr2[1]));

                    if (recvArr2[1] == "MODE") {
                        if (recvArr2[3] == "+i") {
                            socketSend(&mainSocket,std::string("PRIVMSG Q@CServe.quakenet.org :AUTH SVERKERBOT mFKMFxnWvt"));
                            socketSend(&mainSocket,std::string("MODE SVERKER +x"));
                            socketSend(&mainSocket,std::string("JOIN ").insert(5,chan));
                        }
                    }

                    if (recvArr2[1] == "NICK") {
                        if (sH -> getNick(recvArr2[0]) == nick) {
                            nick = sH -> mergeLast(recvArr2,2);
                        }
                    }

                    if (recvArr2[1] == "JOIN") {
                        if (sH -> getNick(recvArr2[0]) == nick) {
                            std::cout << " You're now talking in " << recvArr2[2] << ":\n";
                        } else {
                            std::cout << " [" << timeBuffer << "] " << sH -> getNick(recvArr2[0]) << " joined " << recvArr2[2] << ".\n";
                        }
                    }

                    if (recvArr2[1] == "PART") {
                        if (recvArr2.size() > 3) {
                            std::cout << " [" << timeBuffer << "] " << sH -> getNick(recvArr2[0]) << " left " << recvArr2[2] << ". (" << sH -> parseUni(sH -> mergeLast(recvArr2,3)) << ")\n";
                        } else {
                            std::cout << " [" << timeBuffer << "] " << sH -> getNick(recvArr2[0]) << " left " << recvArr2[2] << ".\n";
                        }
                    }

                    if (recvArr2[1] == "PRIVMSG") {
                        if (recvArr2[2] != nick) {
                            doCmd(timeBuffer,recvArr2[2],sH -> getNick(recvArr2[0]),sH -> generateWhitespace(recvArr2[2],sH -> getNick(recvArr2[0]),sH -> parseUni(sH -> mergeLast(recvArr2,3))));
                            //std::cout << " [" << timeBuffer << "] [" << recvArr2[2] << "] <" << sH -> getNick(recvArr2[0]) << "> " << sH -> generateWhitespace(recvArr2[2],sH -> getNick(recvArr2[0]),sH -> parseUni(sH -> mergeLast(recvArr2,3))) << "\n";

                            for (unsigned int i = 0; i < trigger.size(); i++) {
                                if (checkTriggerString(sH -> mergeLast(recvArr2,3),trigger[i][0],trigger[i][2],trigger[i][3])) {
                                    channelSendMsg(recvArr2[2],sH -> replaceString(trigger[i][1],"$n",sH -> getNick(recvArr2[0])));
                                }
                            }
                        } else {
                            if (sH -> getNick(recvArr2[0]) == "Frasse" || sH -> getNick(recvArr2[0]) == "Burbruee") {
                                if (recvArr2[3] == ":UPDATE") {
                                    channelSendMsg(sH -> getNick(recvArr2[0]),std::string("\x01").append("ACTION is clearing triggers...\x01"));
                                    trigger.clear();
                                    channelSendMsg(sH -> getNick(recvArr2[0]),std::string("\x01").append("ACTION is retrieving new triggers...\x01"));
                                    triggerInit(trigger,getTriggersFrom("sverker.burbruee.se","/api.php"));
                                    channelSendMsg(sH -> getNick(recvArr2[0]),std::string("\x01").append("ACTION is done!\x01"));
                                } else {
                                    socketSend(&mainSocket,sH -> mergeLast(recvArr2,3));
                                }
                            }
                        }
                    }
                }

                recvArr2.clear();
            }

            memset(recv,'\0',1024);
            recvArr.clear();
            srecv = "";
        }

        return true;
    }

private:

    bool socketSend(sf::SocketTCP * sock,std::string buffer) {
        buffer += "\r\n";
        if (sock -> Send(buffer.data(),buffer.length()) != sf::Socket::Done)
            return false;

        buffer = "";
        return true;
    }

    bool sockConnect(sf::SocketTCP * sock,int port,std::string ip) {
        if (sock -> Connect(port,ip) != sf::Socket::Done)
            return false;

        return true;
    }

    void triggerInit(std::vector< std::vector<std::string> > &vec,std::string in) {
        std::vector<std::string> r;
        sH -> splitString(in,r,"|");

        for (unsigned int i = 0; i < r.size(); i++) {
            std::vector<std::string> r2;
            sH -> splitString(r[i],r2,"§");
            vec.push_back(r2);
        }
    }

    std::string getTriggersFrom(std::string host,std::string uri) {
        sf::Http http;
        http.SetHost(host);

        sf::Http::Request req;
        req.SetMethod(sf::Http::Request::Get);
        req.SetURI(uri);

        sf::Http::Response resp = http.SendRequest(req);
        std::string body = resp.GetBody();

        return body;
    }

    bool checkTriggerString(std::string str,std::string str2,std::string cs,std::string ps) {
        if (cs == "no") {
            str = sH -> toLower(str);
            str2 = sH -> toLower(str2);
        }

        if (ps == "yes" && str == str2)
            return true;

        if (ps == "no" && str.find(str2) != std::string::npos)
            return true;

        return false;
    }
};

#endif // IRC_H
