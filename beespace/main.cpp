//
//  main.cpp
//  beespace
//
//  Created by Heath O'Brien on 15/07/2014.
//  Copyright (c) 2014 ___HEATHOBRIEN___. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

int beewavelength[82];
long double uv[82];
long double blue[82];
long double green[82];
long double dayl[82];
long double leaves[82];
long double processed_reflectance[82];
long double averaged_reflectance[82];
long double interpolated_reflectance[82];
double wavelength[3000], measurement[3000];
int final_counter;
long double avx, avy, intx, inty, avb, avg, avuv, intb, intg, intuv;

void read_hexagon_parameters(void);
void calculate_hexagon_values(void);
void calculate_interpolated_reflectances(void);
void calculate_averaged_reflectances(void);

long double xcord, ycord, bcord, gcord, uvcord;

int main (int argc, char* argv[]) {
	char spectrum_type[2];
	char filename[24];
	char outname[20] = "outputfile.csv";
    //  char is_there_another[2];
	int file_type;
	int line_counter;
	char str[40];
	string junk;
	bool file_reading_finished;
	bool stop_here;
    bool another_flag;
    
    do {
        another_flag = false;
        
        //cout << "please enter file name >> ";
        //cin >> filename;
        //	actual_file = strcat(suffixname, filename);
        
        //	file_type = 0;
        file_type=1;
        while (file_type == 0) {
            cout << "please enter datafile type\n\t\tbristol type, with two lines of raw data = 'b'\n\t\t";
            cout << "QMUL type, with lots of extra junk at the beginning and four columns of raw data = 'q'\n\n\t\t>> ";
            cin >> spectrum_type;
            if (*spectrum_type == 'b') {
                cout << "reading a bristol format file\n";
                file_type = 1;
            }
            else if (*spectrum_type == 'q') {
                cout << "reading a QMUL format file\n";
                file_type = 2;
            }
            else cout << "file format (" << spectrum_type << ") not used ...  try again!  >> ";
        }
        
        cout << "reading spectrophotometer dataset\n";
		
        //	ifstream spectrum ("../../testset.txt");
        ifstream spectrum (argv[1]);
        if( !spectrum.is_open() ) {
            cerr << "Error opening input stream" << endl;
            exit(1);
        }
        
        ofstream outfile (outname, ios::app);
        if( !outfile.is_open() ) {
            cerr << "Error opening output stream" << endl;
            exit(1);
        }
        
        
        if (file_type == 1) {				//reading a bristol dataset...
            line_counter = 0;
            file_reading_finished = false;
            
            do {
                line_counter++;
                if (! spectrum.eof()) {
                    spectrum >> str;
                    wavelength[line_counter] = atof(str);
                    spectrum >> str;
                    measurement[line_counter] = atof(str);
                }
                else file_reading_finished = true;
            } while (file_reading_finished == false);
            final_counter = line_counter - 2;
            //		cout << "read " << line_counter << " lines of data:\n";
            //		cout << "  1: " << wavelength[1] << " " << measurement[1] << endl;
            //		cout << "  2: " << wavelength[2] << " " << measurement[2] << endl;
            //		cout << "  3: " << wavelength[3] << " " << measurement[3] << endl;
            //		cout << "     ...         ...\n";
            //		cout << "  " << final_counter - 2 << " : " << wavelength[final_counter - 2] << " " << measurement[final_counter - 2] << endl;
            //		cout << "  " << final_counter - 1 << " : " << wavelength[final_counter - 1] << " " << measurement[final_counter - 1] << endl;
            //		cout << "  " << final_counter << " : " << wavelength[final_counter] << " " << measurement[final_counter] << endl;
        }
        
        if (file_type == 2) {			//reading QMUL dataset...
            line_counter = 0;
            file_reading_finished = false;
            stop_here = false;
            
            do {
                spectrum >> str;
                //			cout << str << endl;
                if (strncmp(str, "transmittance]", 14) == 0) {
                    //				cout << "ooo!";
                    stop_here = true;
                }
            } while (stop_here == false);
            
            
            do {
                line_counter++;
                if (! spectrum.eof()) {
                    spectrum >> str;
                    //				cout << "!! " << str << endl;
                    wavelength[line_counter] = atof(str);
                    spectrum >> str;
                    spectrum >> str;
                    spectrum >> str;
                    spectrum >> str;
                    measurement[line_counter] = atof(str);
                }
                else file_reading_finished = true;
            } while (file_reading_finished == false);
            final_counter = line_counter - 2;
            //		cout << "read " << line_counter << " lines of data:\n";
            //		cout << "  1: " << wavelength[1] << " " << measurement[1] << endl;
            //		cout << "  2: " << wavelength[2] << " " << measurement[2] << endl;
            //		cout << "  3: " << wavelength[3] << " " << measurement[3] << endl;
            //		cout << "     ...         ...\n";
            //		cout << "  " << final_counter - 2 << " : " << wavelength[final_counter - 2] << " " << measurement[final_counter - 2] << endl;
            //		cout << "  " << final_counter - 1 << " : " << wavelength[final_counter - 1] << " " << measurement[final_counter - 1] << endl;
            //		cout << "  " << final_counter << " : " << wavelength[final_counter] << " " << measurement[final_counter] << endl;
        }
        
        read_hexagon_parameters();
        
        calculate_interpolated_reflectances();
        calculate_averaged_reflectances();
        //	int temp_counting_thing;
        //	for (temp_counting_thing = 1; temp_counting_thing <= 81; temp_counting_thing++) {
        //		cout << beewavelength[temp_counting_thing] << "nm: av = " << averaged_reflectance[temp_counting_thing] << "\t in = " << interpolated_reflectance[temp_counting_thing] << endl;
        //	}
        
        calculate_interpolated_reflectances();
        //	cout << "interpolated ";
        calculate_hexagon_values();
        intx = xcord;
        inty = ycord;
        intb = bcord;
        intg = gcord;
        intuv = uvcord;
        
        calculate_averaged_reflectances();
        //	cout << "averaged ";
        calculate_hexagon_values();
        avx = xcord;
        avy = ycord;
        avb = bcord;
        avg = gcord;
        avuv = uvcord;
        
        
        outfile << argv[1] << ",avx," << avx << ",avy," << avy << ",avb," << avb << ",avg," << avg << ",avuv," << avuv << ",intx," << intx << ",inty," << inty << ",intb," << intb << ",intg," << intg << ",inuv," << intuv << endl;
		
        spectrum.close();
        outfile.close();
        
        /*  cout << "another? (y/n) ";
         cin >> is_there_another;
         if ((*is_there_another == 'y') || (*is_there_another == 'Y')) another_flag = true;
         */another_flag = false;
    } while (another_flag == true);
    
	return 0;
}

