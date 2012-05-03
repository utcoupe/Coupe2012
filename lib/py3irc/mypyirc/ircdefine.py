

#####
## <PREFIX_CMD><IRC_CMD><SEP><ARG1><SEP><ARG2>...<SEP_OPTIONS>options
#####
SEP					= " "
SEP_OPTIONS			= "#"
PREFIX_CMD			= "."


CANAL_ERRORS		= "#errors"
CANAL_BIG_ASSERV	= "#asserv"
CANAL_MINI_ASSERV	= "#asservmini"
CANAL_BIG_OTHERS	= "#others"
CANAL_MINI_OTHERS	= "#othersmini"
CANAL_HOKUYO		= "#hokuyo"
CANAL_DEBUG			= "#debug"
CANAL_BIG_EXTRAS	= "#extras"
CANAL_MINI_EXTRAS	= "#extrasmini"

CANAUX				= (
	CANAL_ERRORS,
	CANAL_BIG_ASSERV, CANAL_MINI_ASSERV,
	CANAL_HOKUYO,
	CANAL_BIG_OTHERS, CANAL_MINI_OTHERS,
	CANAL_DEBUG,
	CANAL_BIG_EXTRAS, CANAL_MINI_EXTRAS,
)


ID_MSG_POS		= 101
ID_MSG_HOKUYO	= 102
ID_MSG_JACK		= 102


BLUE	= 0
RED		= 1

"""
1 - Black
2 - Navy Blue
3 - Green
4 - Red
5 - Brown
6 - Purple
7 - Olive
8 - Yellow
9 - Lime Green
10 - Teal
11 - Aqua Light
12 - Royal Blue
13 - Hot Pink
14 - Dark Gray
15 - Light Gray
16 - White
"""
IRCCOLORS = {}
IRCCOLORS['white']			= '0'
IRCCOLORS['black']			= '1'
IRCCOLORS['blue']			= '2'
IRCCOLORS['green']			= '3'
IRCCOLORS['pink']			= '4'
IRCCOLORS['red']			= '5'
IRCCOLORS['purple']			= '6'
IRCCOLORS['brown']			= '7'
IRCCOLORS['yellow']			= '8'
IRCCOLORS['lime_green']		= '9'
IRCCOLORS['teal']			= '10'
IRCCOLORS['aqua_light']		= '11'
IRCCOLORS['royal_blue']		= '12'
IRCCOLORS['hot_pink']		= '13'
IRCCOLORS['dark_gray']		= '14'
IRCCOLORS['light_gray']		= '15'
IRCCOLORS[None]				= ''
COLOR_MARKER				= chr(3)
BOLD_MARKER					= chr(2)


