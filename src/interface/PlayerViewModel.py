from common.APlayer import APlayer
from interface.colors import *

default_stone_colors = [WHITE, BLACK]

class PlayerViewModel(APlayer):
	"""
	Creates a PlayerViewModel
	Arguments:
		name (string): player's name (color)
		type (Player.TYPE): Human or AI
	"""

	def __init__(self, name, type, stone_color=None):
		self.__on_change_type = []
		self.__on_change_captures = []
		self.__captures = 0
		super().__init__(type)
		self.name = name
		if stone_color:
			self.stone_color = stone_color
		else:
			self.stone_color = default_stone_colors.pop()

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