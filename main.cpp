#include <iostream>
//#include <vector>
#include "histogram.h"

//using namespace std;

vector<long double> input_numbers(size_t count) {
  vector<long double> result(count);
  for (size_t i = 0; i < count; i++) {
    cin >> result[i];
  }
  return result;
}

//void find_minmax(const vector<long double>& numbers, long double &min, long double &max) {
//    for (double number : numbers) {
//        if (number < min) {
//            min = number;
//        }
//        if (number > max) {
//            max = number;
//        }
//    }
//}

vector<size_t> build_histogram(const vector<long double>& numbers, const size_t bin_count){
  long double min = numbers[0], max = numbers[0];
  find_minmax(numbers, min, max);
  vector<size_t> bins(bin_count);
  for (double number : numbers) {
      size_t bin = (size_t)((number - min) / (max - min) * bin_count);
      if (bin == bin_count) {
          bin--;
      }
      bins[bin]++;
  }
  return bins;
}

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

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

const size_t HeightOfColumn = 25, Scale = 10; //scale = one char; border height = scale; space between columns = scale/2

void draw_histogram_svg(const vector<size_t>& bins){
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
       << bin_count * (HeightOfColumn+Scale/2) + Scale*1.5 << "' viewBox='0 0 " << (SCREEN_WIDTH+5)*Scale << ' '
       << bin_count * (HeightOfColumn+Scale/2) + Scale*1.5 << "' xmlns='http://www.w3.org/2000/svg'>\n";
  //print all columns and text
  for(auto bin : bins){
      //Drawing text
      cout << "<text text-anchor='end' x='" << Scale*4 << "' y='" << (HeightOfColumn+Scale/2)*(i+1) << "'><tspan font-weight='bold' font-size='"
           << HeightOfColumn << "'>" << bin << "</tspan></text>\n";

      //Drawing rectangle. Separated function isn't needed cause it's single line where we need all paramet
    cout << "<rect x='" << Scale*5 << "' y='" << (HeightOfColumn+Scale/2)*i + Scale << "' width='" << (scaling_needed ?
            ((size_t)(bin * scaling_factor)) : bin) * Scale <<
            "' height='" << HeightOfColumn << "' style='fill:rgb" << colors[i%7] << "' />\n";
    ++i;
    }
  cout << "</svg>";
}

int main() {
    // Ввод данных
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;

    cerr << "Enter numbers: ";
    const auto numbers = input_numbers(number_count);

    size_t bin_count;
    cerr << "Enter column count: ";
    cin >> bin_count;

    // Обработка данных
    auto bins = build_histogram(numbers, bin_count);

    // Вывод данных
    //show_histogram_text(bins);
    draw_histogram_svg(bins);

    return 0;
}
