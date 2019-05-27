import numpy as np
import pygame
from classes.GameObject import GameObject

class Gui(GameObject):
	# position: [top, left]
	# dimensions: [width, height]
	def __init__(self, position=(0, 0), dimensions=(100, 100), background_color=(250, 250, 250, 100)):
		self.position = np.array(position)
		self.dimensions = np.array(dimensions)
		self.surface = pygame.Surface(self.dimensions)
		self.children = []
		self.parent = None
		self.background_color = background_color

	def render(self, screen):
		screen.blit(self.surface, self.abs_position)
		for child in self.children:
			child.render(screen)

	def insert(self, gui):
		if not isinstance(gui, Gui) or gui in self.children:
			return False
		self.children.append(gui)
		gui.parent = self;
		self.scale_children()

	def scale_children(self):
		if len(self.children) == 0:
			return
		rect = self.surface.get_rect()
		for i, child in enumerate(self.children):
			child.surface = pygame.Surface(child.abs_dimensions)
			if (child.background_color):
				child.background_color = child.background_color
			child.scale_children()

	@property
	def background_color(self):
		return self.__background_color
	@background_color.setter
	def background_color(self, background_color):
		self.__background_color = background_color
		if self.surface:
			self.surface.fill(self.__background_color)
			if len(background_color) == 4:
				self.surface.set_alpha(background_color[3])
			else:
				self.surface.set_alpha(100)

	@property
	def parent(self):
		return self.__parent
	@parent.setter
	def parent(self, parent):
		if parent and not isinstance(parent, Gui):
			raise Exception('Parent must be Gui or subclass')
		self.__parent = parent

	@property
	def position(self):
		return self.__position
	@position.setter
	def position(self, position):
		if not isinstance(position, (tuple, np.ndarray)) or len(position) != 2:
			raise Exception('Position must be tuple of length 2')
		self.__position = position

	@property
	def abs_position(self):
		if self.parent:
			node = self
			abs_position = node.parent.abs_position + node.parent.abs_dimensions * self.position / 100
			return abs_position.astype(int)
		else:
			return self.position
	
	@property
	def abs_dimensions(self):
		abs_dimensions = self.dimensions.astype(float)
		if self.parent:
			node = self
			while node.parent:
				abs_dimensions *= (node.parent.abs_dimensions / 100)
				node = node.parent
			return abs_dimensions.astype(int)
		else:
			return abs_dimensions.astype(int)