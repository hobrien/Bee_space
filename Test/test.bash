echo "should give warnings about percent data, values greater than 1:"
python Scripts/ParseSpec.py Test/Test1
echo
echo "should give a warning about no spec data:"
python Scripts/ParseSpec.py Test/Test_empty
echo
echo "should give a warning about wrong column separator:"
python Scripts/ParseSpec.py Test/Test_comma 
echo
echo "should give a warning about incomplete dataset:"
python Scripts/ParseSpec.py Test/Test_incomplete
echo
echo "should give a warning about incomplete dataset:"
python Scripts/ParseSpec.py Test/Test_incomplete
echo
echo "should give a warning about headers in dataset:"
python Scripts/ParseSpec.py Test/Test_header
echo
echo "should give a warning about non-numeric data in the first column:"
python Scripts/ParseSpec.py Test/Test_non_numeric
echo
echo "should give a warning about non-numeric data in the column2:"
python Scripts/ParseSpec.py Test/Test_non_numeric2




