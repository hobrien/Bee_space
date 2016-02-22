# Bee Space
Scripts to plot spec data in bee colour space

* Data can be plotted in a colour hexagon or as a 3D scatter plot
* 3D scatter plot can either interactive plotly plot that can be embedded in websites
  or an animated gif of a rotating plot
  
Requirements:
------------

- [python](https://www.python.org)
- [Anaconda](http://continuum.io/downloads) Scientific python package
- [plotly python library](https://plot.ly/python/)
- [ImageMagick](http://www.imagemagick.org/script/index.php)

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

Testing
-------
```python Scripts/ParseSpec.py -m hexagon -o test.png Examples/Test1/*```

- Plot should show same pattern as Test/scatter.png

Installing on Windows
---------------------
- Download and install the [Anaconda](http://continuum.io/downloads) package for scientific python
- Install plotly python library:
  - open Anaconda Command Prompt
  - type conda install -c https://conda.binstar.org/plotly plotly
  - follow [these instructions](https://plot.ly/python/getting-started/) to set up plotly API credentials
  
- Install Via GitHub:
  - Download and install the [GitHub](https://windows.github.com/) app
  - launch GitHub shell and type "git clone https://github.com/hobrien/Bee_space"
  - open Command Prompt, then type "python Desktop\GitHub\Bee_space\Test\test_ParseSpec.py"
  
- Install [ImageMagick](www.imagemagick.org) 
  - This requires admin access so you'll need to submit an IT request on UoB computers)
  - Obnoxiously, the program needed has the same name as a system command, so an extra steps are necessary
  - Launch the command prompt and type:
  ```
    where convert > Desktop\GitHub\Bee_space\Scripts\IMconvert.cmd
  ```  
  - Open Notepad, then open the above file
  - Delete the line that doesn't mention ImageMagick
  - Put double quotes around the remaining line
  - Place an @ symbol at the fron of the line (no space)
  - place %* at the end of the line (after a space):
  ```
    @"C:Program Files\ImageMagick-6.9.1-Q16\convert.exe" %*
  ```
  - Save file
  
   
    
