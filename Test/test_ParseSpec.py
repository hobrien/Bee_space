import sys, os

sys.path.append(os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Scripts'))
from collections import defaultdict
from ParseSpec import GetIntervals, GetColours, ParseSpec, parse_args

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
        self.args = parse_args(['-c', self.SpecFileName])

class TestParseSpec(SpecDataTestCase):

  def test_GetIntervals(self):
      OutputFileName = os.path.join(os.path.split(os.path.realpath(sys.argv[0]))[0], 'GetIntervals_output.txt')
      ExpectedOutput = pd.DataFrame.from_csv(OutputFileName, sep='\t', index_col=False)
      Output = GetIntervals(self.SpecFile)
      self.assertEqual(len(Output.values[np.where(Output != ExpectedOutput)]), 0)

  def test_GetColours(self):
      self.SpecFile[self.SpecFile.columns[1]] = self.SpecFile[self.SpecFile.columns[1]] / 100
      Output = GetColours(GetIntervals(self.SpecFile), self.args)
      self.assertTrue(abs(Output[0] - 0.455134401483) < 0.0000000001) #U
      self.assertTrue(abs(Output[1] - 0.605944243558) < 0.0000000001) #B
      self.assertTrue(abs(Output[2] - 0.357433862968) < 0.0000000001)#G
      self.assertTrue(abs(Output[3] - 0.199660111332) < 0.0000000001)#Y
      self.assertTrue(abs(Output[4] - -0.0846086663539) < 0.0000000001)#X

  def test_ParseSpec(self):
      traces = {'b': defaultdict(list),
                'g': defaultdict(list),
                'uv': defaultdict(list),
                'x': defaultdict(list),
                'y': defaultdict(list)
               }

      Output = ParseSpec(traces, self.SpecFileName, self.args)
      self.assertEqual(Output['b'].keys()[0], 'Reflectance')
      self.assertTrue(abs(Output['b']['Reflectance'][0] - 0.605944243558) < 0.0000000001) #B
      self.assertTrue(abs(Output['g']['Reflectance'][0] - 0.357433862968) < 0.0000000001) #G
      self.assertTrue(abs(Output['uv']['Reflectance'][0] - 0.455134401483) < 0.0000000001) #U
      self.assertTrue(abs(Output['x']['Reflectance'][0] - -0.0846086663539) < 0.0000000001)#X
      self.assertTrue(abs(Output['y']['Reflectance'][0] - 0.199660111332) < 0.0000000001)#Y



suite = unittest.TestLoader().loadTestsFromTestCase(TestParseSpec)
unittest.TextTestRunner(verbosity=2).run(suite)
