#include <iostream>

using namespace std;

// class map {
// private:
//   string values = "";
//   int keylen = 0;
//
// public:
//   void add_value(char input) {
//     values += input;
//     keylen++;
//   }
//
//   int get_key(char input) {
//     for (int i = 0; i < keylen; i++)
//       if (values[i] == input)
//         return i;
//     return -1;
//   }
//
//   char get_value(int input) { return values[input]; }
// };

struct transition_table {
  int source;
  char letter;
  int destination;
};

class DFA {
private:
  int num_of_states, num_of_letters, num_of_final_states, num_of_delta,
      final_states[50];
  transition_table delta[100];

  int get_destination(int source, char letter) {
    for (int i = 0; i < num_of_delta; i++)
      if (delta[i].source == source && delta[i].letter == letter) {
        return delta[i].destination;
      }
    return -1;
  }

  bool is_final_state(int state) {
    for (int i = 0; i < num_of_final_states; i++)
      if (state == final_states[i])
        return 1;
    return 0;
  }

  bool validate(string input) {
    int str_len = input.length(), current_state = 0;
    for (int i = 0; i < str_len; i++) {
      current_state = get_destination(current_state, input[i]);
    }
    if (is_final_state(current_state))
      return 1;
    return 0;
  }

public:
  DFA(int nos, int nol) : num_of_states(nos), num_of_letters(nol) {
    num_of_delta = num_of_states * num_of_letters;
  }

  void get_final_states() {
    cin >> num_of_final_states;
    for (int i = 0; i < num_of_final_states; i++)
      cin >> final_states[i];
  }

  void get_transition_table() {
    for (int i = 0; i < num_of_delta; i++) {
      cin >> delta[i].source;
      cin >> delta[i].letter;
      cin >> delta[i].destination;
    }
  }

  void validate_multiple() {
    int num_of_inputs;
    bool results[50];
    string temp;
    cin >> num_of_inputs;
    for (int i = 0; i < num_of_inputs; i++) {
      cin >> temp;
      results[i] = validate(temp);
    }
    for (int i = 0; i < num_of_inputs; i++)
      cout << results[i] << endl;
  }
};

int main() {
  int nos, nol;
  char temp;
  cin >> nos;
  cin >> nol;
  for (int i = 0; i < nol; i++)
    cin >> temp;
  DFA machine(nos, nol);
  machine.get_final_states();
  machine.get_transition_table();
  machine.validate_multiple();
}
