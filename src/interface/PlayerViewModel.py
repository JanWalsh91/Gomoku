from common.Player import Player
from interface.colors import *

default_stone_colors = [WHITE, BLACK]

class PlayerViewModel(Player):
	"""
	Creates a PlayerViewModel
	Arguments:
		name (string): player's name (color)
		type (Player.TYPE): Human or AI
	"""

	def __init__(self, name, type, stone_color=None):
		self.__on_change_type = []
		super().__init__(type)
		self.name = name
		if stone_color:
			self.stone_color = stone_color
		else:
			self.stone_color = default_stone_colors.pop()

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

	@property
	def type(self):
		return self.__type
	@type.setter
	def type(self, type):
		self.__type = type
		for callback in self.on_change_type:
			callback(self)

	@property
	def on_change_type(self):
		"""
		Add a callback triggered when type changes
		"""
		return self.__on_change_type
	@on_change_type.setter
	def on_change_type(self, fn):
		self.__on_change_type.append(fn)
