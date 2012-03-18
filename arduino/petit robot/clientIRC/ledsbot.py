#!/usr/bin/python3
# -*- coding:utf-8 -*-


import sys
import os
FILE_DIR  = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(FILE_DIR,"..","..","clientIRC"))

import arduinobot


import pygame as pg
from pygame.locals import *
from pygame.color import *

pg.init()
screen = pg.display.set_mode((600, 400))
clock = pg.time.Clock()



if __name__ == "__main__":
	bot = arduinobot.run(
		nickname="ledsbot",
		channel="#leds",
		serial_port="/dev/ttyACM0",
		protocol_file=os.path.join(FILE_DIR,"..","driver","protocol.h"),
		protocol_prefixe="Q_",
		blocking=False
	)

	allume = getattr(bot, "cmd_leds_allume")
	eteindre = getattr(bot, "cmd_leds_eteindre")
	
	while 1:
		screen.fill(THECOLORS["white"])
		
		for event in pg.event.get():
			if event.type == QUIT:
				break
			elif event.type == KEYDOWN:
				if event.key == K_ESCAPE:
					break
				elif event.key == K_a:
					bot.write(allume('2', id_msg=42))
				elif event.key == K_z:
					bot.write(allume('3', id_msg=42))
				elif event.key == K_e:
					bot.write(allume('4', id_msg=42))
				elif event.key == K_r:
					bot.write(allume('5', id_msg=42))
				elif event.key == K_t:
					bot.write(allume('6', id_msg=42))
				elif event.key == K_y:
					bot.write(allume('7', id_msg=42))
			elif event.type == KEYUP:
				if event.key == K_a:
					bot.write(eteindre('2', id_msg=42))
				elif event.key == K_z:
					bot.write(eteindre('3', id_msg=42))
				elif event.key == K_e:
					bot.write(eteindre('4', id_msg=42))
				elif event.key == K_r:
					bot.write(eteindre('5', id_msg=42))
				elif event.key == K_t:
					bot.write(eteindre('6', id_msg=42))
				elif event.key == K_y:
					bot.write(eteindre('7', id_msg=42))
		
		pg.display.flip()
		clock.tick(60)
		pg.display.set_caption( "fps: " + str(clock.get_fps()))
