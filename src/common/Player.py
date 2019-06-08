from enum import Enum, auto

class Player():
	class TYPE(Enum):
		AI = auto()
		HUMAN = auto()

	def __init__(self, type):
		self.type = type

	def is_AI(self):
		return self.type == Player.TYPE.AI
	
	def is_human(self):
		return self.type == Player.TYPE.HUMAN