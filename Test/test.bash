echo "should give warnings about percent data, values greater than 1 but no errors:"
python Scripts/ParseSpec.py Test/Test1
echo "========================================="
echo
echo "should give a error about no spec data:"
python Scripts/ParseSpec.py Test/Test_empty
echo "========================================="
echo
echo "should give a error about wrong column separator:"
python Scripts/ParseSpec.py Test/Test_comma 
echo "========================================="
echo
echo "should give a error about incomplete dataset:"
python Scripts/ParseSpec.py Test/Test_incomplete
echo "========================================="
echo
echo "should give a error about headers in dataset:"
python Scripts/ParseSpec.py Test/Test_header
echo "========================================="
echo
echo "should give a error about non-numeric data in the first column:"
python Scripts/ParseSpec.py Test/Test_non_numeric
echo "========================================="
echo
echo "should give a error about mixed subfolders and datafiles:"
python Scripts/ParseSpec.py Test/Test_mixed




