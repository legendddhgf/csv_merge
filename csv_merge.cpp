#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

int main (int argc, char **argv) {
  if (argc != 5) {
    fprintf(
        stderr, "Usage: %s input_csv match_to_csv merge_csv assignment_name\n",
        argv[0]);
    exit(0);
  }

  FILE *input = fopen(argv[2], "r");
  if (!input) {
    perror(argv[1]);
    exit(1);
  }
  string asg_name = string(argv[4]);

  vector<string> out_lines;

  char line[8192]; // it can't be larger right???
  while (fgets(line, 8191, input)) {
    out_lines.push_back(string(line));
  }

  fclose(input);

  int asg_col = -1;

  string first_line = out_lines.at(0);
  for (int i = 0; i < first_line.size();) { // this sets asg_col
                                                // appropriately
    if (first_line.find(asg_name, i) == string::npos) {
      break;
    }
    i++;
    i = first_line.find(',', i);
    asg_col++;
  }

  if (asg_col < 0) {
    fprintf(stderr, "Error: assignment %s not yet created\n", asg_name.c_str());
    exit(1);
  }

  printf("asg_col is %d\n", asg_col);

  input = fopen(argv[1], "r");

  fgets(line, 8191, input); // first line is header
  while(fgets(line, 8191, input)) {
    string current_student = string(strtok(line, ","));
    string current_grade = string(strtok(NULL, ",\n"));
    for (int i = 2; i < out_lines.size(); i++) { // first two lines are headers
      string current_line = out_lines.at(i);
      if (current_line.find(current_student) != -1) {
        int j = 0;
        for (int k = 0; k < asg_col; k++) {
          j++;
          j = current_line.find_first_of(',', j);
        }
        j++; // because yolo debugging
        out_lines.at(i).insert(j, current_grade);
        break;
      }
      if (i == out_lines.size() - 1) {
        if (1) continue; //s'all good dog
        fprintf(stderr, "Couldn't find student: (%s)\n", current_student.c_str());
        exit(1);
      }
    }
  }
  fclose(input);

  FILE *out = fopen(argv[3], "w");

  for (int i = 0; i < out_lines.size(); i++) {
    fprintf(out, "%s", out_lines.at(i).c_str());
  }

  fclose(out);

  return 0;
}
