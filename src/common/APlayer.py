from enum import Enum, auto

class APlayer():
	class TYPE(Enum):
		AI = auto()
		HUMAN = auto()

	def __init__(self, type):
		self.type = type
		self.captures = 0

	def is_AI(self):
		return self.type == APlayer.TYPE.AI
	
	def is_human(self):
		return self.type == APlayer.TYPE.HUMAN

	def change_type(self, type=None):
		"""
		Changes or toggles the type of the player
		Arguments:
			type? (Player.TYPE): Human or AI
		"""
		if type == None:
			self.type = APlayer.TYPE.HUMAN if self.is_AI() else APlayer.TYPE.AI
		else:
			self.type = type