void calculate_interpolated_reflectances(void) {
	int counter;
	int jump_value;
	int jump_counter;
	bool end_flag;
	int temp;
	
	end_flag = false;
	jump_value = 300;
	jump_counter = 1;
	counter = 1;
	do {
		if ((wavelength[counter] < jump_value) && ((wavelength[counter + 1]) >= jump_value)) {
            //cout << "   ****  " << counter << " " << wavelength[counter] << " " << jump_counter << endl;
			processed_reflectance[jump_counter] = (measurement[counter] + ((measurement[counter + 1] - measurement[counter]) * ((long double)jump_value - wavelength[counter]) / (wavelength[counter + 1] - wavelength[counter]))) / 100.0;
			jump_counter++;
			jump_value = jump_value + 5;
			if (jump_value > 701) end_flag = true;
		}
		counter++;
	} while ((counter < final_counter - 1) && (end_flag != true));
	for (temp = 1; temp <= 81; temp++) {
		interpolated_reflectance[temp] = processed_reflectance[temp];
	}
}


void calculate_averaged_reflectances(void) {
	int counter;
	long double jump_value;
	int jump_counter;
	bool end_flag;
	long double temp_sum;
	int temp_counter;
	int temp;
	
	end_flag = false;
	jump_value = 297.5;
	jump_counter = 1;
	counter = 1;
	temp_sum = 0.0;
	temp_counter = 0;
	do {
		if (wavelength[counter] > jump_value + (long double)5.0) {
			jump_counter++;
			jump_value = jump_value + 5;
			temp_sum = 0.0;
			temp_counter = 0;
		}
		if ((wavelength[counter] > jump_value) && ((wavelength[counter + 1]) <= jump_value + (long double)5.0)) {
			temp_counter++;
			temp_sum = temp_sum + measurement[counter];
			processed_reflectance[jump_counter] = (long double)0.01 * temp_sum / (long double)temp_counter;
			if (jump_value > 702.5) end_flag = true;
		}
		counter++;
	} while ((counter < final_counter - 1) && (end_flag != true));
	
	for (temp = 1; temp <= 81; temp++) {
		averaged_reflectance[temp] = processed_reflectance[temp];
	}
}


