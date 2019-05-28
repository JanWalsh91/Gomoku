import numpy as np
from enum import Enum, auto
from classes.gui.Gui import Gui
import pygame

class Button(Gui):
	class States(Enum):
		NORMAL = auto()
		HOVER = auto()
		DOWN = auto()

	def __init__(self, position=(0, 0), dimensions=(100, 100), text='', background_color=(250, 250, 250), active_background_color=None, color=(0, 0, 0), border_color=(200, 200, 200), border_width=0, on_click=None):
		super().__init__(position, dimensions, background_color=background_color, border_color=border_color, border_width=border_width)
		self.__text = None
		self.__color = None
		self.__active_background_color = None
		self.__state = None
		self.inactive_background_color = self.background_color

		self.text = text		
		self.color = color
		self.active_background_color = tuple(active_background_color if active_background_color else self.calc_active_background_color(background_color)) 
		self.state = Button.States.NORMAL
		self.on_click_callback = on_click
		pass

	def redraw(self):
		super().redraw()
		# draw text

	def calc_active_background_color(self, background_color):
		mod = lambda x: x + (10 if x < 125 else -10)
		return np.array([mod(x) for x in np.array(background_color)])

	def handle_event(self, event, screen):
		pos = pygame.mouse.get_pos()
		relpos = np.array(pos) - self.abs_position
		rect = self.surface.get_rect().collidepoint(relpos)
		if rect:
			if event.type == pygame.MOUSEBUTTONUP:
				self.on_click()

	def on_click(self):
		if self.on_click_callback:
			self.on_click_callback()