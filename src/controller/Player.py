from common.APlayer import APlayer

class Player(APlayer):
	"""
	Creates a Player.
	Arguments:
		type (Player.TYPE): Human or AI
	"""

	count = 1

	def __init__(self, type):
		super().__init__(type)
		self.captures = 0
		self.index = Player.count
		Player.count += 1
	
