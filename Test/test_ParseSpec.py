import sys, os

sys.path.append(os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Scripts'))

from ParseSpec import GetIntervals, GetColours, ParseSpec

import unittest

import pandas as pd
import numpy as np


class SpecDataTestCase(unittest.TestCase):
    def setUp(self):
        self.SpecFileName = os.path.join(os.path.split(os.path.realpath(sys.argv[0]))[0], 'Spec_test.txt')
        self.SpecFile = pd.DataFrame.from_csv(self.SpecFileName, sep='\t', index_col=False)
        self.BeeSensitivityFileName = os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'BeeSensitivity.txt')
        self.BeeSensitivity = pd.DataFrame.from_csv(self.BeeSensitivityFileName, sep='\t', index_col=False)
        self.BackgroundFileName = os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'Background.txt')
        self.Background = pd.DataFrame.from_csv(self.BackgroundFileName, sep='\t', index_col=False)


class TestParseSpec(SpecDataTestCase):

  def test_GetIntervals(self):
      OutputFileName = os.path.join(os.path.split(os.path.realpath(sys.argv[0]))[0], 'GetIntervals_output.txt')
      ExpectedOutput = pd.DataFrame.from_csv(OutputFileName, sep='\t', index_col=False)
      Output = GetIntervals(self.SpecFile)
      self.assertEqual(len(Output.values[np.where(Output != ExpectedOutput)]), 0)

  def test_GetColours(self):
      Output = GetColours(self.BeeSensitivity, self.Background, GetIntervals(self.SpecFile))
      self.assertEqual(Output[0], 0.98817009028705383)
      self.assertEqual(Output[1], 0.99353884937423742)
      self.assertEqual(Output[2], 0.98234026886614656)
      self.assertEqual(Output[3], 0.008283669797637172)
      self.assertEqual(Output[4], -0.0050486253505056923)

  def test_ParseSpec_both(self):
      Output = ParseSpec(self.BeeSensitivity, self.Background, self.SpecFileName, 'both', 0)
      self.assertEqual(Output[0], ['Spec_test'])
      self.assertEqual(Output[1], [0.99353884937423742])
      self.assertEqual(Output[2], [0.98234026886614656])
      self.assertEqual(Output[3], [0.98817009028705383])
      self.assertEqual(Output[4], [0.008283669797637172])
      self.assertEqual(Output[5], [-0.0050486253505056923])

  def test_ParseSpec_2D(self):
      Output = ParseSpec(self.BeeSensitivity, self.Background, self.SpecFileName, '2D', 0)
      self.assertEqual(Output['name'], 'Spec_test')
      self.assertEqual(Output['mode'], 'markers')
      self.assertEqual(Output['y'], [-0.0050486253505056923])
      self.assertEqual(Output['x'], [0.008283669797637172])

  def test_ParseSpec_3D(self):
      Output = ParseSpec(self.BeeSensitivity, self.Background, self.SpecFileName, '3D', 0)
      self.assertEqual(Output['name'], 'Spec_test')
      self.assertEqual(Output['mode'], 'markers')
      self.assertEqual(Output['y'], [0.98234026886614656])
      self.assertEqual(Output['x'], [0.99353884937423742])
      self.assertEqual(Output['z'], [0.98817009028705383])

  def test_ParseSpec_column_header(self):
      Output = ParseSpec(self.BeeSensitivity, self.Background, self.SpecFileName, '3D', 1)
      self.assertEqual(Output['name'], 'Reflectance')


suite = unittest.TestLoader().loadTestsFromTestCase(TestParseSpec)
unittest.TextTestRunner(verbosity=2).run(suite)
