#!/usr/local/bin/python

import sys, csv, os, warnings, getopt, platform, argparse
import plotly.plotly as py
from plotly.graph_objs import *
import pandas as pd
import numpy as np
from matplotlib import collections  as mc
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from pylab import plt, savefig

from subprocess import call

""""ParseSpec.py -m (hexagon | plotly | rotate | text) -o outfile data_folders\n
    Wavelengths must be in first column. Spec readings must be in all other columns\n
    By default, sample names are read from file name. To use column headers as sample names, use the -c option
    Custom spec readings for Bee Sensitivity and Background Reflectance can be specified with
    the -s and -b flags respectively\n
    Resolution of rotating image can be changed with the -r option (default is 50). Note that
    file size can get very large if this is increased\n.
"""


def main(args):
    traces = []
    for folder in args.infiles:
        sub_folders = 0
        single_folder = 0
        if os.path.isfile(folder):
            traces.append(ParseSpec(folder, args))
            continue
        for file in os.listdir(folder):
            if file[0] == '.':
                continue
            file = os.path.join(folder, file)
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
      
def ParseSpec(folder, args):
    b = []
    g = []
    uv = []
    x = []
    y = []
    sample = []
    files = []
    sample_id = os.path.basename(folder.strip("/"))
    sample_id = os.path.splitext(sample_id)[0]
    if os.path.isdir(folder):
        for file in os.listdir(folder):
            #file = os.path.join(folder, file)
            if '.CSV' in file or '.csv' in file or '.TXT' in file or '.txt' in file:
                files.append(os.path.join(folder, file))
    else:
        files.append(folder) 
    for file in files:
        if '.CSV' in file or '.csv' in file or '.TXT' in file or '.txt' in file:
            SpecData = pd.DataFrame.from_csv(file, sep=args.sep, index_col=False)
            SpecData.rename(columns={SpecData.columns[0]:'Wavelength'}, inplace=True)
            SpecData = SanitiseData(SpecData)
            ReducedSpec = GetIntervals(SpecData)
            assert len(ReducedSpec.index) == 81, "Spec dataset only has %i rows. Are all values from 300-700 included?" % len(ReducedSpec.index)
            for i in range(len(ReducedSpec.columns) -1):
                if args.column_headers:
                    sample_id = ReducedSpec.columns[1+i]
                Colours = GetColours(ReducedSpec.drop(ReducedSpec.columns[1:1+i] | ReducedSpec.columns[2+i:], 1), args)
                uv.append(Colours[0])
                b.append(Colours[1])
                g.append(Colours[2])
                y.append(Colours[3])
                x.append(Colours[4])
                sample.append(sample_id)
    if args.mode == 'hexagon':
        print sample_id
        return Scatter(x=x, y=y, mode='markers', name=sample_id)
    elif args.mode == 'text':
        return (sample, b, g, uv, x, y)
    else:
        return Scatter3d(x=b, y=g, z=uv, mode='markers', name=sample_id)


def SanitiseData(SpecData):
    #print "Sanitising data"
    SpecData = SpecData[np.isfinite(SpecData['Wavelength'])] # Remove blank lines
    # Convert percent data to proportions
    for column in SpecData.columns[1:]:
        if len(SpecData[SpecData[column] > 1]) > 0:
            warnings.warn("Percent data supplied. Converting to proportions")
            SpecData[column] = SpecData[column] / 100        
    # Convert negative values to 0 and values > 1 to 1 (might be better to drop?)
    for column in SpecData.columns[1:]:
        if len(SpecData[SpecData[column] < 0]) > 0:
            warnings.warn("Negative reflectance values being changed to zero")         
        SpecData[column][SpecData[column] < 0] = 0
        if len(SpecData[SpecData[column] > 1]) > 1:
            warnings.warn("Reflectance values greater than one being changed to one")         
        SpecData[column][SpecData[column] > 1] = 1        
    return SpecData

