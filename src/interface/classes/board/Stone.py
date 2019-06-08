import pygame
from interface.classes.GameObject import GameObject 

class Stone(GameObject):
	def __init__(self, color, position, size):
		self.color = color
		self.position = position
		self.size = int(size)

	def render(self, screen):
		pygame.draw.circle(screen, self.color, self.position, self.size)