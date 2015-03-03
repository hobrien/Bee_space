# Bee Space
Scripts to plot spec data in bee colour space

* Data can be plotted in a colour hexagon or as a 3D scatter plot
* 3D scatter plot can either interactive plotly plot that can be embedded in websites
  or an animated gif of a rotating plot
  
Requirements:
------------

-R http://www.r-project.org)
-ggplot2 (http://ggplot2.org)
-python (https://www.python.org)
-matplotlib (http://matplotlib.org)
-plotly python library (https://plot.ly/python/)
-GCC (https://gcc.gnu.org)

Contents
--------
-beespace: C program to convert raw spec readings to bee colour space
-Scripts/ParseSpec.py: python script to run beespace and create plots
-Scripts/Hexagon.R: R script to plot results in colour hexagon (used by ParseSpec.py)

Usage
-----
python Scripts/ParseSpec.py -m [hexagon | plotly | rotate] -o outfile data_folders

Runs beespace on all '.CSV' files in each folder than uses the data to draw the specified
plot, using the folder names as sample names