def GetIntervals(SpecData, Interval=5):
    #print "getting intervals"
    #select wavelengths closest to multiples of interval and round
    SpecData= SpecData.sort_values(by='Wavelength')
    LastSaved = SpecData['Wavelength'].tolist()[-1]+Interval
    for x in reversed(range(1, len(SpecData['Wavelength']))):
        current_dist =  min(SpecData['Wavelength'][x] % Interval, Interval- SpecData['Wavelength'][x] % Interval)
        next_dist =  min(SpecData['Wavelength'][x-1] % Interval, Interval- SpecData['Wavelength'][x-1] % Interval)
        if next_dist < current_dist or LastSaved - SpecData['Wavelength'][x] < Interval/2.0:
            SpecData=SpecData.drop(SpecData.index[x])
        else:
            LastSaved = SpecData['Wavelength'][x]
            SpecData.loc[x:x,'Wavelength'] = int(Interval * round(float(SpecData['Wavelength'][x])/Interval))
    return SpecData[(SpecData['Wavelength'] >= 300) & (SpecData['Wavelength'] <= 700)].reset_index(drop=True)

def GetColours(SpecData, args):
    #print "gettting colours"
    #This will do most of the work of converting a spec reading into values to plot on a 3d plot or hexagon
    BeeSensitivity = GetIntervals(pd.DataFrame.from_csv(args.BeeSensitivityFileName, sep='\t', index_col=False))
    assert len(BeeSensitivity.index) == 81, "Bee Sensitivity dataset only has %i rows. Are all values from 300-700 included?" % len(BeeSensitivity.index)
    Background = GetIntervals(pd.DataFrame.from_csv(args.BackgroundFileName, sep='\t', index_col=False))
    assert len(Background.index) == 81, "Background dataset only has %i rows. Are all values from 300-700 included?" % len(Background.index)
    SpecData.rename(columns={SpecData.columns[1]:'Reflectance'}, inplace=True)
    Combined = pd.merge(pd.merge(BeeSensitivity, Background, on='Wavelength'), SpecData, on='Wavelength')
    U_B_G_Y_X = []
    for colour in ('UV', 'Blue', 'Green'):
        R = 1/sum(Combined[colour]*Combined['Daylight']*Combined['Leaves'])
        raw = sum(Combined[colour]*Combined['Daylight']*Combined['Reflectance'])
        U_B_G_Y_X.append(raw*R/(raw*R+1))
    U_B_G_Y_X.append(U_B_G_Y_X[1]-0.5*(U_B_G_Y_X[0]+U_B_G_Y_X[2]))
    U_B_G_Y_X.append(0.866*(U_B_G_Y_X[2]-U_B_G_Y_X[0]))
    return U_B_G_Y_X
    
def PrintText(traces, outfile):
   if not outfile:
       outfile = 'scatter.txt'
   file_handle = open(outfile, 'w')
   file_handle.write("Sample\tBlue\tGreen\tUV\tX\tY\n")
   for trace in traces:
            for i in range(len(trace[0])):
                row = []
                for column in trace:
                    row.append(str(column[i]))
                file_handle.write('\t'.join(row) + '\n')
   file_handle.close()


def Hexagon(traces, outfile):
    print "making hexagon"
    if not outfile:
        outfile = 'scatter.png'
    colours = ['b', 'y', 'r', 'c', 'm', 'g']
    fig, ax = plt.subplots()
    legend_points = []
    legend_titles = []
    for trace in traces:
        colour = colours.pop(0)
        ax.scatter(x=list(trace['x']), y=list(trace['y']), c=colour, marker='o')
        legend_points.append(plt.Line2D([0],[0], linestyle="none", c=colour, marker = 'o'))
        legend_titles.append(trace['name'])
    #ax.set_xlabel('UV')
    #ax.set_ylabel('Green')
    ax.legend(legend_points, 
              legend_titles, 
              numpoints = 1, 
              ncol=3,
              columnspacing=1,
              loc=9,
              frameon=0,
              borderpad=0,
              handlelength=0
            )
    lines = [[(0, 1), (-.866, .5)], 
         [(0, -1), (-.866, -.5)],
         [(0, 1), (.866, .5)],
         [(0, -1), (.866, -.5)],
         [(.866, .5), (.866, -.5)],
         [(-.866, .5), (-.866, -.5)]
        ]
    lc = mc.LineCollection(lines, colors='black', linewidths=2)
    ax.add_collection(lc)
    ax.margins(0.1)
    print "printing output"
    savefig(outfile)

    