void calculate_hexagon_values(void) {
	long double M90, N90, O90, M89, N89, O89;
	int temp;
	long double summed_uv, summed_blue, summed_green, background_uv, background_blue, background_green, Ruv, Rblue, Rgreen;
    
	summed_uv = 0.0;
	summed_blue = 0.0;
	summed_green = 0.0;
	background_uv = 0.0;
	background_blue = 0.0;
	background_green = 0.0;
	for (temp = 1; temp <= 81; temp++) {
		summed_uv += processed_reflectance[temp] * dayl[temp] * uv[temp];
		summed_blue += processed_reflectance[temp] * dayl[temp] * blue[temp];
		summed_green += processed_reflectance[temp] * dayl[temp] * green[temp];
		background_uv += leaves[temp] * dayl[temp] * uv[temp];
		background_blue += leaves[temp] * dayl[temp] * blue[temp];
		background_green += leaves[temp] * dayl[temp] * green[temp];
	}
	Ruv = 1.0 / background_uv;
	Rblue = 1.0 / background_blue;
	Rgreen = 1.0 / background_green;
	M89 = summed_uv * Ruv;
	M90 = M89 / (M89 + (long double)1.0);
	N89 = summed_blue * Rblue;
	N90 = N89 / (N89 + (long double)1.0);
	O89 = summed_green * Rgreen;
	O90 = O89 / (O89 + (long double)1.0);
	xcord = (long double)0.866 * (O90 - M90);
	ycord = N90 - (long double)0.5 * (M90 + O90);
    bcord = N90;
    gcord = O90;
    uvcord = M90;
    
    //	cout << endl << "\t\tcolour hexagon values:\n\t\t\t\t" << "x = " << xcord << ", y = " << ycord << endl;
}


