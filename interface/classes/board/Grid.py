from classes.gui.Gui import Gui
from colors import *
import pygame

class Grid(Gui):
	def __init__(self, line_num, background_color=FOREST_GREEN, border_color=WHITE, border_width=5, line_color=WHITE, line_width=2, hover_color=(250, 250, 250, 50)):
		self.ready_to_draw = False
		super().__init__((5, 5), (90, 90), background_color=background_color, border_color=border_color, border_width=border_width)
		self.__on_click = []
		self.rects = []
		self.hover_color = hover_color		
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

	def render(self, surface):
		super().render(surface)
		if len(self.rects) > 0:
			for rect in self.rects:
				if rect and rect.collidepoint(pygame.mouse.get_pos()):
					pygame.gfxdraw.box(surface, rect, self.hover_color)
	
	def remove_intersection_at(self, index):
		self.rects[index[0] * self.line_num + index[1]] = None

	def handle_event(self, event, screen):
		super().handle_event(event, screen)
		if event.type == pygame.MOUSEBUTTONUP:
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

	def get_rect_at(self, index):
		return self.rects[index[0] * self.line_num + index[1]]

	@property
	def on_click(self):
		return self.__on_click
	@on_click.setter
	def on_click(self, callback):
		self.__on_click.append(callback)