def RotatingPlot(traces, outfile, resolution):
    
    if not outfile:
        outfile = 'scatter.gif'
        
    colours = ['b', 'y', 'r', 'c', 'm', 'g']
    if len(traces) > len(colours):
        sys.exit("not enough colours available for plotting")
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    #ax.scatter(IrRed[2], IrRed[1], IrRed[0], c='b', marker='o')
    legend_points = []
    legend_titles = []
    for trace in traces:
        colour = colours.pop(0)
        ax.scatter(list(trace['z']), list(trace['y']), list(trace['x']), c=colour, marker='o')
        legend_points.append(plt.Line2D([0],[0], linestyle="none", c=colour, marker = 'o'))
        legend_titles.append(trace['name'])
    ax.set_xlabel('Blue')
    ax.set_ylabel('Green')
    ax.set_zlabel('UV')
    ax.legend(legend_points, 
              legend_titles, 
              numpoints = 1, 
              ncol=3,
              columnspacing=1,
              loc=9,
              frameon=0,
              borderpad=0,
              handlelength=0
            )

    for i in range(1, 361):
        ax.view_init(elev=45, azim=i)
        if i < 45:
            i += 360
        savefig("scatter_%03d.png" % i, dpi=resolution)
    if platform.system() == 'Windows':
        convert = os.path.join(os.path.split(os.path.realpath(sys.argv[0]))[0], 'IMconvert.cmd')
        call([convert, "-delay", "10", "-loop", "0", "scatter_*.png", outfile])
    else:
        call(["convert", "-delay", "10", "-loop", "0", "scatter_*.png", outfile])
    for i in range(45, 405):
        os.remove("scatter_%03d.png" % i)  

                
def Plotly(traces):
    #py.sign_in('heath.obrien', 'bxr8tju4kv')
    #data = Data(traces)
    layout = Layout(
        showlegend=True,
        autosize=True,
        width=933,
        height=868,
        xaxis=XAxis(type='linear'),
        yaxis=YAxis(type='linear'),
        dragmode='rotate',
        barmode='group',
        scene=Scene(
            xaxis=XAxis(
                title='Blue',
                range=[0, 1],
                type='linear',
                autorange=False
            ),
            yaxis=YAxis(
                title='Green',
                range=[0, 1],
                type='linear',
                autorange=False
            ),
            zaxis=ZAxis(
                title='UV',
                range=[0, 1],
                type='linear',
                autorange=False
            ),
            cameraposition=[[0.6702282428741455, 0.49358895421028137, -0.5526835918426514, 0.041290637105703354], [0, 0, 0], 2.8476730120752833]
        )
    )
    fig = Figure(data=traces)
    plot_url = py.plot(fig)

 
def parse_args(input):
  parser = argparse.ArgumentParser(description="Calculate bee colour perceprion from spec data and plot in 2 or 3 dimensions.\nWavelengths must be in first column. Spec readings must be in all other columns")
  parser.add_argument('--outfile', '-o', dest='outfile', default='',
                   help='Output file name')
  parser.add_argument('--mode', '-m', dest='mode', default='hexagon', type=str,
                   help='Output mode (hexagon, plotly, rotating gif, text)')
  parser.add_argument('--background', '-b', dest='BackgroundFileName', 
                   default=os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'Background.txt'), 
                   type=str, help='Location of background reflectance data')  
  parser.add_argument('--sensitivity', '-s', dest='BeeSensitivityFileName', 
                   default=os.path.join(os.path.split(os.path.split(os.path.realpath(sys.argv[0]))[0])[0],'Data', 'BeeSensitivity.txt'), 
                   type=str, help='Location of bee spectral sensitivity data')  
  parser.add_argument('--resolution', '-r', dest='resolution', default=50, type=int,
                   help='GIF resolution (DPI) for rotating plots. Note that file size can get very large if this is increased from 50')  
  parser.add_argument('--delimiter', '-d', dest='sep', default='\t', type=str,
                   help='column separator')
  parser.add_argument('--column_headers', '-c', action="store_true",
                   help='Files have header rows')  
  parser.add_argument('--version', '-v', action='version', version='%(prog)s 1.0')
  parser.add_argument('infiles', nargs='+')
  args = parser.parse_args(input)
  return args

           
if __name__ == "__main__":
   args = parse_args(sys.argv[1:])
   main(args)
