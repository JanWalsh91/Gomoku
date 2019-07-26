
from interface.GameObject import GameObject 
import pygame

class Stone(GameObject):
	def __init__(self, color, position, size):
		self.color = color
		self.position = position
		self.size = int(size)

	def render(self, screen):
		# pygame.draw.circle(screen, self.color, self.position, self.size)
		pygame.gfxdraw.filled_circle(screen, self.position[0], self.position[1], self.size, self.color)
		pygame.gfxdraw.aacircle(screen, self.position[0], self.position[1], self.size, pygame.Color(0, 0, 0))