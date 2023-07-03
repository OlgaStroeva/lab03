#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "curl/curl.h"


using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;
const size_t HeightOfColumn = 25, Scale = 10; //scale = one char; border height = scale; space between columns = scale/2

struct Input {
  vector<long double> numbers;
  size_t bin_count;
  size_t scale;
};

vector<long double> input_numbers(istream& in, size_t count) {
  vector<long double> result(count);
  for (size_t i = 0; i < count; i++) {
    in >> result[i];
  }
  return result;
}

Input read_input(istream& in, bool promt) {
  Input data;
  if(promt) cerr << "Enter number count: ";
  size_t number_count;
  in >> number_count;
  if(promt) cerr << "Enter numbers: ";
  data.numbers = input_numbers(in, number_count);
  if(promt) cerr << "Enter column count: ";
  in >> data.bin_count;
  in >> data.scale;
  if(data.scale<2 || data.scale>9) {
      cerr << "ERROR";
    }
return data;
}

void find_minmax(const vector<long double>& numbers, long double &min, long double &max) {
    for (double number : numbers) {
        if (number < min) {
            min = number;
        }
        if (number > max) {
            max = number;
        }
    }
}

vector<size_t> build_histogram(const Input &data){
  long double min = data.numbers[0], max = data.numbers[0];
  find_minmax(data.numbers, min, max);
  vector<size_t> bins(data.bin_count);
  for (double number : data.numbers) {
      size_t bin = (size_t)((number - min) / (max - min) * data.bin_count);
      if (bin == data.bin_count) {
          bin--;
      }
      bins[bin]++;
  }
  return bins;
}

void show_histogram_text(const vector<size_t>& bins){
  size_t max_count = 0;
  for (size_t count : bins) {
      if (count > max_count) {
          max_count = count;
      }
  }
  const bool scaling_needed = max_count > MAX_ASTERISK;

  for (size_t bin : bins) {
      if (bin < 100) {
          cout << ' ';
      }
      if (bin < 10) {
          cout << ' ';
      }
      cout << bin << "|";

      size_t height = bin;
      if (scaling_needed) {
          const double scaling_factor = (double)MAX_ASTERISK / max_count;
          height = (size_t)(bin * scaling_factor);
      }

      for (size_t i = 0; i < height; i++) {
          cout << '*';
      }
      cout << '\n';
  }
}

//for first task
size_t ChooseYourColor(){
  string color;
  cerr << "WARNING! You can choose only 5 colors: green, blue, pink, yellow, cyan.\n You can't choose red! Remember - here is no red color!\n";
  restart:
  getline(cin, color);

  if(color == "green") return 1;
  else if(color == "blue") return 0;
  else if(color == "pink") return 3;
  else if(color == "yellow") return 4;
  else if(color == "cyan") return 5;
  else goto restart;
}

void Scale_under_image(const size_t max_count, size_t bin_count, size_t scale){ //Code for task 15.
  for(size_t i = 0; i <= MAX_ASTERISK && i <= max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 ) ; ++i){
      if(!(i%scale) || i==MAX_ASTERISK) {
          cout << "<line x1='" << Scale*(5+i)  << "' y1='" << (HeightOfColumn+Scale/2)*bin_count + Scale << "' x2='" <<
                  Scale*(5+i) << "' y2='" << (HeightOfColumn+Scale/2)*bin_count + Scale + HeightOfColumn << "' stroke='black' stroke-width='2'/>";
          if(i==0 || i==scale || i==MAX_ASTERISK || (1+i > max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 )))
          cout << "<text text-anchor='end' x='" << Scale*(5+i) << "' y='" << (HeightOfColumn+Scale/2)*bin_count + Scale + HeightOfColumn*1.5 << "'><tspan font-weight='bold' font-size='"
                 << Scale*2 << "'>" << i << "</tspan></text>\n";
        }
    }
  cout << "<line x1='" << Scale*5  << "' y1='" << (HeightOfColumn+Scale/2)*bin_count + HeightOfColumn*0.9 << "' x2='" <<
          Scale*(5 + min(MAX_ASTERISK, (max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 )))) << "' y2='" <<
          (HeightOfColumn+Scale/2)*bin_count + HeightOfColumn << "' stroke='black' stroke-width='2'/>";
}

