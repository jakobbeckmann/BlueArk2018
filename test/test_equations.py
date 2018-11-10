#!/usr/bin/env python3

"""Tests the equations."""

import unittest

from blueark.equations import *


class TestEquations(unittest.TestCase):
    def test_stringify(self):
        lit1 = LiteralNode(5)
        lit2 = LiteralNode(10)
        binaryadd = NaryPlus(lit1, lit2)
        self.assertEqual(str(binaryadd), "5 + 10")

    def test_constant_propagation(self):
        """Evaluate `5 + 10 + 100`"""
        lit1 = LiteralNode(5)
        lit2 = LiteralNode(10)
        lit3 = LiteralNode(100)
        ternaryadd = NaryPlus(lit1, lit2, lit3)
        self.assertEqual(str(ternaryadd.evaluate()), "115")

    def test_symbolic_evaluation(self):
        """Evaluate symbolic constant propagated equation
        `(x + 10) + (y + 15)`
        """
        sym1 = SymbolicNode("x")
        lit1 = LiteralNode(10)
        binaryadd1 = NaryPlus(sym1, lit1)
        sym2 = SymbolicNode("y")
        lit2 = LiteralNode(15)
        binaryadd2 = NaryPlus(sym2, lit2)
        equation = NaryPlus(binaryadd1, binaryadd2)
        self.assertEqual(str(equation), "x + 10 + y + 15")
        self.assertEqual(str(equation.evaluate()), "25 + x + y")

    def test_equality_constraint(self):
        """Test constant propagated equality output for constraint
        `100 = (x + 10) + (y + 15)`
        """
        sym1 = SymbolicNode("x")
        lit1 = LiteralNode(10)
        binaryadd1 = NaryPlus(sym1, lit1)
        sym2 = SymbolicNode("y")
        lit2 = LiteralNode(15)
        binaryadd2 = NaryPlus(sym2, lit2)
        equation = NaryPlus(binaryadd1, binaryadd2)
        constraint = EqualityConstraint(100, equation)
        self.assertEqual(str(constraint), "100 == 25 + x + y")