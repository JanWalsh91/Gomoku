#include <iostream>
#include "Player.hpp"


Player::Player(int index, Type type): index(index), type(type) {}


// static PyObject* init_players(PyObject* self, PyObject* args) {

// 	players.push_back(Player(0));
// 	players.push_back(Player(1));

// 	std::cout << players.size() << " players instantenisncesd" << std::endl;

// 	return PyLong_FromLong(0);

// }


// static PyObject* switch_type(PyObject* self, PyObject* args) {
// 	return PyLong_FromLong(0);

// }



// static PyMethodDef methods[] = {
// 	{"init_players", init_players, METH_VARARGS, "Returns the init_players value."},
// 	{"switch_type", switch_type, METH_VARARGS, "Returns the switch_type value."},
// 	{NULL, NULL, 0, NULL}
// };

// static struct PyModuleDef definition = {
// 	PyModuleDef_HEAD_INIT,
// 	"Player",
// 	"A Python module containing Classy type and pants() function",
// 	-1,
// 	methods
// };

// PyMODINIT_FUNC PyInit_Player(void) {
// 	Py_Initialize();
// 	PyObject *m = PyModule_Create(&definition);

// 	return m;
// }