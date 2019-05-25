import pygame
from enum import Enum, auto
from GameObject import GameObject

class Gui(GameObject):
	# direction of child element (like css flex)
	class Direction(Enum):
		VERTICAL = auto()
		HORIZONTAL = auto()
	
	def __init__(self, position=(0, 0), height=100, width=100, rect=None, direction=Gui.Direction.VERTICAL):
		self.rect = rect if rect else pygame.Rect(position, (width, height))
		self.direction = direction