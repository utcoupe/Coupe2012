

import unittest



from ia.gamestate import *
from ia.robot import *


class GameStateTestCase(unittest.TestCase):
	CANAL_BIG_ASSERV = "#asserv"
	CANAL_MINI_ASSERV = "#aservmini"
	
	def setUp(self):
		big = Robot((0,0), None)
		mini = Robot((0,0), None)
		enemy1 = Robot((3000,0), None)
		enemy2 = Robot((3000,0), None)
		self.gamestate = GameState(None, CANAL_BIG_ASSERV, CANAL_MINI_ASSERV, CANAL_BIG_VISIO, CANAL_MINI_VISIO, big, mini, enemy1, enemy2)

	def test_on_msg_pos(self):
		self.gamestate.on_msg_pos(0, CANAL_BIG_ASSERV, ("400","300","40"), {})
		self.gamestate.on_msg_pos(0, CANAL_MINI_ASSERV, ("200","100","-50"), {})
		self.assertEqual(self.gamestate.bigrobot.pos, (400,300))
		self.assertEqual(self.gamestate.bigrobot.a, 40)
		self.assertEqual(self.gamestate.minirobot.pos, (200,100))
		self.assertEqual(self.gamestate.minirobot.a, -50)

	def test_on_msg_hokuyo(self):
		self.gamestate.on_msg_hokyo(0, "canaltest", ['((450,350), (150,150), (2000,1000), (2500,500))'], {})
		# les positions de nos robots de doivent pas êtres changées
		self.assertEqual(self.gamestate.bigrobot.pos, (0,0))
		self.assertEqual(self.gamestate.minirobot.pos, (0,0))
		# vérification des positions enemies
		self.assertIn(self.gamestate.enemy1.pos, ((2000,1000), (2500,500)))
		self.assertIn(self.gamestate.enemy2.pos, ((2000,1000), (2500,500)))
