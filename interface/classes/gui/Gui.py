import numpy as np
import pygame
from enum import Enum, auto
from classes.GameObject import GameObject
import sys

class Gui(GameObject):
	# direction of child element (like css flex)
	class Direction(Enum):
		VERTICAL = auto()
		HORIZONTAL = auto()
	
	def __init__(self, position=(0, 0), height=100, width=100, rect=None, background_color=(250, 250, 250, 100), direction=Direction.VERTICAL):
		self.direction = direction
		self.position = np.array((rect.x, rect.y) if rect else position)
		self.width, self.height = (rect.w, rect.h) if rect else (width, height)
		print(self.__class__.__name__, 'about to set surface')
		self.surface = pygame.Surface((self.width, self.height))
		self.children = []
		self.parent = None
		print(self.__class__.__name__, 'about to set bg color')
		self.background_color = background_color
		print('set to ', self.__background_color)
		print('set to ', self.background_color)
		# sys.exit()


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
		self.position_children()

	# set position of children as position relative to parent based on number of children and parent direction
	def position_children(self):
		if len(self.children) == 0:
			return
		rect = self.surface.get_rect()
		length, middle = (rect.h, rect.w / 2) if self.is_vertical else (rect.w, rect.h / 2)
		for i, child in enumerate(self.children):
			if i == 0:
				dist = length / (len(self.children) * 2)
			else: 
				dist = length / len(self.children) * (i + 1)
			if i == len(self.children) - 1:
				dist -= length / (len(self.children) * 2)

			child_position = np.array([dist, middle] if self.is_horizontal else [middle, dist])
			child.position = child_position - np.array([child.width * rect.w, child.height * rect.h]) / 200
			if (child.parent):
				child.position_children()

	def scale_children(self):
		print('scale_children')
		if len(self.children) == 0:
			return
		rect = self.surface.get_rect()
		for i, child in enumerate(self.children):
			print(self.__class__.__name__, 'about to set surface of child')
			child.surface = pygame.Surface(child.abs_dimensions)
			if (child.background_color):
				print(self.__class__.__name__, 'about to set bg color of child')
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
			abs_position = node.position.copy()
			while node.parent:
				abs_position += node.parent.abs_position
				node = node.parent
			return abs_position.astype(int)
		else:
			return self.position
	
	@property
	def abs_dimensions(self):
		abs_dimensions = np.array([self.width, self.height]).astype(float)
		if self.parent:
			node = self
			while node.parent:
				abs_dimensions *= (node.parent.abs_dimensions / 100)
				node = node.parent
			return abs_dimensions.astype(int)
		else:
			return abs_dimensions.astype(int)

	@property
	def is_vertical(self):
		return self.direction == Gui.Direction.VERTICAL
	@property
	def is_horizontal(self):
		return self.direction == Gui.Direction.HORIZONTAL