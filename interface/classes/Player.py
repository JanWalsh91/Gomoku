from enum import Enum, auto

# Add callbacks with player.on_change_type = lambda ...

class Player:
	"""
	Creates a Player
	Arguments:
		type (Player.TYPE): Human or AI
	"""
	class TYPE(Enum):
		AI = auto()
		HUMAN = auto()

	def __init__(self, type):
		self.__on_change_type = []
		self.type = type

	def is_AI(self):
		return self.type == Player.TYPE.AI
	
	def is_human(self):
		return self.type == Player.TYPE.HUMAN

	def change_type(self, type=None):
		"""
		Changes or toggles the type of the player
		Arguments:
			type? (Player.TYPE): Human or AI
		"""
		if type == None:
			self.type = Player.TYPE.HUMAN if self.is_AI() else Player.TYPE.AI
		else:
			self.type = type
		print('changed type to ', self.type)

	@property
	def type(self):
		return self.__type
	@type.setter
	def type(self, type):
		self.__type = type
		if len(self.on_change_type) > 0:
			for x in self.on_change_type:
				x()

	@property
	def on_change_type(self):
		"""
		Add a callback triggered when type changes
		"""
		return self.__on_change_type
	@on_change_type.setter
	def on_change_type(self, fn):
		self.__on_change_type.append(fn)
