import pygame
from interface.gui.Gui import Gui

class TextBox(Gui):
	def __init__(self, position=(0, 0), dimensions=(100, 100), text='test', background_color=(250, 250, 250), active_background_color=None, color=(0, 0, 0), border_color=(200, 200, 200), border_width=0, font_size=30):
		self.__text = None
		self.__color = None
		self.__font_size = None

		super().__init__(position, dimensions, background_color=background_color, border_color=border_color, border_width=border_width)
		self.text = text
		self.color = color
		self.font_size = font_size

	def redraw(self):
		if self.surface and self.text and self.color and self.font_size:
			super().redraw()
			font = pygame.font.Font('freesansbold.ttf', self.font_size)
			text = font.render(self.text, True, self.color, self.background_color) 
			text_rect = text.get_rect()
			surface_rect = self.surface.get_rect()
			self.surface.blit(text, (surface_rect.centerx - text_rect.centerx, surface_rect.centery - text_rect.centery))
	
	def render(self, surface):
		super().render(surface)
		pass
	
	@property
	def text(self):
		return self.__text
	@text.setter
	def text(self, text):
		self.__text = text
		self.redraw()

	@property
	def color(self):
		return self.__color
	@color.setter
	def color(self, color):
		self.__color = color
		self.redraw()

	@property
	def font_size(self):
		return self.__font_size
	@font_size.setter
	def font_size(self, font_size):
		self.__font_size = font_size
		self.redraw()