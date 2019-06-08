from interface.gui.Gui import Gui
import pygame
from interface.colors import *

class Background(Gui):
	def __init__(self, background_color=FOREST_GREEN, border_color=RED, border_width=5):
		super().__init__((0, 0), (100, 100), background_color=background_color, border_color=border_color, border_width=border_width)
		
	def render(self, screen):
		screen.blit(self.surface, self.position)