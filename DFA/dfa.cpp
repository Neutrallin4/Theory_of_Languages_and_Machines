#include <iostream>
#include <string>

using namespace std;

// TODO: CSV reader
// TODO: Abstract format changer class
//
class DFA;

// DFA class only accepts inputs in integer or comma separated formats.
// this class correctly formats the input in a static manner.
class format {
private:
  int num_of_letters;
  string *map_str_array;

public:
  // Inputs the alphabet and stores it in a map structure.
  // Returns the number of letters.
  int input_alphabet() {
    cin >> num_of_letters;
    cin.ignore(1, '\n');
    map_str_array = new string[num_of_letters];
    for (int i = 0; i < num_of_letters; i++)
      getline(cin, map_str_array[i]);
    return num_of_letters;
  }

  // Inputs the final states and returns a comma separated string.
  static string input_final_states() {
    int num_of_final_states;
    string temp, final_states = "";
    cin >> num_of_final_states;
    cin.ignore(1, '\n');
    for (int i = 0; i < num_of_final_states; i++) {
      getline(cin, temp);
      final_states += temp;
      final_states += ",";
    }
    return final_states;
  }

  // Inputs the transition table and outputs a comma separated value.
  // Every letter will be mapped according to the values created in
  // input_alphabet()
  string input_transition_table(int delta_count) {
    string temp, transition_table = "";
    for (int i = 0; i < delta_count; i++) {
      getline(cin, temp);
      transition_table = transition_table + temp + ",";
      temp = "";
      getline(cin, temp);
      for (int j = 0; j < num_of_letters; j++)
        if (temp == map_str_array[j])
          transition_table = transition_table + to_string(j) + ",";
      temp = "";
      getline(cin, temp);
      transition_table = transition_table + temp + ",";
      temp = "";
    }
    return transition_table;
  }

  // Takes a string in the scope of the defined letters and putputs the mapped
  // version of that string.
  string validate(string input) {
    string output = "";
    int len = input.length();
    for (int i = 0; i < len; i++)
      for (int j = 0; j < num_of_letters; j++)
        if (map_str_array[j][0] == input[i]) {
          output = output + to_string(j) + ",";
          break;
        }
    return output;
  }
  // Takes a comma separated string and returns the first value while removing
  // it from the string.
  static string first_csv_value(string &input) {
    int anchor = 0, input_length = input.length();
    if (input[input_length - 1] != ',')
      input = input + ',';
    string temp;
    while (input[++anchor] != ',' && anchor < input_length)
      ;
    temp = input.substr(0, anchor);
    input = input.substr(anchor + 1, input_length - anchor);
    return temp;
  }
};

class DFA {
private:
  // The DFA needs to save the number of letters, number of states, and the
  // transition funcion. It also needs a variable to store its current location
  // among states. All letters are mapped to numbers before being fed to the
  // DFA.
  int *container, *final_states, num_of_final_states, container_total_size,
      container_used_size, num_of_states, num_of_letters, current_state;

  // Adds a row of data to the container.
  // The data is three integers that indicate what letter in which state leads
  // to what state. Notice that the letter is int, so the original letter has to
  // be mapped to an int before it is passed to this function.
  void add_delta(int starting_state, int letter, int ending_state) {
    container_used_size++;
    container[container_used_size] = starting_state;
    container_used_size++;
    container[container_used_size] = letter;
    container_used_size++;
    container[container_used_size] = ending_state;
  }

  // Reads the data stored in container.
  int delta(int starting_state, int letter) {
    for (int i = 0; i < container_total_size; i += 3)
      if (container[i] == starting_state && container[i + 1] == letter)
        return container[i + 2];
    return -1;
  }

public:
  // Initialization of a DFA requires the number of states and the number of
  // letters. This information is used to create the container array container
  // array saves the transition function inside of the DFA in a custom format
  DFA(int nos, int nol)
      : num_of_states(nos), num_of_letters(nol), num_of_final_states(0),
        current_state(0), container_used_size(0) {
    container_total_size = num_of_letters * num_of_states * 3;
    container = new int[container_total_size];
    final_states = new int[num_of_final_states];
  }

  // Accepts a comma separated string of final states.
  // Stores both the number and list of final states.
  void add_final_states(string input) {
    while (input != "")
      final_states[num_of_final_states++] =
          stoi(format::first_csv_value(input));
  }

  // Accepts a comma separated string of the entire transition table.
  // In case of long transition tables, it is possible to call this function
  // multiple times.
  void add_transition_table(string input) {
    while (input != "") {
      container[container_used_size++] = stoi(format::first_csv_value(input));
    }
  }

  // Accepts a comma separated string of input and feeds it to the DFA.
  // Returns true if the machine reaches a final state. False otherwise.
  bool validate(string input) {
    while (input != "") {
      current_state =
          delta(current_state, stoi(format::first_csv_value(input)));
    }
    // Match the last state of the DFA with the list of final states.
    for (int i = 0; i < num_of_final_states; i++) {
      if (final_states[i] == current_state) {
        current_state = 0;
        return true;
      }
    }
    current_state = 0;
    return false;
  }

  friend auto operator<<(std::ostream &os, DFA const &obj) -> std::ostream & {
    os << "-----------------------------" << endl;
    os << "Displaying the DFA\n";
    os << "Number of states: " << obj.num_of_states << endl;
    os << "Alphabet length: " << obj.num_of_letters << endl;
    os << "Final states:\n  ";
    for (int i = 0; i < obj.num_of_final_states; i++)
      os << obj.final_states[i] << " ";
    os << endl;
    os << "Transition function:\n";
    for (int i = 0; i < obj.container_total_size; i += 3)
      os << "  " << obj.container[i] << " " << obj.container[i + 1] << " "
         << obj.container[i + 2] << endl;
    os << "-----------------------------" << endl;
    return os;
  }
};

int main() {
  // Take the initial inputs
  int states, letters, num_of_inputs;
  string final_states, transition_table, temp_input, results = "";
  format driver;
  cin >> states;
  cin.ignore(256, '\n');
  letters = driver.input_alphabet();
  final_states = format::input_final_states();
  transition_table = driver.input_transition_table(states * letters);
  // Create the DFA based on the inputs
  DFA machine(states, letters);
  machine.add_final_states(final_states);
  machine.add_transition_table(transition_table);
  // Take the DFA's inputs and generate a string that stores the results
  cin >> num_of_inputs;
  cin.ignore(256, '\n');
  for (int i = 0; i < num_of_inputs; i++) {
    getline(cin, temp_input);
    temp_input = driver.validate(temp_input);
    results += to_string(machine.validate(temp_input));
  }
  // Print the results in individual lines
  while (results != "") {
    cout << results[0] << endl;
    results = results.substr(1, results.length() - 1);
  }
  return 0;
}