void draw_histogram_svg(const vector<size_t>& bins, size_t scale){
  size_t max_count = 0, i = 0;
  for (size_t count : bins) {
      if (count > max_count) {
          max_count = count;
      }
  }
  const bool scaling_needed = max_count > MAX_ASTERISK;
  const double scaling_factor = (double)MAX_ASTERISK / max_count;
  const size_t bin_count = bins.size();
  const char* colors[] = {"(0, 0, 255)", "(0, 255, 0)", "(255, 0, 0)", "(255, 0, 255)", "(255, 255, 0)", "(0, 255, 255)"};
  //print start of svg
  cout << "<?xml version='1.0' encoding='UTF-8' ?>\n<svg width='" << (SCREEN_WIDTH+5)*Scale << "' height='" //for example, if we don't heed "full screen" we can use (max_count+2)*Scale
       << bin_count * (HeightOfColumn+Scale/2) + Scale*5 /*1.5*/ << "' viewBox='0 0 " << (SCREEN_WIDTH+5)*Scale << ' '
       << bin_count * (HeightOfColumn+Scale/2) + Scale*5/*1.5*/ << "' xmlns='http://www.w3.org/2000/svg'>\n";

  //print all columns and text
  for(auto bin : bins){
      //Drawing text
      cout << "<text text-anchor='end' x='" << Scale*4 << "' y='" << (HeightOfColumn+Scale/2)*(i+1) << "'><tspan font-weight='bold' font-size='"
           << HeightOfColumn << "'>" << bin << "</tspan></text>\n";

      //Drawing rectangle. Separated function isn't needed cause it's single line where we need all paramet
    cout << "<rect x='" << Scale*5 << "' y='" << (HeightOfColumn+Scale/2)*i + Scale << "' width='" << (scaling_needed ?
            ((size_t)(bin * scaling_factor)) : bin) * Scale <<
            "' height='" << HeightOfColumn << "' style='fill:rgb" <<
            //here is a first task. There is some random colors in files.
            //colors[ChooseYourColor()] <<
            colors[i%6] <<
            "' />\n";
    ++i;
    }
  //Task for 15th variant
  Scale_under_image(max_count, bin_count, scale);



  //here we print the second task
  //cout << "<text text-anchor='middle' x='"<< (SCREEN_WIDTH+5)*Scale/2 <<"' y='" << (bin_count * (HeightOfColumn+Scale/2) + Scale*1.5)/2 + Scale
  //     << "'><tspan font-weight='bold' font-size='" << HeightOfColumn << "'>Гистограмма</tspan></text>\n";



//here we print the end of SVG
  cout << "</svg>";
}

std::stringstream error_stream;

size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx) {
    auto data_size = item_size * item_count;
    error_stream.write(reinterpret_cast<const char*>(items), data_size);
    return data_size;
}

Input download(const std::string& address) {
    CURL* curl = curl_easy_init();
    double connect = 0;
    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cout << "curl_easy_perform() failed"  << "\n";
            exit(1);
        }
        res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect);
        curl_easy_cleanup(curl);
    }

    // Output the stringstream contents to cerr
    std::cerr << error_stream.str();

    return read_input(error_stream, false);
}

int main(int argc, char* argv[]) {
  Input input;
  if (argc > 1) {
    input = download(argv[1]);
  } else {
    input = read_input(cin, true);
  }

  //const auto input = read_input(cin, true);
  const auto bins = build_histogram(input);
  draw_histogram_svg(bins, input.scale);
    return 0;
}
