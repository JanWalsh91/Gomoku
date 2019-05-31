from classes.gui.Gui import Gui
from classes.board.Background import Background
from classes.board.Grid import Grid
from classes.board.Stone import Stone
from colors import *

class Board(Gui):

	def __init__(self, size, line_num=19, background_color=FOREST_GREEN, border_color=GREY, border_width=5, line_color=WHITE, line_width=1, on_grid_click=None):
		super().__init__((0, 0), (size, size)) # is only wrapper for visual elements
		self.game_objects = []
		self.__on_grid_click = []
		self.background = Background(background_color=background_color, border_color=border_color, border_width=border_width)
		self.insert(self.background)
		self.grid = Grid(line_num, background_color=background_color, border_color=line_color, border_width=line_width, line_color=line_color, line_width=line_width)
		self.insert(self.grid)

		if on_grid_click:
			self.on_grid_click = on_grid_click

	@property
	def on_grid_click(self):
		return self.grid.on_grid_click
	@on_grid_click.setter
	def on_grid_click(self, callback):
		self.grid.on_click = lambda grid, index: callback(self, index)

	def place_stone_at(self, index, color):
		# get rect
		rect = self.grid.get_rect_at(index)
		stone = Stone(color, (int(rect.x + rect.w / 2), int(rect.y + rect.h / 2)), rect.w * 0.55)
		flat_index = index[0] * self.grid.line_num + index[1]
		self.game_objects.insert(flat_index, stone)
		# remove intersection
		self.grid.remove_intersection_at(index)

	def render(self, screen):
		super().render(screen)
		for game_object in self.game_objects:
			if game_object:
				game_object.render(screen)

	def reset(self):
		self.game_objects = []