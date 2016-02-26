#!/usr/bin/env python

import sys, os, warnings
from gooey import Gooey, GooeyParser
from ParseSpec import ParseSpec, PrintText, Hexagon, RotatingPlot, Plotly

""""ParseSpec.py -m (hexagon | plotly | rotate | text) -o outfile data_folders\n
    Wavelengths must be in first column. Spec readings must be in all other columns\n
    By default, sample names are read from file name. To use column headers as sample names, use the -c option
    Custom spec readings for Bee Sensitivity and Background Reflectance can be specified with
    the -s and -b flags respectively\n
    Resolution of rotating image can be changed with the -r option (default is 50). Note that
    file size can get very large if this is increased\n.
"""


def main(args):
    folder = args.data_directory
    traces = []
    sub_folders = 0
    single_folder = 0
    for file in os.listdir(folder):
        if file[0] == '.':
            continue
        file = os.path.join(folder, file)
        print file
        if os.path.isdir(file):
            sub_folders = 1
            traces.append(ParseSpec(file, args))
        elif '.CSV' in file or '.csv' in file or '.TXT' in file or '.txt' in file:
            single_folder = 1
    if single_folder:
        if sub_folders:
            sys.exit("Script does not know how to handle a mix of subfolders and datafiles")
        traces.append(ParseSpec(folder, args))
    if len(traces) == 0:
        sys.exit("No spec data found in the input files")

    if args.mode == 'plotly':
        Plotly(traces)
    elif args.mode == 'rotate':
        RotatingPlot(traces, args.outfile, args.resolution)
    elif args.mode == 'hexagon':
        Hexagon(traces, args.outfile)
    elif args.mode == 'text':
        PrintText(traces, args.outfile)
    else:
        sys.exit("mode %s not recognized. Use '-m plotly', '-m rotate', '-m hexagon' or '-m text'" % mode)

@Gooey(program_name="Bee Space: Plotting spec data in bee colour space in 2 or 3 dimensions") 
def parse_args():
  parser = GooeyParser(description="Calculate bee colour perception from spec data and plot in 2 or 3 dimensions.\nWavelengths must be in first column. Spec readings must be in all other columns")
  parser.add_argument('data_directory',
                        action='store',
                        widget='DirChooser',
                        help="Source directory that contains spec data")
  parser.add_argument('--mode', '-m', choices=['hexagon', 'text', 'plotly', 'rotate'], default = 'hexagon',
                   help='Output mode')
  parser.add_argument('output_directory',
                        action='store',
                        widget='FileChooser',
                        default=os.getcwd(),
                        help="Output directory to save plot")
  parser.add_argument('--outfile', '-o', dest='outfile', default='scatter.png', type=str,
                   help='Name for output file')  
  parser.add_argument('--delimiter', '-d',  choices=['Tab', 'Comma', 'Space'], 
                   default = 'Tab', dest='sep', help='column separator')
  parser.add_argument('--column_headers', '-c', action="store_true",
                   help='Files have header rows')  
  parser.add_argument('--resolution', '-r', dest='resolution', default=50, type=int,
                   help='GIF resolution (DPI) for rotating plots.')  
  parser.add_argument('--background', '-b', dest='BackgroundFileName', 
                   default=os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'Background.txt'), 
                   type=str, help='Location of background reflectance data')  
  parser.add_argument('--sensitivity', '-s', dest='BeeSensitivityFileName', 
                   default=os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'BeeSensitivity.txt'), 
                   type=str, help='Location of bee spectral sensitivity data')  
  #parser.add_argument('infiles', nargs='+')
  args = parser.parse_args()
  if args.sep == 'Tab':
      args.sep = '\t'
  elif args.sep == 'Comma':
      args.sep = ','
  else:    
      args.sep = ' ' 
  return args

           
if __name__ == "__main__":
   args = parse_args()
   main(args)
