import numpy as np
from interface.gui.Gui import Gui
import pygame
from interface.gui.TextBox import TextBox

class Button(Gui):
	def __init__(self, position=(0, 0), dimensions=(100, 100), text='ClickMe', background_color=(250, 250, 250), color=(0, 0, 0), border_color=(200, 200, 200), border_width=0, on_click=None, font_size=30):
		self.textbox = None
		super().__init__(position, dimensions, background_color=background_color, border_color=border_color, border_width=border_width)
		self.__on_click = []
		self.__color = None

		self.color = color
		if on_click:
			self.on_click = on_click
		
		self.textbox = TextBox(text=text, font_size=font_size, color=self.color, background_color=self.background_color, border_color=border_color, border_width=border_width) 
		self.insert(self.textbox)
		pass

	def handle_event(self, event, screen):
		super().handle_event(event, screen)
		pos = pygame.mouse.get_pos()
		relpos = np.array(pos) - self.abs_position
		rect = self.surface.get_rect().collidepoint(relpos)
		if rect:
			if event.type == pygame.MOUSEBUTTONUP:
				self.click()

	def click(self):
		for callback in self.on_click:
			callback(self)

	@property
	def on_click(self):
		return self.__on_click
	@on_click.setter
	def on_click(self, on_click):
		self.__on_click.append(on_click)
 
	@property
	def background_color(self):
		return self.__background_color
	@background_color.setter
	def background_color(self, background_color):
		# print('[Button] update background color')
		self.__background_color = background_color
		if self.textbox:
			self.textbox.background_color = background_color
		self.redraw()