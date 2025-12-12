#include <bits/stdc++.h>
using namespace std;

//global variables
vector<vector<double>> train_set;
vector<vector<double>> test_set;

double b;
double m;

int size_data = 0;
int size_test = 0;
int size_train = 0;

bool is_stod_safe(const std::string& s) {
    try {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == string::npos)
            return false;

        size_t end = s.find_last_not_of(" \t\n\r");
        string trimmed = s.substr(start, end - start + 1);

        size_t idx = 0;
        stod(trimmed, &idx);

        if (idx != trimmed.size())
            return false;

        return true;
    }
    catch (...) {
        return false;
    }
}

double safe_div(double a, double b){
	if(b >=0 && b < 1e-12) b = 1e-12;
	if(b <= 0 && b > -(1e-12)) b = -(1e-12);

	return a / b;
}

void get_data_from_csv(string name_of_file){
	ifstream file(name_of_file);
	string line;

	while(getline(file, line)) {
		stringstream ss(line);
		string cell;
		vector<double> row;

		while(getline(ss, cell, ',')){
			if(is_stod_safe(cell)){
				row.push_back(stod(cell));
			}
			else {
				continue;
			}
			//row.push_back(stod(cell));
		}

		if(size_data % 5 == 0){
			test_set.push_back(row);
			size_test++;
		}
		else{
			train_set.push_back(row);
			size_train++;
		}

		size_data++;
	}
}

void find_m(){
	double sum_of_xy = 0;
	double sum_of_xsquare = 0;
	double avarage_x = 0;
	double avarage_y = 0;
	
	for(int i=0; i<size_train; i++){
		double xy = train_set[i][0] * train_set[i][1];

		sum_of_xy += xy;
		sum_of_xsquare += train_set[i][0] * train_set[i][0];

		avarage_x += train_set[i][0];
		avarage_y += train_set[i][1];
		
	}

	avarage_x = safe_div(avarage_x, size_train);
	avarage_y = safe_div(avarage_y, size_train);

	m = safe_div( (sum_of_xy - size_train * avarage_x * avarage_y),
		(sum_of_xsquare - size_train * (avarage_x * avarage_x)));
}

void find_b(){
	double avarage_x = 0;
	double avarage_y = 0;

	for(int i=0; i < size_train; i++){
		avarage_x += train_set[i][0];
		avarage_y += train_set[i][1];
	}

	avarage_x = safe_div(avarage_x, size_train);
	avarage_y = safe_div(avarage_y, size_train);

	b = avarage_y - m*avarage_x;
}

void print_formula(){
	cout<<"y = "<<m<<"x + ("<<b<<")"<<endl;
}

void print_error(){
	double mape = 0.0; 
	double avarage_yreal = 0.0; 
	double real_minus_predicted_square = 0.0; 
	double real_minus_avarage_square = 0.0;

	int mape_count = 0; 
	const double EPS = 1e-6;
	
	for(int i = 0; i< size_test; i++){ 	
		double predicted = test_set[i][0] * m + b; 
		double real = test_set[i][1];

		if( fabs(real) > EPS ){
			mape += fabs(real - predicted) / fabs(real);
			mape_count++;
		}
		 
		avarage_yreal += real; 
		real_minus_predicted_square += (real - predicted) * (real - predicted); 
	}
	 
	avarage_yreal = safe_div(avarage_yreal, size_test); 
	 
	for(int i=0; i< size_test; i++){ 
		double real = test_set[i][1]; 
		real_minus_avarage_square += (real - avarage_yreal) * (real - avarage_yreal); 
	}
	 
	double r2 = 1.0 - safe_div(real_minus_predicted_square, real_minus_avarage_square); 
	double mape_percent = 0.0;

	if(mape_count > 0){
		mape = safe_div(mape, mape_count);
		mape_percent = mape * 100.0;
	}

	cout<<"MAPE: " << mape_percent<<endl; 
	cout<<"r2: " << r2 <<endl; 
}

void start_regression(){
	string file_name;

	printf("enter CSV file name: ");
	cin>>file_name;

	get_data_from_csv(file_name);
	find_m();
	find_b();

	print_formula();
	print_error();
}


int main(){

	start_regression();
	
	return 0;
}
