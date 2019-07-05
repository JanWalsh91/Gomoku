from interface.colors import *
from enum import IntEnum, auto

default_stone_colors = [WHITE, BLACK]

class PlayerViewModel():
	"""
	Creates a PlayerViewModel
	Arguments:
		name (string): player's name (color)
		type (Player.TYPE): Human or AI
	"""
	class TYPE(IntEnum):
		AI = 0
		HUMAN = 1

	count = 0

	def __init__(self, name, type, stone_color=None):
		self.__on_change_type = []
		self.__on_change_captures = []
		self.__captures = 0
		self.name = name
		if stone_color:
			self.stone_color = stone_color
		else:
			self.stone_color = default_stone_colors.pop()
		self.type = type
		self.index = PlayerViewModel.count
		PlayerViewModel.count += 1


	def is_AI(self):
		return self.type == PlayerViewModel.TYPE.AI
	
	def is_human(self):
		return self.type == PlayerViewModel.TYPE.HUMAN

	def change_type(self, type=None):
		"""
		Changes or toggles the type of the player
		Arguments:
			type? (Player.TYPE): Human or AI
		"""
		if type == None:
			self.type = PlayerViewModel.TYPE.HUMAN if self.is_AI() else PlayerViewModel.TYPE.AI
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

	@property
	def captures(self):
		return self.__captures
	@captures.setter
	def captures(self, value):
		self.__captures = value
		for callback in self.__on_change_captures:
			callback(self)

	@property
	def on_update_captures(self):
		return self.__on_change_captures
	@on_update_captures.setter
	def on_update_captures(self, fn):
		self.__on_change_captures.append(fn)