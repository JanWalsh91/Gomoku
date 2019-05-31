from classes.gui.Gui import Gui
from classes.board.Background import Background
from classes.board.Grid import Grid
from colors import *

class Board(Gui):

	def __init__(self, size, line_num=19, background_color=FOREST_GREEN, border_color=GREY, border_width=5, line_color=WHITE, line_width=1, on_intersection_click=None):
		super().__init__((0, 0), (size, size)) # is only wrapper for visual elements
		self.__on_intersection_click = []
		self.background = Background(background_color=background_color, border_color=border_color, border_width=border_width)
		self.insert(self.background)
		self.grid = Grid(line_num, background_color=background_color, border_color=line_color, border_width=line_width, line_color=line_color, line_width=line_width)
		self.insert(self.grid)

		if on_intersection_click:
			self.on_intersection_click = on_intersection_click

	@property
	def on_intersection_click(self):
		return self.grid.on_intersection_click
	@on_intersection_click.setter
	def on_intersection_click(self, fn):
		def on_intersection_click_callback(grid, index):
			fn(self, index)
		self.grid.on_click = on_intersection_click_callback

	def place_stone_at(self, index):
		print('place_stone_at', index)