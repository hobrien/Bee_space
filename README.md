# Bee Space
Scripts to plot spec data in bee colour space

* Data can be plotted in a colour hexagon or as a 3D scatter plot
* 3D scatter plot can either interactive plotly plot that can be embedded in websites
  or an animated gif of a rotating plot
  
Requirements:
------------

- [R](http://www.r-project.org)
- [ggplot2](http://ggplot2.org)
- [python](https://www.python.org)
- [matplotlib](http://matplotlib.org)
- [plotly python library](https://plot.ly/python/)

Contents
--------
- Scripts/ParseSpec.py: python script to convert spec reading to BeeSpace and create plots
- Scripts/Hexagon.R: R script to plot results in colour hexagon (used by ParseSpec.py)
- Data/BeeSensitivity.txt: Sensitivity values for each receptor in bee eyes
- Data/Background.txt: Spec values for Daylight and for background reflectance off leaves

Usage
-----
python Scripts/ParseSpec.py -m [hexagon | plotly | rotate] -o outfile data_folders

Runs beespace on all '.CSV' or '.txt' files in each folder than uses the data to draw the 
specified plot, using the folder names as sample names (can also use files with multiple
columns rather than folders with multiple files)

Installing on Windows
---------------------
- Download and install the [Anaconda](http://continuum.io/downloads) package for scientific python
- Install plotly python library:
  - open Anaconda Command Prompt
  - type conda install -c https://conda.binstar.org/plotly plotly
- Install Via GitHub:
  - Download and install the [GitHub](https://windows.github.com/) app
  - launch GitHub shell and type "git clone https://github.com/hobrien/Bee_space"
  - open Command Prompt, then type "python Desktop\GitHub\Bee_space\Test\test_ParseSpec.py"
  

Todo
----
- add unit tests for main functions in ParseSpec.py
- convert hexagon plotting to matplotlib so I can remove ggplot/R dependency
