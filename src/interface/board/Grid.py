from interface.gui.Gui import Gui
from interface.colors import *
import pygame
import sys

class Grid(Gui):
	def __init__(self, line_num, background_color=FOREST_GREEN, border_color=WHITE, border_width=5, line_color=WHITE, line_width=2, hover_color_valid=(VALID_HOVER), hover_color_invalid=(INVALID_HOVER)):
		self.ready_to_draw = False
		super().__init__((5, 5), (90, 90), background_color=background_color, border_color=border_color, border_width=border_width)
		self.surface = pygame.Surface(self.dimensions)
		self.__on_click = []
		self.rects = []
		self.__intersection_validity_array = [[1 for y in range(line_num)] for i in range(line_num)] # 0: no hover, 1: valid, 2: invalid
		self.hover_color_valid = hover_color_valid
		self.hover_color_invalid = hover_color_invalid
		self.line_width = line_width
		self.line_num = line_num
		self.line_color = line_color
		self.ready_to_draw = True
		self.redraw()

	def redraw(self):
		if self.ready_to_draw:
			super().redraw()
			size = self.abs_dimensions[0]
			self.space_between_lines = size / (self.line_num + 1)

			# draw lines
			if self.line_num:
				for i in range(self.line_num):
					i += 1
					dist = i * self.space_between_lines # distance from edge
					pygame.draw.line(self.surface, self.line_color, (dist, 0), (dist, size), self.line_width)
					pygame.draw.line(self.surface, self.line_color, (0, dist), (size, dist), self.line_width)
			
			# create and position intersections
			self.rects = []
			abs_pos = self.abs_position
			hover_size = int(self.space_between_lines * 1.0)
			for y in range(self.line_num):
				y += 1
				for x in range(self.line_num):
					x += 1
					pos = (
						abs_pos[1] + x * self.space_between_lines - hover_size / 2 + self.line_width / 2,
						abs_pos[0] + y * self.space_between_lines - hover_size / 2 + self.line_width / 2
					)
					self.rects.append(pygame.Rect(pos, (hover_size, hover_size)))

	def add_intersection_at(self, index):
		hover_size = int(self.space_between_lines * 1.0)
		abs_pos = self.abs_position
		pos = (
			abs_pos[1] + (index[1] + 1) * self.space_between_lines - hover_size / 2 + self.line_width / 2,
			abs_pos[0] + (index[0] + 1) * self.space_between_lines - hover_size / 2 + self.line_width / 2
		)
		self.rects[index[0] * self.line_num + index[1]] = pygame.Rect(pos, (hover_size, hover_size))

	def render(self, surface):
		super().render(surface)
		if len(self.rects) > 0:
			for rect in self.rects:
				if rect and rect.collidepoint(pygame.mouse.get_pos()):
					color = self.get_color_by_validity(self.get_rect_validity(rect))
					if color:
						pygame.draw.circle(surface, color, rect.center, rect.w // 2, 1)
	
	def remove_intersection_at(self, index):
		self.rects[index[0] * self.line_num + index[1]] = None

	def handle_event(self, event, screen):
		super().handle_event(event, screen)
		if event.type == pygame.MOUSEBUTTONUP and event.button == 1:
			for rect in self.rects:
				if rect and rect.collidepoint(pygame.mouse.get_pos()):
					self.click(self.index_of(rect))
		
	def click(self, index):
		for callback in self.on_click:
			callback(self, index)

	def index_of(self, rect):
		i = self.rects.index(rect)
		return [
			i // self.line_num,
			i % self.line_num
		]

	def get_rect_validity(self, rect):
		row, col = self.index_of(rect)
		return self.intersection_validity_array[row][col]

	def get_color_by_validity(self, validity):
		if validity == 0:
			return None
		elif validity == 1:
			return self.hover_color_valid
		elif validity == 2:
			return self.hover_color_invalid
		else:
			return None

	def get_rect_at(self, index):
		return self.rects[index[0] * self.line_num + index[1]]

	@property
	def on_click(self):
		return self.__on_click
	@on_click.setter
	def on_click(self, callback):
		self.__on_click.append(callback)

	@property
	def intersection_validity_array(self):
		return self.__intersection_validity_array
	@intersection_validity_array.setter
	def intersection_validity_array(self, val):
		if len(val) != self.line_num:
			raise Exception('Invalid intersection_validity_array')
		for row in val:
			if len(row) != self.line_num:
				raise Exception('Invalid intersection_validity_array')
		self.__intersection_validity_array = val