void read_hexagon_parameters(void) {
    beewavelength[1]=300; uv[1]=0.099121173; blue[1]=0.016177153; green[1]=0.120762065; dayl[1]=0.001; leaves[1]=0.059;
    beewavelength[2]=305; uv[2]=0.158226663; blue[2]=0.023073357; green[2]=0.124462094; dayl[2]=0.009; leaves[2]=0.046;
    beewavelength[3]=310; uv[3]=0.239595184; blue[3]=0.031621931; green[3]=0.127135631; dayl[3]=0.026; leaves[3]=0.041;
    beewavelength[4]=315; uv[4]=0.344103377; blue[4]=0.041674932; green[4]=0.128792946; dayl[4]=0.065; leaves[4]=0.046;
    beewavelength[5]=320; uv[5]=0.468689723; blue[5]=0.052860912; green[5]=0.129523139; dayl[5]=0.113; leaves[5]=0.046;
    beewavelength[6]=325; uv[6]=0.605454325; blue[6]=0.064591926; green[6]=0.129497341; dayl[6]=0.164; leaves[6]=0.048;
    beewavelength[7]=330; uv[7]=0.741867705; blue[7]=0.076120275; green[7]=0.128956673; dayl[7]=0.206; leaves[7]=0.054;
    beewavelength[8]=335; uv[8]=0.862386486; blue[8]=0.086645411; green[8]=0.12818557; dayl[8]=0.226; leaves[8]=0.045;
    beewavelength[9]=340; uv[9]=0.951298709; blue[9]=0.095459949; green[9]=0.127476879; dayl[9]=0.237; leaves[9]=0.046;
    beewavelength[10]=345; uv[10]=0.996104482; blue[10]=0.102114284; green[10]=0.127098125; dayl[10]=0.256; leaves[10]=0.045;
    beewavelength[11]=350; uv[11]=0.990423315; blue[11]=0.106574722; green[11]=0.127267368; dayl[11]=0.273; leaves[11]=0.05;
    beewavelength[12]=355; uv[12]=0.935488522; blue[12]=0.109351428; green[12]=0.128143004; dayl[12]=0.284; leaves[12]=0.048;
    beewavelength[13]=360; uv[13]=0.83974552; blue[13]=0.111578297; green[13]=0.129827127; dayl[13]=0.293; leaves[13]=0.045;
    beewavelength[14]=365; uv[14]=0.7167252; blue[14]=0.115033728; green[14]=0.132378577; dayl[14]=0.315; leaves[14]=0.054;
    beewavelength[15]=370; uv[15]=0.58192864; blue[15]=0.122095253; green[15]=0.135830508; dayl[15]=0.332; leaves[15]=0.049;
    beewavelength[16]=375; uv[16]=0.449702891; blue[16]=0.135619789; green[16]=0.1402078; dayl[16]=0.335; leaves[16]=0.053;
    beewavelength[17]=380; uv[17]=0.330945333; blue[17]=0.158736014; green[17]=0.145541247; dayl[17]=0.332; leaves[17]=0.051;
    beewavelength[18]=385; uv[18]=0.232062389; blue[18]=0.194531167; green[18]=0.151877163; dayl[18]=0.352; leaves[18]=0.051;
    beewavelength[19]=390; uv[19]=0.155138917; blue[19]=0.245619221; green[19]=0.159282466; dayl[19]=0.389; leaves[19]=0.053;
    beewavelength[20]=395; uv[20]=0.098937065; blue[20]=0.313598568; green[20]=0.16784611; dayl[20]=0.476; leaves[20]=0.055;
    beewavelength[21]=400; uv[21]=0.060225238; blue[21]=0.39844793; green[21]=0.177678011; dayl[21]=0.579; leaves[21]=0.055;
    beewavelength[22]=405; uv[22]=0.035013984; blue[22]=0.497962982; green[22]=0.188906562; dayl[22]=0.617; leaves[22]=0.057;
    beewavelength[23]=410; uv[23]=0.019454174; blue[23]=0.607386165; green[23]=0.201675533; dayl[23]=0.651; leaves[23]=0.063;
    beewavelength[24]=415; uv[24]=0.010336152; blue[24]=0.719403157; green[24]=0.216140811; dayl[24]=0.671; leaves[24]=0.064;
    beewavelength[25]=420; uv[25]=0.005254695; blue[25]=0.824646887; green[25]=0.232467132; dayl[25]=0.686; leaves[25]=0.067;
    beewavelength[26]=425; uv[26]=0.002557685; blue[26]=0.912752223; green[26]=0.250824677; dayl[26]=0.669; leaves[26]=0.071;
    beewavelength[27]=430; uv[27]=0.001192685; blue[27]=0.973854823; green[27]=0.271385243; dayl[27]=0.671; leaves[27]=0.069;
    beewavelength[28]=435; uv[28]=0.000533153; blue[28]=1; green[28]=0.29431757; dayl[28]=0.729; leaves[28]=0.076;
    beewavelength[29]=440; uv[29]=0.000228608; blue[29]=0.987954029; green[29]=0.319781367; dayl[29]=0.807; leaves[29]=0.081;
    beewavelength[30]=445; uv[30]=9.40835E-05; blue[30]=0.937418579; green[30]=0.347919556; dayl[30]=0.864; leaves[30]=0.083;
    beewavelength[31]=450; uv[31]=3.7186E-05; blue[31]=0.853734994; green[31]=0.378848337; dayl[31]=0.912; leaves[31]=0.086;
    beewavelength[32]=455; uv[32]=1.41239E-05; blue[32]=0.745675839; green[32]=0.412644724; dayl[32]=0.934; leaves[32]=0.088;
    beewavelength[33]=460; uv[33]=5.15821E-06; blue[33]=0.624138847; green[33]=0.449331407; dayl[33]=0.947; leaves[33]=0.088;
    beewavelength[34]=465; uv[34]=1.81247E-06; blue[34]=0.500266642; green[34]=0.488858975; dayl[34]=0.946; leaves[34]=0.086;
    beewavelength[35]=470; uv[35]=6.13097E-07; blue[35]=0.383718123; green[35]=0.531085858; dayl[35]=0.947; leaves[35]=0.084;
    beewavelength[36]=475; uv[36]=1.99768E-07; blue[36]=0.281468406; green[36]=0.575756721; dayl[36]=0.958; leaves[36]=0.083;
    beewavelength[37]=480; uv[37]=6.27352E-08; blue[37]=0.197325737; green[37]=0.622480493; dayl[37]=0.973; leaves[37]=0.088;
    beewavelength[38]=485; uv[38]=1.89992E-08; blue[38]=0.132135071; green[38]=0.670709739; dayl[38]=0.952; leaves[38]=0.087;
    beewavelength[39]=490; uv[39]=5.55193E-09; blue[39]=0.084467415; green[39]=0.719723612; dayl[39]=0.939; leaves[39]=0.09;
    beewavelength[40]=495; uv[40]=1.56632E-09; blue[40]=0.051518826; green[40]=0.768617115; dayl[40]=0.944; leaves[40]=0.09;
    beewavelength[41]=500; uv[41]=4.26856E-10; blue[41]=0.029966043; green[41]=0.816299751; dayl[41]=0.956; leaves[41]=0.098;
    beewavelength[42]=505; uv[42]=1.12431E-10; blue[42]=0.016613927; green[42]=0.861506751; dayl[42]=0.959; leaves[42]=0.102;
    beewavelength[43]=510; uv[43]=2.8637E-11; blue[43]=0.008776044; green[43]=0.9028258; dayl[43]=0.96; leaves[43]=0.109;
    beewavelength[44]=515; uv[44]=7.05724E-12; blue[44]=0.004414934; green[44]=0.938741457; dayl[44]=0.957; leaves[44]=0.119;
    beewavelength[45]=520; uv[45]=1.68359E-12; blue[45]=0.002114338; green[45]=0.967698146; dayl[45]=0.953; leaves[45]=0.132;
    beewavelength[46]=525; uv[46]=3.89003E-13; blue[46]=0.000963576; green[46]=0.988180766; dayl[46]=0.975; leaves[46]=0.147;
    beewavelength[47]=530; uv[47]=8.70978E-14; blue[47]=0.000417739; green[47]=0.998809619; dayl[47]=0.992; leaves[47]=0.159;
    beewavelength[48]=535; uv[48]=1.89067E-14; blue[48]=0.000172222; green[48]=0.998443792; dayl[48]=0.992; leaves[48]=0.165;
    beewavelength[49]=540; uv[49]=3.98101E-15; blue[49]=6.74996E-05; green[49]=0.986284626; dayl[49]=0.986; leaves[49]=0.166;
    beewavelength[50]=545; uv[50]=8.13484E-16; blue[50]=2.51431E-05; green[50]=0.961968926; dayl[50]=0.993; leaves[50]=0.17;
    beewavelength[51]=550; uv[51]=1.61395E-16; blue[51]=8.89868E-06; green[51]=0.925640618; dayl[51]=0.996; leaves[51]=0.175;
    beewavelength[52]=555; uv[52]=3.11044E-17; blue[52]=2.99166E-06; green[52]=0.877989981; dayl[52]=1; leaves[52]=0.177;
    beewavelength[53]=560; uv[53]=5.82566E-18; blue[53]=9.55172E-07; green[53]=0.820251773; dayl[53]=0.979; leaves[53]=0.172;
    beewavelength[54]=565; uv[54]=1.06086E-18; blue[54]=2.89562E-07; green[54]=0.754157371; dayl[54]=0.969; leaves[54]=0.163;
    beewavelength[55]=570; uv[55]=1.87911E-19; blue[55]=8.3331E-08; green[55]=0.681841301; dayl[55]=0.963; leaves[55]=0.155;
    beewavelength[56]=575; uv[56]=3.23908E-20; blue[56]=2.27616E-08; green[56]=0.605708407; dayl[56]=0.966; leaves[56]=0.148;
    beewavelength[57]=580; uv[57]=5.43568E-21; blue[57]=5.90013E-09; green[57]=0.528273573; dayl[57]=0.971; leaves[57]=0.144;
    beewavelength[58]=585; uv[58]=8.88452E-22; blue[58]=1.45117E-09; green[58]=0.451990315; dayl[58]=0.943; leaves[58]=0.14;
    beewavelength[59]=590; uv[59]=1.41496E-22; blue[59]=3.38627E-10; green[59]=0.379086763; dayl[59]=0.923; leaves[59]=0.134;
    beewavelength[60]=595; uv[60]=2.19667E-23; blue[60]=7.49584E-11; green[60]=0.311427008; dayl[60]=0.929; leaves[60]=0.13;
    beewavelength[61]=600; uv[61]=3.32559E-24; blue[61]=1.57388E-11; green[61]=0.250412441; dayl[61]=0.944; leaves[61]=0.128;
    beewavelength[62]=605; uv[62]=4.91172E-25; blue[62]=3.13431E-12; green[62]=0.196932076; dayl[62]=0.95; leaves[62]=0.125;
    beewavelength[63]=610; uv[63]=7.07989E-26; blue[63]=5.91968E-13; green[63]=0.151363878; dayl[63]=0.954; leaves[63]=0.124;
    beewavelength[64]=615; uv[64]=9.96359E-27; blue[64]=1.06028E-13; green[64]=0.11362217; dayl[64]=0.953; leaves[64]=0.12;
    beewavelength[65]=620; uv[65]=1.36951E-27; blue[65]=1.80089E-14; green[65]=0.083240456; dayl[65]=0.951; leaves[65]=0.116;
    beewavelength[66]=625; uv[66]=1.83925E-28; blue[66]=2.90066E-15; green[66]=0.059475437; dayl[66]=0.934; leaves[66]=0.114;
    beewavelength[67]=630; uv[67]=2.41433E-29; blue[67]=4.43047E-16; green[67]=0.041417115; dayl[67]=0.923; leaves[67]=0.116;
    beewavelength[68]=635; uv[68]=3.09879E-30; blue[68]=6.41761E-17; green[68]=0.028091453; dayl[68]=0.927; leaves[68]=0.114;
    beewavelength[69]=640; uv[69]=3.89028E-31; blue[69]=8.81725E-18; green[69]=0.018545559; dayl[69]=0.937; leaves[69]=0.112;
    beewavelength[70]=645; uv[70]=4.77873E-32; blue[70]=1.14944E-18; green[70]=0.01190974; dayl[70]=0.923; leaves[70]=0.108;
    beewavelength[71]=650; uv[71]=5.74563E-33; blue[71]=1.42294E-19; green[71]=0.007435201; dayl[71]=0.913; leaves[71]=0.101;
    beewavelength[72]=655; uv[72]=6.76397E-34; blue[72]=1.67604E-20; green[72]=0.004509711; dayl[72]=0.917; leaves[72]=0.098;
    beewavelength[73]=660; uv[73]=7.79917E-35; blue[73]=1.88728E-21; green[73]=0.002655914; dayl[73]=0.925; leaves[73]=0.099;
    beewavelength[74]=665; uv[74]=8.81087E-36; blue[74]=2.05534E-22; green[74]=0.001517883; dayl[74]=0.945; leaves[74]=0.097;
    beewavelength[75]=670; uv[75]=9.75556E-37; blue[75]=2.22569E-23; green[75]=0.000841352; dayl[75]=0.957; leaves[75]=0.095;
    beewavelength[76]=675; uv[76]=1.05897E-37; blue[76]=2.54147E-24; green[76]=0.000452058; dayl[76]=0.947; leaves[76]=0.095;
    beewavelength[77]=680; uv[77]=1.12733E-38; blue[77]=3.35177E-25; green[77]=0.000235318; dayl[77]=0.931; leaves[77]=0.099;
    beewavelength[78]=685; uv[78]=1.17729E-39; blue[78]=5.45241E-26; green[78]=0.000118614; dayl[78]=0.886; leaves[78]=0.103;
    beewavelength[79]=690; uv[79]=1.20645E-40; blue[79]=1.06252E-26; green[79]=5.7865E-05; dayl[79]=0.853; leaves[79]=0.106;
    beewavelength[80]=695; uv[80]=1.21356E-41; blue[80]=2.28309E-27; green[80]=2.73074E-05; dayl[80]=0.859; leaves[80]=0.128;
    beewavelength[81]=700; uv[81]=1.19856E-42; blue[81]=5.08077E-28; green[81]=1.24601E-05; dayl[81]=0.862; leaves[81]=0.143;
}